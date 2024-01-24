#include "lru.h"
#include "hashtable.h"
#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void remove_node(lru_t *lru, node_t *node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        lru->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        lru->tail = node->prev;
    }
    lru->size--;
}

static void add_node_to_front(lru_t *lru, node_t *node) {
    node->next = lru->head;
    node->prev = NULL;
    if (lru->head != NULL) {
        lru->head->prev = node;
    }
    lru->head = node;
    if (lru->tail == NULL) {
        lru->tail = node;
    }
    lru->size++;
}

lru_t *lru_new(const int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    lru_t *lru = malloc(sizeof(lru_t));
    if (lru == NULL) {
        return NULL;
    }

    lru->capacity = capacity;
    lru->size = 0;
    lru->head = lru->tail = NULL;
    lru->hashtable = hashtable_new(capacity);
    if (lru->hashtable == NULL) {
        free(lru);
        return NULL;
    }

    return lru;
}

bool lru_insert(lru_t *lru, const char *key, void *item) {
    if (lru == NULL || key == NULL || item == NULL) {
        return false;
    }

    void *existing_item = hashtable_find(lru->hashtable, key);
    if (existing_item != NULL) {
        node_t *existing_node = existing_item;
        remove_node(lru, existing_node);
        add_node_to_front(lru, existing_node);
        return false;
    }

    if (lru->size >= lru->capacity) {
        node_t *old_tail = lru->tail;
        int slot = hash_function(old_tail->key, lru->hashtable->num_slots);
        set_t *old_set = lru->hashtable->slots[slot];
        set_t *new_set = set_new();
        for (setnode_t *node = old_set->head; node != NULL; node = node->next) {
            if (strcmp(node->key, old_tail->key) != 0) {
                set_insert(new_set, strdup(node->key), node->item);
            }
        }
        set_delete(old_set, NULL);
        lru->hashtable->slots[slot] = new_set;

        remove_node(lru, old_tail);
        free(old_tail->key);
        free(old_tail);
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;
    }
    new_node->key = strdup(key);
    new_node->item = item;

    add_node_to_front(lru, new_node);
    hashtable_insert(lru->hashtable, key, new_node);

    return true;
}

void *lru_find(lru_t *lru, const char *key) {
    if (lru == NULL || key == NULL) {
        return NULL;
    }

    void *item = hashtable_find(lru->hashtable, key);
    if (item != NULL) {
        node_t *node = item;
        remove_node(lru, node);
        add_node_to_front(lru, node);
        return node->item;
    }

    return NULL;
}

void lru_print(lru_t *lru, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (lru == NULL || fp == NULL) {
        return;
    }

    for (node_t *current = lru->head; current != NULL; current = current->next) {
        if (itemprint != NULL) {
            itemprint(fp, current->key, current->item);
        }
    }
}

void lru_iterate(lru_t *lru, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (lru == NULL || itemfunc == NULL) {
        return;
    }

    for (node_t *current = lru->head; current != NULL; current = current->next) {
        itemfunc(arg, current->key, current->item);
    }
}

void lru_delete(lru_t *lru, void (*itemdelete)(void *item)) {
    if (lru == NULL) {
        return;
    }

    node_t *current = lru->head;
    while (current != NULL) {
        node_t *next = current->next;
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }
        free(current->key);
        free(current);
        current = next;
    }

    hashtable_delete(lru->hashtable, NULL);
    free(lru);
}

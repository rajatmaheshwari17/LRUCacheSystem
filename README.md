# LRUCacheSystem
This repository contains an implementation of a Least Recently Used (LRU) Cache using a combination of Set and Hashtable Abstract Data Types (ADTs). The LRU Cache is designed to store a limited number of elements, discarding the least recently used item when the capacity is exceeded. This implementation provides functions for insertion, search, deletion, and iteration over the cache elements.

## Project Structure

-   `lru.c`: Contains the main implementation of the LRU Cache.
-   `lru.h`: Header file for the LRU Cache.
-   `hashtable.c`: Implementation of the Hashtable ADT.
-   `hashtable.h`: Header file for the Hashtable ADT.
-   `set.c`: Implementation of the Set ADT.
-   `set.h`: Header file for the Set ADT.
-   `test_lru.c`: Testing file for the LRU Cache.
-   `test_set.c`: Testing file for the Set ADT.
-   `test_hashtable.c`: Testing file for the Hashtable ADT.
-   `Makefile`: A Makefile to compile all test files at once.

## Compilation
To compile the project and all test files, simply use the `make` command:

`make` 

This command will compile all the necessary source files and create separate executables for each test file. Make sure your `Makefile` is properly configured to compile all the required files.

## Usage

To use the LRU Cache and test it, you can run the respective test executable:

### Testing the LRU Cache


`./test_lru` 

### Testing the Set ADT


`./test_set` 

### Testing the Hashtable ADT

`./test_hashtable` 

##
This README is a part of the LRUCacheSystem Project by Rajat Maheshwari.
 

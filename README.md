# Implementing malloc(), calloc(), realloc(), free() in c
Dynamic memory allocation is the process of assigning the memory space during runtime of the program. This is implemented in C as the functions malloc, calloc, realloc, and free from stdlib. malloc allocates a single block of memory of specified size and returns the pointer to the allocated block. calloc works similarly to malloc but initializes the allocated memory to zero. realloc is used to change the size of an allocated block of memory. Free deallocates memory associated with a pointer.

## Approach Explained
### Overview
* Each of the functions are implemented in seperate files.
* The memory is being allocated as different blocks as per the requests.
* Each block of memory that is being allocated contains two sections.
* The first part contains the meta information about the block( aka meta_block) and the second part is left to use for storing data.
* Storing meta information about the block is vital and becomes handy when dealing with freeing the memory allocated.
* These blocks are maintained in the order they are being allocated.
* For this purpose, these blocks are made into a doubly linked list. 
* Keeping the list of the blocks of free space and used space as a doubly linked list comes in handy when dealing with fragmentation.

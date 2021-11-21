# Implementing malloc(), calloc(), realloc(), free() in c
Dynamic memory allocation is the process of assigning the memory space during runtime of the program. This is implemented in C as the functions malloc, calloc, realloc, and free from stdlib. malloc allocates a single block of memory of specified size and returns the pointer to the allocated block. calloc works similarly to malloc but initializes the allocated memory to zero. realloc is used to change the size of an allocated block of memory. Free deallocates memory associated with a pointer.

### Contents
* [Approach](#approach-explained)
  * [Overview](#overview)
  * [Malloc.c](#mallocc)
  * [Free.c](#freec)
  * [Calloc.c](#callocc)
  * [Realloc.c](#reallocc)
* [References](#references)

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
* The requested size is aligned to the multiple of 4 greater than or equal to the size. Handling the addresses becomes simpler when they are aligned. 
* The system calls `sbrk()` and `brk()` are used to move the break of the heap.  

### Malloc.c
* ```malloc.c``` contains the structure ```meta_block``` which stores information about the memory block.
* Every ```meta_block``` is a node in the doubly linked list. The head of the doubly linked list in maintained globally in the variable ```base```.
* It stores information such as:
  * Size of the coresponding memory block.
  * Whether the coresponding memory block is free or not.
  * The address of the next and previous node in the doubly linked list.
  * Pointer to the starting of the memory block used to store information.
  * A character array variable which does not store any value but is used to get the address from where data can be stored.
* The function ```find_suitable_block()``` when invoked finds if there exist a free block of memory with aleast the required amount of space which is free by going through the linked list.
* The function ```split_space()``` when invoked splits the memory block into two if it contains enough space that can be used to store data.
* The function ```extend_heap()``` when invoked extends the heap and adds a new memory block to the linked list.
* When the function ```malloc()``` is called:
  * Creates a new block by invoking ```extend_heap()``` if the ```base``` is ```NULL```.
  * Else it iterates through the linkedlist to find if any suitable block(block with aleast requested size).
  * If a suitable block is found, it is returned.
  * Else it extends the heap and returns the newly created block.
  * It also check if the block found in the linkedlist has enough extra space to be splited. In this case, ```split_space()``` is invoked.

### Free.c
* The function `free()` first checks if the pointer passed as argument is a valid pointer or not (i.e., created using malloc()) using `is_addr_valid()`.
* Then using `get_block_addr()` the address of the `meta_block` of that corresponding memory location can be found.
* The block to be freed is merged with the previous or the next memory block it any of it is free using `merge_block()`.
* If the block to be freed is at the end of the linked list, then it is removed from the linkedlist and the break of the heap is modified using `brk()`.

### Calloc.c
* First `malloc()` is used to allocate the required amount of space.
* Then by iterating through every byte the value is set to 0.

### Realloc.c
* If the pointer to the old address(passed as argument) is `NULL`, then malloc is used to allocate the required memory.
* If the memory block of the old address has at least the required size then it is returned as it is.
* Then the next block is checked if it free and merged using `merge_block()`.
* If the space in the block is more than required after merging, then it is split using `split_space()`.
* If the next block is not free then new block is allocated using `malloc()`. 
* The data is then copied from the old location to the new location using `copy_data()`.
* The pointer to the new location is returned.

P.S: Please go through the comments in the code for more techincal details on the functions.

## References
* [Linux Manual page - sbrk()](https://linux.die.net/man/2/sbrk)
* [Linux Manual page - brk()](https://man7.org/linux/man-pages/man2/brk.2.html)
* [GeeksforGeeks](https://www.geeksforgeeks.org/memory-layout-of-c-program/)
* [Malloc tutorial by Dan Luu]()

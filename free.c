#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include"malloc.c"
#include"include/free.h"


/*
 * This file contains the implementaion of the function free()
 * For the details on few data structs such as meta_block and meta_ptr refer to the file malloc.c
 */


/* The function merge_block() checks if the next block is free and merges it with the block passed as an argument */
meta_ptr merge_blocks(meta_ptr block){
    if(block->next && block->next->free){
        block->size += META_BLOCK_SIZE + block->next->size;
        block->next = block->next->next; 
    }
    if(block->next){
        block->next->prev = block;
    }
    return block;
}

/* The function get_block_addr() returns the address of the meta_block from taking the addr of data as an argument.
 * Type casing the passed void pointer to char pointer and subtracting the size of the meta_block will give us the required address. 
 */
meta_ptr get_block_addr(void *p){
    char *tmp = p;
    tmp = tmp - META_BLOCK_SIZE;
    p = tmp;
    return (p);
}

/*
 * This function checks if the given pointer address is indeed created by invoking malloc() or not.
 * We use the field ptr in the meta_block() to check if the passed address is same as the one present in the meta_block()
 */
int is_addr_valid(void *p){
    if(base){
        if(p > base && p < sbrk(0)){
            return (p == get_block_addr(p)->ptr);
        }
    }
    return 0;
}

/*
 * The pointer is first checked if valid. If it's valid we set the free field value of the block to 1
 * Then if the previous block exists, it is checked if its free and then merged with the current block.
 * Same is done for the next block also.
 * And finally if the freed block is at the end of the linkedlist, it is removed from the linkedlist and the break line
 * of the heap is set to the corresponding last address in the linkedlist using the syscall brk()  
 */
void free(void *ptr){
    if(is_addr_valid(ptr)){
        meta_ptr block = get_block_addr(ptr);
        block->free = 1;
        if(block->prev && block->prev->free){
            block = merge_blocks(block->prev);
        }

        if(block->next){
            block = merge_blocks(block);
        }
        else{
            if(block->prev){
                block->prev->next = NULL;
            }
            else{
                base = NULL;
            }
            brk(block);
        }
    }
}
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "malloc.c"
#include "free.c"
#include "include/realloc.h"

/*
 * This file contains the implementation of realloc()
 * To know about meta_ptr, META_BLOCK_SIZE and other similar declarations refer the file malloc.c
 * reallco() can be used to increase or decrease the size of a pre-existing memory block.
 * The values in the pre-existing memory block will be copied in the new-block formed.
 */

/*
 * The function copy_data() is used to copy the data from a source block to destination block.
 * It runs a loop from the starting memory location till the ending memory location.
 */

void copy_data(meta_ptr src, meta_ptr dest)
{
    int *src_data = src->data;
    int *dest_data = dest->data;
    size_t i;
    for (i = 0; i * 4 < src->size && i * 4 < dest->size; i++)
    {
        dest_data[i] = src_data[i];
    }
}

/*
 * realloc() takes two arguments: pointer pointing to old memory location
 * and the size of the new memory block to be allocated.
 * If the pointer given as argument is NULL then malloc() is used to create a momory allocation and the new pointer is returned.
 * The function first checks if the the pointer is a valid memory address,
 * then if the old_block has enough memory then it is returned as it is.
 * If the memory is no sufficient in the old_block then the next block is checked if its free and fused if possible.
 * If the memory formed is more than requried after fusing with the next block, then the extra memory is split into a new block.
 * Memory is allocated newly and the data is copied if enough memory is not found even after fusing the next free block(if available).
 * Finally the pointer to the new memory address is returned.
 */

void *realloc(void *p, size_t size)
{
    size_t s;
    meta_ptr new_block, old_block;
    void *new_ptr;

    /* Checking if the passed pointer is NULL */
    if (!p)
    {
        return malloc(size);
    }

    /* Checking if the passed pointer is a valid memory address or not */
    if (is_addr_valid(p))
    {
        /* Aligning the requested size */
        s = align4(size);
        old_block = get_block_addr(p);
        
        /*
         * s is the new alligned space.
         * old_block->size returns the size of the old memory block passes as an argument when realloc() is invoked
         * META_BLOCK_SIZE is the size of the block storing the meta information of that memory block.
         */
        if (old_block->size >= s)
        {
            if (old_block->size >= (META_BLOCK_SIZE + 4))
            {
                split_space(old_block, s);
            }
        }
        else
        {
            if (old_block->next && old_block->next->free && (old_block->size + old_block->next->size + META_BLOCK_SIZE) >= s)
            {
                merge_blocks(old_block);
                if (old_block - s >= (META_BLOCK_SIZE + 4))
                {
                    split_space(old_block, s);
                }
            }
            else
            {
                new_ptr = malloc(s);
                
                if (!new_ptr)
                    return NULL;
                
                new_block = get_block_addr(new_ptr);
                copy_data(old_block, new_block);
                
                free(p);
                return new_ptr;
            }
        }
        return p;
    }
    return NULL;
}
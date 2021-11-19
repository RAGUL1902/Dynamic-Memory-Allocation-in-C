#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include"malloc.c"

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

meta_ptr get_block_addr(void *p){
    char *tmp = p;
    tmp = tmp - META_BLOCK_SIZE;
    p = tmp;
    return (p);
}

int is_addr_valid(void *p){
    if(base){
        if(p > base && p < sbrk(0)){
            return (p == get_block_addr(p)->ptr);
        }
    }
    return 0;
}

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
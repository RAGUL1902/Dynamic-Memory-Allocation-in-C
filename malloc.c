#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include"malloc.h"

#define META_BLOCK_SIZE 12
#define align4(x) (((((x) -1) > >2) < <2)+4)

typedef struct meta_block *meta_ptr;

void* base = NULL;

struct meta_block
{
    int free;
    size_t size;
    meta_ptr next;
    char data[1];
};

meta_ptr find_suitable_block(meta_ptr *last, size_t size){
    meta_ptr b = base;
    while(b && !(b->free && b->size >= size)){
        *last = b;
        b = b->next;
    }
    return last;
}

void split_space(meta_ptr block, size_t size){
    meta_ptr new_block;
    new_block = block->data + size;
    new_block->size = block->size - size - META_BLOCK_SIZE;
    new_block->next = block->next;
    new_block->free = 1;
    block->next = new_block;
    block->size = size;
}

meta_ptr extend_heap(meta_ptr last, size_t size){
    meta_ptr old_break, new_break;
    old_break = sbrk(0);
    new_break = sbrk(META_BLOCK_SIZE + size);
    if(new_break == (void*) -1){
        return NULL;
    }
    old_break->size = size;
    old_break->free = 0;
    old_break->next = NULL;
    if(last){
        last->next = old_break;
    }
    return (old_break);
}

void* malloc(size_t size){
    meta_ptr block,last;
    size_t s;
    s = allign4(size);
    if(base){
        last = base;
        block = find_suitable_block(&last,s);
        if(block){
            if(block->size - s >= (META_BLOCK_SIZE + 4)){
                split_space(block,s);
            }
            block->free = 0;
        }
        else{
            block = extend_heap(last,s);
            if(!block){
                return NULL;
            }
        }
    }
    else{
        block = extend_heap(NULL,s);
        if(!block){
            return NULL;
        }
        base = block;
    }
    return block->data;
}




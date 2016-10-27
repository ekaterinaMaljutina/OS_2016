#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#include "list.h"


typedef uint64_t u64;
typedef struct alloc_descriptor alloc_t;

struct alloc_descriptor {
    u64 head;
    u64 num_pages;
    node * lists[50];
    node * save;
};

void buddy_init();
u64 do_alloc( u64 );
void do_free( u64 );

#endif // BUDDY_ALLOCATOR_H

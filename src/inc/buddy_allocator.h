#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#include "list.h"

struct buddy_allocator_descriptor
{
    uint8_t level;
    uint8_t is_free;
    list_ list_node;
};
typedef struct buddy_allocator_descriptor buddy_allocator;
buddy_allocator *descriptors;

void  init_buddy_allocator();
void* allocate_empty_page(int level);
void* allocating_page(int level);
void free_page(void *address, int level);

#endif // BUDDY_ALLOCATOR_H

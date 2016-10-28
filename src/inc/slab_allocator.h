#ifndef __SLAB_ALLOCATOR__
#define __SLAB_ALLOCATOR__

#include <stdint.h>
#include <stddef.h>

#include "buddy_allocator.h"

typedef struct slab_allocator slab_t;
typedef struct slab_node slab_node_t;

struct slab_allocator {
	uint64_t memory_begin;

	uint8_t free_space;
	slab_t* next;
	slab_t* prev;
};

void create_slab(uint64_t block_size, uint64_t max_cnt);


#endif  //__SLAB_ALLOCATOR__
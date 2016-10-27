#include "slab_allocator.h"
#include "printf.h"

void slab_init(slab_t* slab, uint64_t* memory, uint64_t block_size, uint64_t max_cnt) {
	slab->memory = memory;
	slab->head = memory;
	slab->free_space = max_cnt;
	slab->next = NULL;
	slab->prev = NULL;
	for (uint64_t i = 0; i < max_cnt - 1; ++i) {
		(memory)[i] = (uint64_t)memory + block_size;
		memory += block_size;
	}
	memory = 0;
}

void create_slab(uint64_t block_size, uint64_t max_cnt){

	if (block_size < 8) 
		block_size = 8;
	uint64_t* memory =(uint64_t*) do_alloc(block_size * max_cnt + sizeof(slab_t));
	printf("memory %llx\n",(uint64_t)memory );
	
	if (!memory) 
		return;
	
	slab_t* slab = (slab_t*)((uint64_t)memory + block_size * max_cnt);
	printf("slab %llx\n",(uint64_t)slab );
	slab_init(slab, memory, block_size, max_cnt);

	//return slab;	
}
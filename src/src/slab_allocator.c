#include "slab_allocator.h"
#include "base.h"
#include "printf.h"

/*void slab_init(slab_t* slab, uint64_t* memory, uint64_t block_size, uint64_t max_cnt) {
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
}*/

void create_slab(uint64_t block_size, uint64_t max_cnt){
	if (block_size >= PAGE_SIZE || !block_size) {
        printf("Wrong size of block requested: %d!\n", block_size);
    }

	/*if (block_size < 8) 
		block_size = 8;*/

	uint64_t pages = (8 * block_size  < PAGE_SIZE ? 1 : 10);
	/*uint64_t addres = do_alloc(pages);
	slab_t* slab = (slab_t*)addres;
	slab->memory_begin = addres + sizeof(slab_t);*/

	uint64_t memory = do_alloc(pages);
	printf("memory %llx\n",(uint64_t)memory ,max_cnt);
	slab_t* slab = (slab_t*)memory;
	slab->memory_begin = (uint64_t)memory + sizeof(slab_t);
	printf("memory %llx\n",(uint64_t)memory );
	
	/*if (!memory) 
		return;
	
	slab_t* slab = (slab_t*)((uint64_t)memory + block_size * max_cnt);
	printf("slab %llx\n",(uint64_t)slab );
	slab_init(slab, memory, block_size, max_cnt);*/

	//return slab;	
}
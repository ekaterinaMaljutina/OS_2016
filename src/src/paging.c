#include "paging.h"
#include "printf.h"
#include "base.h"

#define GB ((uint64_t)(1LL << 30))
#define TABLE 512
#define AFTER 256

#define PTE_PRESENT	(1 << 0)
#define PTE_WRITE	(1 << 1)
#define PTE_LARGE	(1 << 7)

extern uint64_t* bootstrap_pml4;
uint64_t* pml4[TABLE];

extern free_memory memory[32];
extern int free;

extern uint64_t phys_mem_end;

void init_paging(){

	uint64_t map_GB = (phys_mem_end + GB) / GB; 
	uint64_t map_entr = (map_GB + TABLE - 1) / TABLE;
	
	printf("GB = %llx, map_entr = %llx \n", map_GB, map_entr);
	
	for (uint32_t i = 0; i < (uint32_t) free; i++){

		uint64_t left_path_of_memory = align_up(memory[i].first); 

		uint64_t right_path_of_memory = memory[i].until;	

		//first 4GB
		if (right_path_of_memory > (1LL << 32))
			right_path_of_memory = (1LL << 32);

		if ((left_path_of_memory > right_path_of_memory) || 
			((right_path_of_memory + 1 - left_path_of_memory) < (map_entr * PAGE_SIZE)))
			continue;


		for (uint64_t j = 0; j < map_entr; j++, left_path_of_memory += PAGE_SIZE)
			pml4[j] = (uint64_t*)left_path_of_memory;

		memory[i].first = left_path_of_memory + map_entr * PAGE_SIZE; 

		break;
	}

	//set  GB at pml4 
	for (uint64_t i = 0; i < map_GB; i++)
		pml4[i / TABLE + AFTER ][i % TABLE] = (i * GB) | PTE_PRESENT | PTE_WRITE | PTE_LARGE;


	for (uint64_t i = 0; i < map_entr; i++)
		bootstrap_pml4[i] = (*pml4[i]) | PTE_PRESENT | PTE_WRITE;

}
#ifndef MEMMAP_H
#define MEMMAP_H

#include <stdint.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct memory{
    uint64_t first;
    uint64_t until;
    uint64_t len;
};

typedef struct memory free_memory;

extern free_memory memory[32];
extern int free;
extern uint64_t phys_mem_end;


void init_memmap();

uint64_t memory_allocate(uint64_t size);
uint64_t get_size_memory();
uint32_t get_memory_map_size();


void stop_massenge(char* const messenge,...);

#endif // MEMMAP_H

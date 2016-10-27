#include "base.h"
#include "buddy_allocator.h"

int get_buddy(int level){
    return (1ULL << level);
}

uint64_t pa(const void *addr){
    return PA((uint64_t)addr);
}
uint64_t va(uint64_t addr){
    return (uint64_t)VA(addr);
}

uint64_t align_up(uint64_t base) {
    return (base + (base % PAGE_SIZE ? PAGE_SIZE - base % PAGE_SIZE : 0)); 
}
uint64_t align_down(uint64_t base) {
    return (base - base % PAGE_SIZE); 
}

uint64_t div_up(uint64_t a, uint64_t b){
 	if (a % b)
 		return a / b + 1;
 	return a / b;
}
void* align(void* pointer, uint64_t alignment){
	uint64_t value = (uint64_t) pointer;
 	if (value % alignment)
 		value = value / alignment * alignment + alignment;
 	return (void*) value;
}

void memset(void* buf, char c, size_t len) {
    for (size_t i = 0; i < len; i++) {
        ((char*)buf)[i] = c;
    }
}
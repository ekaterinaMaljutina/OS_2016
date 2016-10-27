#ifndef __BASE__H
#define __BASE__H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "memory.h"

#define KERNEL_BASE       0xffffffff80000000
#define PHYSICAL_BASE     0x0000000000000000
#define HIGH_BASE         0xffff800000000000

#define KERNEL_PHYS(x)    ((x) - KERNEL_BASE)
#define KERNEL_VIRT(x)    ((x) + KERNEL_BASE)

#define SMALL_PAGE_BITS      12
#define SMALL_PAGE_SIZE      (1 << 12)
#define SMALL_PAGE_MASK      (SMALL_PAGE_SIZE - 1)
#define BIG_PAGE_BITS        21
#define BIG_PAGE_SIZE        (1 << 21)
#define BIG_PAGE_MASK        (BIG_PAGE_SIZE - 1)
#define KERNEL_SIZE          (1ull << 31)

#define ALL_SIZE             7
#define MAXIMUM_OR           50
#define OFFSET_B             1ULL
#define PAGE_SIZE            0x1000

#define VA(x)                ((x) + HIGH_BASE)
#define PA(x)                ((x) - HIGH_BASE)

extern uint64_t phys_mem_end;


int get_buddy(int level);
uint64_t align_up(uint64_t base);
uint64_t align_down(uint64_t base) ;

uint64_t div_up(uint64_t a, uint64_t b);
void* align(void* pointer, uint64_t alignment);


uint64_t pa(const void *addr);
uint64_t va(uint64_t addr);

void memset(void* buf, char c, size_t len);

uint64_t alloc_big_phys_aligned(size_t size);
#endif /*__BASE__H*/

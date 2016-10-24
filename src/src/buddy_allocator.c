#include "buddy_allocator.h"
#include "memmap.h"


#include "printf.h"

#define LEVEL 20


#define SMALL_PAGE_SIZE  (1<<12)
#define BASE	0xffffffff80000000
#define VA(x)                ((x) + BASE)
#define PA(x)                ((x) - BASE)

int descr_size;

list_* level_head;

int get_buddy(int id, int level){
    return id ^ (1 << level);
}

void* align(void* pointer, uint64_t alignment){
    uint64_t value = (uint64_t) pointer;
    if (value % alignment)
        value = value / alignment * alignment + alignment;
    return (void*) value;
}

uint64_t div_up(uint64_t a, uint64_t b){
    if (a % b)
        return a / b + 1;
    return a / b;
}
static inline uintptr_t pa(const void *addr){
    return PA((uintptr_t)addr);
}
static inline void *va(uintptr_t addr){
    return (void *)VA(addr);
}

void *address;

void bootstrap_init(uint64_t size){
    address = va(memory_allocate(size));
}

void* bootstrap_allocate(uint64_t size){
    void *old_address = address;
    address = (void*) ((uint8_t*) address + size);
    return old_address;
}

void* bootstrap_allocate_with_alignment(uint64_t size, uint64_t alignment){
    address = align(address, alignment);
    return bootstrap_allocate(size);
}


void free_page(void *address, int level){
    int id = pa(address) / SMALL_PAGE_SIZE;
    for (int current_level = level; current_level < LEVEL; current_level++)
    {
        int buddy_id = get_buddy(id, current_level);
        if (buddy_id >= descr_size || !descriptors[buddy_id].is_free
                || (int) descriptors[buddy_id].level != current_level)
        {
            list_add_tail(&descriptors[id].list_node,
                          &level_head[current_level]);
            descriptors[id].is_free = 1;
            descriptors[id].level = current_level;
            break;
        }
        else
        {
            list_del(&descriptors[buddy_id].list_node);
            id = MIN(id, buddy_id);
        }
    }
}

void add_page(int id){
    free_page(va(id * SMALL_PAGE_SIZE), 0);
}


void init_buddy_allocator(){
    printf("Initial buddy : \n");
    uint64_t size_memory = get_size_memory();
    descr_size = size_memory / SMALL_PAGE_SIZE;

    uint64_t descriptors_memory = descr_size *
            (uint64_t)sizeof(buddy_allocator);
    printf("... descr %d (%lu bytes) \n",descr_size , descriptors_memory);

//    printf("here \n");
//    memory_allocate(descriptors_memory);
//    printf("here \n");

    uint64_t level_head_memory = LEVEL * sizeof(list_);

    bootstrap_init(level_head_memory + descriptors_memory);

    descriptors = (buddy_allocator*)bootstrap_allocate(descriptors_memory);
    level_head = (list_*)(uint64_t*)bootstrap_allocate(
                (uint64_t)level_head_memory);
//    memory[use_piece].first += descriptors_memory;
//    memory_allocate(descriptors_memory);

    uint32_t memory_map_size = get_memory_map_size();
    for (int i=0; i<LEVEL; i++)
        list_init(&level_head[i]);
    for (int i=0; i< descr_size;i++){
        descriptors[i].is_free = 0;
        list_init(&descriptors[i].list_node);
    }
    for (int i = 0; i<(int) memory_map_size ; i++){
        uint64_t left_bound = memory[i].first;
        if (left_bound % SMALL_PAGE_SIZE)
            left_bound = left_bound / SMALL_PAGE_SIZE * SMALL_PAGE_SIZE
                    + SMALL_PAGE_SIZE;
        for (; left_bound + SMALL_PAGE_SIZE <= memory[i].first +
             memory[i].len; left_bound += SMALL_PAGE_SIZE)
        {
            descriptors[left_bound / SMALL_PAGE_SIZE].is_free = 1;
            add_page(left_bound / SMALL_PAGE_SIZE);
        }
    }
    printf("finish! \n");
}


void* allocating_page(int level){
    for (int current_up_level = level; current_up_level < LEVEL;
         current_up_level++){
        if (level_head[current_up_level].next != &level_head[current_up_level]){
            int id = LIST_ENTRY(level_head[current_up_level].prev,
                                buddy_allocator, list_node) - descriptors;
            list_del(level_head[current_up_level].prev);
            for (int current_down_level = current_up_level - 1;
                 current_down_level >= level; current_down_level--)
            {
                int buddy_id = get_buddy(id, current_down_level);
                descriptors[buddy_id].is_free = 1;
                descriptors[buddy_id].level = current_down_level;
                list_add_tail(&descriptors[buddy_id].list_node,
                              &level_head[current_down_level]);
            }
            descriptors[id].is_free = 0;
            descriptors[id].level = level;
            return va(id * (uint64_t) SMALL_PAGE_SIZE);
            break;
        }
    }
    return 0;
}

void* allocate_empty_page(int level){
    uint8_t *pointer = (uint8_t*) allocating_page(level);
    for (int i = 0; i < SMALL_PAGE_SIZE; i++)
        pointer[i] = 0;
    return (void*) pointer;
}

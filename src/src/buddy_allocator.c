#include "buddy_allocator.h"
#include "memmap.h"
#include "list.h"
#include "printf.h"
#include "base.h"


alloc_t descriptors[7];
u64 size_alloc_memory = 0;

void init_helper(alloc_t *buddy) {
    buddy->num_pages = buddy->num_pages * PAGE_SIZE / (sizeof(node) + PAGE_SIZE);
    buddy->save = (node *) buddy->head;

    for (int64_t or = MAXIMUM_OR, offset = 0; or > -1; --or) {
        if (buddy->num_pages & get_buddy(or)) {

            create_node_list(&buddy->save[offset], or, false, NULL, NULL);
            buddy->lists[or] = &buddy->save[offset];
            offset += get_buddy(or);
        } else {
            buddy->lists[or] = NULL;
        }
    }

    buddy->head = align_up(buddy->head + buddy->num_pages * sizeof(struct d_linked_list));
    // printf(" buddy_allocator first %lx\n \n",p->head );
}


void buddy_init(){
    u64 count_pages = 0;
    u64 size_memory = get_memory_map_size();

    // u64 size = get_size_memory();
    // u64 total_pages = size / PAGE_SIZE;

    for (u64 i = 0; i < size_memory; ++i) {

        free_memory *memory_ = &memory[i];
        // printf(" mmap_memory__t %llx %llx %llx \n",memory_->first, memory_->len, memory_->until );
        // if (memory_->until > (1LL << 32)){
        //     printf("this memory_ after 4Gb 0x%llx - 0x%llx \n", memory_->first, memory_->until);
        //     memory_->until = (1LL << 32) - 1LL;
        //     printf(" allocate only this part 0x%llx - 0x%llx \n", memory_->first, memory_->until);
        // }
        alloc_t *new_buddy = &descriptors[size_alloc_memory++];
        // printf("\n physical address: %llx \n", align_up(memory_->first + !memory_->first) );

        new_buddy->head = va(align_up(memory_->first + !memory_->first));
        // printf("new_buddy head %llx \n", new_buddy->head );

        u64 buddy_end = va(align_down(memory_->until));
        
        if (new_buddy->head >= buddy_end) {
            --size_alloc_memory;
            continue;
        }
        new_buddy->num_pages = (buddy_end - new_buddy->head) / PAGE_SIZE;

        init_helper(new_buddy);
        count_pages += new_buddy->num_pages;
        
    }
    printf("%d pages use\n", count_pages);
}



u64 alloc_helper(alloc_t *buddy, u64 or, u64 len) {
    if (or > MAXIMUM_OR) 
        return 0;


    u64 current_len = get_buddy(or);
    if (current_len < len || buddy->lists[or] == NULL) 
        return alloc_helper(buddy, or + 1, len);


    u64 current = buddy->lists[or] - buddy->save;
    pop_front(&buddy->lists[or]);
    if (current_len >= 2 * len) {
        create_node_list_and_push(&buddy->save[(u64)(current ^ get_buddy(or-1))], or - 1, false, NULL, NULL,&buddy->lists[or - 1]);
        create_node_list_and_push(&buddy->save[current], or - 1, false, NULL, NULL,&buddy->lists[or - 1]);

        return alloc_helper(buddy, or - 1, len);
    }

    create_node_list(&buddy->save[current], or, true, NULL, NULL);
    return (u64)(buddy->head + PAGE_SIZE * current);
}


u64 do_alloc(u64 len){

   for (u64 i = 0; i < size_alloc_memory; ++i) {

        alloc_t *buddy_allocator_ = &descriptors[i];

        u64 res = alloc_helper(buddy_allocator_, 0, len);
        if (res) 
            return res;
    }
    return 0; 
}


static void free_herler(alloc_t *buddy, u64 current) {
    if (!buddy->save[current].use) {
        printf("address free!\n");
        return;
    }
    u64 or = buddy->save[current].or;
    u64 current_buddy = current ^ get_buddy(or);

    if (!buddy->save[current_buddy].use && buddy->save[current_buddy].or == or &&
            current_buddy < buddy->num_pages) {

        if (&buddy->save[current_buddy] == buddy->lists[or]) 
            pop_front(&buddy->lists[or]);
        else 
            pop_back(&buddy->save[current_buddy]);

        ++buddy->save[current].or;
        if (current_buddy < current) {
            buddy->save[current].use = false;
            buddy->save[current_buddy].use = true;
            current = current_buddy;
        }
        free_herler(buddy, current);
        return;
    }
    buddy->save[current].use = false;
    push_front(&buddy->save[current], &buddy->lists[or]);
    buddy->lists[or] = &buddy->save[current];
}


void do_free(u64 current_len) {
    if (current_len % PAGE_SIZE) {
        printf("address not aligned!\n");
        return;
    }
    for (u64 i = 0; i < size_alloc_memory; ++i) {
        alloc_t * buddy_allocator_ = &descriptors[i];
        if ( buddy_allocator_->head <= current_len &&
            current_len <  buddy_allocator_->head +  buddy_allocator_->num_pages * PAGE_SIZE) {

            u64 size = (current_len -  buddy_allocator_->head) / PAGE_SIZE;
            free_herler( buddy_allocator_, size);

            return;
        }
    }
}
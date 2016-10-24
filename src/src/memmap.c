#include <stdint.h>
#include <stdarg.h>

#include "memmap.h"
#include "multiboot.h"
#include "printf.h"
#include "ints.h"
#include "buddy_allocator.h"

free_memory memory[32];
int free = 0;

void add(free_memory memory[], int* free,
         uint64_t first, uint64_t until){
    if (until - first <= 0)
        return;
    memory[*free].first = first;
    memory[*free].until = until;
    memory[*free].len = until - first;
    (*free)++;
}

void print_free_memory(int free, free_memory memory[]){
    printf("  free : \n");
    for (uint8_t i =0; i<free; i++){
        printf("%x :  0x%lx  -  0x%lx, len = 0x%lx\n",i, memory[i].first,
               memory[i].until,memory[i].len);
    }
}

extern uint32_t multiboot_info;
extern char text_phys_begin[];
extern char bss_phys_end[];


uint8_t flag(multiboot_uint32_t flag,uint8_t bits){
      return flag & (1<<bits);
}


void stop_massenge(char* const messenge,...){
    va_list arg;
    va_start(arg,messenge);
    vprintf(messenge,arg);
    disable_ints();
    __asm__("hlt");
}

void space(multiboot_memory_map_t* memmap, int *free, free_memory* memory){

    uint64_t f_space = MAX((uint64_t) text_phys_begin,memmap->addr);
    uint64_t u_space = MIN((uint64_t) bss_phys_end,memmap->addr + memmap->len);

    if (f_space < u_space){
        if (memmap->addr < f_space)
            add(memory,free,memmap->addr,f_space);
        if (u_space < memmap->addr + memmap->len)
            add(memory,free,u_space,memmap->addr + memmap->len);
    } else
        add(memory,free,memmap->addr,memmap->addr + memmap->len);

}

void init_memmap(){
    multiboot_info_t* multiboot_info_ =
            (multiboot_info_t* )(uint64_t)multiboot_info;

    //Are mmap_* valid ?
    if (!flag(multiboot_info_->flags,6))
        //stop program
        stop_massenge("not memmory map");

    uint64_t krnl_fbyte =(uint64_t) text_phys_begin;
    printf("krnl: 0x%lx",krnl_fbyte);

    uint64_t krnl_ubyte = (uint64_t) bss_phys_end;
    printf(" - 0x%lx\n",krnl_ubyte);

    multiboot_memory_map_t *memmap;

    //  https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

    for (memmap=(multiboot_memory_map_t*)(uint64_t)multiboot_info_->mmap_addr;

         (uint64_t)memmap <
         multiboot_info_->mmap_addr+multiboot_info_->mmap_length;

         memmap = (multiboot_memory_map_t*)
         ((uint64_t)memmap + memmap->size + sizeof(memmap->size))){

        printf ( "size = 0x%x,  base_addr = 0x%lx,"
                 "  length = 0x%lx, until = 0x%lx,  type = 0x%x\n",
                 (unsigned) memmap->size,
                 memmap->addr , memmap->len,
                 memmap->addr + memmap->len,
                 (unsigned) memmap->type);
        if (memmap->type == 1)
            space(memmap,&free,memory);
    }

    print_free_memory(free,memory);
}

uint64_t memory_allocate(uint64_t size){
    for (int i=0; i< free; i++){
        free_memory* memory_ = &memory[i];
        if (memory_->len >= size){
            memory_->len -= size;
            add(memory,&i,memory_->first + memory_->len,size);
            return memory_->first + memory_->len;
        }
    }
    return 0;
}
uint64_t get_size_memory(){
    uint64_t size_memory = 0;
    for (int i=0; i < free; i++){
        size_memory = MAX(size_memory,memory[i].until);
    }
    return size_memory;
}

uint32_t get_memory_map_size(){
    return free;
}

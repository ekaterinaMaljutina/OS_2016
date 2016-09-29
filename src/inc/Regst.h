#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <stdint.h>

struct registers_t {
    uint64_t rg_15;
    uint64_t rg_14;
    uint64_t rg_13;
    uint64_t rg_12;
   	uint64_t rg_11;
    uint64_t rg_10;
    uint64_t rg_9;
    uint64_t rg_8;
    uint64_t ebp;
    uint64_t edi;
    uint64_t esi;
    uint64_t edx;
    uint64_t ecx;
    uint64_t ebx;
    uint64_t eax;
	uint64_t int_code; //номер прерывания
    uint64_t esp; 
	uint64_t err_code; //код ошибки
} __attribute__((packed));

struct idt_entry {
   uint16_t base_lo;
   uint16_t sel; //переключатель сегмента ядра
   uint8_t  always0; // всегда = 0
   uint8_t  flags;
   uint16_t base_hi; //куда происходит переход
   uint32_t base_64;
   uint32_t reserved;
} __attribute__((packed));



#endif /*__REGISTER_H__*/
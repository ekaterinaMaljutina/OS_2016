#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <stdint.h>

struct registers_t { //64x битные
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
   	uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
	uint64_t int_code; //номер прерывания
    uint64_t rsp; 
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
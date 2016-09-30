#include "interrupt.h"
#include "SerialPort.h"
#include "memory.h"

struct idt_entry idt[256];
struct desc_table_ptr descp;

char const * const error_code_out[32]={
	"Деление на ноль\n", //0
	"прерывание отладки, пошаговое исполнение\n", //1
	"Немаскируемое прерывание\n", // 2
	"Прерывание точки останова\n", //3 
	"Переполнение при выполнении команды Into\n" //4,
	"Прерывание выхода за границы данных\n" // 5 
	"Прерывание неправильного кода операции\n", //6
	"Прерывание отсутствие сопроцессора\n", //7 
	"Прерывание двойной ошибки (код ошибки помещается в стек\n)" //8
	};


void idt_install(){
    descp.size = sizeof (struct idt_entry) * 256 ;
    descp.addr = (uint64_t)idt;
    // memset 0
    uint64_t size = descp.size;
	for (uint64_t i = 0; i < size; i++) {
		((char*)idt)[i] = 0;
	}
	write_idtr(&descp);
}

void idt_set_gate(uint8_t const idx, uint64_t const base, \
				  uint16_t const sel, uint8_t const flags){ 
    idt[idx].base_lo = (base & 0xFFFF);
    idt[idx].base_hi = (base >> 16) & 0xFFFF;
	idt[idx].base_64 = (base >> 32) & 0xFFFFFFFF;
    // сегмент селектор   
	idt[idx].sel = sel; 
	idt[idx].always0 = 0;     
	idt[idx].flags = flags; 
}


void handler(struct registers_t const * reg){
	if (reg->int_code < 32) {
		serial_port_write_char("error with code out:   ");
		serial_port_write_char(error_code_out[reg->int_code]);
	}
}

extern void handler0();
extern void handler1();
extern void handler2();
extern void handler3();
extern void handler4();
extern void handler5();
extern void handler6();
extern void handler7();
extern void handler8();
extern void handler9();
extern void handler10();
extern void handler11();
extern void handler12();
extern void handler13();
extern void handler14();
extern void handler15();
extern void handler16();
extern void handler17();
extern void handler18();
extern void handler19();
extern void handler20();
extern void handler21();
extern void handler22();
extern void handler23();
extern void handler24();
extern void handler25();
extern void handler26();
extern void handler27();
extern void handler28();
extern void handler29();
extern void handler30();
extern void handler31();

void intr_install(){
	idt_set_gate(0, (uint64_t) handler0,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler1,  KERNEL_CS, 0x8E);   
	idt_set_gate(0, (uint64_t) handler2,  KERNEL_CS, 0x8E); 
	idt_set_gate(0, (uint64_t) handler3,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler4,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler5,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler6,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler7,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler8,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler9,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler10,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler11,  KERNEL_CS, 0x8E);   
	idt_set_gate(0, (uint64_t) handler12,  KERNEL_CS, 0x8E); 
	idt_set_gate(0, (uint64_t) handler13,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler14,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler15,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler16,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler17,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler18,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler19,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler20,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler21,  KERNEL_CS, 0x8E);   
	idt_set_gate(0, (uint64_t) handler22,  KERNEL_CS, 0x8E); 
	idt_set_gate(0, (uint64_t) handler23,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler24,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler25,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler26,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler27,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler28,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler29,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler30,  KERNEL_CS, 0x8E);
	idt_set_gate(0, (uint64_t) handler31,  KERNEL_CS, 0x8E);   
}
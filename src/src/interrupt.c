#include "interrupt.h"
#include "SerialPort.h"
#include "memory.h"

struct idt_entry idt[256];
struct desc_table_ptr descp = {0,0};

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

void idt_set_gate(unsigned char num,unsigned long base, \
	unsigned short sel, unsigned char flags){
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
	idt[num].base_64 = (base >> 32) & 0xFFFFFFFF;
    // сегмент селектор   
	idt[num].sel = sel; idt[num].always0 =0;     
	idt[num].flags = flags; 
}


void handler(struct registers_t * reg){
	if (reg->int_code < 32) {
		serial_port_write_char("error");
	}
}

extern void handler0();

void intr_install(){
	idt_set_gate(0, (unsigned long) handler0,  KERNEL_CS, 0x8E);    
}
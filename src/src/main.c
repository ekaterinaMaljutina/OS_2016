static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}


#include <ints.h>
#include "ioport.h"
#include "interrupt.h"
#include "SerialPort.h"


#define interrupt_int(value)	__asm__ volatile("int %0" : : "n"(value))

void main(void)
{
    serial_port_init();
    char *hw = "Hello, World\n";
	serial_port_write_char(hw);
	idt_install();
	intr_install();
	
	interrupt_int(0);
	interrupt_int(1);
	interrupt_int(2);
	interrupt_int(3);
	interrupt_int(4);
	interrupt_int(5);
	interrupt_int(7);
	interrupt_int(8);
	interrupt_int(28);

	qemu_gdb_hang();

	while (1);
}

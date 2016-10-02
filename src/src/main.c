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

	qemu_gdb_hang();

	while (1);
}

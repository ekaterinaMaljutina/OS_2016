static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}


#include <ints.h>

//#include "interrupt.h"
#include "SerialPort.h"

void main(void)
{
    serial_port_init();
    char *hw = "Hello, World\n";
	serial_port_write_char(hw);

	//idt_install();

	qemu_gdb_hang();

	while (1);
}

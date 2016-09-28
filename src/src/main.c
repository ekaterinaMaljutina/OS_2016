#include "SerialPort.h"


static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <ints.h>

void main(void)
{
        serial_port_init();
        char const * hw = "Hello, World\n";
	serial_port_write_char(hw);

	qemu_gdb_hang();

	while (1);
}

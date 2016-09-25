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
        char hw[13] = "Hello, World\n";
	serial_port_write_string(hw,13);

	qemu_gdb_hang();

	while (1);
}

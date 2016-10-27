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
#include "pit.h"
#include "memmap.h"
#include "buddy_allocator.h"
#include "printf.h"
#include "slab_allocator.h"


#define interrupt_int(value)	__asm__ volatile("int %0" : : "n"(value))

void main(void)
{
    serial_port_init();
 //    char *hw = "Hello, World\n";
	// serial_port_write_char(hw);
	idt_install();
	intr_install();
	
//	pit_init();

	// interrupt_int(0);
//	timer(10);
	qemu_gdb_hang();
	
    init_memmap();
    buddy_init();

    for (int i=0 ; i<10; i++){
        uint64_t p1 = do_alloc(3);
        uint64_t p2 = do_alloc(1);
        printf("buddy_allocator 0x%llx and 0x%llx \n", p1, p2 );
        do_free(p2);
        do_free(p1);
    }
    printf("OK ... \n");

    create_slab(sizeof(int) * 10, 2);


	while (1);
}

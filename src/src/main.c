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

#define interrupt_int(value)	__asm__ volatile("int %0" : : "n"(value))

void main(void)
{
    serial_port_init();
    char *hw = "Hello, World\n";
	serial_port_write_char(hw);
	idt_install();
	intr_install();
	
//	pit_init();

	interrupt_int(0);
//	timer(10);

    init_memmap();
    /*init_buddy_allocator();
    for (int i = 0; i < 10; i++){
        void *p1 = allocate_empty_page(0);
        void *p2 = allocate_empty_page(1);
        void *p3 = allocate_empty_page(3);
        void *p4 = allocate_empty_page(0);
        void *p5 = allocate_empty_page(0);
        printf("%p %p %p %p %p\n", p1,p2,p3, p4 ,  p5);
        free_page(p5, 0);
        free_page(p4, 0);
        free_page(p3, 3);
        free_page(p2, 1);
        free_page(p1, 0);
    }*/

	qemu_gdb_hang();

	while (1);
}

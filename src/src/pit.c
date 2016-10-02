#include "pit.h"
#include "ioport.h"
#include "SerialPort.h"
#include "interrupt.h"

void pit_init()
{
   // Прежде всего регистрируем обратный вызов (callback)
   // нашего таймера.	
	irq_set_handler(0, &timer_callback);

} 
void timer_callback(struct registers_t const * reg){

	serial_port_write_char("tick: .. \n");
   (void)reg;
}

void timer(int frequency){
	// Значение, которое мы посылаем в PIT, является значением, на которое будет делиться основная частота
   // (1193180 Hz) для того, чтобы получить необходимую нам частоту. Важно отметить, что делитель должен
   // быть достаточно маленьким с тем, чтобы уместиться в 16 битов разрядной сетки.
   uint64_t divisor = 1193180 / frequency;

   // Посылаем байт команды.
   out8(0x43, 0x36);

   // Делитель должен посылаться побайтно, так что мы делим его на старший и младший байты.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Посылаем делитель частоты.
   out8(0x40, l);
   out8(0x40, h);
}
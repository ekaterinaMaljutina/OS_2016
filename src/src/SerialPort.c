
#include "SerialPort.h"
#include "../inc/ioport.h"

static int has_data() {
	return in8(0x3FD) & (1 << 5); // +5 порт можно ли передавать данные
}

void serial_port_init() {
	out8(0x3F9, 0x00); // отключить прерувание +1 = 0
	out8(0x3FB, 0x80); // 1000 0000 запишем правильное значения коэффициэнта деления в порт +3 с установленным 7 битом
	out8(0x3F8, 0x0C); // 1100 младший байт коэффициэнта деления (скорость)
	out8(0x3F9, 0x00); // старший байт коэф деления
	out8(0x3FB, 0x03); 
	out8(0x3FA, 0xC7); 
}

char serial_port_read() {
	while (!has_data())
		;
	return in8(0x3F8);
}

void serial_port_write(char value) {
	while (!has_data())
		;
	out8(0x3F8,value);
}

void serial_port_write_string(char* value, uint32_t size){

	for (uint32_t i=0; i<size ; i++){
		serial_port_write(value[i]);

	}
	
}


#include "SerialPort.h"
#include "ioport.h"

static int has_data() {
	return in8(0x3FD) & (1 << 5); // +5 порт можно ли передавать данные
}

void serial_port_init() {

	out8(0x3F8+3, 0x00);// отключить прерывание +1 = 0
	out8(0x3F8+1, 0x00); 


	out8(0x3F8+3, 0x80); // 1000 0000 запишем правильное значения коэффициэнта деления в порт +3 с установленным 7 битом
	out8(0x3F8 + 0, 0x0C); // 1100 младший байт коэффициэнта деления (скорость)	
	
	out8(0x3F+3, 0x3); // 8 бит на кадр
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


void serial_port_write_char(char* value){
	uint32_t i=0;
	while (value[i]){
		serial_port_write(value[i]);
		i++;
	}	
}


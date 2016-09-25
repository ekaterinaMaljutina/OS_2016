#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <stdint.h>

void serial_port_init();
char serial_port_read();

void serial_port_write(char value);
void serial_port_write_string(char* value, uint32_t size);

#endif // __SERIALPORT_H_

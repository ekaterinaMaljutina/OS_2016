#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <stdint.h>

void serial_port_init();

void serial_port_write(char const value);
void serial_port_write_char(char const * value);
void serial_port_write_string(char const * value, uint32_t size);

#endif // __SERIALPORT_H_

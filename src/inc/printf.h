#ifndef _PRINTF_H
#define _PRINTF_H

#include <stdarg.h>
#include <stddef.h>

int printf (const char* fmt, ...);
int vprintf(const char* fmt, va_list args);

int snprintf (char* buffer, size_t buf_size, const char* fmt, ...);
int vsnprintf(char* buffer, size_t buf_size, const char* fmt, va_list args);

#endif

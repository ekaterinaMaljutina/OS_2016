#include <string.h>

size_t strlen(const char *str)
{
	const char *begin = str;

	while (*str++);
	return str - begin - 1;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;
	return dst;
}

void *memset(void *dst, int fill, size_t size)
{
	char *to = dst;

	while (size--)
		*to++ = fill;
	return dst;
}

int __strcmp(char const *s, char const *t, int len) {
    int i = 0;
    int size = strlen(s);
    for (; s[i] == t[i] && i<len; i++)
        if (s[i] == '\0')
            return 0;
    if (i == size )
    	return 0;
    else 
    	return 1;
}
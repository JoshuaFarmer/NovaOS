#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void *memset(void *ptr, int value, size_t num) {
	unsigned char *p = (unsigned char*)ptr;
	unsigned char v = (unsigned char)value;
	for (size_t i = 0; i < num; i++) {
		p[i] = v;
	}
	return ptr;
}

void* memcpy(void *dest, const void *src, size_t len) {
	char *d = (char*)dest;
	const char *s = (char*)src;
	while (len--)
		*d++ = *s++;
	return dest;
}

uint32_t memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *p1 = (unsigned char*)s1, *p2 = (unsigned char*)s2;
	while (n--) {
		if (*p1 != *p2)
			return *p1 - *p2;
		p1++, p2++;
	}
	return 0;
}
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "malloc.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	while (n) {
		if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0') {
			return *(unsigned char *)s1 - *(unsigned char *)s2;
		}
		s1++;
		s2++;
		n--;
	}
	return 0;
}

char* strpbrk(const char* s, const char* accept) {
	if (s == NULL || accept == NULL)
		return NULL;

	while (*s != '\0') {
		const char* p = accept;
		while (*p != '\0') {
			if (*s == *p)
				return (char*)s;
			p++;
		}
		s++;
	}

	return NULL;
}

char* strtok(char* str, const char* delim) {
	static char* token = NULL;
	if (str != NULL) {
		token = str;
	} else if (token == NULL) {
		return NULL;
	}

	char* start = token;
	char* end = strpbrk(token, delim);
	if (end != NULL) {
		*end = '\0';
		token = end + 1;
	} else {
		token = NULL;
	}

	return start;
}

char* strcpy(register char *to, register const char *from) {
	char *save = to;
	for (int k = 0; from[k] != '\0'; ++k) to[k] = from[k];
	return(save);
}

char* strdup(const char* s) {
	if (s == NULL)
		return NULL;

	size_t len = strlen(s);
	char* dup = (char*)malloc(len + 1); // Allocate memory for the duplicated string
	if (dup == NULL)
		return NULL;

	strcpy(dup, s); // Copy the contents of the original string into the new string
	return dup;
}
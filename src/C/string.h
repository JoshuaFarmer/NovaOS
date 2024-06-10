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

char *strncpy(char *dest, const char *src, size_t n) {
	size_t i;

	// Copy up to n characters from src to dest
	for (i = 0; i < n && src[i] != '\0'; i++) {
		dest[i] = src[i];
	}

	// If src has fewer than n characters, pad the rest with '\0'
	for (; i < n; i++) {
		dest[i] = '\0';
	}

	return dest;
}

size_t numlen(const char* str) {
	size_t i = 0;
	while (*str >= '0' && *str <= '9') {
		str++; i++;
	}
	return i;
}

int atoi(const char* str) {
	int result = 0;
	int sign = 1;

	// Handle leading whitespace
	while (*str == ' ' || *str == '\t') {
		str++;
	}

	// Handle sign
	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	// Convert digits to integer
	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}

	// Apply sign
	return result * sign;
}

char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return (char *)str; // Found the character, return its address
        }
        str++; // Move to the next character in the string
    }
    if (c == '\0') {
        return (char *)str; // Return the null terminator if c is '\0'
    }
    return NULL; // If the character is not found, return NULL
}
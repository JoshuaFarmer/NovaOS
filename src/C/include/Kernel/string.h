#pragma once
#include "typedef.h"
#include "malloc.h"

size_t strlen(const char* str);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char* strpbrk(const char* s, const char* accept);
char* strtok(char* str, const char* delim);
char* strcpy(register char *to, register const char *from);
char* strdup(const char* s);
char *strncpy(char *dest, const char *src, size_t n);
size_t numlen(const char* str);
int atoi(const char* str);
char *strchr(const char *str, int c);
size_t wstrlen(const wchar_t* str);
void wstrcpy(wchar_t* dst, const wchar_t* src);
void memset(void* des, int value, size_t size);
void memcpy(void* des, const void* src, size_t size);
int memcmp(const void *str1, const void *str2, size_t n);
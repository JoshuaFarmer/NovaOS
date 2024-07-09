#pragma once
#include "typedef.h"

void memset(void* des, int value, size_t size) {
	if (des == NULL) return;
	for (size_t x=0; x<size; ++x)
		((char*)des)[x]=value;
}

void memcpy(void* des, void* src, size_t size) {
	if (des == NULL || src == NULL) return;
	for (size_t x=0; x<size; ++x)
		((char*)des)[x]=((char*)src)[x];
}
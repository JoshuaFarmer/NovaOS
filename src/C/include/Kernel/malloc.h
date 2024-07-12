#pragma once
#include "typedef.h"
#include "Kernel/string.h"

#define HEAP_CAP (1024 * 120) // 120k should be enough for everyone, right?

typedef struct {
	uint8_t  raw[HEAP_CAP];
	uint16_t next_free;
	uint16_t len[HEAP_CAP];
} Heap_t;

void init_heap();
void* malloc(size_t size);
void free(void* ptr);
size_t remaining_heap_space();
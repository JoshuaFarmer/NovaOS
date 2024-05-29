#pragma once
#include <stdint.h>
#include "memory.h"

// 720kb should be enough.
#define HEAP_CAP 720000

void puts_coloured(const char* data, const uint8_t col);

typedef struct HEAP {
	uint8_t  raw[HEAP_CAP];
	bool is_free[HEAP_CAP];
	uint32_t len[HEAP_CAP];
} Heap_t;

Heap_t heap;

void init_heap() {
	for (int i = 0; i < HEAP_CAP; ++i) {
		heap.is_free[i] = true;
		heap.len[i] = 0;
	}
}

// VERY BAD - but fuck it
void* malloc(size_t size) {
	// O(N) but fuck it
	for (size_t i = 0; i < HEAP_CAP; ++i) {
		if (heap.is_free[i] == true) {
			for (size_t k = 0; k < size; ++k) {
				if (k > HEAP_CAP) break;
			}

			heap.len[i] = size;
			memset(&heap.is_free[i], false, size);

			return &heap.raw[i];
		}
	}
	puts_coloured("MALLOC - NO SPACE IN HEAP\n", VGA_COLOR_LIGHT_RED);
	return NULL;
}

// VERY BAD - but fuck it
void free(void* ptr) {
	// O(N) but fuck it
	for (int i = 0; i < HEAP_CAP; ++i) {
		if (&heap.raw[i] == ptr) {
			memset(&heap.is_free[i], true, heap.len[i]);
			return;
		}
	}
	puts_coloured("FREE - NOT FOUND IN THE HEAP\n", VGA_COLOR_LIGHT_RED);
}
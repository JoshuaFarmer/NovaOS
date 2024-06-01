#pragma once
#include <stdint.h>
#include "memory.h"

#define HEAP_CAP 512000 // 2^9 * 1000
//#define HEAP_CAP 524288 // 2^19

void puts_coloured(const char* data, const uint8_t col);

typedef struct HEAP {
	uint8_t  raw[HEAP_CAP];
	uint8_t  is_free[HEAP_CAP];
	uint16_t len[HEAP_CAP];
} Heap_t;

Heap_t heap;

void init_heap() {
    memset(heap.is_free, true, HEAP_CAP);
    memset(heap.len, 0, sizeof(heap.len));
}

// VERY BAD - but fuck it
void* malloc(size_t size) {
	if (size == 0 || size > HEAP_CAP) {
        puts_coloured("MALLOC - INVALID SIZE\n", VGA_COLOR_LIGHT_RED);
        return NULL;
    }

    for (size_t i = 0; i < HEAP_CAP; ++i) {
        if (heap.is_free[i]) {
            size_t free_block_size = 0;

            for (size_t j = i; j < HEAP_CAP && heap.is_free[j]; ++j) {
                ++free_block_size;
                if (free_block_size == size) {
                    for (size_t k = i; k < i + size; ++k) {
                        heap.is_free[k] = false;
                    }
                    heap.len[i] = size;
                    return &heap.raw[i];
                }
            }
            i += free_block_size;
        }
    }
    puts_coloured("MALLOC - NO SPACE IN HEAP\n", VGA_COLOR_LIGHT_RED);
    return NULL;
}

// VERY BAD - but fuck it
void free(void* ptr) {
	if (ptr == NULL || ptr < (void*)heap.raw || ptr >= (void*)(heap.raw + HEAP_CAP)) {
        puts_coloured("FREE - INVALID POINTER\n", VGA_COLOR_LIGHT_RED);
        return;
    }

    size_t offset = (uint8_t*)ptr - heap.raw;
    size_t length = heap.len[offset];

    if (length == 0) {
        puts_coloured("FREE - NOT FOUND IN THE HEAP\n", VGA_COLOR_LIGHT_RED);
        return;
    }

    memset(&heap.is_free[offset], true, length);
    heap.len[offset] = 0;
}

size_t remaining_heap_space() {
    size_t remain = 0;
    for (int i = 0; i < HEAP_CAP; ++i) {
        if (heap.is_free[i]) {
            ++remain;
        }
    }
    return remain;
}
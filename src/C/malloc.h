#pragma once
#include <stdint.h>
#include "memory.h"

#define HEAP_CAP (1024 * 120) // 120k should be enough for everyone, right?

void puts_coloured(const char* data, const uint8_t col);

typedef struct {
    uint8_t  raw[HEAP_CAP];
    uint16_t next_free;
    uint16_t len[HEAP_CAP];
} Heap_t;

Heap_t heap = { .next_free = 0 };

void init_heap() {
    memset(heap.len, 0, sizeof(heap.len));
}

void* malloc(size_t size) {
    if (size == 0 || size > HEAP_CAP) {
        puts_coloured("MALLOC - INVALID SIZE\n", VGA_COLOR_LIGHT_RED);
        return NULL;
    }

    size_t i = heap.next_free;
    while (i < HEAP_CAP) {
        if (heap.len[i] == 0) { // Free block
            size_t free_block_size = 0;
            for (size_t j = i; j < HEAP_CAP && heap.len[j] == 0; ++j) {
                ++free_block_size;
                if (free_block_size == size) {
                    heap.len[i] = size;
                    heap.next_free = i + size;
                    return &heap.raw[i];
                }
            }
        }
        i += heap.len[i] ? heap.len[i] : 1;
    }
    puts_coloured("MALLOC - NO SPACE IN HEAP\n", VGA_COLOR_LIGHT_RED);
    return NULL;
}

void free(void* ptr) {
    if (ptr == NULL || ptr < (void*)heap.raw || ptr >= (void*)(heap.raw + HEAP_CAP)) {
        puts_coloured("FREE - INVALID POINTER\n", VGA_COLOR_LIGHT_RED);
        return;
    }

    size_t offset = (uint8_t*)ptr - heap.raw;
    if (heap.len[offset] == 0) {
        puts_coloured("FREE - NOT FOUND IN THE HEAP\n", VGA_COLOR_LIGHT_RED);
        return;
    }

    heap.next_free = offset < heap.next_free ? offset : heap.next_free;
    memset(&heap.raw[offset], 0, heap.len[offset]);
    heap.len[offset] = 0;
}

size_t remaining_heap_space() {
    size_t taken = HEAP_CAP;
    for (size_t i = 0; i < HEAP_CAP; i += heap.len[i] ? heap.len[i] : 1) {
        if (heap.len[i] == 0) {
            taken -= 1; // Only count the starting byte of a free block
        }
    }
    return HEAP_CAP - taken;
}

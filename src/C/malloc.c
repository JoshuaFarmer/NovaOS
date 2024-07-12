#include "Kernel/malloc.h"

Heap_t heap = { .next_free = 0 };

void init_heap() {
	memset(heap.len, 0, sizeof(heap.len));
}

void* malloc(size_t size) {
	if (size == 0 || size > HEAP_CAP) {
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
	return NULL;
}

void free(void* ptr) {
	if (ptr == NULL || ptr < (void*)heap.raw || ptr >= (void*)(heap.raw + HEAP_CAP)) {
		return;
	}

	size_t offset = (uint8_t*)ptr - heap.raw;
	if (heap.len[offset] == 0) {
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

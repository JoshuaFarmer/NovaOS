#pragma once
#include <stdint.h>
#include "typedef.h"

void* heap_start = NULL;
void init_heap(size_t initial_size) {
	heap_start = malloc(initial_size);
	if (heap_start != NULL) {
		((block_t*)heap_start)->size = initial_size - sizeof(block_t);
		((block_t*)heap_start)->next = NULL;
		((block_t*)heap_start)->free = 1;
	}
}

void* malloc(size_t size) {
	block_t* curr = (block_t*)heap_start;
	block_t* best_fit = NULL;

	// Find the first fit block that can accommodate the requested size
	while (curr != NULL) {
		if (curr->free && curr->size >= size && (best_fit == NULL || curr->size < best_fit->size)) {
			best_fit = curr;
		}
		curr = curr->next;
	}

	// If no suitable block is found, return NULL
	if (best_fit == NULL) {
		return NULL;
	}

	// If the block is larger than needed, split it
	if (best_fit->size > size + sizeof(block_t)) {
		block_t* new_block = (block_t*)((char*)best_fit + sizeof(block_t) + size);
		new_block->size = best_fit->size - size - sizeof(block_t);
		new_block->next = best_fit->next;
		new_block->free = 1;
		best_fit->size = size;
		best_fit->next = new_block;
	}

	// Mark the block as allocated and return a pointer to the user
	best_fit->free = 0;
	return (void*)(best_fit + 1);
}

void free(void* ptr) {
	if (ptr == NULL) {
		return;
	}

	// Get a pointer to the block header
	block_t* block = (block_t*)ptr - 1;

	// Mark the block as free
	block->free = 1;

	// Coalesce adjacent free blocks
	block_t* curr = (block_t*)heap_start;
	while (curr != NULL && curr->next != NULL) {
		if (curr->free && curr->next->free) {
			curr->size += sizeof(block_t) + curr->next->size;
			curr->next = curr->next->next;
		}
		curr = curr->next;
	}
}
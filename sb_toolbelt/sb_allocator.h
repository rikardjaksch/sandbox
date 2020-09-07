#pragma once

#include <stddef.h>

typedef struct sb_allocation_result {
	void* p;
	size_t size;
} sb_allocation_result_t;

static inline sb_allocation_result_t allocate_result(void* p, size_t size)
{
	sb_allocation_result_t ar = { 0 };

	ar.p = p;
	ar.size = size;

	return ar;
}

typedef struct sb_allocator_o sb_allocator_o;

typedef struct sb_allocator_i {
	sb_allocator_o* inst;

	void* (*allocate)(sb_allocator_o* inst, size_t size);
	sb_allocation_result_t (*allocate_with_result)(sb_allocator_o* inst, size_t size);
	void (*deallocate)(sb_allocator_o* inst, void* p);
} sb_allocator_i;

// Create a page allocator
sb_allocator_i* sb_allocator_create_page_allocator();
// Destroy a page allocator
void sb_allocator_destroy_page_allocator(sb_allocator_i* allocator);
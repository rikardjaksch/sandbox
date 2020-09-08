#pragma once

#include <stddef.h>

/*
#include <stdint.h>

typedef struct sb_allocator sb_allocator;

typedef struct sb_alloc_result {
	void* ptr;
	uint32_t size;
} sb_alloc_result_t;

sb_alloc_result sb_allocate(sb_allocator* allocator, uint32_t size);
void sb_deallocate(sb_allocator* allocator, void* ptr);

#define sb_alloc(a, sz) sb_allocate(a, sz).ptr
#define sb_free(a, ptr) sb_deallocate(a, ptr)
#define sb_alloc_obj(a, type) (type*)sb_allocate(a, sizeof(type)).ptr
#define sb_alloc_array(a, type, sz) (type*)sb_allocate(a, sizeof(type) * sz).ptr

struct sb_allocator_api
{
	void (*initialize)();
	void (*shutdown)();

	sb_allocator* system;
	sb_allocator* page;

	sb_allocator* (*create_slot_allocator)(sb_allocator* backing);
	sb_allocator* (*create_heap_allocator)(sb_allocator* backing);
	sb_allocator* (*create_frame_allocator)(sb_allocator* backing);
};

extern sb_allocator_api* sb_allocator_api;

*/

typedef struct sb_allocation_result {
	void* p;
	size_t size;
} sb_allocation_result_t;

static inline sb_allocation_result_t allocate_result(void* p, size_t size)
{
	sb_allocation_result_t ar = { 0 };

	ar.p = p;
	ar.size = size;

	// TODO: If debug-build we could set a specific bit-pattern
	//		 here on the memory for un-initialized memory

	return ar;
}

typedef struct sb_allocator_o sb_allocator_o;

// Interface for an allocator
typedef struct sb_allocator_i {
	sb_allocator_o* inst;

	// Allocate 'size' amount of memory and get the point back
	void* (*allocate)(sb_allocator_o* inst, size_t size);
	// Allocate size amount of memory and get an sb_allocation_result back
	sb_allocation_result_t (*allocate_with_result)(sb_allocator_o* inst, size_t size);
	// Deallocate memory previosly allocated by this allocater
	void (*deallocate)(sb_allocator_o* inst, void* p);
	// Get the size of a specific allocation
	size_t (*allocated_size)(void* ptr);
} sb_allocator_i;

#define sb_alloc(a, sz) a->allocate(a->inst, sz)
#define sb_dealloc(a, p) a->deallocate(a->inst, p)

// Get the singleton system allocator (will initialize it the first time it's called)
sb_allocator_i* sb_allocator_get_system_allocator();
// Checks to see if the system allocator still holds allocations
void sb_allocator_destroy_system_allocator();

// Create a page allocator
sb_allocator_i* sb_allocator_create_page_allocator();
// Destroy a page allocator
void sb_allocator_destroy_page_allocator(sb_allocator_i* allocator);
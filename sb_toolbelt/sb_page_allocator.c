#include "sb_allocator.h"

#include <malloc.h>
#include <assert.h>

#include <Windows.h>

size_t get_page_size();
void* allocate_pages(size_t size);

typedef struct page_allocator {
	size_t page_size, total;
} page_allocator;

sb_allocation_result_t page_allocator_allocate_with_result(sb_allocator_o* inst, size_t size)
{
	// 1) Round up to nearest page-size (shouldn't be needed on Win at least)
	// ...

	// 2) Allogate page memory using OS functions for the rounded up size
	void* p = allocate_pages(size);
	assert(p);
	
	// 4) Store how much memory we allocated
	// ...

	// 5) Return the result
	return allocate_result(p, size);
}

void* page_allocator_allocate(sb_allocator_o* inst, size_t size)
{
	return page_allocator_allocate_with_result(inst, size).p;
}

void page_allocator_deallocate(sb_allocator_o* inst, void* p)
{

}

sb_allocator_i* sb_allocator_create_page_allocator()
{
	sb_allocator_i* pg_allocator = (sb_allocator_i*)malloc(sizeof(sb_allocator_i));
	assert(pg_allocator);

	pg_allocator->allocate = page_allocator_allocate;
	pg_allocator->allocate_with_result = page_allocator_allocate_with_result;
	pg_allocator->deallocate = page_allocator_deallocate;

	page_allocator* pg_inst = (page_allocator*)malloc(sizeof(page_allocator));
	assert(pg_inst);

	pg_inst->total = 0;
	pg_inst->page_size = get_page_size();

	pg_allocator->inst = (sb_allocator_o*)pg_inst;
	return pg_allocator;
}

void sb_allocator_destroy_page_allocator(sb_allocator_i* allocator)
{

}

size_t get_page_size()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	DWORD granularity = si.dwAllocationGranularity;
	if (si.dwPageSize > granularity)
		granularity = si.dwPageSize;
	return granularity;
}

void* allocate_pages(size_t size)
{
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
}
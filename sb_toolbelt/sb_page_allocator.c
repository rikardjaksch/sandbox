#include "sb_allocator.h"

#include <assert.h>

#include <Windows.h>

static size_t round_up_page_size(size_t size, size_t page_size)
{
	return ((size + page_size - 1) / page_size) * page_size;
}

// Platform specific utilties
size_t get_page_size();
void* allocate_pages(size_t size);
size_t allocated_size(void* ptr);
void deallocate_pages(void* ptr);

typedef struct page_allocator {
	size_t page_size, total;
} page_allocator;

size_t page_allocator_allocated_size(void* ptr)
{
	return allocated_size(ptr);
}

sb_allocation_result_t page_allocator_allocate_with_result(sb_allocator_o* inst, size_t size)
{
	page_allocator* pa = (page_allocator*)inst;

	// 1) Round up to nearest page-size
	size_t vm_size = round_up_page_size(size, pa->page_size);

	// 2) Allogate page memory using OS functions for the rounded up size
	void* p = allocate_pages(vm_size);
	assert(p);
		
	// 4) Store how much memory we allocated
	size_t s = allocated_size(p);
	pa->total += s;

	// 5) Return the result
	return allocate_result(p, s);
}

void* page_allocator_allocate(sb_allocator_o* inst, size_t size)
{
	return page_allocator_allocate_with_result(inst, size).p;
}

void page_allocator_deallocate(sb_allocator_o* inst, void* p)
{
	if (!p)
		return;

	page_allocator* pa = (page_allocator*)inst;

	size_t s = allocated_size(p);
	pa->total -= s;

	deallocate_pages(p);
}

sb_allocator_i* sb_allocator_create_page_allocator()
{
	sb_allocator_i* system = sb_allocator_get_system_allocator();	
	sb_allocator_i* pg_allocator = (sb_allocator_i*)sb_alloc(system, sizeof(sb_allocator_i));
	assert(pg_allocator);

	pg_allocator->allocate = page_allocator_allocate;
	pg_allocator->allocate_with_result = page_allocator_allocate_with_result;
	pg_allocator->deallocate = page_allocator_deallocate;
	pg_allocator->allocated_size = page_allocator_allocated_size;

	page_allocator* pg_inst = (page_allocator*)sb_alloc(system, sizeof(page_allocator));
	assert(pg_inst);

	pg_inst->total = 0;
	pg_inst->page_size = get_page_size();

	pg_allocator->inst = (sb_allocator_o*)pg_inst;
	return pg_allocator;
}

void sb_allocator_destroy_page_allocator(sb_allocator_i* allocator)
{
	page_allocator* pa = (page_allocator*)allocator->inst;
	assert(pa->total == 0);

	sb_allocator_i* system = sb_allocator_get_system_allocator();
	sb_dealloc(system, pa);
	sb_dealloc(system, allocator);
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

size_t allocated_size(void* ptr)
{
	MEMORY_BASIC_INFORMATION info;
	VirtualQuery(ptr, &info, sizeof(info));
	return info.RegionSize;
}

void deallocate_pages(void* ptr)
{
	BOOL res = VirtualFree(ptr, 0, MEM_RELEASE);
	assert(res);
}
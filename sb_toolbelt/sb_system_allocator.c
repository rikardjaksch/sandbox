#include "sb_allocator.h"
#include <malloc.h>
#include <assert.h>

static size_t total_allocated = 0;
sb_allocator_i global_system_allocator = { 0 };

size_t system_allocator_allocated_size(void* ptr)
{
	return _msize(ptr);
}

sb_allocation_result_t system_allocator_allocate_with_result(sb_allocator_o* inst, size_t size)
{
	(void)inst;
	void* p = malloc(size);
	size_t s = system_allocator_allocated_size(p);
	total_allocated += s;
	return allocate_result(p, s);
}

void* system_allocator_allocate(sb_allocator_o* inst, size_t size)
{
	return system_allocator_allocate_with_result(inst, size).p;
}

void system_allocator_deallocate(sb_allocator_o* inst, void* p)
{
	size_t s = system_allocator_allocated_size(p);
	total_allocated -= s;
	free(p);
}

void initialize_pointers()
{
	if (!global_system_allocator.allocate)
		global_system_allocator.allocate = system_allocator_allocate;

	if (!global_system_allocator.allocate_with_result)
		global_system_allocator.allocate_with_result = system_allocator_allocate_with_result;

	if (!global_system_allocator.deallocate)
		global_system_allocator.deallocate = system_allocator_deallocate;

	if (!global_system_allocator.allocated_size)
		global_system_allocator.allocated_size = system_allocator_allocated_size;
}

sb_allocator_i* sb_allocator_get_system_allocator()
{
	initialize_pointers();
	return &global_system_allocator;
}

void sb_allocator_destroy_system_allocator()
{
	assert(total_allocated == 0);
}
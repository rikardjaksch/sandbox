#include "sb_allocator.h"

int main()
{
	sb_allocator_i* pa = sb_allocator_create_page_allocator();

	int* p = sb_alloc(pa, sizeof(int) * 1234);
	sb_dealloc(pa, p);

	sb_allocator_destroy_page_allocator(pa);
	sb_allocator_destroy_system_allocator();
	return 0;
}
#include "sb_allocator.h"

int main()
{
	sb_allocator_i* pa = sb_allocator_create_page_allocator();

	sb_allocation_result_t res = pa->allocate_with_result(pa->inst, sizeof(int) * 1000);
	int* p = res.p;

	return 0;
}
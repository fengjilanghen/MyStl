#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <stdlib.h>

void* M_malloc(size_t size)
{
	return malloc(size);
}

void M_free(void* p)
{
	free(p);
}

class CAllocator
{
public:
	static char* Allocate(size_t size)
	{
		return (char*)M_malloc(size);
	}

	static void Deallocate(void* p, size_t old_size)
	{
		M_free(p);
	}
};


#endif //_ALLOCATOR_H_
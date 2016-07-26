#ifndef _MEMBASE_H_
#define _MEMBASE_H_

#include <new>
#include "MyAllocator.h"

typedef CMyAllocator  pool_alloc;

class CMemBase
{
public:
	void *operator new(size_t size) throw(std::bad_alloc)
	{
		char* pRet = pool_alloc::Allocate(size);
		if (pRet == 0)
		{
			throw "no memory is available";
		}
		return (void*)pRet;
	}

	void *operator new(size_t size, const std::nothrow_t& nothrow_value)
	{
		return (void*)pool_alloc::Allocate(size);
	}

	void operator delete(void* p, size_t size)
	{
		pool_alloc::Deallocate(p, size);
	}



	void *operator new[](size_t size) throw(std::bad_alloc)
	{
		char* pRet = pool_alloc::Allocate(size);
		if (pRet == 0)
		{
			throw "no memory is available";
		}
		return (void*)pRet;
	}

	void *operator new[](size_t size, const std::nothrow_t& nothrow_value)
	{
		return (void*)pool_alloc::Allocate(size);
	}

	void operator delete[](void* p, size_t size)
	{
		pool_alloc::Deallocate(p, size);
	}

};


#endif //_MEMBASE_H_
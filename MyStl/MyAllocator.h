#ifndef _MYALLOCATOR_H_
#define _MYALLOCATOR_H_

#include<string.h>
#include <cassert>
#include "Allocator.h"

typedef CAllocator malloc_alloc;

class CMyAllocator
{
private:
	enum {_ALIGN = 8, };
	enum {_MAX_SIZE = 128, };
	enum {_FREELIST_SIZE = 16, };

	union UObj
	{
		UObj* next;
		char client_data[1];
	};
	
private:
	static UObj*		free_list[_FREELIST_SIZE];
	static char*		start_free;
	static char*		end_free;
	static size_t		heap_size;	//附加内存

public:
	static size_t ROUND_UP(size_t size)
	{
		return (size + _ALIGN - 1) & ~(_ALIGN - 1);
	}

	static size_t FREELIST_INDEX(size_t size)
	{
		return size / _ALIGN - 1;
	}

	static char* Allocate(size_t size)
	{
		if (size > _MAX_SIZE)
		{
			return malloc_alloc::Allocate(size);
		}
		UObj** ppSlot = free_list + FREELIST_INDEX(size);
		if (*ppSlot == 0)
		{
			return refill(ROUND_UP(size));
		}
		char* result = (*ppSlot)->client_data;
		(*ppSlot) = (*ppSlot)->next;
		return result;
	}

	static char* Reallocate(char* pMem, size_t old_size, size_t new_size)
	{
		char* p = Allocate(new_size);
		if (p == NULL)
		{
			return NULL;
		}
		memcpy(p, pMem, old_size);
		Deallocate(pMem, old_size);
		return p;
	}

	static void Deallocate(void* pMem, size_t old_size)
	{
		int size = ROUND_UP(old_size);
		if (size > _MAX_SIZE)
		{
			malloc_alloc::Deallocate(pMem, size);
		}
		else
		{
			UObj** ppSlot = free_list + FREELIST_INDEX(size);
			if (*ppSlot == 0)
			{
				*ppSlot = reinterpret_cast<UObj*>(pMem);
			}
			else
			{
				UObj* q = reinterpret_cast<UObj*>(pMem);
				q->next = *ppSlot;
				(*ppSlot) = q;
			}
		}
	}

private:
	static char* refill(size_t size)
	{
		UObj** ppSlot = free_list + FREELIST_INDEX(size);
		size_t nobjs = 20;
		char* pRet = chunk_alloc(size, nobjs);
		if (nobjs == 0){return 0;}
		else if (nobjs == 1) { return pRet; }

		UObj* cur, *next;
		cur = next = (*ppSlot) = reinterpret_cast<UObj*>(pRet + size);
		for (int i = 1; i < nobjs; ++i)
		{
			next = reinterpret_cast<UObj*>((nobjs == i + 1) ? 0 : (next->client_data + size));
			cur->next = next;
			cur = next;
		}
		return pRet;
	}

	static char* chunk_alloc(size_t size, size_t& nobjs)
	{
		size_t total_size = nobjs * size;
		size_t free_left = end_free - start_free;
		
		if (free_left >= total_size)
		{
			char* ret = start_free;
			start_free = start_free + nobjs * size;
			return ret;
		}
		else if (free_left >= size)
		{
			nobjs = free_left / size;
			char* ret = start_free;
			start_free = start_free + nobjs * size;
			return ret;
		}
		else
		{
			if (free_left > 0)
			{
				UObj **ppSlot = free_list + FREELIST_INDEX(free_left);
				UObj * p = reinterpret_cast<UObj*>(start_free);
				p->next = *ppSlot;
				*ppSlot = p;
				start_free = end_free;
			}

			size_t bytes_to_get = 2 * size * nobjs + ROUND_UP(heap_size >> 4);
			start_free = (char*)malloc_alloc::Allocate(bytes_to_get);
			if (start_free == 0)
			{
				for (int i = size; i <= _MAX_SIZE; i += _ALIGN)
				{
					UObj ** p = free_list + FREELIST_INDEX(i);
					if (*p != 0)
					{
						start_free = (*p)->client_data;
						end_free += i;
						*p = (*p)->next;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				assert(0);
				return 0;
			}
			else
			{
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, nobjs);
			}
		}
	}

};

CMyAllocator::UObj*		CMyAllocator::free_list[_FREELIST_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
char*		CMyAllocator::start_free	= 0;
char*		CMyAllocator::end_free		= 0;
size_t		CMyAllocator::heap_size		= 0;	//附加内存


#endif //_MYALLOCATOR_H_
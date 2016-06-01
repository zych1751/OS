#include "vmem.h"
#include <cstdio>

VBlock::VBlock()
{
	valid = false;
	match = NULL;
}

Vmem::Vmem(int size, Pmem* mem, int p_id): size(size), p_id(p_id)
{
	a_id = 0;
	arr = new VBlock[size];
	for(int i = 0; i < size; i++)
	{
		arr[i].a_id = -1;
		arr[i].p_id = p_id;
	}
	pmem = mem;
}

void Vmem::allocate(int obj_size)
{
	VBlock* start;
	
	for(int i = 0; i <= size-obj_size; i++)
	{
		bool check = true;
		for(int j = i; j < i+obj_size; j++)
		{
			if(arr[j].valid)
			{
				check = false;
				i = j;
				break;
			}
		}
		if(check)
		{
			start = arr + i;
			break;
		}
	}

	pmem->allocate(start, obj_size, p_id, a_id);
	a_id++;
}

void Vmem::access(int a_id)
{
	for(int i = 0; i < size; i++)
	{
		if(arr[i].a_id == a_id)
		{
			if(!arr[i].valid)
			{
				if(arr[i].match->reverse != NULL)
					arr[i].match->reverse->valid = false;
				arr[i].valid = true;
			}
		}
	}
	pmem->update_LRU(p_id, a_id);
}

void Vmem::deallocate(int a_id) // LRU처리
{
	for(int i = 0; i < size; i++)
	{
		if(arr[i].a_id == a_id)
		{
			arr[i].valid = false;
			if(arr[i].match->reverse == (arr+i))
				arr[i].match->reverse = NULL;
			arr[i].match = NULL;
			arr[i].a_id = -1;
		}
	}
}

#include "vmem.h"
#include <cstdio>

VBlock::VBlock()
{
	valid = false;
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
	VBlock* start = NULL;

	for(int i = 0; i <= size-obj_size; i++)
	{
		bool check = true;
		for(int j = i; j < i+obj_size; j++)
		{
			if(arr[j].match != NULL)
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

	int t_id = pmem->allocate(start, obj_size, p_id, a_id);
	t_idx.push_back(make_pair(t_id, obj_size));
	a_id++;
}

void Vmem::access(int a_id1){
	bool valid = false;
	for(int i = 0; i < size; i++)
		if(arr[i].a_id == a_id1 && arr[i].valid)
		{
			valid = true;
			break;
		}

	if(!valid)
	{
		int t_id = pmem->allocate(arr+a_id1, t_idx[a_id1].second, p_id, a_id1);
		t_idx[a_id1].first = t_id;
	}
	else
	{
		pmem->update_LRU(p_id, a_id1, t_idx[a_id1].first);
	}
}

void Vmem::deallocate(int a_id1) // LRU처리
{
	for(int i = 0; i < size; i++)
	{
		if(arr[i].a_id == a_id1)
		{
			arr[i].valid = false;
			if(arr[i].match->reverse == (arr+i))
				arr[i].match->reverse = NULL;
			arr[i].match = NULL;
			arr[i].a_id = -1;
		}
	}
	pmem->pop_LRU(p_id, a_id1);
}

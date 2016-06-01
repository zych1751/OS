#include<cstdio>
#include "pmem.h"
#include "vmem.h"
#include<cstdlib>

Memory::Memory()
{
	reverse = NULL;
}

PBlock::PBlock()
{
	matched = false;
	child_selected = 0;
}

Pmem::Pmem(int size)
{
	total_size = size;
	arr = new PBlock[size*2]; // make tree;
	mem = new Memory[size];

	int arr_size = 1;
	for(int i = 1; i < size*2; i*=2)
		arr_size *= 2;
	arr_size /= 2;

	for(int i = 1; i < size*2; i*=2)
	{
		Memory* cur = mem;
		for(int j = i; j < i*2; j++)
		{
			arr[j].size = arr_size;
			arr[j].start = cur;
			cur += arr_size;
		}
		arr_size /= 2;
	}
}

int Pmem::find_empty(int size, int idx) // return PBlock idx , if not exit return -1
										// dfs
{
	if(arr[idx].size/2 < size && arr[idx].size >= size)
	{
		if(arr[idx].matched || arr[idx].child_selected != 0)	return -1;
		else	return idx;
	}
	if(arr[idx].matched)	return -1;

	int result = find_empty(size, idx*2);
	if(result != -1)	return result;
	result = find_empty(size, idx*2+1);
	return result;
}

void Pmem::update_LRU(int p_id, int a_id)
{
	if(LRU.size() == 0)	return;

	pair<pair<int,int>, int> selected;
	int idx;
	for(int i = 0; i < LRU.size(); i++)
	{
		if(LRU[i].first.first == p_id && LRU[i].first.second == a_id)
		{
			selected = LRU[i];
			idx = i;
			break;
		}
	}
	LRU.erase(LRU.begin()+idx);
	LRU.push_back(selected);
}

void Pmem::pop_LRU()
{
	if(LRU.size() != 0)
	{
		deallocate(LRU[0].second);
		LRU.erase(LRU.begin());
	}
}

void Pmem::deallocate(int p_id)
{
	for(int i = 0; i < arr[p_id].size; i++)
	{
		VBlock* cur = (arr[p_id].start+i)->reverse;
		if(cur == NULL)	break;
		(arr[p_id].start+i)->reverse = NULL;
		cur->valid = false;
	}
	arr[p_id].matched = false;
	
	while(p_id != 0)
	{
		arr[p_id].child_selected++;
		p_id /= 2;
	}
}

void Pmem::allocate(VBlock* vmem, int size, int p_id, int a_id)
{
	while(1)
	{
		int idx = find_empty(size, 1);

		if(idx == -1)
		{
			printf("%d %d : %d %d\n", p_id, a_id, LRU[0].first.first, LRU[1].first.second);
			pop_LRU();
			continue;
		}
	
		printf("selected idx : %d\n", idx);
		arr[idx].matched = true;
		int temp = idx;
		while(temp != 0)
		{
			arr[temp].child_selected--;
			temp /= 2;
		}

		for(int i = 0; i < size; i++)
		{
			((arr[idx].start)+i)->reverse = (vmem+i);
			(vmem+i)->a_id = a_id;
			(vmem+i)->match = ((arr[idx].start)+i);
			(vmem+i)->valid = true;
		}
		LRU.push_back(make_pair(make_pair(p_id, a_id), idx));
		return;
	}
}

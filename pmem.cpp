#include "pmem.h"

Pmem::Pmem(int size)
{
	total_size = size;
	arr = new PBlock[size*2]; // make tree;
	mem = new Memory[size];

	int arr_size = 1;
	for(int i = 1; i < size*2; i*=2)
		arr_size *= 2;

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
	if(arr[idx].size >= size && arr[idx].size < size*2)
	{
		if(arr[idx].matched)	return -1;
		else	return idx;
	}

	int result = find_empty(size, idx*2);
	if(result != -1)	return result;
	result = find_empty(size, idx*2+1);
	return result;
}

void Pmem::allocate(VBlock* vmem, int size)
{
	while(1)
	{
		int idx = find_empty(size, 1);

		
		for(int i = 0; i < size; i++)
		{
			((arr[idx].start)+i)->reverse = (vmem+i);
			(vmem+i)->match = ((arr[idx].start)+i);
			(vmem+i)->valid;
		}
	}
}



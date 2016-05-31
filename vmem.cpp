#include "vmem.h"
#include "pmem.h"

Vmem::Vmem(int size, Pmem* mem)
{
	arr = new VBolck[size];
	for(int i = 0; i < size; i++)
		arr[i].a_id = -1;
	pmem = mem;
}

void Vmem::allocate(int size)
{
	Vblock* start;

	//calcuate
	pmem->allocate(start, size);
}

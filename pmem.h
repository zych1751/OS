#include "vmem.h"

class Pmem
{
public:
	Pmem(int size);
	int find_empty(int size); // return idx
	void allocate(VBlock* vmem, int size);

	int total_size;
	PBlock* arr;
	Memory* mem;
};

class PBlock
{
public:
	PBlock()
	{
		matched = false;
	}
	Memory* start;
	bool matched;
	int size;
};

class Memory // physical_memory
{
public:
	Vblock* reverse;

	Memory()
	{
		reverse = NULL;
	}
};

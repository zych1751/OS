#ifndef VMEM_H_
#define VMEM_H_
#include "pmem.h"

class Memory;
class Pmem;

class VBlock
{
public:
	VBlock();
	bool valid;
	Memory* match;
	int a_id, p_id;
};

class Vmem
{
public:
	int a_id;
	VBlock* arr;
	Pmem* pmem;
	int size;
	int p_id;

	Vmem(int size, Pmem* mem, int p_id);
	void allocate(int obj_size);
	void access(int a_id);
	void deallocate(int a_id);
};

#endif

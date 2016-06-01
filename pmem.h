#ifndef PMEM_H_
#define PMEM_H_
#include "vmem.h"
#include <vector>

using namespace std;

class VBlock;

class Memory // physical_memory
{
public:
	VBlock* reverse;

	Memory();
};

class PBlock
{
public:
	PBlock();
	Memory* start;
	bool matched;
	int size;
	int child_selected;
};

class Pmem
{
public:
	Pmem(int size);
	int find_empty(int size, int idx); // return idx
	void pop_LRU();
	void allocate(VBlock* vmem, int size, int p_id, int a_id);
	void deallocate(int p_id);	//PBlock idx
	void update_LRU(int p_id, int a_id);

	int total_size;
	PBlock* arr;
	Memory* mem;
	vector<pair<pair<int, int>, int> > LRU; // <P_id, a_id>, tree_idx
};

#endif

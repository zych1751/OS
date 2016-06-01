#ifndef PMEM_H_
#define PMEM_H_
#include "vmem.h"
#include <vector>
#include <cstdio>

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
	void pop_LRU(int t_idx);
	void pop_LRU(int p_id, int a_id);
	int allocate(VBlock* vmem, int size, int p_id, int a_id); // return PBlock idx
	void deallocate(int p_id);	//PBlock idx
	void update_LRU(int p_id, int a_id, int tree_idx);
	int allocate2(VBlock* vmem, int p_id, int a_id, int idx);
	void clear(int idx);
	void print(FILE* out, int idx);

	int total_size;
	PBlock* arr;
	Memory* mem;
	vector<pair<pair<int, int>, int> > LRU; // <P_id, a_id>, tree_idx
};

#endif

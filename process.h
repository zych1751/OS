#ifndef PROCESS_H_
#define PROCESS_H_

#include <vector>
#include <string>
#include <utility>
#include <cstdio>
#include "vmem.h"
using namespace std;

class Process
{
public:
	string name;
	int p_id;
	int time;
	vector<pair<int, int>> code_array;
	int code_idx;
	int code_length;
	int sleep_time;
	FILE* f;
	Vmem* vmem;
	int virtual_page_num;

	Process(int p_id, string name, int feed_size, int virtual_page_num, Pmem* mem);
	int do_process();
};

#endif

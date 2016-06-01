#include "process.h"

Process::Process(int p_id, string name, int feed_size, int virtual_page_num, Pmem* mem):
	p_id(p_id), name(name), time(feed_size), virtual_page_num(virtual_page_num)
{
	vmem = new Vmem(virtual_page_num, mem, p_id);
	f = fopen(name.c_str(), "r");
	fscanf(f, "%d", &code_length);
	code_idx = 0;
	for(int i = 0; i < code_length; i++)
	{
		pair<int,int> p;
		fscanf(f, "%d %d", &p.first, &p.second);
		code_array.push_back(p);	
	}
}

int Process::do_process()
{
	int op = code_array[code_idx].first, arg = code_array[code_idx].second;
	code_idx++;
	time--;

	switch(op)
	{
		case 0:
			vmem->allocate(arg);
			break;
		case 1:
			vmem->access(arg);
			break;
		case 2:
			vmem->deallocate(arg);
			break;
		case 3:
			break;
		case 4:
			sleep_time = arg;
			break;
		case 5:
			break;
	}

	if(op <= 3)
	{
		//print
	}

	return op;
}

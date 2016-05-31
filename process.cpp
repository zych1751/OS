#include "process.h"

Process::Process(int p_id, string name, int feed_size, int page_num):
	p_id(p_id), name(name), time(feed_size), page_num(page_num)
{
	f = fopen(name.c_str(), "w+");
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
	return 0;
}

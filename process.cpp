#include "process.h"

Process::Process(int p_id, string name, int feed_size, int virtual_page_num, Pmem* mem):
	p_id(p_id), name(name), time(feed_size), virtual_page_num(virtual_page_num)
{
	vmem = new Vmem(virtual_page_num, mem, p_id);
	f = fopen(name.c_str(), "r");
	out = fopen((to_string(p_id)+".txt").c_str(), "w");
	string str = to_string(p_id);

	fscanf(f, "%d", &code_length);
	code_idx = 0;
	for(int i = 0; i < code_length; i++)
	{
		pair<int,int> p;
		fscanf(f, "%d %d", &p.first, &p.second);
		code_array.push_back(p);	
	}
}

void Process::do_print(int cycle)
{
	fprintf(out, "%d Cycle#Instruction op %d arg %d\n", cycle, code_array[code_idx-1].first, code_array[code_idx-1].second);

	fprintf(out, "AllocID");
	int cnt = 0;
	for(int i = 0; i < virtual_page_num; i++)
	{
		if(cnt == 0)
		{
			fprintf(out, "|");
			cnt = 4;
		}

		if(vmem->arr[i].match != NULL)
			fprintf(out, "%d", vmem->arr[i].a_id);
		else
			fprintf(out, "-");
		cnt--;
	}

	fprintf(out, "|\nValid  ");
	cnt = 0;
	for(int i = 0; i < virtual_page_num; i++)
	{
		if(cnt == 0)
		{
			fprintf(out, "|");
			cnt = 4;
		}

		fprintf(out, "%d", vmem->arr[i].valid);
		cnt--;
	}
	fprintf(out, "|\n\n");
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

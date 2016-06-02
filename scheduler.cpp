#include <cstdio>
#include "process.h"
#include "pmem.h"
#include "vmem.h"
#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

class Scheduler
{
public:
	Scheduler();
	deque<Process*> q;
	Process* cur;
	vector<Process*> sleep;
	vector<Process*> IO;
	Pmem* pmem;
	FILE* scheduler;
	FILE* system;
	
	int remain_time; // if remain_time == 0, give time_quantum
	int max_remain; // 프로세스에 사이클을 주는 주기

	int give_time; // 모든 프로세스에 주는 사이클 수
	
	int v_mem, page_size;
	int process_num;

	int process_time;
	int max_process;
	int p_id;
	int page_num;

	void give_time_quantum(int time);
	void process();
	void make_process(string name);
	void IO_pop(int p_id);

	int cur_cycle;
	vector<string> input;
};

Scheduler::Scheduler()
{
	scheduler = fopen("scheduler.txt", "w");
	system = fopen("system.txt", "w");

	cur = NULL;
	cur_cycle = 0;
	p_id = 0;
}

void Scheduler::IO_pop(int p_id)
{
	for(int i = 0; i < IO.size(); i++)
	{
		if(IO[i]->p_id == p_id)
		{
			q.push_back(IO[i]);
			IO.erase(IO.begin()+i);
			return;
		}
	}
}

void Scheduler::give_time_quantum(int time)
{
	for(auto it = q.begin(); it != q.end(); it++)
		(*it)->time += time;
	for(auto it = sleep.begin(); it != sleep.end(); it++)
		(*it)->time += time;
	for(auto it = IO.begin(); it != IO.end(); it++)
		(*it)->time += time;
	if(cur != NULL)
		cur->time += time;
}

void Scheduler::make_process(string name)
{
	Process* a = new Process(p_id, name, give_time, v_mem, pmem);
	q.push_back(a);
	p_id++;
}

void Scheduler::process()
{
	//일정주기마다 cycle 분배
	if(remain_time == 0)
	{
		remain_time = max_remain;
		give_time_quantum(give_time);
	}
	////////////////
	
	//sleep된 프로세스의 검사
	int i;
	for(i = 0; i < sleep.size(); i++)
	{
		if(sleep[i]->sleep_time == 0)
		{
			Process* temp = sleep[i];
			sleep.erase(sleep.begin()+i);
			q.push_back(temp);
			i--;
		}
	}
	/////////////////////////
	
	//input으로 주어진 IO작업의 시행
	
	///////////////////////////////
	
	//input으로 주어진 Process 생성작업의 시행
	//언제??????????????
	while(input.size() != 0)
	{
		make_process(input[0]);
		input.erase(input.begin());
	}
	///////////////////////////////////////////

	//모든 큐에 퀀텀이 0일때 사이클 앞당기기
	bool all_zero = true;
	for(auto it = q.begin(); it != q.end(); it++)
		if((*it)->time != 0)
		{
			all_zero = false;
			break;
		}

	if(all_zero && cur == NULL && !q.empty())
	{
		remain_time = max_remain;
		give_time_quantum(give_time);
	}
	/////////////////////////////////////

	bool change = false;
	bool do_nothing = false;
	//현재 수행중인 Process가 없을경우 획득
	if(cur == NULL)
	{
		while(1)
		{
			if(q.size() == 0)
			{
				do_nothing = true;
				break;	//실행될 프로세스가 없음
			}
			Process* temp = q.front();
			q.pop_front();
			cur = temp;
			process_time = max_process;
			change = true;

			if(cur->time != 0)
				break;

			q.push_back(cur);
		}
	}
	//////////////////////////////////////////////

	//system.txt 정보 출력
	if(change)
	{
		fprintf(scheduler, "%d\t%d\t%s\n", cur_cycle, cur->p_id, cur->name.c_str());
	}

	fprintf(system, "%d Cycle: ", cur_cycle);

	if(cur != NULL)
	{
		fprintf(system, "Process#%d running code %s line %d(op %d, arg %d)", cur->p_id, cur->name.c_str(), cur->code_idx+1, cur->code_array[cur->code_idx].first, cur->code_array[cur->code_idx].second);
	}
	fprintf(system, "\n");

	fprintf(system, "RunQueue: ");
	if(q.empty())
	{
		fprintf(system, "Empty");
	}
	else
	{
		for(auto it = q.begin(); it != q.end(); it++)
		{
			fprintf(system, "%d(%s) ", (*it)->p_id, (*it)->name.c_str());
		}
	}
	fprintf(system, "\n");

	fprintf(system, "SleepList: ");
	if(sleep.empty())
	{
		fprintf(system, "Empty");
	}
	else
	{
		for(auto it = sleep.begin(); it != sleep.end(); it++)
		{
			fprintf(system, "%d(%s) ", (*it)->p_id, (*it)->name.c_str());
		}
	}
	fprintf(system, "\n");

	fprintf(system, "IOWait List: ");
	if(IO.empty())
	{
		fprintf(system, "Empty");
	}
	else
	{
		for(auto it = IO.begin(); it != IO.end(); it++)
		{
			fprintf(system, "%d(%s) ", (*it)->p_id, (*it)->name.c_str());
		}
	}
	fprintf(system, "\n");

	fprintf(system, "|");
	pmem->print(system, 1);
	fprintf(system, "\nLRU:");
	for(auto it = pmem->LRU.begin(); it != pmem->LRU.end(); it++)
	{
		fprintf(system, " (%d:%d)", it->first.first, it->first.second);
	}
	fprintf(system, "\n");
	fprintf(system, "\n");

	///////////////////////////

	//Process 명령 수행
	

	if(!do_nothing)
	{
		int temp = cur->do_process();
		cur->do_print(cur_cycle);
		if(cur->code_idx == (int)cur->code_array.size())
		{
			cur->vmem->all_deallocate();
			process_time = max_process;
			cur = NULL;
		}

		if(cur != NULL)
		{	
			if(temp == 0 || temp == 1 || temp == 2 || temp == 3);
			else if(temp == 4)
			{
				//Sleep
				if(cur->sleep_time == 1)
				{
					q.push_back(cur);
					cur = NULL;
				}
				else
				{
					sleep.push_back(cur);
					cur = NULL;
				}
			}
			else if(temp == 5)
			{
				//IOWait
				IO.push_back(cur);
				cur = NULL;
			}
		}
	}
	/////////////////////////////////

	if(cur != NULL)
	{
		if(cur->time == 0)
		{
			process_time = max_process;
			q.push_back(cur);
			cur = NULL;
		}
		else if(--process_time == 0)
		{
			process_time = max_process;
			q.push_back(cur);
			cur = NULL;
		}
	}
	else
		process_time = max_process;

	for(int i = 0; i < sleep.size(); i++)
		sleep[i]->sleep_time--;

	cur_cycle++;
	remain_time--;
}


int main(int argc, char** argv)
{
	Scheduler sch;
	FILE* input;
	input = fopen(argv[1], "r");

	int process_num, max_process, v_mem, p_mem, page_size, max_remain, give_time;
	fscanf(input, "%d %d %d %d %d %d %d", &process_num, &max_process, &v_mem, &p_mem, &page_size, &max_remain, &give_time);

	sch.process_num = process_num;
	sch.max_remain = max_remain;
	sch.v_mem = v_mem/page_size;
	sch.page_num = p_mem/page_size;
	sch.pmem = new Pmem(p_mem/page_size);
	sch.max_process = max_process;
	sch.give_time = give_time;

	sch.remain_time = max_remain;
	sch.process_time = max_process;

	int time, p_id;
	char temp_str[100];
	string str;
	for(int i = 0; i < process_num; i++)
	{
		fscanf(input, "%d %s", &time, temp_str);
		str = temp_str;
		while(sch.cur_cycle != time)
			sch.process();


		if(str == "INPUT")
		{
			fscanf(input, "%d", &p_id);
			sch.IO_pop(p_id);
		}
		else
		{
			sch.input.push_back(str);
		}
	}

	while(!sch.q.empty() || sch.cur != NULL || !sch.sleep.empty() || !sch.IO.empty())
	{
		sch.process();
	}

	return 0;
}

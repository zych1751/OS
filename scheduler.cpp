#include <cstdio>
#include "process.h"
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
	
	int remain_time; // if remain_time == 0, give time_quantum
	int max_remain; // 프로세스에 사이클을 주는 주기

	int give_time; // 모든 프로세스에 주는 사이클 수
	
	int v_mem, p_mem, page_size;
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
	bool input;

	int next_process_cycle;
	string next_process_name;
};

Scheduler::Scheduler()
{
	cur = NULL;
	remain_time = max_remain;
	process_time = max_process;
	cur_cycle = 0;
	p_id = 0;
	input = false;
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
}

void Scheduler::make_process(string name)
{
	Process* a = new Process(p_id, name, give_time, page_num);
	q.push_back(a);
	p_id++;
}

void Scheduler::process()
{
	//일정주기마다 cycle 분배
	remain_time--;
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
	if(input)
	{
		make_process(next_process_name);
		input = false;
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

	if(q.size() != 0 && all_zero)
	{
		remain_time = max_remain;
		give_time_quantum(give_time);
	}
	/////////////////////////////////////

	bool change = false;
	//현재 수행중인 Process가 없을경우 획득
	if(cur == NULL)
	{
		while(1)
		{
			if(q.size() == 0)
			{
				cur_cycle++;
				return;	//실행될 프로세스가 없음
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
		printf("%d\t%d\t", cur_cycle, cur->p_id);
		cout << cur->name << "\n";
		/*for(auto it = q.begin(); it != q.end(); it++)
			cout << (*it)->name << " ";
		printf("\n");
		for(auto it = sleep.begin(); it != sleep.end(); it++)
			cout << (*it)->name << " ";
		printf("\n");
		for(auto it = IO.begin(); it != IO.end(); it++)
			cout << (*it)->name << " ";
		printf("\n");*/
	}
	///////////////////////////
	
	//Process 명령 수행
	int temp = cur->do_process();
	if(cur->code_idx == (int)cur->code_array.size())
	{
		process_time = max_process;
		cur = NULL;
	}

	if(cur != NULL)
	{	
		if(temp == 0 || temp == 1 || temp == 2 || temp == 3);
		else if(temp == 4)
		{
			//Sleep
			if(cur->sleep_time == 0)
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
}


int main()
{
	Scheduler sch;

	int process_num, max_process, v_mem, p_mem, page_size, max_remain, give_time;
	scanf("%d %d %d %d %d %d %d", &process_num, &max_process, &v_mem, &p_mem, &page_size, &max_remain, &give_time);

	sch.process_num = process_num;
	sch.max_remain = max_remain;
	sch.v_mem = v_mem;
	sch.p_mem = p_mem;
	sch.page_size = page_size;
	sch.max_process = max_process;
	sch.give_time = give_time;

	int time, p_id;
	string str;
	for(int i = 0; i < process_num; i++)
	{
		cin >> time >> str;
		while(sch.cur_cycle != time)
			sch.process();

		if(str == "INPUT")
		{
			cin >> p_id;
			sch.IO_pop(p_id);
		}
		else
		{
			sch.input = true;
			sch.next_process_cycle = time;
			sch.next_process_name = str;
		}

		sch.process();
	}

	while(!sch.q.empty() || sch.cur != NULL || !sch.sleep.empty())
	{
		exit(0);
		sch.process();
	}

	return 0;
}

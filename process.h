#include<vector>
#include<string>
#include<utility>

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
	int page_num;
	FILE* f;

	Process(int p_idx, string name, int feed_size, int page_num);
	int do_process();
};

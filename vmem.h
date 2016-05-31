class Vmem
{
public:
	int a_id;
	VBlock* arr;
	Pmem* pmem;

	Vmem(int size, Pmem* mem);
	void allocate(int size);
};

class VBlock
{
public:
	VBlock()
	{
		valid = false;
		match = NULL;
	}
	bool valid;
	Memory* match;
	int a_id;
};

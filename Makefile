all : $(OBJS)
	g++ -std=c++11 pmem.cpp vmem.cpp process.cpp scheduler.cpp -o simulator
	./simulator input

clean :
	rm -f *.txt simulator

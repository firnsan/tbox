#include <stdio.h>
#include <unistd.h>
#include <string>
#include <functional>



#include <tbox/base/ThreadPool.h>
#include <tbox/base/Logger.h>
#include <tbox/base/Condition.h>


using namespace tbox;
using namespace std;

bool allDone = false;

void printStr(const string& msg)
{
	LOG_INFO << msg;
	usleep(100*1000);
}

void setFlag()
{
	allDone = true;
}

int main(int argc, char *argv[])
{
	int numOfTask = 20;
    if (argc==2 && atoi(argv[1]))
		numOfTask = atoi(argv[1]);

	ThreadPool pool;
	pool.start(5);

	LOG_INFO << "Adding";
	for (int i=0; i<numOfTask; i++) {
		char temp[16];
		snprintf(temp, sizeof(temp), "task %d", i);
		pool.run(bind(&printStr, string(temp)));
	}
	pool.run(bind(&setFlag));
	
	LOG_INFO << "All task added";

	while (!allDone) {
		sleep(1);
	}
	// 然而有可能到这了，有些线程还没有start，导致join的时候错误
	// 通过在printStr里加上延迟来解决
    return 0;
}



#include <unistd.h>
#include <functional>

#include <tbox/Thread.h>
#include <tbox/Logger.h>


using namespace tbox;

void threadFunc(int param)
{
	printf("%d\n", param);
	printf("%d\n", Thread::currentTid());
	printf("%s\n", Thread::currentName());
}

void threadFunc2(int param)
{
	printf("%d\n", param);
	sleep(10);
}

int main(int argc, char *argv[])
{

	LOG_INFO << "main's tid:" << Thread::currentTid();
	LOG_INFO << "main's name:" << Thread::currentName();

	{
		// 线程可能在对象析构后才启动
		Thread t1(bind(threadFunc, 1));
		t1.start();
	}

	{
	// 线程可能在对象析构前启动
		Thread t2(bind(threadFunc, 2));
		t2.start();
		sleep(3);
	}

	{
		Thread t3(bind(threadFunc2, 3));
		t3.start();
		//主线程将等待10秒
		t3.join();
	}

	LOG_INFO << "test done";
    return 0;
}


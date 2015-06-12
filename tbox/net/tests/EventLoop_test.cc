#include <stdio.h>

#include <tbox/base/Thread.h>
#include <tbox/net/EventLoop.h>

using namespace tbox;

EventLoop *g_loop;

void threadFunc1()
{
	printf("threadFunc(): tid = %d\n", Thread::currentTid());
	EventLoop loop;
	loop.loop();
}

void test1()
{

    Thread thread(threadFunc1);
	thread.start();
}

void threadFunc2()
{
	g_loop->loop(); //will assert faild
}

void test2()
{
	Thread thread(threadFunc2);
	thread.start();
	thread.join();
}

int main(int argc, char *argv[])
{
	printf("main(): tid = %d\n", Thread::currentTid());
	EventLoop loop;
	g_loop = &loop;

	test1();
	test2();

	// loop.loop();
    return 0;
}


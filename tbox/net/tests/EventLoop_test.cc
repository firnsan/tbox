#include <stdio.h>
#include <string.h>
#include <sys/timerfd.h>

#include <tbox/base/Thread.h>
#include <tbox/net/EventLoop.h>
#include <tbox/net/Channel.h>


using namespace tbox;

EventLoop *g_loop;


void threadFunc1()
{
	g_loop->loop(); 
}

void testLoopInAnotherThread()
{
	Thread thread(threadFunc1);
	thread.start();
	thread.join();
}

void timeout(TimeStamp)
{
	printf("Time out\n");
	g_loop->quit();
}

int main(int argc, char *argv[])
{
	printf("main(): tid = %d\n", Thread::currentTid());
	EventLoop loop;
	g_loop = &loop;

	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.setReadCallback(timeout);
	channel.enableReading();

	struct itimerspec delayTime;
	bzero(&delayTime, sizeof(delayTime));
	delayTime.it_value.tv_sec = 5;
	::timerfd_settime(timerfd, 0, &delayTime, NULL);

	loop.loop();

	testLoopInAnotherThread(); // will assert faild

    return 0;
}


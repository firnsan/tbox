#include <assert.h>
#include <poll.h>


#include <tbox/base/Thread.h>
#include <tbox/base/Logger.h>


#include <tbox/net/EventLoop.h>

using namespace std;
using namespace tbox;

__thread EventLoop *EventLoop::t_LoopInThisThread = 0;

EventLoop::EventLoop()
	: looping_(false),
	  threadId_(Thread::currentTid())
{
	LOG_TRACE << "EventLoop created " << this << "in thread " << threadId_;
	if (t_LoopInThisThread) {
		LOG_FATAL << "Another EventLoop exists";
	} else {
		t_LoopInThisThread = this;
	}

}

EventLoop::~EventLoop()
{
	assert(!looping_);
	t_LoopInThisThread = 0;
}

bool EventLoop::isInLoopThread() const {
	return threadId_ == Thread::currentTid();
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();

	looping_ = true;
	::poll(NULL, 0, 5*1000);

	LOG_TRACE << "EventLoop " << this << "stop looping";
	looping_ = false;
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "Not in LoopThread";
}
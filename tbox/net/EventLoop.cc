#include <assert.h>
#include <poll.h>


#include <tbox/base/Thread.h>
#include <tbox/base/Logger.h>
#include <tbox/net/Poller.h>
#include <tbox/net/EventLoop.h>
#include <tbox/net/Channel.h>


using namespace std;
using namespace tbox;

const int EventLoop::kPollTimeMs = 10000;
__thread EventLoop *EventLoop::t_LoopInThisThread = 0;

EventLoop::EventLoop()
	: looping_(false),
	  quit_(false),
	  dispatching_(false),
	  threadId_(Thread::currentTid()),
	  poller_(Poller::newDefaultPoller(this))
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

	quit_ = false;
	looping_ = true;
	while (!quit_) {
		activeChannels_.clear();
		TimeStamp returnTime = poller_->poll(kPollTimeMs, &activeChannels_);

		dispatching_ = true;
		for (ChannelList::iterator it = activeChannels_.begin();
			 it != activeChannels_.end(); it++) {
			(*it)->handleEvent(returnTime);
		}
		dispatching_ = false;
	}
	LOG_TRACE << "EventLoop " << this << "stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	//TODO:wakeup if in anothoer thread
}

void EventLoop::updateChannel(Channel *channel)
{
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "Not in LoopThread";
}
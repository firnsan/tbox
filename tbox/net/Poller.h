#ifndef TBOX_POLLER_H
#define TBOX_POLLER_H

#include <vector>

#include <tbox/base/NonCopyable.h>
#include <tbox/base/TimeStamp.h>
#include <tbox/net/EventLoop.h>



namespace tbox
{
	class Channel;
	class EventLoop;

	class Poller : NonCopyable
	{
	public:
		typedef std::vector<Channel*> ChannelList;

		Poller(EventLoop *loop);
		virtual ~Poller();


		// 只能在io线程调用，免去加锁
		virtual TimeStamp poll(int timeoutMs, ChannelList *activeChannels) = 0;

		// 只能在io线程调用，免去加锁
		virtual void updateChannel(Channel *channel) = 0;
		
		// 只能在io线程调用，免去加锁
		virtual void removeChannel(Channel* channel) = 0;

		void assertInLoopThread()
		{
		    ownerLoop_->assertInLoopThread();
		}

		static Poller *newDefaultPoller(EventLoop *loop);

	private:

		EventLoop *ownerLoop_;
	};
}

#endif
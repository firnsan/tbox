#ifndef TBOX_EVENTLOOP_H
#define TBOX_EVENTLOOP_H

#include <sys/types.h>
#include <memory>
#include <vector>


#include <tbox/base/NonCopyable.h>


namespace tbox
{
	class Channel;
	class Poller;

	class EventLoop : NonCopyable
	{
	public:
		EventLoop();
		~EventLoop();

		void loop();
		void quit();

		void assertInLoopThread()
		{
			if (!isInLoopThread())
				abortNotInLoopThread();
		}

		bool isInLoopThread() const;
		void updateChannel(Channel *channel);


	private:
		typedef std::vector<Channel*> ChannelList;

		void abortNotInLoopThread();

		bool looping_;
		bool quit_;
		bool dispatching_;

		const pid_t threadId_;

		std::unique_ptr<Poller> poller_;
		ChannelList activeChannels_;

		static const int kPollTimeMs;
		static __thread EventLoop *t_LoopInThisThread;
	};
}


#endif
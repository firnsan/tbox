#ifndef TBOX_EVENTLOOP_H
#define TBOX_EVENTLOOP_H

#include <sys/types.h>


#include <tbox/base/NonCopyable.h>


namespace tbox
{
	class EventLoop : NonCopyable
	{
	public:
		EventLoop();
		~EventLoop();

		void loop();

		void assertInLoopThread()
		{
			if (!isInLoopThread())
				abortNotInLoopThread();
		}

		bool isInLoopThread() const;


	private:
		void abortNotInLoopThread();
		bool looping_;
		const pid_t threadId_;

		static __thread EventLoop *t_LoopInThisThread;
	};
}


#endif
#ifndef TBOX_THREAD_H
#define TBOX_THREAD_H

#include <pthread.h>
#include <functional>

#include "Atomic.h"


using namespace std;

namespace tbox
{

	class Thread
	{
	public:
		typedef function<void()> ThreadFunc;
		Thread(const ThreadFunc &func, const string &name = string());
		~Thread();
		void start();
		int join();

	private:
		static void *runInThread(void *);

		ThreadFunc func_;
		string name_;
		pthread_t pthreadId_;

		bool started_;
		bool joined_;

		static __thread const char *t_threadName; //static在前面
		static __thread int t_tid;

		static AtomicInt32 numCreated;

	};
}

#endif
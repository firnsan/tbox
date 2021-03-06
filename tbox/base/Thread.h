#ifndef TBOX_THREAD_H
#define TBOX_THREAD_H

#include <pthread.h>
#include <functional>
#include <memory>


#include <tbox/base/Atomic.h>


namespace tbox
{

	class Thread
	{
	public:
		typedef std::function<void()> ThreadFunc;
		Thread(const ThreadFunc &func, const std::string &name = std::string());
		~Thread();
		void start();
		int join();

		bool started() const { return started_; }
		pid_t tid() const { return *tid_; }
		const std::string& name() const { return name_; }

		static int numCreated() { return numCreated_.get(); }
		static pid_t currentTid() { return t_tid; }
		static const char *currentTidStr() { return t_tidStr; }
		static const char *currentName() { return t_threadName; }

		//一下两static变量需要在MainThreadInitializer访问，所以是public
		static __thread const char *t_threadName; //static在前面
		static __thread int t_tid;
		static __thread char t_tidStr[16];
		
	private:
		void setDefaultName();
		static void *runInThread(void *);

		ThreadFunc func_;
		std::string name_;
		pthread_t pthreadId_;
		std::shared_ptr<pid_t> tid_;

		bool started_;
		bool joined_;


		static AtomicInt32 numCreated_;

	};
}

#endif

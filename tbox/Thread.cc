#include <assert.h>

#include "Logger.h"

#include "Thread.h"

namespace tbox
{
	
	namespace detail
	{
		struct ThreadData
		{
			typedef Thread::ThreadFunc ThreadFunc;
			ThreadFunc func_;
			string name_;

			ThreadData(const ThreadFunc& func,
					   const string& name)
				: func_(func),
				  name_(name)
			{}
		};
	}

	__thread const char *Thread::t_threadName = "unkonw";
	__thread int Thread::t_tid = 0;
	AtomicInt32 numCreated;

	Thread::Thread(const ThreadFunc &func, const string &name)
		: func_(func),
		  name_(name)
	{
		//TODO:设置numCretead和name_
	}

	Thread::~Thread()
	{
		LOG_INFO << "Thread destructed" ;
		if (started_ && !joined_)
			pthread_detach(pthreadId_); //线程结束后释放资源
	}

	void Thread::start()
	{
		assert(!started_);
		
		started_ = true;
		
		detail::ThreadData *data = new detail::ThreadData(func_, name_);
		if (pthread_create(&pthreadId_, NULL, &runInThread, data)) {
			started_ = false;
			delete data;
			LOG_FATAL << "Failed create new thread";
		}

	}

	int Thread::join()
	{
		return pthread_join(pthreadId_, NULL);
	}

	void *Thread::runInThread(void *param)
	{
		detail::ThreadData *data = static_cast<detail::ThreadData*>(param);
		
		Thread::t_threadName = data->name_.c_str(); 

		data->func_();
		//started_ = false; TODO:无法设置成员变量started为false
		delete data;

		return NULL;
	}
}
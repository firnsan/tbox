#include <functional>

#include "ThreadPool.h"

using namespace std;

namespace tbox
{
	ThreadPool::ThreadPool()
		: threads_(),
		  queue_(),
		  running_(false)
	{
	}

	void ThreadPool::start(int numOfThreads)
	{
		running_ = true;
		for(int i=0 ; i<numOfThreads; i++)
		{
			//绑定成员函数时需要加上&
			function<void()> fn = bind(&ThreadPool::runInThread, this);
			threads_.push_back(
				unique_ptr<Thread>(new Thread(fn)));

			threads_[i]->start();
		}
	}

	void ThreadPool::runInThread()
	{
		while (running_)
		{
			Task task = queue_.take();
			task();
		}
	}

	void ThreadPool::stop()
	{
		running_ = false;
	}
}

#include <functional>
#include <algorithm>


#include "ThreadPool.h"

using namespace std;
using namespace std::placeholders;

namespace tbox
{
	ThreadPool::ThreadPool()
		: threads_(),
		  queue_(),
		  running_(false)
	{
	}

	ThreadPool::~ThreadPool()
	{
		if (running_)
			stop();
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

	void ThreadPool::run(const Task &task)
	{
		if (threads_.empty()) {
			task();
		}
		else {
			queue_.put(task);
		}

	}

	void ThreadPool::runInThread()
	{
		while (running_)
		{
			Task task = queue_.take();

			// 如果阻塞于take时被唤醒，task将是一个value初始化的对象
			if (task) {
				task();
			}
		}
	}

	void ThreadPool::stop()
	{
		running_ = false;
		// 唤醒阻塞的线程
		queue_.wakeup();
		for_each(threads_.begin(), threads_.end(), bind(&Thread::join, _1) );
	}
}

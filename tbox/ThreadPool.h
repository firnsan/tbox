#ifndef TBOX_THREADPOOL_H
#define TBOX_THREADPOOL_H

#include <vector>
#include <memory>
#include <functional>


#include "Thread.h"
#include "BlockingQueue.h"



namespace tbox
{
	class ThreadPool
	{
	public:
		typedef std::function<void()> Task;

		ThreadPool();
		void start(int numOfThreads);
		void stop();

	private:
		void runInThread();

		std::vector<std::unique_ptr<Thread>> threads_;
		BlockingQueue<Task> queue_;
		bool running_;
	};
}

#endif
#ifndef TBOX_THREADPOOL_H
#define TBOX_THREADPOOL_H

#include <vector>
#include <memory>
#include <functional>


#include <tbox/base/Thread.h>
#include <tbox/base/BlockingQueue.h>



namespace tbox
{
	class ThreadPool
	{
	public:
		typedef std::function<void()> Task;

		ThreadPool();
		~ThreadPool();
		void start(int numOfThreads);
		void stop();
		void run(const Task&);

	private:
		void runInThread();

		// Thread对象的生命周期完全由线程池管理，所以使用unique_ptr
		std::vector<std::unique_ptr<Thread>> threads_;
		BlockingQueue<Task> queue_;
		bool running_;
	};
}

#endif

#ifndef TBOX_BLOCKINGQUEUE_H
#define TBOX_BLOCKINGQUEUE_H

#include <deque>


#include <tbox/Mutex.h>
#include <tbox/Condition.h>



namespace tbox
{
	template<typename T>
	class BlockingQueue
	{
	public:
		BlockingQueue()
			: queue_(),
			mutex_(),
			notEmpty_(mutex_),
			running_(true)
			{

			}

		T take()
		{
			MutexGuard lock(mutex_);
			while (queue_.empty() && running_) {
				notEmpty_.wait();
			}
			T e = T();
			if (!queue_.empty()) {
				e = queue_.front();
				queue_.pop_front();
			}

			// 如果是被唤醒的，返回value初始化的对象
			return e;
		}

		void put(const T &e)
		{
			MutexGuard lock(mutex_);
			queue_.push_back(e);
			notEmpty_.notify();
		}
		
		// 调用wakeup后take返回的对象是value初始化的
		void wakeup()
		{
			MutexGuard lock(mutex_);
			running_ = false;
			notEmpty_.notifyAll();
		}

	private:
		std::deque<T> queue_;
		MutexLock mutex_;
		Condition notEmpty_;
		bool running_;
	};
}



#endif
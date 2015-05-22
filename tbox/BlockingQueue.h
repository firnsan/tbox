#ifndef TBOX_BLOCKINGQUEUE_H
#define TBOX_BLOCKINGQUEUE_H

#include <deque>


#include "Mutex.h"
#include "Condition.h"



namespace tbox
{
	template<typename T>
	class BlockingQueue
	{
	public:
		BlockingQueue()
			: queue_(),
			mutex_(),
			notEmpty_(mutex_)
			{

			}

		T take()
		{
			MutexGuard lock(mutex_);
			while (queue_.empty()) {
				notEmpty_.wait();
			}
			T e(queue_.front());
			queue_.pop_front();

			return e;
		}
		void put(T &e)
		{
			MutexGuard lock(mutex_);
			queue_.push_back(e);
			notEmpty_.notify();
		}

	private:
		std::deque<T> queue_;
		MutexLock mutex_;
		Condition notEmpty_;
	};
}



#endif
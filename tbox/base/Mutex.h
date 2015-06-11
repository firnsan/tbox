#ifndef TBOX_MUTEX_H
#define TBOX_MUTEX_H

#include <pthread.h>
#include <assert.h>

#include <tbox/base/Thread.h> //TODO: extract currentThread
#include <tbox/base/NonCopyable.h>


namespace tbox
{
	class MutexLock : NonCopyable
	{
	public:
		MutexLock()
			: holder_(0)
		{
			pthread_mutex_init(&mutex_, NULL);
		}

		~MutexLock()
		{
			assert(holder_==0);
			pthread_mutex_destroy(&mutex_);
		}

		void lock()
		{
			// 先锁上才设置holder
			pthread_mutex_lock(&mutex_);
			holder_ = Thread::currentTid();
		}

		void unlock()
		{
			holder_ = 0;
			pthread_mutex_unlock(&mutex_);
		}

		pthread_mutex_t* get()
		{
			return &mutex_;
		}

	private:
		pthread_mutex_t mutex_;
		pid_t holder_;
	};


	class MutexGuard
	{
	public:
		MutexGuard(MutexLock &mutex)
			: mutex_(mutex)
		{
			mutex_.lock();
		}

		~MutexGuard()
		{
			mutex_.unlock();
		}

	private:
		//注意是引用
		MutexLock &mutex_;
	};
}


#endif

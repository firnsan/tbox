#ifndef TBOX_CONDITION_H
#define TBOX_CONDITION_H

#include <pthread.h>


#include <tbox/base/Mutex.h>
#include <tbox/base/NonCopyable.h>


namespace tbox
{
	class Condition : NonCopyable
	{
	public:
		explicit Condition(MutexLock &mutex)
			: mutex_(mutex)
		{
			pthread_cond_init(&cond_, NULL);
		}

		~Condition()
		{
			pthread_cond_destroy(&cond_);
		}

		void wait()
		{
			pthread_cond_wait(&cond_, mutex_.get());
		}

		void notify()
		{
			pthread_cond_signal(&cond_);
		}

		void notifyAll()
		{
			pthread_cond_broadcast(&cond_);
		}

	private:
		// const MutexLock &mutex_; 不可以const，否则得提供const版的MutexLock::get()
		MutexLock &mutex_;
		pthread_cond_t cond_;
	};
}

#endif

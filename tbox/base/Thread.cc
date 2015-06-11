#include <unistd.h>
#include <sys/syscall.h>

#include <assert.h>


#include <tbox/base/Logger.h>

#include <tbox/base/Thread.h>

namespace tbox
{
	
	namespace detail
	{
		pid_t gettid()
		{
			return static_cast<pid_t>(::syscall(SYS_gettid));
		}

		class MainThreadInitializer
		{
		public:
			MainThreadInitializer()
			{
				Thread::t_threadName = "main";
				Thread::t_tid = gettid();
				int n = snprintf(Thread::t_tidStr, sizeof(Thread::t_tidStr), "%5d ", Thread::t_tid);
				assert(n==6);
			}
		};
		MainThreadInitializer mainThreadInit;

		struct ThreadData
		{
			typedef Thread::ThreadFunc ThreadFunc;
			ThreadFunc func_;
			string name_;
			std::weak_ptr<pid_t> tid_;

			ThreadData(const ThreadFunc& func,
				const string& name,
				const std::shared_ptr<pid_t>& tid)
				: func_(func),
				  name_(name),
				  tid_(tid)
			{}
		};
	} //detail

	__thread const char *Thread::t_threadName = "unkonw";
	__thread int Thread::t_tid = 0;
	__thread char Thread::t_tidStr[16];
	AtomicInt32 Thread::numCreated_;

	Thread::Thread(const ThreadFunc &func, const string &str)
		: func_(func),
		  name_(str),
		  pthreadId_(0),
		  tid_(new pid_t(0)),
		  started_(false),
		  joined_(false)
	{
		setDefaultName();
	}

	Thread::~Thread()
	{
		LOG_INFO << "Thread destructed" ;
		// 确保线程没有被join才能detach，否则会segfault
		if (started_ && !joined_)
			pthread_detach(pthreadId_); //线程结束后释放资源
	}

	void Thread::start()
	{
		assert(!started_);
		started_ = true;
		
		detail::ThreadData *data = new detail::ThreadData(func_, name_, tid_);

		if (pthread_create(&pthreadId_, NULL, &runInThread, data)) {
			started_ = false;
			delete data;
			LOG_FATAL << "Failed create new thread";
		}

	}

	int Thread::join()
	{
		assert(started_);
		assert(!joined_);
		joined_ = true;
		return pthread_join(pthreadId_, NULL);
	}

	void *Thread::runInThread(void *param)
	{

		detail::ThreadData *data = static_cast<detail::ThreadData*>(param);


		//设置线程局部数据
		Thread::t_tid = detail::gettid();
		int n = snprintf(Thread::t_tidStr, sizeof(Thread::t_tidStr), "%5d ", Thread::t_tid);
		assert(n==6);
		Thread::t_threadName = data->name_.c_str();

		LOG_INFO << data->name_ << " started";

		auto ptid = data->tid_.lock();
		if (ptid) {
			*ptid = Thread::t_tid;
			ptid.reset(); //降低引用计数减1，令其所指对象可以在Thread析构时释放
		} else {
			LOG_INFO << "tid is already be freed";
		}


		data->func_();
		//started_ = false; TODO:无法设置成员变量started为false
		delete data;

		return NULL;
	}

	void Thread::setDefaultName()
	{
		int num = numCreated_.incrementAndGet();
		if (name_.empty()) {
			char temp[32];
			snprintf(temp, 32, "Thread%d", num);
			name_ = temp;
		}
	}
}

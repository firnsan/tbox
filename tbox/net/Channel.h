#ifndef TBOX_CHANNEL_H
#define TBOX_CHANNEL_H

#include <functional>
#include <memory>


#include <tbox/base/NonCopyable.h>
#include <tbox/base/TimeStamp.h>


namespace tbox
{

	class EventLoop;

	class Channel : NonCopyable
	{
	public:
		typedef std::function<void()> EventCallback;
		typedef std::function<void(TimeStamp)> ReadEventCallback;

		Channel(EventLoop *loop, int fd);
		~Channel();

		EventLoop *ownerLoop() { return loop_; }
		int fd() const { return fd_; }

		void handleEvent(TimeStamp);

		void setReadCallback(const ReadEventCallback& cb)
		{ readCallback_ = cb; }
		void setWriteCallback(const EventCallback& cb)
		{ writeCallback_ = cb; }
		void setErrorCallback(const EventCallback& cb)
		{ errorCallback_ = cb; }
		void setCloseCallback(const EventCallback& cb)
		{ closeCallback_ = cb; }

		void enableReading() { events_ |= kReadMask; update(); }
		void enableWriting() { events_ |= kWriteMask; update(); }
		// 保证handleEvent时，loop_没被析构
		void tie(std::shared_ptr<EventLoop>);

		int events() const { return events_; }
		void setRevents(int revt) { revents_ = revt; } // used by Poller
		int index() { return index_; }
		void setIndex(int idx) { index_ = idx; }

		bool isNoneEvent() { return events_ == kNoneMask; }

	private:
		void update();
		void handleEventWithGuard(TimeStamp time);

		EventLoop *loop_;
		int fd_;


		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback errorCallback_;
		EventCallback closeCallback_;

		const static int kReadMask;
		const static int kWriteMask;
		const static int kNoneMask;

		int events_;
		int revents_;

		std::weak_ptr<EventLoop> tie_;
		bool tied_;

		int index_; //在eventloop::pollfds_数组中的索引
	};
}

#endif
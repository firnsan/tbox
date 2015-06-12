#include <poll.h>

#include <tbox/net/Channel.h>
#include <tbox/base/Logger.h>


using namespace std;
using namespace tbox;

const int Channel::kReadMask = POLLIN | POLLPRI;
const int Channel::kWriteMask = POLLOUT;
const int Channel::kNoneMask = 0;

Channel::Channel(EventLoop *loop, int fdArg)
	: loop_(loop),
	  fd_(fdArg),
	  events_(0),
	  revents_(0)
{

}

void Channel::handleEvent(TimeStamp time)
{
	shared_ptr<void> guard;
	if(tied_) {
		guard = tie_.lock();
		if(guard) {
			handleEventWithGuard(time);
		}
	} else {
			handleEventWithGuard(time);
	}
}

void Channel::handleEventWithGuard(TimeStamp time)
{
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
		if (closeCallback_)
			closeCallback_();
	}

	if (revents_ & POLLNVAL) {
		LOG_WARN << "Channel::handleEvent() POLLNVAL";
	}

	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
		if (readCallback_)
			readCallback_(time);
	}

	if (revents_ & POLLOUT) {
		if (writeCallback_)
			writeCallback_();
	}
}

#include <poll.h>

#include <tbox/net/Channel.h>

using namespace tbox;

const int Channel::kReadMask = POLLIN | POLLPRI;
const int Channel::kWriteMask = POLLOUT;
const int Channel::kNoneMask = 0;

Channel::Channel(EventLoop *loop, int fd)
	: loop_(loop),
	  fd_(fd),
	  events_(0),
	  revents_(0)
{

}

void Channel::handleEvent(TimeStamp time)
{
	
}

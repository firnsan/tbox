#include <tbox/net/Poller.h>
#include <tbox/net/poller/PollPoller.h>


using namespace tbox;


Poller::Poller(EventLoop *loop)
	: ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

Poller *Poller::newDefaultPoller(EventLoop *loop)
{
	return new PollPoller(loop);
}

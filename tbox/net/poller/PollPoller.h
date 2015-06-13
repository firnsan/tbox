#ifndef TBOX_POLLPOLLER_H
#define TBOX_POLLPOLLER_H

#include <vector>
#include <map>


#include <tbox/net/Poller.h>

struct pollfd;

namespace tbox
{
	class PollPoller : public Poller
	{
	public:
		PollPoller(EventLoop* loop);
		virtual ~PollPoller();

		virtual TimeStamp poll(int timeoutMs, ChannelList* activeChannels);
		virtual void updateChannel(Channel* channel);
		virtual void removeChannel(Channel* channel);

	private:

		void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

		typedef std::vector<struct pollfd> PollFdList;
		typedef std::map<int, Channel*> ChannelMap;

		PollFdList pollfds_;
		ChannelMap channels_;
	};
}


#endif
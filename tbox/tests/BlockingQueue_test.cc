#include <functional>
#include <stdio.h>
#include <unistd.h>



#include <tbox/BlockingQueue.h>
#include <tbox/Thread.h>
#include <tbox/Logger.h>

using std::bind;

using namespace tbox;

BlockingQueue<int> bq;

void producer()
{
	LOG_INFO << "producer started!";
	int i = 0;
	while (i<20) {
		bq.put(i);
		i++;
		int st = rand()%5;
		sleep(st);
	}
	LOG_INFO << "producer quited!";
}


void customer()
{
	LOG_INFO << "customer started!";
	int i = 0;
	while (1) {
		i = bq.take();
		printf("%d\n", i);

		// int st = rand()%5;
		// sleep(st);

		if (i==19)
			break;
	}
	LOG_INFO << "customer quited";
}


int main(int argc, char *argv[])
{
    Thread p(bind(producer));
	Thread c(bind(customer));

	p.start();
	c.start();
	p.join();
	c.join();
	
    return 0;
}


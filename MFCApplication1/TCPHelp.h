#pragma once

#include "common/zmq_poller_reactor.h"
//#include "common/zmq_poll_events.h"
#include <string>
#include "common/TCPMonitor.h"
enum TCP_TYPE {
	SERVER = 1,
	CLIENT = 2
};

class TCPHelp : public zmq_poller_reactor, public zmq_poll_events
{
public:
	TCPHelp(TCP_TYPE type, std::string& addr, HWND target);
	~TCPHelp();

	virtual bool init();
	virtual void run();
	bool publish(void* msg, int length);
	void checkTcpStatus();
	virtual void onTimer();
	virtual void zmq_in_event(zmq::socket_t* socket);
private:
	zmq::context_t zcontext_;
	zmq::socket_t* socket_;
	TCPMonitor* tcpMonitor_;
	TCP_TYPE type_;
	std::string addr_;
	HWND target_;
	bool init_;
};


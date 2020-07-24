#pragma once
#include <zmq.hpp>
class TCPMonitor : public zmq::monitor_t
{
public:
	TCPMonitor();
	~TCPMonitor();

	virtual void on_event_connected(const zmq_event_t &event_, const char *addr_);
};


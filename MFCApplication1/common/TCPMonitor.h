#pragma once

#include <zmq.hpp>
class TCPMonitor : public zmq::monitor_t {
public:
	TCPMonitor();
	~TCPMonitor();

	void checkEventEx(HWND tartget);
	virtual void on_event_connected(const zmq_event_t & event_, const char * addr_);

	virtual void on_event_accepted(const zmq_event_t &event_, const char *addr_);

	virtual void on_event_closed(const zmq_event_t &event_, const char *addr_);

	virtual void on_event_disconnected(const zmq_event_t &event_, const char *addr_);

	virtual void on_event_bind_failed(const zmq_event_t &event_, const char *addr_);

	virtual void on_event_listening(const zmq_event_t &event_, const char *addr_);
};
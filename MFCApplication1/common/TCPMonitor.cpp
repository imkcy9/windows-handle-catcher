#include "stdafx.h"
#include "TCPMonitor.h"


TCPMonitor::TCPMonitor()
{
}


TCPMonitor::~TCPMonitor()
{
	
}

HWND g_tartget = NULL;
void TCPMonitor::checkEventEx(HWND tartget)
{
	g_tartget = tartget;
	zmq::monitor_t::check_event();
}

void TCPMonitor::on_event_connected(const zmq_event_t & event_, const char * addr_)
{

	std::string* log = new std::string("on_event_connected ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, 0, (LPARAM)log);
}

void TCPMonitor::on_event_accepted(const zmq_event_t & event_, const char * addr_)
{
	std::string* log = new std::string("on_event_accepted ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, 0, (LPARAM)log);
}

void TCPMonitor::on_event_closed(const zmq_event_t & event_, const char * addr_)
{
	std::string* log = new std::string("on_event_closed ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, (WPARAM)RGB(255, 0, 0), (LPARAM)log);
}

void TCPMonitor::on_event_disconnected(const zmq_event_t & event_, const char * addr_)
{
	std::string* log = new std::string("on_event_disconnected ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, (WPARAM)RGB(255, 0, 0), (LPARAM)log);
}

void TCPMonitor::on_event_bind_failed(const zmq_event_t & event_, const char * addr_)
{
	std::string* log = new std::string("on_event_bind_failed ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, (WPARAM)RGB(255, 0, 0), (LPARAM)log);
}

void TCPMonitor::on_event_listening(const zmq_event_t & event_, const char * addr_)
{
	std::string* log = new std::string("on_event_listening ");
	log->append(addr_);
	::PostMessage(g_tartget, WM_RECV_TCP_LOG, 0, (LPARAM)log);
}

#include "stdafx.h"
#include "TCPHelp.h"


TCPHelp::TCPHelp(TCP_TYPE type, std::string& addr, HWND target)
	: zcontext_()
	, socket_(NULL)
	, tcpMonitor_(NULL)
	, type_(type)
	, addr_(addr)
	, target_(target)
	, init_(false)
{
}


TCPHelp::~TCPHelp()
{
	if (tcpMonitor_) {
		delete tcpMonitor_;
	}

	if (socket_) {
		socket_->close();
		delete socket_;
	}

	zcontext_.close();
}

bool TCPHelp::init()
{
	if (init_) {
		return init_;
	}
	try {

		if (!tcpMonitor_) {
			tcpMonitor_ = new TCPMonitor();
		}

		if (type_ == SERVER) {
			socket_ = new zmq::socket_t(zcontext_, zmq::socket_type::pub);
			tcpMonitor_->init(*socket_, "inproc://server");

			socket_->bind(addr_);

			
		}
		else {
			socket_ = new zmq::socket_t(zcontext_, zmq::socket_type::sub);
			socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
			tcpMonitor_->init(*socket_, "inproc://client");
			socket_->connect(addr_);
			this->add_socket(socket_, this);

			
		}
	}
	catch (zmq::error_t &e) {
		std::string* log = new std::string(e.what());
		::PostMessage(target_, WM_RECV_TCP_LOG, (WPARAM)RGB(255, 0, 0), (LPARAM)log);
		return false;
	}
	
	init_ = true;
	return init_;
}

void TCPHelp::run()
{
	zmq_poller_reactor::run();
}

bool TCPHelp::publish(void* msg, int length)
{
	if (type_ == CLIENT) {
		return false;
	}
	if (!socket_) {
		return false;
	}
	socket_->send(msg, length);
	return true;
}

void TCPHelp::checkTcpStatus()
{
}

void TCPHelp::onTimer()
{
	if (this->tcpMonitor_) {
		this->tcpMonitor_->checkEventEx(target_);
	}
	
}

void TCPHelp::zmq_in_event(zmq::socket_t* socket) { 
	zmq::message_t msg;
	socket_->recv(&msg);

	std::string str((char*)msg.data(), msg.size());
	CString cstring;
	cstring = CA2T(str.c_str());
	::SendMessage(target_, WM_RECV_SUB_MSG, 0, (LPARAM)&cstring);
	// todo;
};
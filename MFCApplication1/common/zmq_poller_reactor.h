/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_poller_reactor.h
 * Author: chengyeke
 *
 * Created on 2017年8月31日, 下午2:10
 */

#ifndef ZMQ_POLLER_REACTOR_H
#define ZMQ_POLLER_REACTOR_H
#include "thread.h"
#include <zmq.hpp>
//#include "log.h"


#include "zmq_poll_events.h"

#include <vector>

class zmq_poller_reactor : public thread_t {
public:
    zmq_poller_reactor();

    virtual ~zmq_poller_reactor();

    virtual bool init() = 0;
    
    virtual void stop();

	virtual void onTimer() = 0;
    
    void add_socket(zmq::socket_t* socket, zmq_poll_events* event);
    
    
protected:
    virtual void run();
    
private:
    
    void process_term();

    bool m_stop;

    //std::vector<zmq::socket_t*> m_poll_sockets;
    std::vector<std::pair<zmq::socket_t*,zmq_poll_events*> > m_poll_sockets;
};

#endif /* ZMQ_POLLER_REACTOR_H */


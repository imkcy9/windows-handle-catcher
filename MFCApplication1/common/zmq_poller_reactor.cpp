/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_poller_reactor.cpp
 * Author: chengyeke
 * 
 * Created on 2017年8月31日, 下午2:10
 */

#include <zmq.h>

#include "zmq_poller_reactor.h"

zmq_poller_reactor::zmq_poller_reactor()
: m_stop(false) {
}

zmq_poller_reactor::~zmq_poller_reactor() {
}

void zmq_poller_reactor::run() {

    int socket_count = m_poll_sockets.size();
    zmq::pollitem_t *items = (zmq::pollitem_t *)malloc(sizeof(zmq::pollitem_t) * socket_count);

    for(size_t i = 0; i < m_poll_sockets.size(); i++) {
        //items[i+1] = {*(m_poll_sockets[i].first), 0, ZMQ_POLLIN, 0};
        items[i].socket = *(m_poll_sockets[i].first);
        items[i].fd = 0;
        items[i].events = ZMQ_POLLIN;
        items[i].revents = 0;
    }

    while (!m_stop) {
        zmq::message_t message;
        
		long wait_time = 500;
		//timers_timeout();
  //      if(wait_time == 0) {
  //          timers_execute();
  //          wait_time = timers_timeout();
  //      }
        int rc = zmq::poll(items, socket_count, wait_time);
        if (rc == 0) {
            //LOG_INFO("timer out rc == 0");
			this->onTimer();
            continue;
        }
        if (rc == -1) {
			this->onTimer();
            continue;
        }
        
        for(size_t i = 0; i < m_poll_sockets.size(); i++) {
            
            if(items [i].revents & ZMQ_POLLIN) {
                m_poll_sockets[i].second->zmq_in_event(m_poll_sockets[i].first);
            }
        }
    }
    
    free(items);
}

void zmq_poller_reactor::stop() {
	m_stop = true;
}

void zmq_poller_reactor::add_socket(zmq::socket_t* socket, zmq_poll_events* event) {
    //assert(socket);
    //m_poll_sockets.push_back(socket);
    m_poll_sockets.push_back(std::make_pair(socket,event));
}



void zmq_poller_reactor::process_term() {
    m_stop = true;
}

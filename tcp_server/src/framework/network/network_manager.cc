/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   network_manager.cc
@ author:	   Connor
@ version:	   1.0.0
@ date:		   2017-04-01
@ description: network manager.
@ others:
@ history:
1.date:
author:
modification:
********************************************************************************/

#include "network_manager.h"

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/event.h"
#include "event2/listener.h"

#include "framework/base/log_headers.h"

namespace gamer {

NetworkManager::NetworkManager() : NetworkManager("127.0.0.1", 4994) {
}

NetworkManager::NetworkManager(const std::string& ip, int port)
	:ip_(ip)
	,port_(port)
	,evbase_(nullptr)
	,connlistener_(nullptr) {
}

NetworkManager* NetworkManager::instance() {
	static NetworkManager s_network_mgr;
	return &s_network_mgr;
}

void NetworkManager::InitSocket() {
	struct sockaddr_in sin;

#ifdef _WIN32
	WSADATA wsadata;
	WSAStartup(0x0201, &wsadata);
#endif

	if (nullptr == evbase_) {
		evbase_ = event_base_new();
	}
	if (nullptr == evbase_) {
		LOGERROR("[NetworkManager::InitSocket] event_base_new failed!");
		return;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_.c_str());
	//sin.sin_addr.s_addr = htonl(0);
	sin.sin_port = htons(port_);

	if (nullptr == connlistener_) {
		connlistener_ = evconnlistener_new_bind(evbase_,
			(evconnlistener_cb)OnConnAccept,
			NULL,
			LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
			-1,
			(struct sockaddr*)&sin,
			sizeof(sin));
	}
	if (nullptr == connlistener_) {
		LOGERROR("[NetworkManager::InitSocket] evconnlistener_new_bind failed!");
		return;
	}

	LOGGREEN("[NetworkManager::InitSocket] tcp listen on : %s, port : %d", ip_.c_str(), port_);

	evconnlistener_set_error_cb(connlistener_, OnConnErrorOccur);

	event_base_dispatch(evbase_);
}

void NetworkManager::OnConnAccept(struct evconnlistener* listener, 
								  evutil_socket_t fd,
                                  struct socketaddr* address,
								  int socklen,
                                  void* ctx) {
	// We got a new connection! Set up a bufferevent for it.
	LOGGREEN("[NetworkManager::InitSocket] one client connected");
	auto base = evconnlistener_get_base(listener);
	//int ret = evutil_make_socket_nonblocking(fd);
	auto bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, OnBuffereventRead, OnBuffereventWrite, OnBuffereventArrive, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void NetworkManager::OnConnErrorOccur(struct evconnlistener* listener, void* ctx) {
	auto base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	//fprintf(stderr, "Got an error %d (%s) on the listener. "
	//	"Shutting down.", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}

void NetworkManager::OnBuffereventArrive(struct bufferevent* bev, short event, void* ctx) {
	if (event & BEV_EVENT_ERROR) {
		LOGERROR("[NetworkManager::OnBuffereventArrive] error from bufferevent!");
	}

	if (event & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
		bufferevent_free(bev);
	}

	if (event & BEV_EVENT_CONNECTED) {
		LOGGREEN("[NetworkManager::OnBuffereventArrive] client connected");
		//write_cb(bev, NULL);
		//char msg[] = "client connected";
		//bufferevent_write(bev, msg, sizeof(msg));
		auto input = bufferevent_get_input(bev);
		auto output = bufferevent_get_output(bev);
		evbuffer_add_printf(output, "client msg : %s", "client connected");
	} else if (event & BEV_EVENT_TIMEOUT) {
		LOGERROR("[NetworkManager::OnBuffereventArrive] client connect timeout");
	}
}

void NetworkManager::OnBuffereventRead(struct bufferevent* bev, void* ctx) {
	// This callback is invoked when there is data to read on bev.
	auto input = bufferevent_get_input(bev);
	auto output = bufferevent_get_output(bev);

	//if (my_n < 5) {
	//	my_n++;
		char buf[4096] = { 0 };
		size_t n = evbuffer_get_length(input);
		//if (evbuffer_remove(input, buf, n) > 0 ) { // read and remove
		if (evbuffer_copyout(input, buf, n) > 0 ) {  // read only
			printf("read data from client : %s", buf);
			//evbuffer_add_printf(output, "client msg %d", my_n);
		}
	//}
}

void NetworkManager::OnBuffereventWrite(struct bufferevent* bev, void* ctx) {
	//printf("client read_cb");
	//char msg[] = "client msg";
	//bufferevent_write(bev, msg, sizeof(msg));
}

void NetworkManager::InitIPAndPort() {
	// TODO : init from cfg
	ip_ = "127.0.0.1";
	port_ = 4994;
}

} // namespace gamer
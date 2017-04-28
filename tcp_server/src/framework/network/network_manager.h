/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   network_manager.h
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

#ifndef CONNOR_GAME_SRC_NETWORK_MANAGER_H_
#define CONNOR_GAME_SRC_NETWORK_MANAGER_H_

#include <string>

#include "event2/util.h"

struct bufferevent;
struct event_base;
struct evconnlistener;

namespace gamer {

class NetworkManager {
  public:
	NetworkManager& operator=(const NetworkManager&) = delete;

	NetworkManager(const NetworkManager&) = delete;

	~NetworkManager() {};

	static NetworkManager* instance();

	void InitSocket();

  private:
	NetworkManager() : NetworkManager("127.0.0.1", 4994) {}

	NetworkManager(const std::string& ip, int port)
		:ip_(ip)
		,port_(port)
		,evbase_(nullptr)
		,connlistener_(nullptr) {
	}

	bool Init();

	static void OnConnAccept(struct evconnlistener* listener, 
		                     evutil_socket_t fd,
	                         struct socketaddr* addr, 
							 int socklen, 
							 void* ctx);

	static void OnConnErrorOccur(struct evconnlistener* listener, void* ctx);

	static void OnBuffereventArrive(struct bufferevent* bev, short event, void* ctx);

	static void OnBuffereventRead(struct bufferevent* bev, void* ctx);

	static void OnBuffereventWrite(struct bufferevent* bev, void* ctx);

	void InitIPAndPort();

	struct event_base* evbase_;
	struct evconnlistener* connlistener_;

	std::string ip_;
	int port_;
}; // namespace gamer

} // namespace gamer

#endif // CONNOR_GAME_SRC_NETWORK_MANAGER_H_

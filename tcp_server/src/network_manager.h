
#ifndef CITY_HUNTER_SRC_NETWORK_MANAGER_H_
#define CITY_HUNTER_SRC_NETWORK_MANAGER_H_

#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/event.h"
#include "event2/listener.h"

namespace gamer {

class NetworkManager {
  public:
	~NetworkManager();

	static NetworkManager* GetInstance();

	static void DestoryInstance();

	void InitSocket();

  private:
	NetworkManager();

	bool Init();

	static void OnConnAccept(struct evconnlistener* listener, 
		                     evutil_socket_t fd,
	                         struct socketaddr* addr, 
							 int socklen, 
							 void* ctx);

	static void OnConnErrorOccur(struct evconnlistener* listerner, void* ctx);

	static void OnBuffereventArrive(struct bufferevent* bev, short event, void* ctx);

	static void OnBuffereventRead(struct bufferevent* bev, void* ctx);

	static void OnBuffereventWrite(struct bufferevent* bev, void* ctx);

	static NetworkManager* s_shared_network_manager_;
};

} // namespace gamer

#endif // CITY_HUNTER_SRC_NETWORK_MANAGER_H_

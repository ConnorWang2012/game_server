#include "network_manager.h"

#ifdef _WIN32
#include <WinSock2.h>
#endif

namespace gamer {

NetworkManager* NetworkManager::s_shared_network_manager_ = nullptr;

NetworkManager::NetworkManager() {

}

NetworkManager::~NetworkManager() {

}

NetworkManager* NetworkManager::GetInstance() {
	if (!s_shared_network_manager_) {
		s_shared_network_manager_ = new NetworkManager();
		if( !s_shared_network_manager_->Init() ) {
			//SAFE_DELETE(s_shared_network_manager_);
			//CCLOG("event_manager init failed!");
	    }
	}
	return s_shared_network_manager_;
}

void NetworkManager::DestoryInstance() {
	//SAFE_DELETE(s_shared_network_manager_);
}

void NetworkManager::InitSocket() {
	struct event_base* base;
	struct evconnlistener* connlistener;
	struct sockaddr_in sin;
	const char* ip = "127.0.0.1";
	int port = 9876;

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	base = event_base_new();
	if (!base) {
		perror("event_base_new failed!");
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	//sin.sin_addr.s_addr = htonl(0);
	sin.sin_port = htons(port);

	connlistener = evconnlistener_new_bind(base, 
		                                   (evconnlistener_cb)OnConnAccept, 
										   NULL,
		                                   LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 
										   -1, 
										   (struct sockaddr*)&sin, 
										   sizeof(sin));
	if (!connlistener) {
		perror("evconnlistener_new_bind failed!");
	}
	printf("tcp listen on : %s, port : %d\n", ip, port);

	evconnlistener_set_error_cb(connlistener, OnConnErrorOccur);

	event_base_dispatch(base);
}

void NetworkManager::OnConnAccept(struct evconnlistener* listener, 
								  evutil_socket_t fd,
                                  struct socketaddr* address,
								  int socklen,
                                  void* ctx) {
	// We got a new connection! Set up a bufferevent for it.
	printf("one client connected\n");
	struct event_base* base = evconnlistener_get_base(listener);
	//int ret = evutil_make_socket_nonblocking(fd);
	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, OnBuffereventRead, OnBuffereventWrite, OnBuffereventArrive, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void NetworkManager::OnConnErrorOccur(struct evconnlistener* listerner, void* ctx) {
	struct event_base* base = evconnlistener_get_base(listerner);
	int err = EVUTIL_SOCKET_ERROR();
	//fprintf(stderr, "Got an error %d (%s) on the listener. "
	//	"Shutting down.\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}

void NetworkManager::OnBuffereventArrive(struct bufferevent* bev, short event, void* ctx) {
	if (event & BEV_EVENT_ERROR) {
		printf("error from bufferevent!\n");
	}

	if (event & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
		bufferevent_free(bev);
	}

	if (event & BEV_EVENT_CONNECTED) {
		printf("client connected\n");
		//write_cb(bev, NULL);
		//char msg[] = "client connected";
		//bufferevent_write(bev, msg, sizeof(msg));
		struct evbuffer* input = bufferevent_get_input(bev);
		struct evbuffer* output = bufferevent_get_output(bev);
		evbuffer_add_printf(output, "client msg : %s", "client connected");
	} else if (event & BEV_EVENT_TIMEOUT) {
		printf("client connect timeout");
	}
}

void NetworkManager::OnBuffereventRead(struct bufferevent* bev, void* ctx) {
	// This callback is invoked when there is data to read on bev.
	struct evbuffer* input = bufferevent_get_input(bev);
	struct evbuffer* output = bufferevent_get_output(bev);

	//if (my_n < 5) {
	//	my_n++;
		char buf[4096] = { 0 };
		size_t n = evbuffer_get_length(input);
		//if (evbuffer_remove(input, buf, n) > 0 ) { // read and remove
		if (evbuffer_copyout(input, buf, n) > 0 ) {  // read only
			printf("read data from client : %s\n", buf);
			//evbuffer_add_printf(output, "client msg %d", my_n);
		}
	//}
}

void NetworkManager::OnBuffereventWrite(struct bufferevent* bev, void* ctx) {
	//printf("client read_cb");
	//char msg[] = "client msg";
	//bufferevent_write(bev, msg, sizeof(msg));
}

bool NetworkManager::Init() {
	return true;
}

} // namespace gamer
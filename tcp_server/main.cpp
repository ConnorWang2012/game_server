#include "stdafx.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"

static void accept_conn_cb(struct evconnlistener* listener, evutil_socket_t fd,
	struct socketaddr* addr,
	int socklen,
	void* ctx);

static void accept_error_cb(struct evconnlistener* listerner, void* ctx);

static void event_cb(struct bufferevent* bev, short events, void* ctx);

static void read_cb(struct bufferevent* bev, void* ctx);

static void write_cb(struct bufferevent* bev, void* ctx);


int _tmain(int argc, _TCHAR* argv[]) {
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
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	//sin.sin_addr.s_addr = htonl(0);
	sin.sin_port = htons(port);

	connlistener = evconnlistener_new_bind(base, (evconnlistener_cb)accept_conn_cb, NULL,
		LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
	if (!connlistener) {
		perror("evconnlistener_new_bind failed!");
		return 1;
	}
	printf("tcp listen on : %s, port : %d", ip, port);

	evconnlistener_set_error_cb(connlistener, accept_error_cb);

	event_base_dispatch(base);

	return 0;
}


static void accept_conn_cb(struct evconnlistener* listener,
	evutil_socket_t fd,
	struct socketaddr* address,
	int socklen,
	void* ctx) {
	// We got a new connection! Set up a bufferevent for it.
	struct event_base* base = evconnlistener_get_base(listener);
	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

static void accept_error_cb(struct evconnlistener* listerner, void* ctx) {
	struct event_base* base = evconnlistener_get_base(listerner);
	int err = EVUTIL_SOCKET_ERROR();
	//fprintf(stderr, "Got an error %d (%s) on the listener. "
	//	"Shutting down.\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}

static void event_cb(struct bufferevent* bev, short events, void* ctx) {
	if (events & BEV_EVENT_ERROR) {
		printf("Error from bufferevent!");
	}

	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
		bufferevent_free(bev);
	}
}

static void read_cb(struct bufferevent* bev, void* ctx) {
	// This callback is invoked when there is data to read on bev.
	struct evbuffer* input = bufferevent_get_input(bev);
	struct evbuffer* output = bufferevent_get_output(bev);
	// Copy all the data from the input buffer to the output buffer.
	evbuffer_add_buffer(output, input);
}

static void write_cb(struct bufferevent* bev, void* ctx) {

}
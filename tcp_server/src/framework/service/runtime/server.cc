/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   server.cc
@ author:	   Connor
@ version:	   1.0.0
@ date:		   2017-04-01
@ description: start and stop server.
@ others:
@ history:
1.date:
author:
modification:
********************************************************************************/

#include "server.h"

#include "framework/network/network_manager.h"

namespace gamer {

Server* Server::GetInstance() {
	static Server s_server;
	return &s_server;
}

void Server::Start() {
	NetworkManager::GetInstance()->InitSocket();
}

void Server::Stop() {
}

void Server::ServerDidFinishLaunching() {
}

void Server::ServerWillExit() {
}

} // namespace gamer
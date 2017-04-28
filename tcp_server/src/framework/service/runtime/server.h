/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   server.h
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

#ifndef CONNOR_GAME_SRC_SERVER_H_
#define CONNOR_GAME_SRC_SERVER_H_

namespace gamer {

class Server {
  public:
	Server() = default;

	Server& operator=(const Server&) = delete;

	Server(const Server&) = delete;
	
	static Server* instance();

	void Start();

	void Stop();

	void ServerDidFinishLaunching();

	void ServerWillExit();
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_SERVER_H_
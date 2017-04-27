/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   server_protocal.h
@ author:	   Connor
@ version:	   1.0.0
@ date:		   2017-04-01
@ description: server protocal.
@ others:
@ history:
1.date:
author:
modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_SERVER_PROTOCAL_H_
#define CONNOR_GAME_SRC_SERVER_PROTOCAL_H_

namespace gamer {

class ServerProtocal {
  public:
	virtual ~ServerProtocal() {}
	
	virtual void ServerDidFinishLaunching() = 0;

	virtual void ServerWillExit() = 0;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_SERVER_PROTOCAL_H_
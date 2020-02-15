#ifndef FES_PLAYER_H
#define FES_PLAYER_H


#include <nel/net/service.h>
#include <nel/misc/mem_stream.h>
#include <server_share/game_def.h>
#include <queue>
#include <list>

class CClient
{
public:

	CClient( UID );
	~CClient() {}

	UID                     m_UID;
    std::string             m_UserName;
    std::string             m_App;
	NLMISC::TTime           m_LastRecvMsg;      ///	最后一次收到消息的时间

    NLNET::TSockId          m_SockId;

};


#endif

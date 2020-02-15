#ifndef FES_CLIENT_MGR_H
#define FES_CLIENT_MGR_H

#include <nel/misc/singleton.h>
#include "client.h"

namespace google{ namespace protobuf { class Message; } }

class CClientMgr : public NLMISC::CSingleton<CClientMgr>
{
public:

	CClientMgr();

    CClient*    FindClient( UID );
	void        RemoveClient( UID );
    CClient*    UpdateClientSockId( UID, NLNET::TSockId );

    void        ClearAllClient();
    uint32      ClientNumber()   { return _ClientMap.size(); }

    void        SendTo( UID, std::string name, google::protobuf::Message* );
    void        SendToAll( NLNET::CMessage& msg, uint32 unInterval );


private:

    typedef std::map<UID, CClient*>                 TClientMap;
    TClientMap          _ClientMap;
};

#define ClientMgr CClientMgr::instance()

#endif

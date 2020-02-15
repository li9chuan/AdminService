#include "client_mgr.h"
#include "admin_service.h"
#include <server_share/server_def.h>

using namespace NLNET;
using namespace NLMISC;
using namespace std;

CClientMgr::CClientMgr()
{
}

CClient* CClientMgr::FindClient(UID uid)
{
    if ( uid>0 )
    {
        TClientMap::iterator iter = _ClientMap.find( uid );

        if(iter != _ClientMap.end())
        {
            return iter->second;
        }
    }
    return NULL;
}

void CClientMgr::RemoveClient(UID uid)
{
    CClient* pClient = FindClient(uid);

    if ( pClient != NULL )
    {
        pClient->m_SockId->setAppId(0);
        _ClientMap.erase(uid);
        delete pClient;
    }
}

void CClientMgr::ClearAllClient()
{
    TClientMap::iterator iter = _ClientMap.begin();

    while( iter!=_ClientMap.end() )
    {
        iter->second->m_SockId->setAppId(0);
        delete iter->second;
        ++iter;
    }
    _ClientMap.clear();
}

void CClientMgr::SendTo( UID uid, std::string name, google::protobuf::Message* pMsg )
{
    CClient* pClient = FindClient(uid);

    if ( pClient != NULL )
    {
        AdminService.SendToClient(pClient->m_SockId, name, pMsg);
    }
}

void CClientMgr::SendToAll( NLNET::CMessage& msg, uint32 unInterval )
{
    //   if (unInterval > 30000)
    //   {
    //       unInterval = 30000;
    //   }
    //   
    //   TClientMap::iterator iterBegin = _ClientMap.begin();
    //   TClientMap::iterator iterEnd = _ClientMap.end();

    //while( iterBegin!=iterEnd )
    //{
    //       CClient* pClient = iterBegin->second;

    //	if(pClient != NULL)
    //	{
    //           uint32 wait_time = GetRandom(unInterval) + 1;
    //		pClient->AddBroadcastTimer( msg, wait_time, auto_resend );
    //	}

    //	++iterBegin;
    //}
}

CClient* CClientMgr::UpdateClientSockId( UID uid, NLNET::TSockId from )
{
    CClient* pClient = FindClient( uid );

    if ( pClient == NULL )
    {
        pClient = new(std::nothrow) CClient(uid);
        if ( pClient==NULL ) {
            return NULL;
        }

        if ( !_ClientMap.insert( make_pair(uid, pClient) ).second )
        {
            SAFE_DELETE(pClient);
            return NULL;
        }
    }

    from->setAppId(uid);
    pClient->m_SockId = from;
    return pClient;
}





#include "client.h"
#include "admin_service.h"
#include "client_mgr.h"
#include <server_share/server_def.h>

// for htonl
#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#elif defined NL_OS_UNIX
#	include <arpa/inet.h>
#endif

CClient::CClient( UID _uid ) : m_UID(_uid), m_LastRecvMsg(0)
{
    m_LastRecvMsg = LocalTime.GetCurrTime();
}


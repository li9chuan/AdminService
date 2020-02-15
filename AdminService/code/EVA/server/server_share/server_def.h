#ifndef SERVICE_SHARED_SERVER_DEF_H
#define SERVICE_SHARED_SERVER_DEF_H

#include <nel/net/message.h>
#include <nel/net/unified_network.h>
#include <nel/net/callback_server.h>
//#include <nel/misc/log.h>
#include <nel/misc/debug.h>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include "game_def.h"
#include "tools.h"
#include "pb/msg_client.pb.h"

//NLMISC::CLog& Loger();

#define     MaxUDPPacketSize   512

//#ifdef NL_RELEASE
//#	if defined(NL_COMP_VC) && NL_COMP_VC_VERSION >= 71
//#		define DebugLoger __noop
//#	else
//#		define DebugLoger 0&&
//#	endif
//#else // NL_RELEASE
//#	define DebugLoger Loger().displayNL
//#endif // NL_RELEASE

#define  Network NLNET::CUnifiedNetwork::getInstance()
#define  Config  NLNET::IService::getInstance()->ConfigFile

//const std::string LogicService          = "PLS";
//const std::string SchedulingServer      = "SCH";
//const std::string Persistent            = "PDS";
//const std::string FrontendService       = "FES";

/// msg name
const std::string T2C       = "T2C";
const std::string ERR       = "ERR";
const std::string DB_PUT    = "DB_PUT";
const std::string DB_GET    = "DB_GET";

inline void SendToClient( NLNET::TServiceId conFES, UID uid, std::string name, google::protobuf::Message* message )
{
    if ( NLNET::TServiceId::InvalidId == conFES || conFES.get()==0 )
    {
        nlwarning("------ %s  %d",name.c_str(), conFES.get());
        return;
    }


    NLNET::CMessage _msgout(T2C);
    _msgout.serial(uid);
    _msgout.serial(name);

    std::string msg_tpname = "";

    if ( message!=NULL )
    {
        msg_tpname = message->GetTypeName();
        _msgout.serial(msg_tpname);
        _msgout.serial(message);
    }
    else
    {
        _msgout.serial(msg_tpname);
    }

    Network->send( conFES, _msgout );
}

inline void SendERR( NLNET::TServiceId sid, UID uid, ERROR_TYPE errNO )
{
    if ( NLNET::TServiceId::InvalidId == sid || sid.get()==0 ) { return; }

    NLNET::CMessage msgout(ERR);
    msgout.serial(errNO);
    msgout.serial(uid);
    Network->send(sid, msgout);
}

inline void SendERR( const std::string& serviceName, UID uid, ERROR_TYPE errNO )
{
    NLNET::CMessage msgout(ERR);
    msgout.serial(errNO);
    msgout.serial(uid);
    Network->send(serviceName, msgout);
}

///////////////   DB
template<class T> inline T* CloneRec(const T &val) { return new(std::nothrow) T(val); }

#define  SaveToDB( id, stru, svopt ) do {\
    uint8 _opt = svopt;\
    uint64 _id = id;\
    NLNET::CMessage msg_pds( DB_PUT );\
    msg_pds.serial(stru.m_TableEnum);\
    msg_pds.serial(_opt);\
    msg_pds.serial(_id);\
    msg_pds.serial(stru);\
    Network->send( Persistent, msg_pds ); \
} while(0)

#define GetFromDB( opt , id , stru ) do{\
    DB_OPT _opt = opt;\
    uint64 _id = id;\
    NLNET::CMessage msg_pds( DB_GET );\
    msg_pds.serial(_opt);\
    msg_pds.serial( _id );\
    msg_pds.serial( stru );\
    Network->send( Persistent , msg_pds );\
}while( 0 )

#define PreProcessMsg(__MSGTYPE)\
    CClient* __pClient = ClientMgr.FindClient(from->appId());\
    if (__pClient==NULL)  { return; }\
    PB::__MSGTYPE* __pMsg = dynamic_cast<PB::__MSGTYPE*>(pMsg);\
    if ( pMsg==NULL )  { return; }\
    PB::__MSGTYPE& pbmsg = *__pMsg;\
    CClient& client = *__pClient;


#endif


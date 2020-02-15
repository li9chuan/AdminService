#ifndef ADMIN_MODULE_MGR_H
#define ADMIN_MODULE_MGR_H

#include <nel/misc/singleton.h>
#include <nel/net/buf_net_base.h>
#include <server_share/pb/msg_client.pb.h>

class CAdminModule;
class CClient;
namespace NLNET {
    class CMessage;
}

class CAdminModuleMgr : public NLMISC::CSingleton<CAdminModuleMgr>
{
public:

    CAdminModuleMgr() {}

    CAdminModule*   GetAdminModule( std::string shard, std::string name, uint32 sid, bool auto_add=false );
    CAdminModule*   GetAdminModule( std::string key );
    CAdminModule*   GetAdminModule( NLNET::TSockId );
public:
    ///     from web client
    void            ReqExecCommand( CClient&, PB::MsgExecCommand& );
    void            ReqSvrList( NLNET::TSockId );

public:
    ///     from admin modules
    void            ProcRemove( NLNET::TSockId );
    void            ProcReg( NLNET::CMessage&, NLNET::TSockId );
    void            ProcExecResult( NLNET::CMessage&, NLNET::TSockId );
    

    

public:

    std::string     GetKey( std::string& shard, std::string& name, uint32 id );


    typedef std::map<std::string, CAdminModule*>    TModules;
    TModules    _Modules;
};

#define AdminModuleMgr CAdminModuleMgr::instance()

#endif  // ADMIN_MODULE_MGR_H

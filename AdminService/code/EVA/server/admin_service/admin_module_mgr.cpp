#include "admin_module_mgr.h"
#include "nel/misc/string_common.h"
#include "admin_module.h"
#include "admin_service.h"
#include "server_share/pb/msg_client.pb.h"
#include "client_mgr.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace PB;

CAdminModule* CAdminModuleMgr::GetAdminModule(std::string shard, std::string name, uint32 sid, bool auto_add)
{
    string key = GetKey(shard, name, sid);
    TModules::iterator iter = _Modules.find(key);

    if ( auto_add && iter==_Modules.end() )
    {
        iter = _Modules.insert( make_pair(key, new CAdminModule(key, shard, name, sid)) ).first;
    }
    
    if ( iter!=_Modules.end() )
    {
        return iter->second;
    }

    return NULL;
}

CAdminModule* CAdminModuleMgr::GetAdminModule(std::string key)
{
    TModules::iterator iter = _Modules.find(key);

    if ( iter!=_Modules.end() )
    {
        return iter->second;
    }

    return NULL;
}

CAdminModule* CAdminModuleMgr::GetAdminModule(NLNET::TSockId sockid)
{
    TModules::iterator iter = _Modules.begin();

    while (iter!=_Modules.end())
    {
        if ( iter->second->sockid == sockid )
        {
            return iter->second;
        }
        ++iter;
    }
    return NULL;
}

void CAdminModuleMgr::ReqExecCommand(CClient& client, PB::MsgExecCommand& pbmsg)
{
    CMessage msg("EXEC_COMMAND");
    uint32 qms = (uint32)CTime::getLocalTime();
    msg.serial(client.m_UID);
    msg.serial(qms);
    msg.serial(*pbmsg.mutable_command());

    for ( uint32 i=0; i<pbmsg.svrlist_size(); ++i )
    {
        const MsgSvr& pbsvr = pbmsg.svrlist(i);
        CAdminModule* pModule = GetAdminModule( pbsvr.shard(), pbsvr.name(), pbsvr.sid() );

        if ( pModule!=NULL )
        {
            AdminService.SendToAdminModule(pModule->sockid, msg);
        }
    }
}

void CAdminModuleMgr::ReqSvrList(NLNET::TSockId sockid)
{
    MsgSvrList pbmsg;

    for ( TModules::iterator iter=_Modules.begin(); iter!=_Modules.end(); ++iter )
    {
        CAdminModule* pModule = iter->second;

        if (pModule!=NULL)
        {
            PB::MsgSvr* pMsgSvr = pbmsg.add_svrlist();
            pMsgSvr->set_shard( pModule->shard );
            pMsgSvr->set_name( pModule->name );
            pMsgSvr->set_sid( pModule->sid );
            pMsgSvr->set_ip( pModule->GetIP() );
        }
    }

    AdminService.SendToClient( sockid, "MsgSvrList", &pbmsg );
}

void CAdminModuleMgr::ProcReg( NLNET::CMessage& msgin, TSockId sockid )
{
    std::string     shard;
    std::string     name;
    uint32          sid;

    msgin.serial(shard);
    msgin.serial(name);
    msgin.serial(sid);

    CAdminModule* pModule = GetAdminModule(shard, name, sid, true);

    if ( pModule!=NULL )
    {
        pModule->sockid = sockid;
    }
}

void CAdminModuleMgr::ProcExecResult(NLNET::CMessage& msgin, NLNET::TSockId sockid)
{
    UID uid;
    uint32 qms;
    std::string cmd;
    std::string from;
    msgin.serial(uid);
    msgin.serial(qms);
    msgin.serial(cmd);
    msgin.serial(from);

    // retrieve the text from the input message
    CSString result;
    msgin.serial(result);

    MsgExecCommand pbmsg;

    uint32 runms = (uint32)CTime::getLocalTime();
    runms -= qms;
    pbmsg.set_runms(runms);
    pbmsg.set_command(cmd);
    pbmsg.set_result(result);
    pbmsg.set_from(from);

    ClientMgr.SendTo(uid, "MsgExecCommand", &pbmsg);
}

void CAdminModuleMgr::ProcRemove(NLNET::TSockId sockid)
{
    TModules::iterator iter = _Modules.begin();

    while (iter!=_Modules.end())
    {
        if ( iter->second->sockid == sockid )
        {
            delete iter->second;
            _Modules.erase(iter);
            break;
        }
        ++iter;
    }
}

std::string CAdminModuleMgr::GetKey(std::string& shard, std::string& name, uint32 sid)
{
    return shard+"@"+name+"["+toString(sid)+"]";
}


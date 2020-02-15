#include "msg_as.h"
#include "callback_wsserver.h"
#include "admin_module_mgr.h"
#include "client_mgr.h"
#include "admin_service.h"
#include "server_share/pb/msg_client.pb.h"
#include "server_share/server_def.h"

using namespace NLMISC;
using namespace NLNET;

void cbAdminModuleDisConnect(NLNET::TSockId from, void *arg)
{
    AdminModuleMgr.ProcRemove(from);
}

void cbAdminModuleReg(NLNET::CMessage &msgin, TSockId from, NLNET::CCallbackNetBase &netbase)
{
    AdminModuleMgr.ProcReg(msgin, from);
}

void cbAdminModuleExecResult(NLNET::CMessage &msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase)
{
    AdminModuleMgr.ProcExecResult(msgin, from);
}


//////////////////////////////////   client msg

void cbSvrList(google::protobuf::Message* pMsg, NLNET::TSockId from)
{
    AdminModuleMgr.ReqSvrList(from);
}

void cbSvrExecCommand(google::protobuf::Message* pMsg, NLNET::TSockId from)
{
    PreProcessMsg(MsgExecCommand);
    AdminModuleMgr.ReqExecCommand(client, pbmsg);
}

NLMISC::CVariable<uint32>	VAR_UID("as", "UID", "memo", 1);

void cbWssClientConnect( NLNET::TSockId from, void *arg )
{
    nlinfo("cbWssClientConnect %s", from->asString().c_str());
    nlinfo("cbWssClientConnect %s", from->getTcpSock()->remoteAddr().asString().c_str());
    VAR_UID = VAR_UID + 1;
    ClientMgr.UpdateClientSockId(VAR_UID.get(), from);
}

void cbWssClientDisConnect( NLNET::TSockId from, void *arg )
{
    nlinfo("cbWssClientDisConnect %s", from->asString().c_str());
    nlinfo("cbWssClientDisConnect %s", from->getTcpSock()->remoteAddr().asString().c_str());

    ClientMgr.RemoveClient(from->appId());
}









#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H
#include "admin_service.h"
#include "msg_as.h"
#include <server_share/server_def.h>
#include <server_share/timer.h>
#include <server_share/client_msg_desc.h>
#include <nel/misc/window_displayer.h>
#include <nel/net/naming_client.h>

#ifdef NL_OS_WINDOWS
#include <Windows.h>
#endif

using namespace std;
using namespace NLMISC;
using namespace NLNET;

static TCallbackItem AdminModuleCallbackArray[] =
{
    { "REG",                    cbAdminModuleReg },
    { "EXEC_COMMAND_RESULT",    cbAdminModuleExecResult }
};

void CAdminService::init()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );
    TimerManager->init();
    MsgDesc.LoadMsgXml();

    m_CallbackAdminModule.init(10397);
    m_CallbackAdminModule.addCallbackArray(AdminModuleCallbackArray, sizeof(AdminModuleCallbackArray)/sizeof(NLNET::TCallbackItem));
    m_CallbackAdminModule.setDisconnectionCallback( cbAdminModuleDisConnect, NULL );

    m_CallbackWebSocket.Init(10390);
    m_CallbackWebSocket.setConnectionCallback( cbWssClientConnect, NULL );
    m_CallbackWebSocket.setDisconnectionCallback( cbWssClientDisConnect, NULL );
    m_CallbackWebSocket.addCallback("MsgSvrList", cbSvrList);
    m_CallbackWebSocket.addCallback("MsgExecCommand", cbSvrExecCommand);
}

bool CAdminService::update()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );
    TimerManager->tickUpdate();
    m_CallbackWebSocket.Update();
    m_CallbackAdminModule.update2();
    return true;
}

void CAdminService::release()
{
    TimerManager->release();
    m_CallbackWebSocket.Release();
    google::protobuf::ShutdownProtobufLibrary();
}

void CAdminService::SendToClient(NLNET::TSockId sid, std::string msgtype, google::protobuf::Message* pMessage)
{
    m_CallbackWebSocket.Send(sid, msgtype, pMessage);
}

void CAdminService::SendToAdminModule(NLNET::TSockId sid, CMessage& msgout)
{
    m_CallbackAdminModule.send(msgout, sid, false);
}

NLMISC_COMMAND(info, "display information about this service", "")
{

    return true;
}

/****************************************************************************   
 * FRONTEND SERVICE MAIN Function
 *
 * This call create a main function for a service:
 *
 *    - based on the "CAdminService" class
 *    - having the short name "AS"
 *    - having the long name "frontend_service"
 *    - listening on the port "0" (dynamically determined)
 *    - and shard callback set to "CallbackArray"
 *
 ****************************************************************************/
NLNET_SERVICE_MAIN (CAdminService, "AS", "admin_service", 0, EmptyCallbackArray, "", "")
/* end of file */


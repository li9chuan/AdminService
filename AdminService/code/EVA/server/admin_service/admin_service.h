#ifndef FRONTEND_SERVICE_H
#define FRONTEND_SERVICE_H

#include "callback_wsserver.h"
#include <nel/net/service.h>
#include <nel/net/callback_server_tcp.h>
#include <vector>
#include <string>

class CAdminService : public NLNET::IService
{
public:

    CAdminService() {}

    /// Return the instance of the service
    static CAdminService *instance() { return (CAdminService*)IService::getInstance(); }

    // Initialisation
    void init();
    bool update();
    void release();

    void SendToClient(NLNET::TSockId, std::string msgtype, google::protobuf::Message* pMessage=NULL);
    void SendToAdminModule( NLNET::TSockId, NLNET::CMessage& msgout );

    CCallbackWSJson             m_CallbackWebSocket;
    NLNET::CCallbackServerTcp   m_CallbackAdminModule;
};

#define  AdminService  (*(CAdminService*)IService::getInstance())

#endif

#ifndef MSG_ADMIN_SERVICE_H
#define MSG_ADMIN_SERVICE_H

#include "nel/net/buf_net_base.h"

namespace NLNET {
    class CMessage;
    class CCallbackNetBase;
}

///////     admin module msg
void cbAdminModuleDisConnect( NLNET::TSockId from, void *arg );
void cbAdminModuleReg (NLNET::CMessage &msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);
void cbAdminModuleExecResult (NLNET::CMessage &msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);

///////     client msg
namespace google {
    namespace protobuf {
        class Message;
    }
}

void cbWssClientConnect( NLNET::TSockId from, void *arg );
void cbWssClientDisConnect( NLNET::TSockId from, void *arg );


void cbSvrList(google::protobuf::Message* pMsg, NLNET::TSockId from);
void cbSvrExecCommand(google::protobuf::Message* pMsg, NLNET::TSockId from);


#endif      // MSG_ADMIN_SERVICE_H


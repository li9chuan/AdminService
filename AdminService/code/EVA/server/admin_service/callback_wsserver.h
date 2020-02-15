#ifndef FES_CALLBACK_SERVER_WEBSOCKET_SERVER_H
#define FES_CALLBACK_SERVER_WEBSOCKET_SERVER_H

#include "nel/misc/types_nl.h"
#include "nel/net/callback_net_base.h"
#include "nel/net/buf_server.h"
#include "nel/net/buf_server_websocket.h"

class CCallbackWSJson;
typedef void(*TJsonMsgCallback) (google::protobuf::Message* pMsg, NLNET::TSockId from);

class CCallbackWSJson : public NLNET::CBufServerWebsocket
{
public:

    void    Init(uint16 port);
    void    Update();
    void    Release() { if(_ZlibBuff!=NULL){ free(_ZlibBuff); _ZlibBuff=NULL; } }

    /**	Appends callback array with the specified array. You can add callback only *after* adding the server or the client.
     * \param arraysize is the number of callback items.
     */
    void	addCallback(std::string msgtype, TJsonMsgCallback callback) { m_Callbacks[msgtype] = callback; }

    /// Sets default callback for unknown message types
    void	setDefaultCallback(TJsonMsgCallback defaultCallback) { _DefaultCallback = defaultCallback; }

    void    Send(NLNET::TSockId, std::string msgtype, google::protobuf::Message* pMessage=NULL);

private:

    /// Read a message from the network and process it
    void    processOneMessage();
    void    __Callback(NLNET::TSockId tsid, NLMISC::CMemStream& msgin, google::protobuf::Message* pMessage);

    /// Returns the sockid (cf. CCallbackClient)
    virtual NLNET::TSockId	getSockId(NLNET::TSockId hostid = NLNET::InvalidSockId);

private:
    
    uint64	                    _BytesSent, _BytesReceived;
    std::string                 _MsgType;
    std::string                 _JsonStr;
    NLMISC::CMemStream          _mstream;
    uint8*                      _ZlibBuff;
    static const uint32         ZLIB_BUFF_SIZE = 1024*512;

    // called if the received message is not found in the callback array
    TJsonMsgCallback            _DefaultCallback;

    // If not null, called before each message is dispached to it's callback
    TJsonMsgCallback		    _PreDispatchCallback;

    typedef std::map<std::string, TJsonMsgCallback>     TMsgCallBackMap;
    TMsgCallBackMap         m_Callbacks;
};


#endif// FES_CALLBACK_SERVER_WEBSOCKET_SERVER_H

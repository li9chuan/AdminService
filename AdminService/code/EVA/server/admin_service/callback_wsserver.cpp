#include "callback_wsserver.h"
#include "client_mgr.h"
#include <nel/net/service.h>
#include <google/protobuf/util/json_util.h>

#include <server_share/json/reader.h>
#include <server_share/client_msg_desc.h>
#include <server_share/server_def.h>
#include <server_share/json/writer.h>
#include <server_share/pb/msg_client.pb.h>
#include "zlib.h"

using namespace NLMISC;
using namespace NLNET;
using namespace google;

void CCallbackWSJson::Init(uint16 port)
{
    _BytesSent      = 0;
    _BytesReceived  = 0;

    _DefaultCallback        = NULL;
    _PreDispatchCallback    = NULL;
    _ZlibBuff               = (uint8*)malloc(sizeof(uint8) * ZLIB_BUFF_SIZE);

    
    if (Config.getVarPtr("SslCA")!=NULL)
    {
        std::string ca  = CPath::lookup(Config.getVar("SslCA").asString());
        std::string crt = CPath::lookup(Config.getVar("SslCrt").asString());
        std::string pk  = CPath::lookup(Config.getVar("SslPrivateKey").asString());

        setupSsl(ca, crt, pk);
    }

    init(port);
}

void CCallbackWSJson::Update()
{
    while (dataAvailable())
    {
        processOneMessage();
    }
}


void CCallbackWSJson::processOneMessage()
{
    H_AUTO(CCallbackWebSocket_processOneMessage)

    std::string buff;
    _MsgType = "";
    protobuf::Message* pMessage = NULL;
    NLNET::TSockId tsid;
    NLMISC::CMemStream msgin("");

    try
    {
        receive(msgin, &tsid);
        _BytesReceived += msgin.length();

        uint16 size = 0;
        msgin.serial(size);

        buff.resize(size);
        msgin.serialBuffer((uint8 *)&buff[0], size);

        Json::Value     jval;
        Json::Reader    jreader;

        if (jreader.parse(buff, jval))
        {
            _MsgType = jval["msgtype"].asString();

            if (!_MsgType.empty())
            {
                MsgLeaf* pLeaf = MsgDesc.GetMsgLeaf(_MsgType);
                if ( pLeaf==NULL || pLeaf->sendto.size()==0 ) { return; }

                /// callback
                if (pLeaf->format_msg.size()>0)
                {
                    pMessage = MsgDesc.CreateMessage(pLeaf->format_msg[0]);
                }

                if (pMessage != NULL)
                {
                    protobuf::util::JsonParseOptions jsonopt;
                    jsonopt.ignore_unknown_fields = true;

                    if ( protobuf::util::JsonStringToMessage(buff, pMessage, jsonopt)!=protobuf::util::Status::OK )
                    {
                        nlwarning("JsonStringToMessage:%s", buff.c_str());
                        SAFE_DELETE(pMessage);
                        return;
                    }
                }

                if (pLeaf->sendto[0] == "AS")
                {
                    __Callback(tsid, msgin, pMessage);
                }
                else if( tsid->appId()>0 )
                {
                    UID uid = tsid->appId();
                    CMessage msg(_MsgType);
                    msg.serial(uid);
                    if ( pMessage!=NULL ) { msg.serial(pMessage); }
                    Network->send( pLeaf->sendto[0], msg, false );
                }

                SAFE_DELETE(pMessage);
            }
        }

        // {"msgtype":"TkList"}
    }
    catch (const Exception &e)
    {
        nlwarning(e.what());
        SAFE_DELETE(pMessage);
    }
}

void CCallbackWSJson::__Callback(NLNET::TSockId tsid, NLMISC::CMemStream& msgin, protobuf::Message* pMessage)
{
    if (_MsgType.size() > 0)
    {
        TJsonMsgCallback    cb = NULL;
        TMsgCallBackMap::iterator iter = m_Callbacks.find(_MsgType);
        TSockId realid = getSockId(tsid);

        if (iter != m_Callbacks.end())
        {
            cb = iter->second;
            cb(pMessage, realid);
        }
    }
}

TSockId CCallbackWSJson::getSockId(TSockId hostid)
{
    nlassert(hostid != InvalidSockId);	// invalid hostid
    //nlassert(connected());
    nlassert(hostid != NULL);
    return hostid;
}

void CCallbackWSJson::Send(NLNET::TSockId sid, std::string msgtype, protobuf::Message* pMessage)
{
    H_AUTO(CCallbackWSJson_Send);
    uLongf destLen = ZLIB_BUFF_SIZE;

    if (pMessage != NULL)
    {
        _JsonStr = "";
        protobuf::util::JsonOptions jsonopt;
        jsonopt.add_whitespace = false;
        jsonopt.always_print_primitive_fields = true;
        jsonopt.always_print_enums_as_ints = true;
        jsonopt.preserve_proto_field_names = true;
        protobuf::util::Status status = MessageToJsonString(*pMessage, &_JsonStr, jsonopt);

        if (status.ok())
        {
            if ( _JsonStr.size()<=2 )
            {
                _JsonStr = "{\"msgtype\":\""; _JsonStr.append(msgtype); _JsonStr.append("\"}");
            }
            else
            {
                _JsonStr[_JsonStr.size()-1] = ',';
                _JsonStr.append("\"msgtype\":\""); _JsonStr.append(msgtype); _JsonStr.append("\"}");
            }

            if ( compress2( (Bytef*)_ZlibBuff, &destLen, (Bytef*)_JsonStr.c_str(), _JsonStr.size(), 5) == Z_OK )
            {
                _mstream.clear();
                _mstream.serialBuffer(_ZlibBuff, destLen);
                send(_mstream, sid);
            }
        }
    }
    else
    {
        _JsonStr = "{\"msgtype\":\""; _JsonStr.append(msgtype); _JsonStr.append("\"}");

        if ( compress2( (Bytef*)_ZlibBuff, &destLen, (Bytef*)_JsonStr.c_str(), _JsonStr.size(), 5) == Z_OK )
        {
            _mstream.clear();
            _mstream.serialBuffer(_ZlibBuff, destLen);
            send(_mstream, sid);
        }
    }
}

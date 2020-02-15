#ifndef _ADMIN_MODULE_H
#define _ADMIN_MODULE_H

#include <nel/misc/types_nl.h>
#include <nel/net/buf_net_base.h>

class CAdminModule
{
public:

	CAdminModule( std::string _key, std::string _shard, std::string _name, uint32 _sid ):
        key(_key), shard(_shard), name(_name), sid(_sid)
    {}
	~CAdminModule() {}

    std::string     GetIP() { return sockid->getTcpSock()->remoteAddr().ipAddress(); }
public:
    std::string     key;
    std::string     shard;
    std::string     name;
    uint32          sid;
    NLNET::TSockId  sockid;
};

#endif  // _ADMIN_MODULE_H

// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef NL_ADMIN_MODULE_H
#define NL_ADMIN_MODULE_H

#include <nel/misc/singleton.h>
#include <nel/net/callback_net_base.h>

namespace NLNET {
class CCallbackClient;
class CMessage;

class CAdminModule : public NLMISC::CSingleton<CAdminModule>
{
public:

    CAdminModule();

    void initAdmin ( std::string host, std::string shard, std::string name, uint32 svrid );
    void updateAdmin ();
    void releaseAdmin ();
    void sendMsg ( const CMessage& );
    void addCallbackArray (const TCallbackItem *callbackarray, sint arraysize);
    
public:
    void sendMsgRegister();         /// ÏòAdminService×¢²á×Ô¼º

public:
    void procExecCommand( CMessage& );

private:
    std::string             _Host;
    std::string             _Shard;
    std::string             _Name;
    uint32                  _SvrId;
    uint32                  _Lastime;

    NLNET::CCallbackClient *_Client;
    NLMISC::CLog            _CommandLog;
};

} // NLNET

#define AdminModule NLNET::CAdminModule::instance()

#endif // NL_ADMIN_MODULE_H



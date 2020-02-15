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

#include "stdnet.h"
#include "nel/net/callback_client.h"
#include "nel/net/admin_module.h"
#include "nel/net/varpath.h"
#include "nel/net/admin_callback.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;

TCallbackItem AdminModuleCbArray[] =
{
    { "EXEC_COMMAND", cbAdminExecCommand },
};

CAdminModule::CAdminModule() : _Lastime(0)
{
    _Client = new CCallbackClient();
    _Client->addCallbackArray( AdminModuleCbArray, sizeof(AdminModuleCbArray)/sizeof(NLNET::TCallbackItem) );
}

void CAdminModule::addCallbackArray(const TCallbackItem *callbackarray, sint arraysize)
{
    _Client->addCallbackArray(callbackarray, arraysize);
}

void CAdminModule::initAdmin(std::string host, std::string shard, std::string name, uint32 svrid)
{
    _Host   = host;
    _Shard  = shard;
    _Name   = name;
    _SvrId  = svrid;
}

void CAdminModule::sendMsg( const CMessage& msgout )
{
    if (_Client != NULL && _Client->connected ())
    {
        _Client->send( msgout );
    }
}

void CAdminModule::sendMsgRegister()
{
    CMessage msgout ("REG");
    msgout.serial (_Shard);
    msgout.serial (_Name);
    msgout.serial (_SvrId);

    sendMsg (msgout);
}

void CAdminModule::updateAdmin()
{
    if (_Client != NULL && _Client->connected ())
    {
        _Client->update ();
    }
    else if ( !_Host.empty() )
    {
        uint32 curr_sec = CTime::getSecondsSince1970();
        if ( curr_sec - _Lastime > 10 )
        {
            _Lastime = curr_sec;

            try
            {
                _Client->connect( NLNET::CInetAddress( _Host ) );
                sendMsgRegister();
            }
            catch (const ESocketConnectionFailed &)
            {
                nlwarning ("HNETL5: Could not connect to the Admin Service (%s). Retrying in a few seconds...", _Host.c_str());
            }
        }
    }
}

void CAdminModule::releaseAdmin()
{
    if ( _Client )
        _Client->disconnect ();
        delete _Client;
    _Client = NULL;
}

void CAdminModule::procExecCommand(CMessage& msgin)
{
    // create a displayer to gather the output of the command
    class CStringDisplayer: public IDisplayer
    {
    public:
        void serial(NLMISC::IStream &stream)
        {
            stream.serial(_Data);
        }

    protected:
        virtual void doDisplay( const CLog::TDisplayInfo& /* args */, const char *message)
        {
            _Data += message;
        }

        std::string _Data;
    };
    CStringDisplayer stringDisplayer;
    _CommandLog.addDisplayer(&stringDisplayer);

    // retreive the command from the input message and execute it
    uint32 session;
    uint32 qms;
    string command;
    msgin.serial (session);
    msgin.serial (qms);
    msgin.serial (command);
    nlinfo ("ADMIN: Executing command from network : '%s'", command.c_str());
    ICommand::execute (command, _CommandLog, true);

    // unhook our displayer as it's work is now done
    _CommandLog.removeDisplayer(&stringDisplayer);

    std::string from = _Shard+"@"+_Name+"["+toString(_SvrId)+"]";

    // send a reply message to the admin service
    CMessage msgout("EXEC_COMMAND_RESULT");
    msgout.serial (session);
    msgout.serial (qms);
    msgout.serial (command);
    msgout.serial (from);
    stringDisplayer.serial(msgout);
    sendMsg(msgout);
}

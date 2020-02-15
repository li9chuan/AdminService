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

#include "nel/misc/log.h"
#include "nel/misc/string_common.h"
#include "nel/misc/command.h"
#include "nel/misc/hierarchical_timer.h"
#include "nel/misc/debug.h"
#include "nel/misc/window_displayer.h"
#include "nel/misc/gtk_displayer.h"
#include "nel/misc/win_displayer.h"
#include "nel/misc/stdin_monitor_thread.h"
#include "nel/net/admin_module.h"
#include "nel/misc/sstring.h"

using namespace NLMISC;
using namespace std;

static bool isExit = false;

#if defined(NL_OS_WINDOWS) && defined(_WINDOWS)
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char **argv)
#endif
{
    //NLMISC::CApplicationContext	serviceContext;
    //NLMISC::createDebug(NULL, false);
    CVectorSString vct;

#if defined(NL_OS_WINDOWS) && defined(_WINDOWS)
    CSString cmd = lpCmdLine;
    cmd.splitBySeparator(' ', vct);
#else
    vct.push_back("admin.mulanshanzhuang.com:10397");
    vct.push_back("us");
    vct.push_back("proxy");
    vct.push_back("1");
#endif

    AdminModule.initAdmin( vct[0], vct[1], vct[2], vct[3].atoi() );

    CHTimer::startBench(false, true);
    CHTimer::endBench();

#ifdef NL_OS_WINDOWS
    CWindowDisplayer* WindowDisplayer = new CWinDisplayer ("DEFAULT_WD");
#endif // NL_OS_WINDOWS

    if ( WindowDisplayer!=NULL )
    {
        WindowDisplayer->create ( "*INIT* AdminModule Sample.", false, -1, -1, -1, -1, -1, 0, "", false);
        WindowDisplayer->setTitleBar ("App version 1.0.0");
        WindowDisplayer->createLabel ("@Measure|displayMeasures");
        WindowDisplayer->createLabel ("@Quit|quit");

        INelContext::getInstance().getInfoLog()->addDisplayer(WindowDisplayer, true);
        INelContext::getInstance().getWarningLog()->addDisplayer(WindowDisplayer, true);
    }
    else
    {
        IStdinMonitorSingleton::getInstance()->init();
    }

    //ICommand::execute("displayMeasures", *INelContext::getInstance().getInfoLog());

    while ( !isExit )
    {
        CHTimer::startBench(false, true, false);

        if ( WindowDisplayer!=NULL )
        {
            WindowDisplayer->update ();
        }
        else
        {
            H_AUTO(NLNETStdinMonitorUpdate);
            IStdinMonitorSingleton::getInstance()->update();
        }

        AdminModule.updateAdmin();

        nlSleep(10);
        CHTimer::endBench();
    }

    AdminModule.releaseAdmin();
    // remove the stdin monitor thread
    IStdinMonitorSingleton::getInstance()->release(); // does nothing if not initialized
    CHTimer::clear();
    return EXIT_SUCCESS;
}

//NLMISC_CATEGORISED_COMMAND(nel, quit, "exit the service", "")
NLMISC_COMMAND(quit, "exit the service", "")
{
    if(args.size() != 0) return false;

    isExit = true;
    log.displayNL("User ask me with a command to quit");
    return true;
}


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

#ifndef NL_ADMIN_CALLBACK_H
#define NL_ADMIN_CALLBACK_H

#include "nel/net/buf_net_base.h"

namespace NLNET {
class CMessage;
class CCallbackNetBase;

void cbAdminDisconnect (TSockId from, void *arg);
void cbAdminRegister (CMessage &msgin, TSockId from, CCallbackNetBase &netbase);
void cbAdminExecCommand (CMessage &msgin, TSockId from, CCallbackNetBase &netbase);

} // NLNET

#endif // NL_ADMIN_CALLBACK_H



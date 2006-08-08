/**************************************************************************
 ** Dynamic Networking Solutions                                         **
 **************************************************************************
 ** pBurst, Internet Relay Chat Protocol Burster                         **
 ** Copyright (C) 1999 Gregory A. Carter                                 **
 **                    Dynamic Networking Solutions                      **
 **                                                                      **
 ** This program is free software; you can redistribute it and/or modify **
 ** it under the terms of the GNU General Public License as published by **
 ** the Free Software Foundation; either version 1, or (at your option)  **
 ** any later version.                                                   **
 **                                                                      **
 ** This program is distributed in the hope that it will be useful,      **
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of       **
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        **
 ** GNU General Public License for more details.                         **
 **                                                                      **
 ** You should have received a copy of the GNU General Public License    **
 ** along with this program; if not, write to the Free Software          **
 ** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            **
 **************************************************************************
 $Id$
 **************************************************************************/

#include <list>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <new>
#include <iostream>
#include <string>
#include <exception>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "StringToken.h"
#include "Command.h"
#include "Server.h"

#include "stdinCommands.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using namespace std;

/**************************************************************************
 ** stdinACTION Class                                                    **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/

const int stdinACTION::Execute(Packet ePacket) {
  string eTarget;			// target
  string eMessage;			// message

  aServer->writeLog(PBURST_LOG_DEBUG, "stdinACTION::Execute> /ACTION received.");

  if (ePacket.getArguments().size() < 2)
    return CMDERR_SYNTAX;

  // initialize variables
  eTarget = ePacket.getArguments()[0];
  eMessage = ePacket.getArguments().getRange(1, ePacket.getArguments().size());

  aServer->Send("%s P %s :\001ACTION %s\001", (PBURST_CONFIG_SERVER_NUMERIC).c_str(), eTarget.c_str(), eMessage.c_str());
  return 1;
} // stdinACTION::Execute

}

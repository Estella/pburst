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

#include "msgCommands.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using namespace std;

/**************************************************************************
 ** msgSERVER Class                                                      **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/
// SERVER irc.netburst.org 1 849427169 1056824323 J10 MJAf] +h :TestNet Server\r\n
const int msgSERVER::Execute(Packet ePacket) {
  string eNumnick;			// server numeric nick

  aServer->writeLog(PBURST_LOG_DEBUG, "msgSERVER::Execute> Server received.");

  if (ePacket.getArguments().size() < 7)
    return CMDERR_SYNTAX;

  // initialize variables
  eNumnick = ePacket.getArguments()[5];

  if (eNumnick.length() != 5)
    return CMDERR_SYNTAX;

  UnsetServerPreAuth(aServer);
  aServer->setUplink(eNumnick.substr(0, 2));

  return 1;
} // msgSERVER::Execute

}

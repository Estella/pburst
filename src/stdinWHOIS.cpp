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

#include "stdinCommands.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using namespace std;

/**************************************************************************
 ** stdinWHOIS Class                                                     **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/

const int stdinWHOIS::Execute(Packet ePacket) {
  aServer->writeLog(PBURST_LOG_DEBUG, "stdinWHOIS::Execute> /WHOIS received.");

  return 1;
} // stdinWHOIS::Execute

}

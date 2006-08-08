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
 ** msgVERSION Class                                                     **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/

// MJAG9 V :ZX\r\n
// MJ 351 AKAAA u2.10.11.04. irc.netburst.org :B27AeEFfHIKMpRSU\r\n
const int msgVERSION::Execute(Packet ePacket) {
  string eTarget;			// target of the version
  string eSource;			// source of the version

  aServer->writeLog(PBURST_LOG_DEBUG, "msgVERSION::Execute> Version received.");

  if (ePacket.getArguments().size() < 1)
    return CMDERR_SYNTAX;

  // initialize variables
  eSource = ePacket.getSource();
  eTarget = ePacket.getArguments()[0];

  aServer->Send("%s 351 %s v%s %s :%s", (PBURST_CONFIG_SERVER_NUMERIC).c_str(), eSource.c_str(), VERSION, (PBURST_CONFIG_SERVER_NAME).c_str(), (PBURST_CONFIG_VERSION_REPLY).c_str());
  return 1;
} // msgVERSION::Execute

}

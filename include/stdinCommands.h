/**************************************************************************
 ** Dynamic Networking Solutions                                         **
 **************************************************************************
 ** pBurst, Internet Relay Chat Protocol Burster                         **
 ** Copyright (C) 1999 Gregory A. Carter                                 **
 **                    Daniel Robert Karrels                             **
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

#ifndef __STDINCOMMANDS_H
#define __STDINCOMMANDS_H

#include <map>
#include <string>
#include <list>

#include <time.h>
#include <sys/time.h>

#include "StringToken.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using std::string;
using std::map;
using std::list;
using std::pair;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

CREATE_COMMAND(stdinACTION);
CREATE_COMMAND(stdinECHO);
CREATE_COMMAND(stdinHELP);
CREATE_COMMAND(stdinLOAD);
CREATE_COMMAND(stdinQUIT);
CREATE_COMMAND(stdinSAY);
CREATE_COMMAND(stdinWALLOPS);
CREATE_COMMAND(stdinWHOIS);

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

}
#endif

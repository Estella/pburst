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
 **************************************************************************
 $Id$
 **************************************************************************/

#ifndef __PBURST_MISC_H
#define __PBURST_MISC_H

#include <string>

namespace pburst {

using std::string;
using std::map;
using std::list;
using std::pair;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class noCaseCompare {
  public:
  inline bool operator()(const string &compareString1, const string &compareString2) const
    { return (strcasecmp(compareString1.c_str(), compareString2.c_str()) < 0); } 
};

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

}
#endif

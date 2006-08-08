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

#include <cstdlib>
#include <cstring>

#include <time.h>
#include <unistd.h>

#include "Options.h"

#include "pburst.h"

namespace pburst {

/******************
 ** Constructors **
 ******************/
Option::Option() {
} // Option

Option::~Option() {
} // Option

/*****************
 ** Destructors **
 *****************/
OptionsList::OptionsList() {
} // OptionsList::OptionsList

OptionsList::~OptionsList() {
} // OptionsList::~OptionsList

/*********************
 ** Options Members **
 *********************/
const bool OptionsList::addOption(const string addName, const int addInt, const bool addBool, const string addString) {
  Option aOption;                           // option
  optionMapType::iterator ptr;              // pointer to a map
      
  // is there already an option by that name?
  ptr = optionList.find(addName);
  if (ptr != optionList.end()) {
    // update that options
    ptr->second.setInteger(addInt);
    ptr->second.setBoolean(addBool);
    ptr->second.setString(addString);

    return false;
  } // if
  
  // initialize variables
  aOption.setInteger(addInt);
  aOption.setBoolean(addBool);
  aOption.setString(addString);
    
  optionList.insert(pair<string, Option>(addName, aOption));
   
  return true;
} // OptionsList::addOption

const bool OptionsList::removeOption(const string &removeMe) {
  optionMapType::iterator ptr;              // pointer to a map

  ptr = optionList.find(removeMe);
  if (ptr == optionList.end())
    return false;

  optionList.erase(ptr);

  return true;
} // OptionsList::removeOption

const int OptionsList::clearOptions() {
  unsigned int optionCount;         // number of options cleared
      
  optionCount = optionList.size(); 
        
  optionList.clear();
    
  return optionCount;
} // OptionsList::clearOptions

}

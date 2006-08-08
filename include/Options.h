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

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <string>
#include <map>

#include <time.h>
#include <sys/time.h>


#include "pburst.h"

namespace pburst {

using std::string;
using std::map;
using std::pair;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

#define PBURST_OPTION_DEFAULT_INT				0
#define PBURST_OPTION_DEFAULT_STRING				""
#define PBURST_OPTION_DEFAULT_BOOL				false

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class Option {
  public:

    Option();
    virtual ~Option();

    /*********************
     ** Boolean Members **
     *********************/

    const bool getBoolean() const
      { return myBoolean; }

    void setBoolean(const bool setMe)
      { myBoolean = setMe; }

    /*********************
     ** Integer Members **
     *********************/

    const int getInteger() const
      { return myInteger; }

    void setInteger(const int setMe)
      { myInteger = setMe; }

    /********************
     ** String Members **
     ********************/

    const string &getString() const
      { return myString; }

    void setString(const string &setMe)
      { myString = setMe; }

  protected:
    bool myBoolean;			// boolean value
    int myInteger;			// my integer
    string myString;			// my string
};

class OptionsList {
  public:
    OptionsList();
    virtual ~OptionsList();

    typedef map<string, Option> optionMapType;

    /********************
     ** Option Members **
     ********************/

    const bool addOption(const string, const int, const bool, const string);
    const bool removeOption(const string &);

    const int clearOptions();

    const int getOptionInt(const string &findMe) {
      optionMapType::iterator ptr;		// pointer to a map

      ptr = optionList.find(findMe);
      if (ptr == optionList.end())
        return PBURST_OPTION_DEFAULT_INT;

      return ptr->second.getInteger();
    } // getOption

    const bool getOptionBool(const string &findMe) {
      optionMapType::iterator ptr;		// pointer to a map

      ptr = optionList.find(findMe);
      if (ptr == optionList.end())
        return PBURST_OPTION_DEFAULT_BOOL;

      return ptr->second.getBoolean();
    } // getOption

    const string getOptionString(const string &findMe) {
      optionMapType::iterator ptr;		// pointer to a map

      ptr = optionList.find(findMe);
      if (ptr == optionList.end())
        return PBURST_OPTION_DEFAULT_STRING;

      return ptr->second.getString();
    } // getOption

    const unsigned int optionSize() const
    { return optionList.size(); }

    // iterators!
    inline optionMapType::iterator optionBegin()
      { return optionList.begin(); }

    inline optionMapType::iterator optionEnd()
      { return optionList.end(); }

  private:
    optionMapType optionList;		// option list

  protected:
};

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

}
#endif

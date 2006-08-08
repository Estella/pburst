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

#ifndef __PBURST_STRINGTOKEN_H
#define __PBURST_STRINGTOKEN_H

#include <vector>
#include <string>

#include <time.h>
#include <sys/time.h>

namespace pburst {

using std::string;
using std::vector;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

#define STRINGTOKEN_DELIMITER	' '

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class StringToken {
  private:
    typedef vector<string> stringTokenListType;

  public:
    typedef stringTokenListType::size_type size_type;

    StringToken();				// constructor
    StringToken(const string &);		// constructor
    StringToken(const string &, const char);	// constructor
    virtual ~StringToken();			// destructor


    /*********************
     ** All Get Members **
     *********************/

    // get a range of tokens in one string
    const string &getRange(const size_type, const size_type);

    // return string
    const string &getToken(const size_type) const;

    // return token count
    inline const size_type size() const
      { return stringList.size(); }

    /*********************
     ** All Set Members **
     *********************/

    // set delim
    void setDelimiter(const char setMe)
      { myDelimiter = setMe; }

    // set myString
    virtual StringToken &operator=(const string &);
    // return string
    virtual const string &operator[](const size_type) const;

  private:
    char myDelimiter;			// normally a space
    string myString;			// temporariry string to parse

    vector<string> stringList;		// list of strings

    void add(const string &);			// add a string to list
    void clear();				// clear string list
    void tokenize();			// save data into vector

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

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

#include <vector>
#include <new>
#include <cassert>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "StringToken.h"

namespace pburst {

using std::string;
using std::vector;

StringToken::StringToken() {
  // set default
  myDelimiter = STRINGTOKEN_DELIMITER;
}

StringToken::StringToken(const string &parseMe) {
  // set the string we want to parse
  myString = parseMe;
  myDelimiter = STRINGTOKEN_DELIMITER;

  tokenize();
}

StringToken::StringToken(const string &parseMe, char splitMe) {
  // set the string we want to parse
  myString = parseMe;
  myDelimiter = splitMe;

  tokenize();
}

StringToken::~StringToken() {
}

StringToken &StringToken::operator=(const string &parseMe) {
  // set the string we want to parse
  myString = parseMe;

  tokenize();

  return *this;
}

const string &StringToken::operator[](const size_type myID) const {
  assert(myID <= size());

  return stringList[myID];
}

const string &StringToken::getToken(const size_type myID) const {
  assert(myID <= size());

  return stringList[myID];
}

void StringToken::add(const string &addMe) {
  stringList.push_back(addMe);

  return;
}

void StringToken::clear() {
  stringList.clear();
  return;
}

void StringToken::tokenize() {
  string::size_type pos;		// position in string

  clear();

  while(!myString.empty()) {
    pos = myString.find(myDelimiter);

    if (pos != string::npos) {
      // add as a token then erase
      add(myString.substr(0, pos));
      myString.erase(0, pos + 1);

      // try again next time
      continue;
    }

    // we hit the end of the string
    add(myString.substr(0, myString.length()));
    myString.erase(0, myString.length());
  }

  return;
}

const string &StringToken::getRange(const size_type x, const size_type y) {
  size_type i;			// counter

  assert(x <= size());

  myString = "";

  for(i = x; i < y; i++) {
    if (i > x)
      myString.append(" ");

    myString.append(getToken(i));
  }

  return myString;
}

}

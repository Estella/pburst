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

#ifndef __PBURST_STRING_H
#define __PBURST_STRING_H

#include <list>
#include <string>

#include "regex.h"

namespace pburst {

using std::string;
using std::list;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class regexMatch { 
  public:
    string matchString;			// matched string
    regoff_t matchBegin;		// begining of match
    regoff_t matchEnd;			// end of match
    regoff_t matchLength;		// length of match

  private:  
  protected:
}; // regexReturn

struct pburst_duration {
  long int days;
  long int days_total;
  long int hours;
  long int hours_total;
  long int minutes;
  long int minutes_total;
  long int seconds;
  long int seconds_total;
};

struct timeframe { 
  long int hours;
  long int minutes;
  long int seconds;
};

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

int cmp(const char *s1, const char *s2);
char *mask(char *hostmask, const char *host, const int hostsize);
int stpcrlf(char *string);
bool isnum(const char *ch);
string &string_to_lower(string &);

const string pburst_string_graph(const int, const int, const int);
const bool pburst_string_acceptable(const char *, const string &);
const bool pburst_string_duration(const string, unsigned int &);
const bool pburst_string_valid_hostmask(const string, const bool);
const bool pburst_string_valid_username(const string);
const bool pburst_timeval_duration(const time_t, struct pburst_duration &);
const bool pburst_timeval_difference(const struct timeval &, const struct timeval &, struct timeval &);
const bool pburst_string_regex(const string &regexRegex, const string &regexString, list<regexMatch> &);
const string &pburst_string_int(string &, int);
const string &pburst_string_trim(string &);
const unsigned int pburst_string_pad(string &, const unsigned int);
const unsigned int pburst_string_strip_crlf(string &);
const unsigned int pburst_string_to_lower(string &);
const unsigned int pburst_string_replace(const string &, const string &, string &);
const unsigned int pburst_string_wordwrap(const string &, unsigned int, list<string> &);
const unsigned int pburst_string_create_list(const list<string> &, string &, const string &);
const char *pburst_date(time_t);

}
#endif

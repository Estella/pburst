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

#ifndef __PBURST_LOG_H
#define __PBURST_LOG_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ios>

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "pburst.h"
#include "pburst_string.h"

namespace pburst {

using std::string;
using std::stringstream;
using std::ofstream;
using std::ios;
using std::ios_base;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class LogHandler;

class Log {
  public:
    /**********************
     ** Type Definitions **
     **********************/
    typedef unsigned int logId;

    /******************************
     ** Constructor / Destructor **
     ******************************/

    Log(const logId, const string &, LogHandler *);		// File opening constructor.
    virtual ~Log(); 						// Default destructor.

    /****************
     ** Id Members **
     ****************/

    inline const logId getId() const
      { return myId; }

    /*******************
     ** Match Members **
     *******************/

    inline void setMatch(const string &setMe)
      { myMatch = setMe; }

    inline const string &getMatch() const
      { return myMatch; }

    /*****************
     ** Log Members **
     *****************/

    virtual const bool openLog(const string &, ios_base::openmode);
    virtual const bool closeLog();
    virtual const unsigned int appendLog(const string &);
    virtual const unsigned int appendLog(const stringstream &);
    virtual void flushLog();
    virtual const bool isLogOpen()
      { return myFile.is_open(); }

    /************************
     ** LogHandler Members **
     ************************/

    inline void setLogHandler(LogHandler *setMe)
      { myLogHandler = setMe; }

    inline LogHandler *getLogHandler()
      { return myLogHandler; }

    /**********************
     ** Operator Members **
     **********************/

    Log &operator<<(const char &);
    Log &operator<<(const string &);
    Log &operator<<(const int &);
    Log &operator<<(const unsigned int &);
    Log &operator<<(const long int &);
    Log &operator<<(const unsigned long int &);
    Log &operator<<(const short int &);
    Log &operator<<(const unsigned short int &);
    Log &operator<<(const stringstream &);
    Log &operator<<(const double &);

  protected:
  private:
    /***************
     ** Variables **
     ***************/

    LogHandler *myLogHandler;		// Pointer to our LogHandler.
    logId myId;				// Id I have been assigned.
    ofstream myFile;			// Stream to file output.
    string myMatch;			// Requested match for logging.
};

class LogHandler {
  public:
    LogHandler() {}			// Default constructor.
    virtual ~LogHandler() {} 		// Default destructor.

    /**********************
     ** Type Definitions **
     **********************/

    /*******************
     ** OnLog Members **
     *******************/

    virtual const int OnLog(Log *, const Log::logId, const string &) = 0;

    /**********************
     ** Operator Members **
     **********************/

  protected:
  private:
    /***************
     ** Variables **
     ***************/
};

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

} // namespace pburst
#endif

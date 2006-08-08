/**************************************************************************
 ** Dynamic Networking Solutions                                         **
 **************************************************************************
 ** pBurst, Internet Relay Chat Protocol Burster                         **
 ** Copyright (C) 1999 Gregory A. Carter                                 **
 **                    Daniel Robert Karrels99                           **
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
#include <sstream>
#include <string>
#include <ios>
#include <iostream>

#include <time.h>
#include <unistd.h>

#include "Log.h"

#include "pburst.h"

namespace pburst {

using std::stringstream;
using std::string;

/**************************************************************************
 ** Log Class                                                            **
 **************************************************************************/

/**************************************
 ** Constructor / Destructor Members **
 **************************************/

Log::Log(const logId addId, const string &addMatch, LogHandler *addLogHandler) {
  myId = addId;
  myMatch = addMatch;
  myLogHandler = addLogHandler;
} // Log::Log

Log::~Log() {
  closeLog();
} // Log::~Log

/*****************
 ** Log Members **
 *****************/

const bool Log::openLog(const string &openFilename, ios_base::openmode openModes) {
  /*
   * 06/20/2003: Try and open a file stream from fileName.
   */

  if (myFile.is_open())
    return true;

  myFile.open(openFilename.c_str(), openModes);

  if (myFile.is_open())
    return true;

  return false;
} // Log::openLog

const bool Log::closeLog() {
  if (myFile.is_open() == true)
    myFile.close();

  return true;
} // Log::closeLog

void Log::flushLog() {
  if (myFile.is_open() == true)
    myFile.flush();
} // Log::flushLog

const unsigned int Log::appendLog(const string &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;

  return appendMe.length();
} // Log::appendLog

const unsigned int Log::appendLog(const stringstream &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe.str();

  return 1;
} // Log::appendLog

/**********************
 ** Operator Members **
 **********************/

Log &Log::operator<<(const char &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const string &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const unsigned int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const long int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const unsigned long int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const short int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const unsigned short int &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const stringstream &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

Log &Log::operator<<(const double &appendMe) {
  if (myFile.is_open() == true)
    myFile << appendMe;
  return *this;
} // Log &Log::operator<<

/**************************************************************************
 ** Log Class                                                            **
 **************************************************************************/

 /*
  * 06/20/2003: There will probably never be anything here for the log
  *             log class but just in case we'll leave it space. -GCARTER
  */
}

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

#include <list>
#include <new>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#include "Log.h"
#include "LineBuffer.h"
#include "Server.h"
#include "Timer.h"

#include "msgCommands.h"
#include "stdinCommands.h"

#include "config.h"
#include "pburst.h"
#include "pburst_string.h"

namespace pburst {

using std::string;
using std::list;
using std::ofstream;
using std::ostream;
using std::stringstream;
using std::ios;
using std::endl;
using std::cout;

/**************************************************************************
 ** Server Class                                                         **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/

Server::Server() {
  // set defaults
  myConnected = false;
  myEcho = true;
  myFlags = 0;
  myUplink = "";
  myBurstFd = 0;
  setLinkTS(0);
  setStartTS(0);
}

Server::~Server() {
  logMapType::iterator logPtr;			// Iterator to a log map.

  /*
   * 06/20/2003: Clean up our logging facility list and delete any storage
   *             used. -GCARTER
   */
  for(logPtr = myLogList.begin(); logPtr != myLogList.end(); logPtr++) {
    delete logPtr->second;
    myLogList.erase(logPtr);
  } // for

  // cleanup
}

/***************  
 ** Variables **
 ***************/

const Server::flagType Server::FLAG_SHUTDOWN	=	0x0001;
const Server::flagType Server::FLAG_PREAUTH	=	0x0002;
const Server::flagType Server::FLAG_CONNECTED	=	0x0004;
const Server::flagType Server::FLAG_NOAB	=	0x0008;

/*******************
 ** Burst Members **
 *******************/

const bool Server::Burst(const string &burstFile) {
  int newFd;				// fd of file we're opening

  writeLog(PBURST_LOG_DEBUG, "Server::Burst> Burst: %s", burstFile.c_str());

  if ((newFd = open(burstFile.c_str(), O_RDONLY)) == -1) {
    aServer->writeLog(PBURST_LOG_STDOUT, "Could not open file: %s", burstFile.c_str());
    aServer->writeLog(PBURST_LOG_DEBUG, "Server::Burst> Could not open file: %s", burstFile.c_str());
    return false;
  } // if
 
  aServer->setBurstFd(newFd);
  aServer->addConnection(newFd, "BURST FILE");

  return true;
} // Server::Burst

/*********************
 ** Connect Members **
 *********************/

const bool Server::Connect() {
  Connection *nConnection;	// pointer to a connection

  nConnection = aServer->addConnection(getServer(), getPort(), getVhost(), "PBURST Connection", false);

  if (nConnection == NULL)
    return false;
  

  nConnection->setCompress(PBURST_CONFIG_COMPRESS);

  aServer->setSocket(nConnection->getFd());
  SetServerConnected(aServer);
  aServer->setLinkTS(time(NULL));
  
  return true;
} // Server::Connect

/*****************
 ** Die Members **
 *****************/

void Server::Die(const string &dieReason) {
  writeLog(PBURST_LOG_STDOUT, "*** Die: %s", dieReason.c_str());
  writeLog(PBURST_LOG_NORMAL, "*** Die: %s", dieReason.c_str());
  writeLog(PBURST_LOG_DEBUG, "*** Die: %s", dieReason.c_str());

  if (IsServerConnected(this) == true) {
    if (getUplink().length() > 0)
      Send("%s SQ %s :pBurst Unloading\n", (PBURST_CONFIG_SERVER_NUMERIC).c_str(), getUplink().c_str());

    writeConnections();
    removeConnection(getSocket());
  } // if

  exit(0);
} // Server::Die

/****************************
 ** Initialization Members **
 ****************************/

/**
 * Server::initializeSystem
 *
 * Starts all Timers and Logs needed to run system.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::initializeSystem() {


  initializeLogs();

  addConnection(0, "STDIN");

  writeLog(PBURST_LOG_STDOUT, "*** System Initialized");
  writeLog(PBURST_LOG_NORMAL, "*** System Initialized");
  writeLog(PBURST_LOG_DEBUG, "*** System Initialized");

  initializeCommands();
  initializeTimers();

  return true;
} // Server::initializeSystem

/**
 * Server::initializeCommands
 *
 * Adds all commands to system.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::initializeCommands() {

  // user commands
  addCommand("ACTION", "STDIN", "/ACTION", -1, false, new stdinACTION);
  addCommand("ECHO", "STDIN", "/ECHO", -1, false, new stdinECHO);
  addCommand("HELP", "STDIN", "/HELP", -1, false, new stdinHELP);
  addCommand("QUIT", "STDIN", "/QUIT", -1, false, new stdinQUIT);
  addCommand("LOAD", "STDIN", "/LOAD <filename>", -1, false, new stdinLOAD);
  addCommand("SAY", "STDIN", "/SAY <target> <message>", -1, false, new stdinSAY);
  addCommand("WALLOPS", "STDIN", "/WALLOPS <message>", -1, false, new stdinWALLOPS);

  // client commands

  // server commands
  addCommand("EB", "SERVER", "", -1, false, new msgEB);
  addCommand("G", "SERVER", "", -1, false, new msgPING);
  addCommand("SERVER", "SERVER", "", -1, false, new msgSERVER);
  addCommand("V", "SERVER", "", -1, false, new msgVERSION);

  writeLog(PBURST_LOG_STDOUT, "*** Commands Initialized");
  writeLog(PBURST_LOG_NORMAL, "*** Commands Initialized");
  writeLog(PBURST_LOG_DEBUG, "*** Commands Initialized");

  return true;
} // Server::initializeCommands

/**
 * Server::initializeTimers
 *
 * Starts all Timers neeed to connect and run and all other run once actions.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::initializeTimers() {
  myOnTimerList["pburst.timer.connect"] = addTimer(PBURST_TIMER_CONNECT, TIMER_CONTINUOUS, true, this);
  myOnTimerList["pburst.timer.read"] = addTimer(0, TIMER_CONTINUOUS, false, this);

  writeLog(PBURST_LOG_STDOUT, "*** Timers Initialized");
  writeLog(PBURST_LOG_NORMAL, "*** Timers Initialized");
  writeLog(PBURST_LOG_DEBUG, "*** Timers Initialized");

  return true;
} // Server::initializeTimers

/**
 * Server::initializeLogs
 *
 * Starts all Logs needed.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::initializeLogs() {

  /*
   * 06/25/2003: Start the logging facilities for the following internal
   *             logs.  These really need to be done otherwise none of
   *             out critical logs for the primary process will be written. -GCARTER
   */
  myOnLogList[PBURST_LOG_STDOUT] = addLog(PBURST_LOG_STDOUT, this);
  myOnLogList[PBURST_LOG_NORMAL] = addLog(PBURST_LOG_NORMAL, this);
  myOnLogList[PBURST_LOG_DEBUG] = addLog(PBURST_LOG_DEBUG, this);
  myOnLogList[PBURST_LOG_DEBUG_SOCKET] = addLog(PBURST_LOG_DEBUG_SOCKET, this);

  writeLog(PBURST_LOG_STDOUT, "*** Logs Initialized");
  writeLog(PBURST_LOG_NORMAL, "*** Logs Initialized");
  writeLog(PBURST_LOG_DEBUG, "*** Logs Initialized");

  return true;
} // Server::initializeLogs

/**
 * Server::deinitializeSystem
 *
 * Stops all Timers and Logs needed to run system.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::deinitializeSystem() {

  writeLog(PBURST_LOG_STDOUT, "*** System Deinitialized");
  writeLog(PBURST_LOG_NORMAL, "*** System Deinitialized");
  writeLog(PBURST_LOG_DEBUG, "*** System Deinitialized");

  return (deinitializeTimers() && deinitializeCommands() && deinitializeLogs());
} // Server::deinitializeSystem

/**
 * Server::deinitializeCommands
 *
 * Removes all commands from system.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::deinitializeCommands() {

  removeCommand("SERVER/EB");
  removeCommand("SERVER/P");

  writeLog(PBURST_LOG_STDOUT, "*** Commands Deinitialized");
  writeLog(PBURST_LOG_NORMAL, "*** Commands Deinitialized");
  writeLog(PBURST_LOG_DEBUG, "*** Commands Deinitialized");

  return true;
} // Server::deinitializeCommands

/**
 * Server::deinitializeTimers
 *
 * Stops all Timers.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::deinitializeTimers() {

  writeLog(PBURST_LOG_STDOUT, "*** Timers Deinitialized");
  writeLog(PBURST_LOG_NORMAL, "*** Timers Deinitialized");
  writeLog(PBURST_LOG_DEBUG, "*** Timers Deinitialized");

  return true;
} // Server::initializeTimers

/**
 * Server::deinitializeLogs
 *
 * Stops all Logs needed.
 *
 * Returns: true if successful, false if unsuccessful.
 */
const bool Server::deinitializeLogs() {

  writeLog(PBURST_LOG_STDOUT, "*** Logs Deinitialized");
  writeLog(PBURST_LOG_NORMAL, "*** Logs Deinitialized");
  writeLog(PBURST_LOG_DEBUG, "*** Logs Deinitialized");

  /*
   * 06/25/2003: Stop the logging facilities for the following internal
   *             logs. -GCARTER
   */
  removeLog(myOnLogList[PBURST_LOG_STDOUT]);
  removeLog(myOnLogList[PBURST_LOG_NORMAL]);
  removeLog(myOnLogList[PBURST_LOG_DEBUG]);
  removeLog(myOnLogList[PBURST_LOG_DEBUG_SOCKET]);

  return true;
} // Server::deinitializeLogs

/*****************
 ** Log Members **
 *****************/

const Log::logId Server::addLog(const string &addMatch, LogHandler *addLogHandler) {
  Log *nLog;				// pointer to a new log
  logId newId;				// new log id

  assert(addLogHandler != NULL);

  // initialize variables
  newId = getUniqueLogId();
  nLog = new Log(newId, addMatch, addLogHandler);

  writeLog(PBURST_LOG_DEBUG, "Server::addLog> Added Id: %d", newId);

  myLogList.insert(pair<logId, Log *>(newId, nLog));

  return newId;
} // Server::addLog

Log *Server::findLog(const logId findMe) {
  logMapType::iterator logPtr;			// pointer to an Log

  if ((logPtr = myLogList.find(findMe)) != myLogList.end())
    return logPtr->second;

  return NULL;
} // Server::findLog

const bool Server::removeLog(const logId removeMe) {
  logMapType::iterator logPtr;			// pointer to a log

  if ((logPtr = myLogList.find(removeMe)) == myLogList.end())
    return false;

  writeLog(PBURST_LOG_DEBUG, "Server::removeLog> Removed Id: %d", removeMe);

  // cleanup
  delete logPtr->second;

  myLogList.erase(logPtr);

  return true;
} // Server::removeLog

const unsigned int Server::writeLog(const string &writeName, const char *writeFormat, ...) {
  Log *lLog;					// Log we're looping through.
  LogHandler *lLogHandler;			// LogHandler we've looped to.
  logMapType::iterator logPtr;			// pointer to an Log
  char writeBuffer[MAXBUF + 1] = {0};
  va_list va;

  va_start(va, writeFormat);
  vsnprintf(writeBuffer, sizeof(writeBuffer), writeFormat, va);
  va_end(va);

  /*
   * 06/20/2003: Loop through the list of members that would like to receive
   *             a logged event.
   */
  for(logPtr = myLogList.begin(); logPtr != myLogList.end(); logPtr++) {
    // initialize variables
    lLog = logPtr->second;
    lLogHandler = lLog->getLogHandler();

    if (match(lLog->getMatch().c_str(), writeName.c_str()))
      lLogHandler->OnLog(lLog, logPtr->first, writeBuffer);
  } // for

  // myLogBuffer.add(writeBuffer);

  return strlen(writeBuffer);
} // Server::writeLog

const int Server::OnLog(Log *logInfo, const logId forId, const string &logBuffer) {
  struct tm *tm;				// struct tm for localtime
  time_t now;					// current time
  string tempBuf;				// temp local buffer

  now = time(NULL);
  tm = localtime(&now);

  if (forId == myOnLogList[PBURST_LOG_STDOUT]) {
    if (aServer->getEcho() == true)
      cout << logBuffer << endl;
  } // if
  else if (forId == myOnLogList[PBURST_LOG_NORMAL]) {
    if (logInfo->isLogOpen() == false)
      logInfo->openLog((PBURST_PATH_LOGS + PBURST_PATH_LOGS_PBURST), ofstream::out | ofstream::app);

    logInfo->appendLog((logBuffer + "\n"));
    if (PBURST_LOG_FLUSH == true)
      logInfo->flushLog();
  } // if
  else if (forId == myOnLogList[PBURST_LOG_DEBUG]) {
    /*
     * 06/20/2003: If log file isn't open, try to open it using pburst's default
     *             log path. -GCARTER
     */
    if (logInfo->isLogOpen() == false)
      logInfo->openLog((PBURST_PATH_LOGS + PBURST_PATH_LOGS_DEBUG), ofstream::out | ofstream::app);

    logInfo->appendLog((logBuffer + "\n"));
    if (PBURST_LOG_FLUSH == true)
      logInfo->flushLog();
  } // else if
  else if (forId == myOnLogList[PBURST_LOG_DEBUG_SOCKET]) {
    /*
     * 06/20/2003: If log file isn't open, try to open it using pburst's default
     *             log path. -GCARTER
     */
    if (logInfo->isLogOpen() == false)
      logInfo->openLog((PBURST_PATH_LOGS + PBURST_PATH_LOGS_DEBUG_SOCKET), ofstream::out | ofstream::app);

    tempBuf = logBuffer;
    stripLF(tempBuf);

    logInfo->appendLog((tempBuf + "\n"));

    if (PBURST_LOG_FLUSH == true)
      logInfo->flushLog();
  } // else if

  return logBuffer.length();
} // Server::OnLog

/**
 * Server::getUniqueLogId
 *
 * Find a log Id that is not currently in use and return it for
 * a new registering timer.
 *
 * Returns: Number of type logId.
 */
const Log::logId Server::getUniqueLogId() {
  logId uniqueId;                     // Uniqure timer Id to return.
    
  /*
   * 06/24/2003: The idea here is to loop from 1 to find an id that is not in use.
   *             There's a bunch of different ways to find a unique id, this just
   *             seems to be the easiest and most unwastful. -GCARTER
   */
  for(uniqueId = 1; myLogList.find(uniqueId) != myLogList.end(); uniqueId++);
 
  return uniqueId;
} // Server::getUniqueLogId

/*********************
 ** OnTimer Members **
 *********************/

const int Server::OnTimer(Timer *aTimer) {
  unsigned int onTimerId;			// timer id for timer event we received

  // initialize variables
  onTimerId = aTimer->getId();

  if (onTimerId == myOnTimerList["pburst.timer.connect"])
    Connect();
  else if (onTimerId == myOnTimerList["pburst.timer.read"])
    Read();

  return 1;
} // Server::OnTimer

/***********************
 ** OnConnect Members **
 ***********************/

/**
 * Server::OnConnect
 *
 * Run when we first connect to a server socket successfuly.  We now need
 * to initiate the parser and stop the connection sequences.
 *
 * Returns: Nothing, void function.
 */
void Server::OnConnect() {
  writeLog(PBURST_LOG_STDOUT, "*** Connected to %s (%s)", getServer().c_str(), getPort().c_str());
  writeLog(PBURST_LOG_NORMAL, "*** Connected to %s (%s)", getServer().c_str(), getPort().c_str());
  writeLog(PBURST_LOG_DEBUG, "*** Connected to %s (%s)", getServer().c_str(), getPort().c_str());

  removeTimer(myOnTimerList["pburst.timer.connect"]);
  myOnTimerList["pburst.timer.connect"] = 0;

  /*
   * 06/27/2003: We're connected, do auto connect burst.
   */
  if (IsServerNoAutoBurst(this) == false)
    Burst(PBURST_CONFIG_CONNECT_BURST);

} // Server::OnConnect

/*******************
 ** Parse Members **
 *******************/

const unsigned int Server::Parse(const string &parseMe) {
  Packet aPacket;			// packet to send to command parser
  Command *tCommand;			// pointer to a target command
  StringToken aToken;			// string tokenizer
  StringToken parseCommand;		// string tokenizer for command parsing
  string parseSource;			// source of the message
  string tempBuf;			// temporary buffer
  unsigned int nodeLevel;		// node level in command

  if (parseMe.length() < 1)
    return 0;

  // initialize variables
  tempBuf = parseMe;
  pburst_string_strip_crlf(tempBuf);
  aToken = tempBuf;

  if (aToken.size() < 2)
    return 0;

  // initialize variables
  if (IsServerPreAuth(this) == true) {
    parseSource = "";
    parseCommand = tempBuf;
  } // if
  else {
    parseSource = aToken[0];
    parseCommand = aToken.getRange(1, aToken.size());
  } // else

  nodeLevel = 0;

  // find the command
  tCommand = findCommand("SERVER", parseCommand, nodeLevel, NULL);

  aPacket.setSource(parseSource);
  aPacket.setArguments(parseCommand.getRange(nodeLevel, parseCommand.size()));

  if (tCommand != NULL)
    tCommand->getCommandHandler()->Execute(aPacket);

  return parseMe.length();
} // Server::Parse

const unsigned int Server::ParseSTDIN(const string &parseMe) {
  Packet aPacket;			// packet to send to command parser
  Command *tCommand;			// pointer to a target command
  StringToken aToken;			// string tokenizer
  StringToken parseCommand;		// string tokenizer for command parsing
  int returnResult;			// command return result
  string tempBuf;			// temporary buffer
  unsigned int nodeLevel;		// node level in command

  if (parseMe.length() < 1)
    return 0;

  // initialize variables
  tempBuf = parseMe;
  pburst_string_strip_crlf(tempBuf);

  /*
   * 06/27/2003: If our command has no / prefix send the message
   *             directly to our server, do not pass go. -GCARTER
   */
  if (tempBuf[0] != '/') {
    Send("%s", tempBuf.c_str());
    return 0;
  } // if
  else
    tempBuf.replace(0, 1, "");

  // initialize variables
  parseCommand = tempBuf;
  nodeLevel = 0;
  returnResult = time(NULL);

  // find the command
  tCommand = findCommand("STDIN", parseCommand, nodeLevel, NULL);

  if (tCommand != NULL) {
    aPacket.setSource("STDIN");
    aPacket.setArguments(parseCommand.getRange(nodeLevel, parseCommand.size()));

    returnResult = tCommand->getCommandHandler()->Execute(aPacket);
  } // if

  switch(returnResult) {
    case CMDERR_SUCCESS:
      break;
    case CMDERR_SYNTAX:
      cout << "Syntax Error: " << tCommand->getSyntax() << endl;
      cout << "Type /HELP for a list of available commands." << endl;
      break;
    case CMDERR_ACCESS:
      break;
    default:
      cout << "Invalid Command: " << tempBuf << endl;
      cout << "Type /HELP for a list of available commands." << endl;
      break;
  } // switch

  return parseMe.length();
} // Server::ParseSTDIN

/******************
 ** Read Members **
 ******************/

const unsigned int Server::Read() {
  Connection *aConnection;                      // pointer to a connection
  string tempBuf;                               // temp buffer
  unsigned numBytes;				// number of bytes parsed
  stringstream s;				// string stream for replacements

  // initialize variables
  numBytes = 0;

  writeConnections();
  readConnections();

  /*
   * 06/27/2003: Read and parse STDIN. -GCARTER
   */
  aConnection = findConnection(0);
  if (aConnection == NULL)
    return 0;

  while(aConnection->readLine(tempBuf)) {
    if (PBURST_CONFIG_SCRIPTING == true) {
      s.str("");
      pburst_string_replace("%N", PBURST_CONFIG_SERVER_NUMERIC, tempBuf);
      s << time(NULL);
      pburst_string_replace("%T", s.str(), tempBuf);
      pburst_string_replace("%%", "%", tempBuf);
    } // if

    ParseSTDIN(tempBuf);
  } // while

  /*
   * 06/27/2003: Read and send burst file to socket. -GCARTER
   */
  if (aServer->getBurstFd() > 0) {
    aConnection = findConnection(aServer->getBurstFd());
    if (aConnection == NULL) {
      aServer->setBurstFd(0);
      return numBytes;
    } // if

    while(aConnection->readLine(tempBuf)) {
      // strip crlf
      pburst_string_strip_crlf(tempBuf);

      if (PBURST_CONFIG_SCRIPTING == true) {
        s.str("");
        pburst_string_replace("%N", PBURST_CONFIG_SERVER_NUMERIC, tempBuf);
        s << time(NULL);
        pburst_string_replace("%T", s.str(), tempBuf);
        pburst_string_replace("%%", "%", tempBuf);
      } // if

      writeLog(PBURST_LOG_STDOUT, " > %s", tempBuf.c_str());
      sendConnectionF(getSocket(), "%s\n", tempBuf.c_str());
    } // while
  } // if

  /*
   * 06/27/2003: Read and parse Socket. -GCARTER
   */
  aConnection = findConnection(getSocket());
  if (aConnection == NULL) {
    if (IsServerConnected(this) == true) {
      writeLog(PBURST_LOG_STDOUT, "*** Disconnected");
      writeLog(PBURST_LOG_NORMAL, "*** Disconnected");
      writeLog(PBURST_LOG_DEBUG, "*** Disconnected");
    } // if

    UnsetServerConnected(aServer);
    return numBytes;
  } // if
  
  while(aConnection->readLine(tempBuf)) {
    numBytes += tempBuf.length();
    Parse(tempBuf);

    stripLF(tempBuf);
    writeLog(PBURST_LOG_STDOUT, " < %s", tempBuf.c_str());
    writeLog(PBURST_LOG_DEBUG_SOCKET, "%s", tempBuf.c_str());
  } // while

  return numBytes;
} // Server::Read

/*********************
 ** Restart Members **
 *********************/

/**
 * Server::restart
 *
 * Attempt to cleanly restart the process with the previous
 * command line parameters.  Closes STDOUT and STDERR, and unloads
 * all modules that were previously loaded cleanly.
 *
 * Returns: Nothing void function.
 */
void Server::restart(const string &myReason) {

  // debug(HERE, "Server::restart(%s) restarting...\n", myReason.c_str());
  writeLog(PBURST_LOG_NORMAL, "Restart[%s] %s", aServer->getServer().c_str(), myReason.c_str());
  writeLog(PBURST_LOG_DEBUG, "*** Restart (%s)", myReason.c_str());

  close(1);
  close(0);

  aServer->writeConnections();
  aServer->removeConnection(getSocket());

  execv(SPATH, getArgV());

  writeLog(PBURST_LOG_NORMAL, "Restart[%s] %s", aServer->getServer().c_str(), ERROR_REPLY_FAILED_RESTART);
  Die(ERROR_REPLY_FAILED_RESTART);
  exit(-1);
}

/******************
 ** Send Members **
 ******************/

const unsigned int Server::Send(const char *sendFormat, ...) {
  char sendBuffer[MAXBUF + 1] = {0};
  va_list va;

  va_start(va, sendFormat);
  vsnprintf(sendBuffer, sizeof(sendBuffer), sendFormat, va);
  va_end(va);

  sendConnectionF(getSocket(), "%s\r\n", sendBuffer);

  writeLog(PBURST_LOG_STDOUT, " > %s", sendBuffer);
  writeLog(PBURST_LOG_DEBUG_SOCKET, "%s\n", sendBuffer);

  return strlen(sendBuffer);
} // Server::Send


/*******************
 ** Timer Members **
 *******************/

/**
 * Server::addTimer
 *
 * Adds a timer to the timer queue.
 *
 * Returns: Unique Id of timer added.
 */
const Timer::timerId Server::addTimer(const time_t addInterval, const unsigned int addRuncount, const bool addPersistent, TimerHandler *addTimerHandler) {
  Timer *nTimer;				// pointer to a new time
  timerId newId;				// new timer id

  assert(addTimerHandler != NULL);

  // initialize variables
  newId = getUniqueTimerId();
  nTimer = new Timer(newId, addInterval, addRuncount, addPersistent, addTimerHandler);

  writeLog(PBURST_LOG_DEBUG, "Server::addTimer> Added Id: %d", newId);

  myTimerList.insert(pair<timerId, Timer *>(newId, nTimer));

  return newId;
} // Server::addTimer

/**
 * Server::findTimer
 *
 * Attempts to find a timer previously added to the timer queue.
 *
 * Returns: Pointer to a Timer class if successful, NULL if failed.
 */
Timer *Server::findTimer(const timerId findMe) {
  timerMapType::iterator timerPtr;		// pointer to a timer

  if ((timerPtr = myTimerList.find(findMe)) != myTimerList.end())
    return timerPtr->second;

  return NULL;
} // Server::findTimer

/**
 * Server::removeTimer
 *
 * Remove a timer from our timer queue.
 *
 * Returns: true if successful, false if the remove failed.
 */
const bool Server::removeTimer(const timerId removeMe) {
  timerMapType::iterator timerPtr;		// pointer to a log

  if ((timerPtr = myTimerList.find(removeMe)) == myTimerList.end()) {
    writeLog(PBURST_LOG_DEBUG, "Server::removeTimer> Timer Id not found: %d", removeMe);
    return false;
  } // if

  writeLog(PBURST_LOG_DEBUG, "Server::removeTimer> Removed Id: %d", removeMe);

  /*
   * 06/25/2003: Schedule the timer for removal.
   */
  timerPtr->second->setRemove(true);

  return true;
} // Server::removeTimer

/**
 * Server::clearTimers
 *
 * Clears all timers that are not set to be persistent.  Persistent timers
 * are not cleared when we disconnect from the server.
 *
 * Returns: Number of timers cleared.
 */
const unsigned int Server::clearTimers() {
  Timer *lTimer;				// pointer to a looped to timer
  timerMapType::iterator timerPtr;		// iterator to a timer map
  timerMapType::iterator timerErasePtr;		// iterator to a timer map
  unsigned int timerCount;			// number of timers removed

  // initialize variables
  timerCount = 0;

  for(timerPtr = myTimerList.begin(); timerPtr != myTimerList.end();) {
    // initialize variables
    lTimer = timerPtr->second;

    /*
     * 06/245/2003: Some timers needs to stay forever like our conection
     *              timer. -GCARTER
     */
    if (lTimer->getPersistent() == false) {
      timerErasePtr = timerPtr++;
      delete lTimer;
      myTimerList.erase(timerErasePtr);
      continue;
    } // if

    ++timerPtr;
  } // for

  return timerCount;  
} // Server::clearTimers


/**
 * Server::checkTimer
 *
 * Loops through all added timers and either runs, ignores or removes timers.
 *
 * Returns: Number of timers ran.
 */
const unsigned int Server::checkTimer() {
  Timer *lTimer;			// Pointer to a Timer we've looped to.
  TimerHandler *lTimerHandler;		// Pointer to a TimerHandler we've looped to.
  timerMapType::iterator timerPtr;	// Iterator to a timer map.
  timerMapType::iterator timerErasePtr;	// Iterator to a timer map.
  unsigned int timerCount;		// Number of timers run.

  // initialize variables
  timerCount = 0;

  for(timerPtr = myTimerList.begin(); timerPtr != myTimerList.end(); timerPtr++) {
    // initialize variables
    lTimer = timerPtr->second;
    lTimerHandler = lTimer->getTimerHandler();

    // if it's not ready to run skip it
    if (lTimer->getNextRun() > time(NULL) ||
        lTimer->getRemove() == true)
      continue;

    if (lTimerHandler == NULL) {
      writeLog(PBURST_LOG_DEBUG, "Server::checkTimer> Timer was destroyed so I can't use it.");
      lTimer->setRemove(true);
      continue;
    } // if

    // go ahead and run the function
    lTimerHandler->OnTimer(lTimer);
    timerCount++;

    // set the next time we should run
    lTimer->setNextRun(time(NULL) + lTimer->getInterval());

    // deincrement or remove timers
    if (lTimer->getRuncount() > 0) {
      if (lTimer->getRuncount() == 1) {
        lTimer->setRemove(true);
        continue;
      } // if
          
      lTimer->setRuncount(lTimer->getRuncount() - 1);
    } // if
  } // for

  /*
   * 06/25/2003: Iterate through our list of timers and safely
   *             perform removals. -GCARTER
   */
  for(timerPtr = myTimerList.begin(); timerPtr != myTimerList.end();) {
    // initialize variables
    lTimer = timerPtr->second;

    if (lTimer->getRemove() == true) {
      timerErasePtr = timerPtr++;
      delete lTimer;
      myTimerList.erase(timerErasePtr);
      continue;
    } // if

    ++timerPtr;
  } // for

  return timerCount;
} // checkTimer

/**
 * Server::getUniqueTimerId
 *
 * Find a timer Id that is not currently in use and return it for
 * a new registering timer.
 *
 * Returns: Number of type timerId.
 */
const Timer::timerId Server::getUniqueTimerId() {
  timerId uniqueId;			// Uniqure timer Id to return.

  /*
   * 06/24/2003: The idea here is to loop from 1 to find an id that is not in use.
   *             There's a bunch of different ways to find a unique id, this just
   *             seems to be the easiest and most unwastful. -GCARTER
   */
  for(uniqueId = 1; myTimerList.find(uniqueId) != myTimerList.end(); uniqueId++);

  return uniqueId;
} // Server::getUniqueTimerId

/********************
 ** Config Members **
 ********************/

const unsigned int Server::loadConfig(const string configPath) {
  StringToken aToken;			// tokenize a string
  LineBuffer aLine;			// line buffer
  char acceptChars[] = "0123456789";
  char fileBuf[512 + 1];		// file buffer
  int fd;				// pointer to a file
  string tempBuf;			// temp buffer
  unsigned int bytesRead;		// bytes read;

  // initialize variables
  bytesRead = 0;

  if ((fd = open(configPath.c_str(), O_RDONLY)) == -1)
    return bytesRead;

  while((bytesRead = read(fd, fileBuf, 512)) > 0) {
    fileBuf[bytesRead] = '\0';
    aLine.add(fileBuf);
  } // while

  while(aLine.readLine(tempBuf)) {
    pburst_string_strip_crlf(tempBuf);

    // is it a comment?  Yes?  Skip!
    if (tempBuf.size() < 1 || tempBuf[0] == '#')
      continue;

    aToken = tempBuf;

    // is it a properly formated line?  No?  Skip!
    if (aToken.size() < 2)
      continue;

    // should we set a variable?
    if (!cmp(aToken[0].c_str(), "set")) {
      // set command should have three arguments at least
      if (aToken.size() < 3)
        continue;

      if (pburst_string_acceptable(acceptChars, aToken.getRange(2, aToken.size())) == true)
        aServer->addOption(aToken[1], atoi(aToken.getRange(2, aToken.size()).c_str()), true, aToken.getRange(2, aToken.size()));
      else if (aToken.getRange(2, aToken.size()) == "true")
        aServer->addOption(aToken[1], 1, true, aToken.getRange(2, aToken.size()));
      else if (aToken.getRange(2, aToken.size()) == "false")
        aServer->addOption(aToken[1], 0, false, aToken.getRange(2, aToken.size()));
      else
        aServer->addOption(aToken[1], 1, true, aToken.getRange(2, aToken.size()));
    } // if
  } // while

  return bytesRead;
} // Server::loadConfig

}

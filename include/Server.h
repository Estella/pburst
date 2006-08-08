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

#ifndef __PBURST_SERVER_H
#define __PBURST_SERVER_H

#include <string>
#include <map>

#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "LineBuffer.h"
#include "Log.h"
#include "Network.h"
#include "Options.h"
#include "Command.h"
#include "Timer.h"

#include "pburst.h"
#include "pburst_string.h"
#include "misc.h"

namespace pburst {

using std::string;
using std::map;
using std::list;
using std::pair;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

#define PBURST_SERVER_OPTION_BOOL(pburst_option, pburst_option_default) (aServer->getOptionBool(pburst_option) == true ? aServer->getOptionBool(pburst_option) : pburst_option_default)
#define PBURST_SERVER_OPTION_INT(pburst_option, pburst_option_default) (aServer->getOptionBool(pburst_option) == true ? aServer->getOptionInt(pburst_option) : pburst_option_default)
#define PBURST_SERVER_OPTION_STRING(pburst_option, pburst_option_default) (aServer->getOptionBool(pburst_option) == true ? aServer->getOptionString(pburst_option) : string(pburst_option_default))
#define PBURST_SERVER_OPTION_CHAR(pburst_option, pburst_option_default) (aServer->getOptionBool(pburst_option) == true ? aServer->getOptionString(pburst_option).c_str() : pburst_option_default)

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class Server : public LogHandler, public TimerHandler, public CommandTree, public Network {
  public:
    Server();				// constructor
    virtual ~Server();			// destructor

    /**********************
     ** Type Definitions **
     **********************/
    typedef Timer::timerId timerId;
    typedef Log::logId logId;
    typedef map<logId, Log *> logMapType;
    typedef map<string, logId> onLogMapType;
    typedef map<timerId, Timer *> timerMapType;
    typedef map<string, timerId> onTimerMapType;
    typedef unsigned int flagType;

    /***************
     ** Variables **
     ***************/
    const static flagType FLAG_SHUTDOWN;
    const static flagType FLAG_PREAUTH;
    const static flagType FLAG_CONNECTED;
    const static flagType FLAG_NOAB;

    /******************
     ** ArgV Members **
     ******************/

    char **getArgV()
      { return myArgV; }
    void setArgV(char **setMe)
      { myArgV = setMe; }

    /*******************
     ** Burst Members **
     *******************/

    const bool Burst(const string &);

    /*********************
     ** BurstFd Members **
     *********************/

    const int getBurstFd() const
      { return myBurstFd; }

    void setBurstFd(const int setMe)
      { myBurstFd = setMe; }

    /********************
     ** Config Members **
     ********************/

    void setConfigPath(const string &setMe)
      { myConfigPath = setMe; }

    const string &getConfigPath()
      { return myConfigPath; }

    const unsigned int loadConfig(const string);

    /*********************
     ** Connect Members **
     *********************/

    const bool Connect();

    /***********************
     ** Connected Members **
     ***********************/

    // is the bot connected?
    const bool getConnected()
      { return myConnected; }

    // set if I'm connected or not
    void setConnected(const bool setMe)
      { myConnected = setMe; }

    /*******************
     ** Debug Members **
     *******************/

    const bool getDebug() const
      { return myDebug; }

    void setDebug(const bool setMe)
      { myDebug = setMe; }

    /*****************
     ** Die Members **
     *****************/

    void Die(const string &);

    /******************
     ** Echo Members **
     ******************/

    const bool getEcho() const
      { return myEcho; }

    void setEcho(const bool setMe)
      { myEcho = setMe; }

    /*********************
     ** Execute Members **
     *********************/

    const int Execute(Packet ePacket) { return 1; }

    /******************
     ** Flag Members **
     ******************/
    
    const bool addFlag(const flagType addMe) {
      if (myFlags & addMe)
        return false;
    
      myFlags |= addMe;
    
      return true;
    } // addFlag
    
    const bool removeFlag(const flagType removeMe) {
      if (!(myFlags & removeMe))
        return false;
    
      myFlags &= ~removeMe;
     
      return true;
    } // removeFlag
     
    const bool findFlag(const flagType findMe) const {
      return myFlags & findMe ? true : false; 
    } // findFlag
        
    const unsigned int getFlags(string &);

    /*****************
     ** I/O Members **
     *****************/

    void stripLF(string &stripMe) {
      unsigned int pos;			// pointer to a string

      for(pos = stripMe.find("\n");
          pos != string::npos;
          pos = stripMe.find("\n"))
        stripMe.replace(pos, 1, "\\n");

      for(pos = stripMe.find("\r");
          pos != string::npos;
          pos = stripMe.find("\r"))
        stripMe.replace(pos, 1, "\\r");

    } // stripLF

    /***********************************************
     ** Initialization / Deinitialization Members **
     ***********************************************/

    const bool initializeSystem();
    const bool initializeCommands();
    const bool initializeLogs();
    const bool initializeTimers();
    const bool deinitializeSystem();
    const bool deinitializeCommands();
    const bool deinitializeLogs();
    const bool deinitializeTimers();

    /*********************
     ** Link TS Members **
     *********************/

    void setLinkTS(const time_t setMe)
      { myLinkTS = setMe; }

    const time_t getLinkTS() const
      { return myLinkTS; }

    /*******************
     ** OnLog Members **
     *******************/

    const logId getUniqueLogId();
    const unsigned int writeLog(const string &, const char *, ...);
    const logId addLog(const string &, LogHandler *);
    const bool removeLog(const logId);
    Log *findLog(const logId);
    const int OnLog(Log *, const logId, const string &);

    /***********************
     ** OnConnect Members **
     ***********************/

    void OnConnect();

    /********************
     ** Option Members **
     ********************/

    const bool addOption(const string addName, const int addInt, const bool addBool, const string addString)
      { return myOptionsList.addOption(addName, addInt, addBool, addString); }
    
    const bool removeOption(const string &removeMe)   
      { return myOptionsList.removeOption(removeMe); }
    
    const int clearOptions()
      { return myOptionsList.clearOptions(); }
    
    const bool getOptionBool(const string &findMe)   
      { return myOptionsList.getOptionBool(findMe); }
    
    const int getOptionInt(const string &findMe)
      { return myOptionsList.getOptionInt(findMe); }

    const string getOptionString(const string &findMe)
      { return myOptionsList.getOptionString(findMe); }

    const unsigned int optionSize()
      { return myOptionsList.optionSize(); }
    
    // iterators!
    inline OptionsList::optionMapType::iterator optionBegin()
      { return myOptionsList.optionBegin(); }
      
    inline OptionsList::optionMapType::iterator optionEnd()
      { return myOptionsList.optionEnd(); }

    /*******************
     ** Parse Members **
     *******************/

    const unsigned int ParseSTDIN(const string &);
    const unsigned int Parse(const string &);
      
    /*****************
     ** PID Members **
     *****************/

    const pid_t getPID() const
      { return myPID; }

    void setPID(const pid_t setMe)
      { myPID = setMe; }

    /********************
     ** Prompt Members **
     ********************/

    const string &getPrompt() const
      { return myPrompt; }

    void setPrompt(const string &setMe)
      { myPrompt = setMe; }

    /******************
     ** Port Members **
     ******************/

    const string &getPort() const
      { return myPort; }

    void setPort(const string &setMe)
      { myPort = setMe; }

    /******************
     ** Read Members **
     ******************/

    const unsigned int Read();

    /*********************
     ** Restart Members **
     *********************/

    void restart(const string &);	// restart the process

    /******************
     ** Send Members **
     ******************/

    const unsigned int Send(const char *, ...);    

    /********************
     ** Server Members **
     ********************/

    const string &getServer() const
      { return myServer; }

    void setServer(const string & setMe)
      { myServer = setMe; }

    /**********************
     ** Start TS Members **
     **********************/

    void setStartTS(const time_t setMe)
      { myStartTS = setMe; }
    
    const time_t getStartTS() const
      { return myStartTS; }

    /********************
     ** Socket Members **
     ********************/

    void setSocket(const int setMe)
      { mySocket = setMe; }

    const int getSocket() const
      { return mySocket; }

    /*******************
     ** Timer Members **
     *******************/

    const timerId getUniqueTimerId();
    const int OnTimer(Timer *);

    const timerId addTimer(const time_t, const unsigned int, const bool, TimerHandler *);
    const bool removeTimer(const timerId);
    Timer *Server::findTimer(const timerId);

    const unsigned int checkTimer();
    const unsigned int clearTimers();

    /***********************
     ** Timestamp Members **
     ***********************/

    const time_t getTimestamp() const
      { return myTimestamp; }

    void setTimestamp(const time_t setMe)
      { myTimestamp = setMe; }

    /*******************
     ** Vhost Members **
     *******************/

    void setVhost(const string &setMe)
      { myVhost = setMe; }

    const string &getVhost()
      { return myVhost; }

    /********************
     ** Rehash Members **
     ********************/

    void rehash();			// rehash the bot's config

    /********************
     ** Uplink Members **
     ********************/

    const string &getUplink() const
      { return myUplink; }

    void setUplink(const string & setMe)
      { myUplink = setMe; }

    /***************
     ** Variables **
     ***************/
    Network aNetwork;			// network connection

  protected:
  private:
    LineBuffer myLogBuffer;		// log buffer
    OptionsList myOptionsList;		// option list
    bool myConnected;			// am i connected?
    bool myDebug;			// debug mode?
    bool myEcho;			// echo mode?
    char **myArgV;			// command line arguments
    flagType myFlags;			// my flags
    int mySocket;			// my socket
    int myBurstFd;			// fd of burst file
    logMapType myLogList;		// Log list.
    onLogMapType myOnLogList;		// List of log id's we're listening for.
    onTimerMapType myOnTimerList;	// List of timer id's we're listening for.
    pid_t myPID;			// my pid
    string myConfigPath;		// my config file path
    string myPrompt;			// my prompt
    string myVhost;			// my virtual host
    string myPort;			// my port
    string myServer;			// my server
    string myUplink;			// my uplink
    time_t myTimestamp;			// my timestamp
    time_t myLinkTS;			// my link ts
    time_t myStartTS;			// my start ts
    timerMapType myTimerList;		// timer list
};

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

#define IsServerShutdown(x)		x->findFlag(Server::FLAG_SHUTDOWN)
#define IsServerPreAuth(x)		x->findFlag(Server::FLAG_PREAUTH)
#define IsServerConnected(x)		x->findFlag(Server::FLAG_CONNECTED)
#define IsServerNoAutoBurst(x)		x->findFlag(Server::FLAG_NOAB)
#define SetServerShutdown(x)		x->addFlag(Server::FLAG_SHUTDOWN);
#define SetServerPreAuth(x)		x->addFlag(Server::FLAG_PREAUTH);
#define SetServerConnected(x)		x->addFlag(Server::FLAG_CONNECTED);
#define SetServerNoAutoBurst(x)		x->addFlag(Server::FLAG_NOAB);
#define UnsetServerShutdown(x)		x->removeFlag(Server::FLAG_SHUTDOWN);
#define UnsetServerPreAuth(x)		x->removeFlag(Server::FLAG_PREAUTH);
#define UnsetServerConnected(x)		x->removeFlag(Server::FLAG_CONNECTED);
#define UnsetServerNoAutoBurst(x)	x->removeFlag(Server::FLAG_NOAB);

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

extern Server *aServer;		// establish a global variable

}
#endif

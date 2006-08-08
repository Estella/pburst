/**************************************************************************
 ** Dynamic Networking Solutions                                         **
 **************************************************************************
 * pBurst, Internet Relay Chat Protocol Burster                         **
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
 **************************************************************************/

#ifndef __COMMAND_H
#define __COMMAND_H

#include <map>
#include <string>
#include <list>

#include <time.h>
#include <sys/time.h>

#include "StringToken.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using std::string;
using std::map;
using std::list;
using std::pair;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

#define COMMAND_ARGUMENTS	Packet aPacket
#define COMMAND_PROTOTYPE	Packet

#define CMDERR_ACCESS  		0		// functions return 0 if user access too low
#define CMDERR_SUCCESS 		1		// successfully executed.
#define CMDERR_SYNTAX  		-1		// syntax error

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

class CommandHandler;

class Packet {
  public:
    typedef map<string, void *> variableMapType;

    /***********************
     ** Arguments Members **
     ***********************/

    StringToken getArguments()
      { return myArguments; }

    void setArguments(const string setMe)
      { myArguments = setMe; }

    /*********************
     ** Channel Members **
     *********************/

    const string getChannel() const
      { return myChannel; }

    void setChannel(const string setMe)
      { myChannel = setMe; }

    /******************
     ** Host Members **
     ******************/

    const string getHost() const
      { return myHost; }

    void setHost(const string setMe)
      { myHost = setMe; }

    /*******************
     ** Level Members **
     *******************/

    const int getLevel() const
      { return myLevel; }

    void setLevel(const int setMe)
      { myLevel = setMe; }
    
    /******************
     ** Name Members **
     ******************/

    const string getName() const
      { return myName; }

    void setName(const string setMe)
      { myName = setMe; }

    /******************
     ** Nick Members **
     ******************/

    const string getNick() const
      { return myNick; }

    void setNick(const string setMe)
      { myNick = setMe; }

    /******************
     ** Path Members **
     ******************/

    const string getPath() const
      { return myPath; }

    void setPath(const string setMe)
      { myPath = setMe; }

    /*****************
     ** Raw Members **
     *****************/

    const string getRaw() const
      { return myRaw; }

    void setRaw(const string setMe)
      { myRaw = setMe; }

    /********************
     ** Source Members **
     ********************/

    const string getSource() const
      { return mySource; }

    void setSource(const string setMe)
      { mySource = setMe; }

    /**********************
     ** Variable Members **
     **********************/

    const bool addVariable(const string, void *);
    const bool removeVariable(const string);
    void *getVariable(const string);

    // iterators
    inline variableMapType::iterator variableBegin()
      { return myVariableList.begin(); }

    inline variableMapType::iterator variableEnd()
      { return myVariableList.end(); }

    const unsigned int variableSize();

  private:
    StringToken myArguments;		// arguments
    int myLevel;			// level
    variableMapType myVariableList;	// list of variables
    string myChannel;			// channel
    string myHost;			// host
    string myName;			// name
    string myNick;			// nick
    string myPath;			// path
    string myRaw;			// raw
    string mySource;			// source
  protected:
}; // CommandPacket

class Command {
  public:
    Command(const string &, const string &, const string &, const string &, const int, const bool, CommandHandler *);
    virtual ~Command();

    /*******************
     ** Level Members **
     *******************/

    const int getLevel() const
      { return myLevel; }

    /******************
     ** Name Members **
     ******************/

    const string getName() const
      { return myName; }

    /********************
     ** Syntax Members **
     ********************/

    const string getSyntax() const
      { return mySyntax; }

    /**********************
     ** Suppress Members **
     **********************/

    void setSuppress(const bool setMe)
      { mySuppress = setMe; }

    /******************
     ** Path Members **
     ******************/

    const string getPath() const
      { return myPath; }

    /****************************
     ** CommandHandler Members **
     ****************************/

    // return function type
    CommandHandler *getCommandHandler()
      { return myCommandHandler; }

    /***********************
     ** Member Of Members **
     ***********************/

    const string getMember() const
      { return myMember; }

  protected:
    CommandHandler *myCommandHandler;		// command handler
    bool mySuppress;				// suppress the debug output of command
    string myName;				// command name
    string myPath;				// command path
    string mySyntax;				// command syntax
    string myMember;				// command I am a member of
    int myLevel;
};

class CommandHandler {
  public:
    CommandHandler() {}
    virtual ~CommandHandler() {}

    virtual const int Execute(Packet) = 0;
  protected:
  private:
}; // CommandHandler

class CommandTree : public CommandHandler {
  public:
    CommandTree();
    virtual ~CommandTree();

    typedef map<string, Command *> commandMapType;

    /*********************
     ** Command Members **
     *********************/

    virtual const bool addCommand(const string &, const string &, const string &, const int, const bool, CommandHandler *);
    virtual const bool removeCommand(const string &);

    virtual Command *findCommand(const string, const StringToken, unsigned int &, Command *);
    virtual const unsigned int findCommands(const string, const StringToken, unsigned int &, Command *, list<Command *> &);

    virtual void showTree(const string, const int, const string, list<string> &);

    const unsigned int commandSize() const
      { return myCommandList.size(); }
 
    // iterators!
    virtual inline commandMapType::iterator commandBegin()
      { return myCommandList.begin(); }

    virtual inline commandMapType::iterator commandEnd()
      { return myCommandList.end(); }

  private:
    commandMapType myCommandList;	 // recursive command list

  protected:
};

#define CREATE_COMMAND(name) \
class name : public CommandHandler { \
  public: \
    name() {} \
    virtual ~name() {} \
    const int Execute(Packet); \
};

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

}
#endif

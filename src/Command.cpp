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
 **************************************************************************/

#include <list>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <new>
#include <iostream>
#include <string>
#include <exception>
#include <sstream>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "StringToken.h"
#include "Command.h"

#include "match.h"
#include "pburst_string.h"

namespace pburst {

using namespace std;

/**************************************************************************
 ** Command Class                                                        **
 **************************************************************************/

/******************************
 ** Constructor / Destructor **
 ******************************/

Command::Command(const string &addName, const string &addPath, const string &addMember,
                 const string &addSyntax, const int addLevel, const bool addSuppress, 
                 CommandHandler *addCommandHandler) {
  myName = addName;
  myPath = addPath;
  myMember = addMember;
  mySyntax = addSyntax;
  myLevel = addLevel;
  mySuppress = addSuppress;
  myCommandHandler = addCommandHandler;

  return;
} // Command::Command

Command::~Command() {
  return;
} // Command::~Command

/**************************************************************************
 ** CommandTree Class                                                    **
 **************************************************************************/

CommandTree::CommandTree() {
  return;
} // CommandTree::CommandTree

CommandTree::~CommandTree() {
  commandMapType::iterator ptr;		// pointer to a map

  /*****************************
   ** Cleanup Command Members **
   *****************************/
  for(ptr = commandBegin(); ptr != commandEnd(); ptr++)
    delete ptr->second;

  myCommandList.clear();

  return;
} // CommandTree::~CommandTree

const bool CommandTree::addCommand(const string &addName, const string &addMember, const string &addSyntax, const int addLevel, const bool addSuppress, CommandHandler *addCommandHandler) {
  Command *aCommand;				// new command to add

  assert(addCommandHandler != NULL);

  // create a new command
  try {
    aCommand = new Command(addName, addMember + string("/") + addName, 
                           addMember, addSyntax, addLevel, addSuppress, addCommandHandler);
  } // try
  catch(bad_alloc xa) {
    assert(false);
  } // catch

  myCommandList.insert(pair<string, Command *>(aCommand->getPath(), aCommand));

  return true;
} // CommandTree::addCommand

const bool CommandTree::removeCommand(const string &removePath) {
  commandMapType::iterator commandPtr;		// pointer to a map

  if ((commandPtr = myCommandList.find(removePath)) == myCommandList.end())
    return false;

  // cleanup
   delete commandPtr->second;

  myCommandList.erase(commandPtr);

  return true;
} // CommandTree::removeCommand

Command *CommandTree::findCommand(const string commandMember, const StringToken aToken, unsigned int &commandPosition, Command *currentCommand) {
  Command *lCommand;					// pointer to a Command we've looped to
  commandMapType::iterator commandPtr;			// pointer to a map

  if (commandPosition == aToken.size())
    return currentCommand;

  for(commandPtr = commandBegin(); commandPtr != commandEnd(); commandPtr++) {
    // initialize variables
    lCommand = commandPtr->second;

    if (!cmp(lCommand->getMember().c_str(), commandMember.c_str()) &&
        !cmp(aToken[commandPosition].c_str(), lCommand->getName().c_str()))
      return findCommand(lCommand->getPath(), aToken, ++commandPosition, lCommand);
  }  // for

  return currentCommand;
} // CommandTree::findCommand

const unsigned int CommandTree::findCommands(const string commandMember, const StringToken aToken, unsigned int &commandPosition, Command *currentCommand, list<Command *> &findList) {
  Command *lCommand;					// pointer to a command we've looped to
  commandMapType::iterator commandPtr;			// pointer to a map

  if (commandPosition == aToken.size())
    return findList.size();

  for(commandPtr = commandBegin(); commandPtr != commandEnd(); commandPtr++) {
    // initialize variables
    lCommand = commandPtr->second;

    if (!cmp(lCommand->getMember().c_str(), commandMember.c_str()) &&
        match(aToken[commandPosition].c_str(), lCommand->getName().c_str())) {
      findList.push_back(lCommand);
      findCommands(lCommand->getPath(), aToken, ++commandPosition, lCommand, findList);
    }
  }  // for

  return findList.size();
} // CommandTree::findCommands

void CommandTree::showTree(const string treeMember, int treeLevel, string treeSpacer,  list<string> &treeList) {
  Command *lCommand;					// pointer to a command we've looped to
  commandMapType::iterator treePtr;			// pointer to a list
  int numMatches;					// number of matches
  int maxMatches;					// max matches
  stringstream s;					// stringstream

  if (treeLevel == 1) {
    s << treeSpacer << treeMember << " (0)";
    treeList.push_back(s.str());
  } // if

  // count matches
  maxMatches = 0;
  for(treePtr = commandBegin(); treePtr != commandEnd(); treePtr++) {
    // initialize variables
    lCommand = treePtr->second;

    if (!cmp(lCommand->getMember().c_str(), treeMember.c_str()))
      maxMatches++;
  } // for

  numMatches = 0;
  for(treePtr = commandBegin(); treePtr != commandEnd(); treePtr++) {
    // initialize variables
    lCommand = treePtr->second;

    if (!cmp(lCommand->getMember().c_str(), treeMember.c_str())) {
      numMatches++;
      treeSpacer += (numMatches == maxMatches) ? " " : " |";
      s.str("");
      s << treeSpacer << ((numMatches == maxMatches) ? "`" : "") << "-" 
        << lCommand->getName() << " (" << treeLevel << ") " << lCommand->getLevel();
      treeList.push_back(s.str());
//      send_to_server(PRIORITY_QUEUE_MEDIUM, "%s %s :%s%s-\003%.2d%s (%d) %d\003\n", IRC_PRIVATE, "OmniDynmc", commandSpacer.c_str(), (numMatches == maxMatches) ? "`" : "", localColor, ptr->second->getName().c_str(), commandLevel, ptr->second->getLevel());
      treeSpacer += (numMatches == maxMatches) ? " " : "";
      showTree(lCommand->getPath(), ++treeLevel, treeSpacer, treeList);
      treeSpacer.replace((treeSpacer.length() - 2), 2, "");
      treeLevel--;
    } // if
  }  // for

  return;
} // CommandTree::showTree

/**************************************************************************
 ** Packet Class                                                         **
 **************************************************************************/

/**********************
 ** Variable Members **
 **********************/

const bool Packet::addVariable(const string variableName, void *variablePointer) {
  variableMapType::iterator ptr;			// pointer to a variable list

  if ((ptr = myVariableList.find(variableName)) != myVariableList.end())
    return false;

  myVariableList.insert(pair<string, void *>(variableName, variablePointer));

  return true;
} // CommandPacket::addVariable

const bool Packet::removeVariable(const string variableName) {
  variableMapType::iterator ptr;			// pointer to a variable list

  if ((ptr = myVariableList.find(variableName)) == myVariableList.end())
    return false;

  myVariableList.erase(ptr);

  return true;
} // Packet::removeVariable

void *Packet::getVariable(const string variableName) {
  variableMapType::iterator ptr;

  if ((ptr = myVariableList.find(variableName)) == myVariableList.end())
    return NULL;

  return ptr->second;
} // Packet::getVariable

}

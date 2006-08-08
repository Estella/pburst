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

#include <signal.h>
#include <unistd.h>

#include "Server.h"

#include "config.h"
#include "pburst.h"
#include "pburst_signal.h"

namespace pburst {

/*
 * This function is designed intentionally to handle all
 * signals that might interrupt the process.
 * It prevents the unblocking nature of signals from
 * corrupting buffers and possibly crashing the machine.
 */
void sighandle(int sig) {
  static int lastsig;

  if (sig == NOSIGNAL) {
    switch(lastsig) {
      case SIGHUP:
        aServer->writeLog(PBURST_LOG_STDOUT, "*** SIGHUP Received");
        aServer->writeLog(PBURST_LOG_NORMAL, "*** SIGHUP Received");
        aServer->writeLog(PBURST_LOG_DEBUG, "*** SIGHUP Received");

        aServer->restart(DEFAULT_RESTART);

        break;
      case SIGPIPE:
        aServer->writeLog(PBURST_LOG_STDOUT, "*** SIGPIPE Received");
        aServer->writeLog(PBURST_LOG_NORMAL, "*** SIGPIPE Received");
        aServer->writeLog(PBURST_LOG_DEBUG, "*** SIGPIPE Received");

        break;
      case SIGALRM:
        break;
      case SIGUSR1:

        aServer->setDebug(!aServer->getDebug());

        aServer->writeLog(PBURST_LOG_STDOUT, "*** SIGUSR1 Received, debug: %s", (aServer->getDebug() == true) ? "ON" : "OFF");
        aServer->writeLog(PBURST_LOG_NORMAL, "*** SIGUSR1 Received, debug: %s", (aServer->getDebug() == true) ? "ON" : "OFF");
        aServer->writeLog(PBURST_LOG_DEBUG, "*** SIGUSR1 Received, debug: %s", (aServer->getDebug() == true) ? "ON" : "OFF");
        break;
      case SIGUSR2:
        break;
      case SIGINT:
        aServer->writeLog(PBURST_LOG_STDOUT, "*** SIGINT Received");
        aServer->writeLog(PBURST_LOG_NORMAL, "*** SIGINT Received");
        aServer->writeLog(PBURST_LOG_DEBUG, "*** SIGINT Received");

        aServer->Die("SIGINT Received");
        break;
      default:
        return;
        break;
    }

    lastsig = 0;
  }
  else
    lastsig = sig;
}

}

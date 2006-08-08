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

#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <new>
#include <iostream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/uio.h>
#include <sys/param.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>

#include "LineBuffer.h"
#include "Server.h"
#include "Timer.h"

#include "config.h"
#include "pburst.h"
#include "pburst_signal.h"
#include "pburst_string.h"


using namespace pburst;

using std::string;

// establish global variables
pburst::Server *pburst::aServer;		// this will be the only global variable soon

void die(const string &dieMessage) {
  cout << "Error: " << dieMessage << endl << endl;   
  cout << "Please email gcarter@infodns.com with the error your recieved." << endl;
  exit(1);
} // die

void usage() {
  fprintf(stderr, "Usage: %s -d [-v] [-c <config_file_path>] [-b vhost]\n", aServer->getPrompt().c_str());
  exit(1);
}

void header() {
  return;
}

void version() {
  printf("pBurst: Internet Relay Char Protocol Burster\n");
  printf("Version: %s\n", VERSION);
  printf("Compiled: %s\n", __DATE__);
}

int main(int argc, char **argv) {
  char *prompt;		// process prompt
  int ch;		// misc pointer
  pid_t pid;		// process pid
  time_t now;		// current time

  FILE *PID;

  // try to create our new variables
  try {
    aServer = new Server;
  } // try
  catch(bad_alloc xa) {
    assert(false);
  } // catch

  aServer->setVhost("");
  aServer->setDebug(true);
  aServer->setStartTS(time(NULL));

  // install new signal handlers
  signal(SIGPIPE, sighandle);
  signal(SIGHUP, sighandle);
  signal(SIGALRM, sighandle);
  signal(SIGINT, sighandle);
  signal(SIGUSR1, sighandle);
  signal(SIGUSR2, sighandle);

  // time I was first started
  aServer->setTimestamp(time(NULL));

  // save our argv
  aServer->setArgV(argv);

  umask(077);

  // remember prompt
  if ((prompt = strrchr(argv[0], '/')))
    prompt++;
  else
    prompt = argv[0];

  aServer->setPrompt(prompt);
  aServer->setConfigPath(PBURST_PATH_CONFIG);

  // parse command line options
  while ((ch = getopt(argc, argv, "dvb:c:")) != -1) {
    switch (ch) {
      case 'c':
	aServer->setConfigPath(optarg);
	break;
      case 'd':
        SetServerNoAutoBurst(aServer);
	break;
      case 'b':
	aServer->setVhost(optarg);
	break;
      case 'v':
	(void) version();
	exit(0);
	break;
      case '?':
      default:
	(void) usage();
	break;
    } // switch
  } // while

  // parse mysql record id for configuration
  argc -= optind;
  argv += optind;

  // load config file
  aServer->loadConfig(aServer->getConfigPath());

  aServer->setServer(PBURST_CONFIG_SERVER);
  aServer->setPort(PBURST_CONFIG_PORT);

  // Verify the log directory exists.
  if (!is_dir((PBURST_PATH_LOGS).c_str())) {
    fprintf(stdout, "%s: Fatal error, can not find ``%s''.\n", aServer->getPrompt().c_str(), (PBURST_PATH_LOGS).c_str());
    exit(1);
  } // if

  // fork to background unless in debug mode
  if (aServer->getDebug()) {
    pid = getpid();
    // fprintf(stdout, "\[Forground(PID: %d)]\n", pid);
  } // if
  else {
    if ((pid = fork())) {
      // printf("[Background(PID:  %d)]\r\n", pid);
      PID = fopen((PBURST_PATH_PID).c_str(), "w");
      fprintf(PID, "%i\n", pid);
      fclose(PID);

      // close stdout and stderr
      close(0);
      close(1);
      exit(0);
    } // if
  } // else

  /***********************
   ** Initialize System **
   ***********************/

  aServer->initializeSystem();

  if (PBURST_CONFIG_PREAUTH == true)
    SetServerPreAuth(aServer);

  aServer->Connect();

  /****************
   ** Enter loop **
   ****************/

  while(true) {
    /******************
     ** Check Timers **
     ******************/
    aServer->checkTimer();

    // if we are still not connected
    // keep running our timer until we are
    if (IsServerConnected(aServer) == false)
      continue;

    /*
     * 06/27/2003: Perform actions required right when we connect. -GCARTER
     */
    aServer->OnConnect();

    // loop until we can't read anymore
    while (IsServerConnected(aServer) == true) {
      now = time(NULL);

      /******************
       ** Check Timers **
       ******************/
      aServer->checkTimer();

      (void) sighandle(NOSIGNAL);
    } // while

    // we got disconnected let's cleanup
    UnsetServerConnected(aServer);
    aServer->clearTimers();

    // startup connect timer
    if (PBURST_CONFIG_PREAUTH == true)
      SetServerPreAuth(aServer);

    aServer->deinitializeTimers();
    aServer->initializeTimers();
  } // while

  // cleanup
  delete aServer;

  return 1;
} // main

int sectotf(time_t ts, struct timeframe *tf) {
  tf->hours = ts / 3600;
  tf->minutes = (ts - (tf->hours * 3600)) / 60;
  tf->seconds = ts - (tf->hours * 3600) - (tf->minutes * 60);
  return 1;
} // sectotf

int is_dir(const char *path) {
  int ret;
  struct stat sb;
    
  ret = lstat(path, &sb);
  if (ret == -1) {
    // debug(HERE, "is_dir(): could not stat %s, %s\n", path, strerror(errno));
    return 0;
  } // if
   
  if (sb.st_mode & S_IFDIR) {
    // debug(HERE, "is_dir(): found that %s is a directory.\n", path);
    return 1;
  } // if
   
  // debug(HERE, "is_dir(): found that %s is not a directory, aborting.\n", path);
  return 0;
} // is_dir

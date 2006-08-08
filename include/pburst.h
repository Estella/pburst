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

#ifndef __PBURST_H
#define __PBURST_H

#include <map>
#include <string>

#include <math.h>
#include <stdarg.h>

#include "defs.h"
#include "config.h"
#include "pburst_string.h"

using std::string;

using namespace pburst;

/**************************************************************************
 ** General Defines                                                      **
 **************************************************************************/

/************
 ** Server **
 ************/

#define PBURST_CONFIG_COMPRESS			PBURST_SERVER_OPTION_BOOL("compress", false)
#define PBURST_CONFIG_CONNECT_BURST		PBURST_SERVER_OPTION_STRING("burst", "burst/auth.burst")
#define PBURST_CONFIG_PORT			PBURST_SERVER_OPTION_STRING("port", "4400")
#define PBURST_CONFIG_PREAUTH			PBURST_SERVER_OPTION_BOOL("preauth", false)
#define PBURST_CONFIG_SCRIPTING			PBURST_SERVER_OPTION_BOOL("scripting", true)
#define PBURST_CONFIG_SERVER			PBURST_SERVER_OPTION_STRING("server", "127.0.0.1")
#define PBURST_CONFIG_SERVER_NUMERIC		PBURST_SERVER_OPTION_STRING("numeric", "ZX")
#define PBURST_CONFIG_SERVER_NAME		PBURST_SERVER_OPTION_STRING("name", "pburst.netburst.org")
#define PBURST_CONFIG_VERSION_REPLY		PBURST_SERVER_OPTION_STRING("version_reply", "Internet Relay Chat Protocol Burster")

/************
 ** Timers **
 ************/

#define PBURST_TIMER_CONNECT			60
#define PBURST_TIMER_LOG			2

/************************
 ** Logging Facilities **
 ************************/

#define PBURST_LOG_STDOUT			"pburst.log.stdout"
#define PBURST_LOG_NORMAL			"pburst.log.normal"
#define PBURST_LOG_DEBUG			"pburst.log.debug"
#define PBURST_LOG_DEBUG_SOCKET			"pburst.log.debug.socket"
#define PBURST_LOG_FLUSH			PBURST_SERVER_OPTION_BOOL("pburst_log_flush", false)

/*******************
 ** Default Paths **
 *******************/

#define PBURST_PATH_CONFIG		PBURST_SERVER_OPTION_STRING("pburst_path_config", "etc/pburst.conf")
#define PBURST_PATH_PID			PBURST_SERVER_OPTION_STRING("pburst_path_pid", "pburst.pid")
#define PBURST_PATH_LOGS		PBURST_SERVER_OPTION_STRING("pburst_path_logs", "logs/")
#define PBURST_PATH_LOGS_PBURST		PBURST_SERVER_OPTION_STRING("pburst_path_logs_pburst", "pburst.log")
#define PBURST_PATH_LOGS_DEBUG		PBURST_SERVER_OPTION_STRING("pburst_path_logs_debug", "pburst.debug.log")
#define PBURST_PATH_LOGS_DEBUG_SOCKET	PBURST_SERVER_OPTION_STRING("pburst_path_logs_debug_socket", "pburst.debug.socket.log")

#define TRUE 				1
#define FALSE				0

#define PBURST_MIN(a, b)			((a) < (b) ? (a) : (b))
#define PBURST_MAX(a, b)			((a) > (b) ? (a) : (b))

// time frames
#define PBURST_TIME_YEAR			31536000
#define PBURST_TIME_MONTH			1814400
#define PBURST_TIME_WEEK			604800
#define PBURST_TIME_DAY			86400
#define PBURST_TIME_HOUR			3600
#define PBURST_TIME_MINUTE			60

#define DEFAULT_QUIT			"Shutdown"
#define DEFAULT_RESTART			"Restart"

/**************************************************************************
 ** Structures                                                           **
 **************************************************************************/

/**************************************************************************
 ** Macro's                                                              **
 **************************************************************************/

#define ERR_BUFFER_ALLOC	"buffer allocation error"

#define ERROR_REPLY_BUFFER_ALLOCATION	"Buffer allocation error."
#define ERROR_REPLY_FAILED_JUMP		"Failed to jump."
#define ERROR_REPLY_FAILED_RESTART	"Failed to restart."

#define MAXBUF			512

#define UNUSED(x)		(void) x

/**************************************************************************
 ** Proto types                                                          **
 **************************************************************************/

int is_dir(const char *);
int send_auth(const char *, const char *, const char *);
int sectotf(time_t, struct timeframe *);

void deactivate(const char *);
void header();
void usage();
void version();

#endif

/*
** tinylog - minimalistic logging facility supporting stderr/syslog
**
** Copyright (c) 2016 Victor Toni.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, version 3.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Lesser Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
*/

#ifndef __TESTUTIL_H
#define __TESTUTIL_H


#include <stdlib.h>     /* exit() */
#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <unistd.h>

#include "../src/tinylog.h"

extern const char* USAGE;
/*
 =
"Usage: %s [-h] [-d] [-s] [-v]\n"
"\n"
"   -h   Display this help screen\n"
"   -e   Output all messages on stderr\n"
"   -s   Output messages to syslog (up to LOG_DEBUG)\n"
"   -d   Turn on dev_logging (prints __FUNCTION__ & __LINE__)\n"
"   -v   Increase log level (default is LOG_WARNING)\n"
"\n"
;
*/

struct LogConf {
    int log_threshold;
    log_dest_t log_dest;
    bool dev_logging;
};
typedef struct LogConf log_conf_t;

void do_test_logging( int err_no );
void do_test_logging_l( int err_no, int log_threshold );
void do_test_logging_all( int err_no );

log_conf_t parseopt( const int argc, char* const argv[], const char *usage );

#endif

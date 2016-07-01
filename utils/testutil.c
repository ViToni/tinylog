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


#include "testutil.h"

const char* USAGE=
"Usage: %s [-h] [-d] [-s] [-v]\n"
"\n"
"   -h   Display this help screen\n"
"   -e   Output all messages on stderr\n"
"   -s   Output messages to syslog (up to LOG_DEBUG)\n"
"   -d   Turn on dev_logging (prints __FUNCTION__ & __LINE__)\n"
"   -v   Increase log level (default is LOG_WARNING)\n"
"\n"
;


log_conf_t parseopt( const int argc, char* const argv[], const char *usage )
{
    log_conf_t log_conf;

    log_conf.log_threshold = LOG_WARNING;
    log_conf.log_dest = 0;
    log_conf.dev_logging = false;

    int c;

    if (argc == 1) {
        fprintf(stderr, usage, argv[0]);
        exit(0);
    }

    // Parse the commandline options and setup basic settings..
    while ((c = getopt(argc, argv, "esdhv")) != -1) {
        switch (c) {
        case 'e':
            log_conf.log_dest |= STDERR;
            break;
        case 's':
            log_conf.log_dest |= SYSLOG;
            break;
        case 'd':
            log_conf.dev_logging = true;
            break;
        case 'v':
            log_conf.log_threshold++;
            break;
        case 'h':
            fprintf(stderr, usage, argv[0]);
            exit(0);
            break;
        default:
            exit(1);
            break;
        }
    }

    return log_conf;
}


void do_test_logging( int err_no ) {
    tinylog( -1, err_no, "-----------------------------------------" );
    log_EMERG(   err_no, "Test log for %s", "LOG_EMERG" );
    log_ALERT(   err_no, "Test log for %s", "LOG_ALERT" );
    log_CRIT(    err_no, "Test log for %s", "LOG_CRIT" );
    log_ERR(     err_no, "Test log for %s", "LOG_ERR" );
    log_WARNING( err_no, "Test log for %s", "LOG_WARNING" );
    log_NOTICE(  err_no, "Test log for %s", "LOG_NOTICE" );
    log_INFO(    err_no, "Test log for %s", "LOG_INFO" );
    log_DEBUG(   err_no, "Test log for %s", "LOG_DEBUG" );
    log_TRACE(   err_no, "Test log for %s", "LOG_TRACE" );
    log_INIT(    err_no, "Test log for %s", "LOG_INIT" );
    tinylog( -1, err_no, "-----------------------------------------" );
}

void do_test_logging_l( int err_no, const int log_threshold ) {
    set_log_threshold( log_threshold );
    do_test_logging( err_no );
}

void do_test_logging_all( int err_no ) {
    do_test_logging_l( err_no, LOG_EMERG );
    do_test_logging_l( err_no, LOG_ALERT );
    do_test_logging_l( err_no, LOG_CRIT );
    do_test_logging_l( err_no, LOG_ERR );
    do_test_logging_l( err_no, LOG_WARNING );
    do_test_logging_l( err_no, LOG_NOTICE );
    do_test_logging_l( err_no, LOG_INFO );
    do_test_logging_l( err_no, LOG_DEBUG );
    do_test_logging_l( err_no, LOG_TRACE );
    do_test_logging_l( err_no, LOG_INIT );
}


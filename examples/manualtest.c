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


#include "../utils/testutil.h"

extern const char* USAGE;

int main( const int argc, char* const argv[] ) {

    log_conf_t log_conf = parseopt (argc, argv, USAGE);

    char* log_dest_str =
            log_conf.log_dest == STDERR ?   "STDERR" :
            log_conf.log_dest == SYSLOG ?   "SYSLOG" :
            log_conf.log_dest == BOTH   ?   "BOTH" :
                                            "unknown";

    fprintf( stderr, "Set log destination to '%s'\n", log_dest_str );
    fprintf( stderr, "Set log threshold to '%s'\n", strseverity(log_conf.log_threshold) );

    fprintf( stderr, "Set up logging...\n" );

    open_tinylog(
        argv[0],
        LOG_PID, 
        LOG_USER,
        log_conf.log_threshold, // Log threshold, LOG_WARNING .... LOG_DEBUG, LOG_TRACE, LOG_INIT
        log_conf.log_dest,      // Where should the log go to
        false,                  // Whether the log should quit the program on errors
        log_conf.dev_logging    // dev_logging - Should __FUNCTION__ & __LINE__ appear on stderr
    );

    fprintf( stderr, "Starting log tests...\n" );

    // do test logging with given defaults
    do_test_logging( 0 );
}

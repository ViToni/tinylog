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


#ifndef _TINYLOG_H
#define _TINYLOG_H


#include <stdlib.h>     /* exit() */
#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <stdbool.h>    /* bool data type */

#include <string.h>     /* strerror() */
#include <time.h>       /* strftime(), localtime() */

#include <sys/time.h>   /* gettimeofday() */

#include <syslog.h>


#ifdef __cplusplus
extern "C" {
#endif

//#################################################################################
//  Globals
//#################################################################################

// additional log levels
#define LOG_TRACE   (LOG_DEBUG+1)
#define LOG_INIT    (LOG_DEBUG+2)

/**
** Possible logging destinations
*/
enum LogDestination {
    STDERR=1,
    SYSLOG=2,
    BOTH=3
};
typedef enum LogDestination log_dest_t;

//#################################################################################
//  Lib function prototypes.
//#################################################################################

/**
** Changes the defaults for the logging environment.
** No need to call, if the defaults fit already
*/
void setup_tinylog (
        const int log_threshold,        // Log threshold, LOG_CRIT, ..., LOG_WARNING, ..., LOG_DEBUG, LOG_TRACE, LOG_INIT
        const log_dest_t log_dest,      // Where the log messages should go to
        const bool exit_on_error,       // Whether the log should quit the program on errors
        const bool dev_logging          // Whether __FUNCTION__ & __LINE__ should appear on stderr log
);

/**
** Setup tinylog and call openlog to be prepared for log messages to syslog.
** No need to call if openlog() is called on its own and defaults are fine.
*/
void open_tinylog (
        const char *ident,              // the identifier to use for syslog
        const int options,
        const int facility,
        const int log_threshold,        // Log threshold, LOG_CRIT, ..., LOG_WARNING, ..., LOG_DEBUG, LOG_TRACE, LOG_INIT
        const log_dest_t log_dest,      // Where the log messages should go to 
        const bool exit_on_error,       // Whether the log should quit the program on errors
        const bool dev_logging          // Whether __FUNCTION__ & __LINE__ should appear on stderr log
);


/**
** Log threshold, LOG_CRIT, ..., LOG_WARNING, ..., LOG_DEBUG, LOG_TRACE, LOG_INIT
**
** default: LOG_WARNING
*/
void set_log_threshold( const int log_threshold );
int  get_log_threshold( void );


/**
** Returns true, if the given severity is equal to or below the log threshold.
** Returns false otherwise.
*/
bool is_enabled( const int severity );


/**
** Whether the log severity would quit the program on errors and 
** the 'exit_on_error' feature is active
*/
bool would_exit( const int severity );


/**
** Where the log messages should go to
**
** default: STDERR
*/
void        set_log_dest( const log_dest_t log_dest );
log_dest_t  get_log_dest( void );


/**
** Whether the log should quit the program on errors
**
** default: false
*/
void set_exit_on_error( const bool exit_on_error );
bool get_exit_on_error( void );


/**
** Whether the stderr log should include __FUNCTION__ and __LINE__ information
** (no need to spam syslog with these details)
**
** default: false
*/
void set_dev_logging( const bool dev_logging );
bool get_dev_logging( void );


/**
** Exit if a 'LOG_ERROR' or anything more critical was reported
** and 'exit_on_error' is set.
*/
void do_exit_on_error( const int severity );


/**
** Retrieve the string representation (5 chars) of the given log severity.
** If the given severity is unknown UNKNOWN_SEVERITY ('*****') will be returned.
*/
const char *strseverity( const int severity );


/**
** Retrieve the string representation (6 chars) of the given log destination.
** If the given log destination is unknown UNKNOWN_LOG_DEST ('******') will be returned.
*/
const char *strlog_dest( const log_dest_t log_dest );


/**
** Main routine handling the logging.
*/
void __tinylog( const int severity, const int errno, const char *func, const int line, const char *fmt_str, ... );


/**
** Short circuit log level evaluation to avoid unnecessary function calls
** for argruments pretty printing, etc.
*/
#define tinylog(severity, errno, fmt_str, args...)  do \
{   /* return fast if no message would be logged to avoid unnecessary function calls */ \
    /* but only if would not exit, so the last error message ist still shown */ \
    /* the logging routine will take care of the exit */ \
    if( !is_enabled( (severity) ) && !would_exit( (severity) ) ) \
    { \
        break; \
    } \
   __tinylog((severity), (errno), __FUNCTION__, __LINE__, (fmt_str), ##args); \
} while (0)


/**
** Convenience macros for shorthand log calls.
*/
#define log_EMERG(   errno, fmt_str, args...)  tinylog(LOG_EMERG,   (errno), (fmt_str), ##args)
#define log_ALERT(   errno, fmt_str, args...)  tinylog(LOG_ALERT,   (errno), (fmt_str), ##args)
#define log_CRIT(    errno, fmt_str, args...)  tinylog(LOG_CRIT,    (errno), (fmt_str), ##args)
#define log_ERR(     errno, fmt_str, args...)  tinylog(LOG_ERR,     (errno), (fmt_str), ##args)
#define log_WARNING( errno, fmt_str, args...)  tinylog(LOG_WARNING, (errno), (fmt_str), ##args)
#define log_NOTICE(  errno, fmt_str, args...)  tinylog(LOG_NOTICE,  (errno), (fmt_str), ##args)
#define log_INFO(    errno, fmt_str, args...)  tinylog(LOG_INFO,    (errno), (fmt_str), ##args)
#define log_DEBUG(   errno, fmt_str, args...)  tinylog(LOG_DEBUG,   (errno), (fmt_str), ##args)
#define log_TRACE(   errno, fmt_str, args...)  tinylog(LOG_TRACE,   (errno), (fmt_str), ##args)
#define log_INIT(    errno, fmt_str, args...)  tinylog(LOG_INIT,    (errno), (fmt_str), ##args)


#ifdef __cplusplus
}
#endif


#endif // __TINYLOG_H

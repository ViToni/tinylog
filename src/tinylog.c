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

#include "tinylog.h"

/**
** Textual representation of log levels / severities
*/
static const char SEVERITIES[][ 6 ] =
{
        "EMERG", "ALERT", "CRIT ", "ERROR",     // supported by syslog 
        "Warn ", "Notic", "Info ", "Debug",     // supported by syslog
        "Trace", "Init "                        // NOT supported by syslog,  will go only to stderr
};

/**
** Textual representation for unknown log levels / severities
*/
static const char UNKNOWN_SEVERITIY[ 6 ] = "*****";


/**
** Count of log levels / severites, used to detect if log level / severity is known
*/
static const int SEVERITY_COUNT = ( int) sizeof( SEVERITIES ) / sizeof( SEVERITIES[0] );


/**
** Textual representation of log destination
*/
static const char LOG_DEST[3][ 7 ] =
{
        "stderr", 
        "syslog", 
        "both"
};


/**
** Textual representation for unknown log destination
*/
static const char UNKNOWN_LOG_DEST[ 7 ] = "******";

/**
** Log threshold, LOG_WARNING .... LOG_DEBUG, LOG_TRACE, LOG_INIT
*/
static int         __log_threshold = LOG_WARNING;    

/**
** Whether the log should quit the program on errors
*/
static bool        __exit_on_error = false;          

/**
** Where the log should go to
*/
static log_dest_t  __log_dest = STDERR;

/**
** Should __FUNCTION__ & __LINE__ appear on stderr
*/
static bool        __dev_logging = false;

// internal prototypes

static void __print_log_prefix( FILE *stream, const int severity, const char *func, const int line );


// functions

void setup_tinylog (
        const int log_threshold,        // Log threshold, LOG_CRIT, ..., LOG_WARNING, ..., LOG_DEBUG, LOG_TRACE, LOG_INIT
        const log_dest_t log_dest,      // Where the log messages should go to
        const bool exit_on_error,       // Whether the log should quit the program on errors
        const bool dev_logging          // Whether __FUNCTION__ & __LINE__ should appear on stderr log
) {
    set_log_threshold( log_threshold );
    set_log_dest( log_dest );
    set_exit_on_error( exit_on_error );
    set_dev_logging( dev_logging );
}


void set_log_threshold( const int log_threshold )
{
    if( log_threshold < 0 )
    {
        log_WARNING(0, "Log threshold my not be less than zero, was: %d. Ignoring", log_threshold);
        return;
    }

    if( log_threshold != __log_threshold )
    {
        __log_threshold = log_threshold;
        log_TRACE(0, "Set 'log_dest' to: %s", strseverity( log_threshold ) );
    }
}

int get_log_threshold( void )
{
    return __log_threshold;
}

bool is_enabled( const int severity )
{
    return (severity <= __log_threshold);
}

/**
** Where the log messages should go to
**
** default: STDERR
*/
void set_log_dest( const log_dest_t log_dest )
{
    if(     log_dest == STDERR ||
            log_dest == SYSLOG || 
            log_dest == BOTH
    )
    {
        if( log_dest != __log_dest )
        {
            __log_dest = log_dest;

            log_TRACE(0, "Set 'log_dest' to: %s", strlog_dest( log_dest ) );
        }
        return;
    }

    log_WARNING(0, "Unknown log destination: %d. Ignoring.", log_dest);
}

log_dest_t get_log_dest( void )
{
    return __log_dest;
}

/**
** Whether the log should quit the program on errors (default: false)
**
** default: false
*/
void set_exit_on_error( const bool exit_on_error )
{
    if ( exit_on_error != __exit_on_error )
    {
        __exit_on_error = exit_on_error;

        log_TRACE(0, "Set 'exit_on_error' to: %s", exit_on_error ? "true" : "false" );
    }
}

bool get_exit_on_error( void )
{
    return __exit_on_error;
}

/**
** Whether the stderr log should include __FUNCTION__ and __LINE__ information
**
** default: false
*/
void set_dev_logging( const bool dev_logging )
{
    if( dev_logging != __dev_logging )
    {   
        __dev_logging = dev_logging;

        log_TRACE(0, "Set 'dev_logging' to: %s", dev_logging ? "true" : "false" );
    }
}

bool get_dev_logging( void )
{
    return __dev_logging;
}

/**
** Would the given severity and actual configuration exit the calling program?
*/
bool would_exit( const int severity )
{
    return( severity <= LOG_ERR && __exit_on_error );
}

/**
** Exit if a 'LOG_ERROR' or something more critical was reported
** and 'exit_on_error' is set.
*/
inline void do_exit_on_error( const int severity )
{
    if( would_exit( severity ) )
    {
        exit( -1 );
    }
}

void open_tinylog (
        const char *ident,
        const int options,
        const int facility,
        const int log_threshold,        // Log threshold, LOG_CRIT, ..., LOG_WARNING, ..., LOG_DEBUG, LOG_TRACE, LOG_INIT
        const log_dest_t log_dest,      // Where the log messages should go to
        const bool exit_on_error,       // Whether the log should quit the program on errors
        const bool dev_logging          // Whether __FUNCTION__ & __LINE__ should appear on stderr log
)
{
    openlog( ident, options, facility );

    setup_tinylog(
            log_threshold,
            log_dest,
            exit_on_error,
            dev_logging
    );
}

/**
** Main routine handling the logging.
*/
void __tinylog(
    const int severity,
    const int errno,
    const char *func,
    const int line,
    const char *fmt_str, ... 
)
{
    // nothing to log, return fast
    if( __log_threshold < severity )
    {
        if( severity <= LOG_ERR && __exit_on_error )
        {
            exit( -1 );
        }

        return;
    }

    char log_msg[ 128 ];    // stores the log message
    unsigned len;           // current length of thwe log message

    va_list arg_pt;
    va_start( arg_pt, fmt_str );
    len = vsnprintf( log_msg, sizeof( log_msg ), fmt_str, arg_pt );

    // get the verbose name for errno
    if( errno > 0 )
    {
        snprintf(
                log_msg + len,
                sizeof( log_msg ) - len,
                "; Errno(%d): %s", errno, strerror( errno )
        );
    }
    va_end( arg_pt );

    if(severity <= __log_threshold )
    {
        if( (__log_dest & STDERR) == STDERR )
        {
            __print_log_prefix ( stderr, severity, func, line );

            fprintf(stderr, "%s\n", log_msg);
        }

        if( (__log_dest & SYSLOG) == SYSLOG 
            && severity <= LOG_DEBUG
        ) 
        {
            // log only known severity levels to syslog
            syslog( severity, "%s", log_msg );
        }
    }

    // check severity an exit eventually
    do_exit_on_error( severity );
}


/**
** Retrieve the string representation (5 chars) of the given severity.
** If the given severity is unknown UNKNOWN_SEVERITY ('*****') will be returned.
*/
const char *strseverity( const int severity )
{
    // check for valid severity
    if( 0 <= severity && severity < SEVERITY_COUNT )
    {
        return SEVERITIES[ severity ];
    }

    // return default for unknown severity
    return UNKNOWN_SEVERITIY;
}


/**
** Retrieve the string representation (6 chars) of the given log destination.
** If the given log destination is unknown UNKNOWN_LOG_DEST ('******') will be returned.
*/
const char *strlog_dest( const log_dest_t log_dest )
{
    // check for valid log destination
    switch( log_dest ){
        case STDERR:
        case SYSLOG: 
        case BOTH:
            return LOG_DEST[ log_dest - 1 ];    // log_dest_t starts at 1
    }

    // return default for unknown log destination
    return UNKNOWN_LOG_DEST;
}


static void __strtime ( char* str, const int str_len, const time_t *timep )
{
    struct tm result;

    // Format as 14:37:52 
    strftime(str, str_len, "%H:%M:%S", localtime_r(timep, &result));

}

static void __print_log_prefix (FILE *stream, const int severity, const char *func, const int line )
{
    const char *severity_str = strseverity( severity );

    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
  
    char cur_time_str[10];
    __strtime( cur_time_str, 10, &cur_time.tv_sec );

    const int millis = cur_time.tv_usec / 1000;

    // looks like:
    // [Trace] 14:37:52,628 
    fprintf( stream, "[%5s] %s,%03d ", severity_str, cur_time_str, millis );

	if( __dev_logging )
    {
        // append to stream:
        // function_name():<line_number>: 
        fprintf( stream, "%s():%03d: ", func, line );
    }
}

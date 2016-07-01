# tinylog

Introduction
============

**tinylog** is a minimalistic logging interface supporting logging to `stderr` and/or `syslog`.

To avoid (re)implementing log functionality for every small project **tinylog** provides an easy start for logging.  
Configuration breaks down to four options:
 * Log threshold, can be any of
    + Default log levels (can be used both for `syslog` and `stderr`)
        - `LOG_EMERG`  
        - `LOG_ALERT`  
        - `LOG_CRIT`  
        - `LOG_ERR`  
        - `LOG_WARNING` (**default**)
        - `LOG_NOTICE`  
        - `LOG_INFO`  
        - `LOG_DEBUG`  
    + Custom log levels (these go only to `stderr`)
        - `LOG_TRACE`
        - `LOG_INIT`
 * Log destination
   - `STDERR` (default)
   - `SYSLOG`
   - `BOTH`
 * Exit on errors (default: **off**)
 * Developer logging (default: **off**, includes `__FUNCTION__` and `__LINE__` information into logging to `stderr`)

Build
=====

Aften getting the latest source via `git clone https://github.com/ViToni/tinylog.git`  
Just call:

    gmake

in the `tinylog` directory.

The created `bin` directory contains some test appilcations for an easy start.  
The corresponding code for the executables is found in `examples` (including the `hello-world.c` example below)
and `utils`. 

Download
========

Get `tinylog.h` and `tinylog.c` here:  
https://github.com/ViToni/tinylog/tree/master/src

You do not need the rest of this repository unless you wish to run the tests or develop `tinylog` yourself.  

Usage
=====

1. Download the latest version of `tinylog.h` and `tinylog.c` and drop it somewhere in your
   project.
2. Include the header file anywhere you wish to use logging.

Example `hello-world.c` (resides in `examples`):

    #include <stdio.h>

    #include "../src/tinylog.h"

    int main() {

        /* Set minimum log level to debug */
        /* (default: LOG_WARNING) */
        set_log_threshold(LOG_DEBUG);

        /* Write a message */
        tinylog(LOG_INFO, 0, "Hello, %s!", "world");

        /* add __FUNCTION__ and __LINE__ of log callee to output (only for stderr) */
        /* (default: false) */
        set_dev_logging( true );

        /* Write a message */
        tinylog(LOG_DEBUG, 0, "Hello, %s!", "developer world");

        /* initializes connection to syslog */
        openlog("hello-world", LOG_PID, LOG_USER);

        /* set the log destination to SYSLOG */
        /* (default: STDERR) */
        set_log_dest( SYSLOG );

        /* Write a message */
        tinylog(LOG_NOTICE, 0, "Hello, %s!", "syslog world");

        /* set the log destination to SYSLOG & STDERR*/
        /* (default: STDERR) */

        /* Write a message */
        tinylog(LOG_NOTICE, 0, "Hello, %s!", "both dev worlds");

        /* Turn off __FUNCTION__ and __LINE__ of log callee to output (only for stderr) */
        /* (default: false) */
        set_dev_logging( false );

        /* Write a message */
        tinylog(LOG_NOTICE, 0, "Hello, %s!", "both worlds");

        return 0;
    }

will create the following output on `stderr`:

    [Info ] 19:11:15,547 Hello, world!
    [Debug] 19:11:15,548 main():019: Hello, developer world!
    [Notic] 19:11:15,548 main():036: Hello, both dev worlds!
    [Notic] 19:11:15,549 Hello, both worlds!

and on `syslog`:

    Jul  3 19:11:15  hello-world[24490]: Hello, syslog world!
    Jul  3 19:11:15  hello-world[24490]: Hello, both dev worlds!
    Jul  3 19:11:15  hello-world[24490]: Hello, both worlds!

While logging to `stderr` works out of the box, logging to `syslog`
has to be initialized with a call of `openlog()` such as:  

        /* initializes connection to syslog */
        openlog("hello-world", LOG_PID, LOG_USER);

Performance
===========

**tinylog** is very compact and tries to minimize impact on performance.  
While pretty printing is especially useful for `LOG_DEBUG` or `LOG_TRACE` level messages
it's important that the number of `LOG_DEBUG` or `LOG_TRACE` message have as little impact as possible on performace.  
Ideally the additionally log messages should have no impact at all if the corresponding log level is not active.  

This is achieved by using a macro wrapper around the log routine.
The macro wrapper checks the log level before arguments for the log message are evaluated
thus preventing the execution of any functions doing pretty printing needed for the log message.

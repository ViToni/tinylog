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
        set_log_dest( BOTH );

        /* Write a message */
        tinylog(LOG_NOTICE, 0, "Hello, %s!", "both dev worlds");

        /* Turn off __FUNCTION__ and __LINE__ of log callee to output (only for stderr) */
        /* (default: false) */
        set_dev_logging( false );

        /* Write a message */
        tinylog(LOG_NOTICE, 0, "Hello, %s!", "both worlds");

        return 0;
    }


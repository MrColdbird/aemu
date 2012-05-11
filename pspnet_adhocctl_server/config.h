#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <time.h>

// Server Listening Port
#define SERVER_PORT 27312

// Listener Connection Backlog (aka. Max Concurrent Logins)
#define SERVER_LISTEN_BACKLOG 128

// Server User Maximum
#define SERVER_USER_MAXIMUM 1024

// Server User Timeout (in seconds)
#define SERVER_USER_TIMEOUT 15

// Server SQLite3 Database
#define SERVER_DATABASE "database.db"

// Server Status Logfile
#define SERVER_STATUS_LOGFILE "status.log"

// Server Shutdown Message
#define SERVER_SHUTDOWN_MESSAGE "PROMETHEUS HUB IS SHUTTING DOWN!"

#endif

#ifndef _ADHOCCTL_GLOBALS_H_
#define _ADHOCCTL_GLOBALS_H_

// Adhoc-Control Metaport
#define ADHOCCTL_METAPORT 27312
#define ADHOCCTL_MAX_PLAYERS 16
#define ADHOCCTL_SEND_TIMEOUT 1000000
#define ADHOCCTL_RECV_TIMEOUT (ADHOCCTL_SEND_TIMEOUT / ADHOCCTL_MAX_PLAYERS)
#define ADHOCCTL_DEAD_FRIEND_TIMEOUT (ADHOCCTL_SEND_TIMEOUT * 20)

// PC Debugger
#define ADHOCCTL_PC_DEBUGGER_IP "192.168.1.103"
#define ADHOCCTL_PC_DEBUGGER_MAC "\x00\x1F\x1F\x3C\x59\x48"
#define ADHOCCTL_PC_DEBUGGER_NAME "Debugger"

// Error Codes
#define ADHOC_NO_ENTRY 0x80410716
#define ADHOCCTL_INVALID_ARG 0x80410B04
#define ADHOCCTL_ID_NOT_FOUND 0x80410B06
#define ADHOCCTL_ALREADY_INITIALIZED 0x80410B07
#define ADHOCCTL_NOT_INITIALIZED 0x80410B08
#define ADHOCCTL_NOT_ENTER_GAMEMODE 0x80410B0C
#define ADHOCCTL_CHANNEL_NOT_AVAILABLE 0x80410B0D
#define ADHOCCTL_BUSY 0x80410B10
#define ADHOCCTL_HANDLER_MAX 0x80410B12
#define ADHOCCTL_STACKSIZE_TOO_SHORT 0x80410B13

// Library State
extern int _init;

// Game Product Code
extern SceNetAdhocctlAdhocId _product_code;

// Event Handler
#define ADHOCCTL_MAX_HANDLER 4
extern SceNetAdhocctlHandler _event_handler[ADHOCCTL_MAX_HANDLER];
extern void * _event_args[ADHOCCTL_MAX_HANDLER];

// Friend Control Status
extern SceNetAdhocctlStatusFriend * _friends;

// Local Control Status
extern SceNetAdhocctlStatusBase _status;

// Access Point Setting
extern int _hotspot;

// Meta Socket
extern int _metasocket;

// Peer Locker
extern int _peerlock;

// Netconf Status
extern int _netconf_status;

// Bit-Values
extern int _one;
extern int _zero;

#endif

#ifndef _ADHOCCTL_GLOBALS_H_
#define _ADHOCCTL_GLOBALS_H_

// Adhoc-Control Metaport
#define ADHOCCTL_METAPORT 27312
#define ADHOCCTL_RECV_TIMEOUT 100000
#define ADHOCCTL_PING_TIMEOUT 5000000

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

// Thread Status
extern int _thread_status;

// UPNP Library Handle
extern int _upnp_uid;

// Game Product Code
extern SceNetAdhocctlAdhocId _product_code;

// Game Group
extern SceNetAdhocctlParameter _parameter;

// Peer List
extern SceNetAdhocctlPeerInfo * _friends;

// Scan Network List
extern SceNetAdhocctlScanInfo * _networks;

// Event Handler
#define ADHOCCTL_MAX_HANDLER 4
extern SceNetAdhocctlHandler _event_handler[ADHOCCTL_MAX_HANDLER];
extern void * _event_args[ADHOCCTL_MAX_HANDLER];

// Access Point Setting
extern int _hotspot;

// Meta Socket
extern int _metasocket;

#ifdef ENABLE_PEERLOCK
// Peer Locker
extern int _peerlock;
#endif

#ifdef ENABLE_NETLOCK
// Network Locker
extern int _networklock;
#endif

// Netconf Status
extern int _netconf_status;

// Bit-Values
extern int _one;
extern int _zero;

#endif

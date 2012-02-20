#ifndef _ADHOCCTL_STRUCTURES_H_
#define _ADHOCCTL_STRUCTURES_H_

// Network Types for Scan Procedure
#define ADHOCCTL_MODE_ADHOC 0
#define ADHOCCTL_MODE_GAMEMODE 1

// Network Types for Internal Use
#define ADHOCCTL_NETWORK_TYPE_DISCONNECTED 0
#define ADHOCCTL_NETWORK_TYPE_ADHOC 1
#define ADHOCCTL_NETWORK_TYPE_GAME_1A 2
#define ADHOCCTL_NETWORK_TYPE_GAME_2A 3
#define ADHOCCTL_NETWORK_TYPE_GAME_2B 4

// Event Types for Event Handler
#define ADHOCCTL_EVENT_CONNECT 1
#define ADHOCCTL_EVENT_DISCONNECT 2
#define ADHOCCTL_EVENT_SCAN 3

// Internal Thread States
#define ADHOCCTL_STATE_DISCONNECTED 0
#define ADHOCCTL_STATE_CONNECTED 1
#define ADHOCCTL_STATE_GAMEMODE 3

// Kernel Utility Netconf Adhoc Types
#define UTILITY_NETCONF_TYPE_CONNECT_ADHOC 2
#define UTILITY_NETCONF_TYPE_CREATE_ADHOC 4
#define UTILITY_NETCONF_TYPE_JOIN_ADHOC 5

// Kernel Utility States
#define UTILITY_NETCONF_STATUS_NONE 0
#define UTILITY_NETCONF_STATUS_FINISHED 3

// Ethernet Address
#define ETHER_ADDR_LEN 6
typedef struct SceNetEtherAddr {
	uint8_t data[ETHER_ADDR_LEN];
} SceNetEtherAddr;

// Adhoc ID (Game Product Key)
#define ADHOCCTL_ADHOCID_LEN 9
typedef struct SceNetAdhocctlAdhocId {
	int type;
	uint8_t data[ADHOCCTL_ADHOCID_LEN];
	uint8_t padding[3];
} SceNetAdhocctlAdhocId;

// Adhoc Virtual Network Name
#define ADHOCCTL_GROUPNAME_LEN 8
typedef struct SceNetAdhocctlGroupName {
	uint8_t data[ADHOCCTL_GROUPNAME_LEN];
} SceNetAdhocctlGroupName;

// Virtual Network Host Information
typedef struct SceNetAdhocctlBSSId {
	SceNetEtherAddr mac_addr;
	uint8_t padding[2];
} SceNetAdhocctlBSSId;

// Virtual Network Information
typedef struct SceNetAdhocctlScanInfo{
	struct SceNetAdhocctlScanInfo * next;
	int channel;
	SceNetAdhocctlGroupName group_name;
	SceNetAdhocctlBSSId bssid;
	int mode;
} SceNetAdhocctlScanInfo;

// Player Nickname
#define ADHOCCTL_NICKNAME_LEN 128
typedef struct SceNetAdhocctlNickname {
	uint8_t data[ADHOCCTL_NICKNAME_LEN];
} SceNetAdhocctlNickname;

// Active Virtual Network Information
typedef struct SceNetAdhocctlParameter {
	int channel;
	SceNetAdhocctlGroupName group_name;
	SceNetAdhocctlBSSId bssid;
	SceNetAdhocctlNickname nickname;
} SceNetAdhocctlParameter;

// Peer Information
typedef struct SceNetAdhocctlPeerInfo {
	struct SceNetAdhocctlPeerInfo * next;
	SceNetAdhocctlNickname nickname;
	SceNetEtherAddr mac_addr;
	uint32_t ip_addr;
	uint8_t padding[2];
	uint64_t last_recv;
} SceNetAdhocctlPeerInfo;

// Game Mode Peer List
#define ADHOCCTL_GAMEMODE_MAX_MEMBERS 16
typedef struct SceNetAdhocctlGameModeInfo {
	int num;
	SceNetEtherAddr member[ADHOCCTL_GAMEMODE_MAX_MEMBERS];
} SceNetAdhocctlGameModeInfo;

// Control Status Base Data
typedef struct SceNetAdhocctlStatusBase {
	SceNetAdhocctlGroupName group_name;
	SceNetAdhocctlNickname player_name;
	SceNetEtherAddr player_mac;
	uint8_t network_type;
	uint8_t gamemode_master;
} SceNetAdhocctlStatusBase;

// Control Status Network Addon
typedef struct SceNetAdhocctlStatusPacket {
	SceNetAdhocctlStatusBase base;
	uint8_t product_code[ADHOCCTL_ADHOCID_LEN];
} SceNetAdhocctlStatusPacket;

// Local Control Status
typedef struct SceNetAdhocctlStatusFriend {
	struct SceNetAdhocctlStatusFriend * next;
	SceNetAdhocctlStatusBase base;
	uint32_t ip_addr;
	uint64_t last_recv;
} SceNetAdhocctlStatusFriend;

// Adhoc Network Control Handler
typedef void(*SceNetAdhocctlHandler)(int event, int error_code, void * arg);

typedef struct SceUtilityParamBase {
	unsigned int size;
	int message_lang;
	int ctrl_assign;
	int main_thread_priority;
	int sub_thread_priority;
	int font_thread_priority;
	int sound_thread_priority;
	int result; 
	int reserved1;
	int reserved2;
	int reserved3;
	int reserved4;
} SceUtilityParamBase;

typedef struct SceUtilityNetconfAdhocParam {
	SceNetAdhocctlGroupName group_name;
	uint32_t timeout;
} SceUtilityNetconfAdhocParam;

typedef struct SceUtilityNetconfParam {
	SceUtilityParamBase base;
	int type;
	SceUtilityNetconfAdhocParam * adhoc_param;
	uint32_t browser_available;
	uint32_t browser_flag;
	uint32_t wifisvc_available;
} SceUtilityNetconfParam;

#endif

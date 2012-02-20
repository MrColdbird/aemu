#ifndef _ADHOC_STRUCTURES_H_
#define _ADHOC_STRUCTURES_H_

// Ethernet Address
#define ETHER_ADDR_LEN 6
typedef struct SceNetEtherAddr {
	uint8_t data[ETHER_ADDR_LEN];
} SceNetEtherAddr;

// Socket Polling Event Listener
typedef struct SceNetAdhocPollSd {
	int id;
	int events;
	int revents;
} SceNetAdhocPollSd;

// PDP Socket Status
typedef struct SceNetAdhocPdpStat {
	struct SceNetAdhocPdpStat * next;
	int id;
	SceNetEtherAddr laddr;
	uint16_t lport;
	uint32_t rcv_sb_cc;
} SceNetAdhocPdpStat;

// PTP Socket Status
typedef struct SceNetAdhocPtpStat {
	struct SceNetAdhocPtpStat * next;
	int id;
	SceNetEtherAddr laddr;
	SceNetEtherAddr paddr;
	uint16_t lport;
	uint16_t pport;
	uint32_t snd_sb_cc;
	uint32_t rcv_sb_cc;
	int state;
} SceNetAdhocPtpStat;

// Gamemode Optional Peer Buffer Data
typedef struct SceNetAdhocGameModeOptData {
	uint32_t size;
	uint32_t flag;
	uint64_t last_recv;
} SceNetAdhocGameModeOptData;

// Gamemode Buffer Status
typedef struct SceNetAdhocGameModeBufferStat {
	struct SceNetAdhocGameModeBufferStat * next;
	int id;
	void * ptr;
	uint32_t size;
	uint32_t master;
	SceNetAdhocGameModeOptData opt;
} SceNetAdhocGameModeBufferStat;

#endif

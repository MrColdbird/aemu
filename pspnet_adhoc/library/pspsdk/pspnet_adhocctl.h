#ifndef _PSPNET_ADHOCCTL_H_
#define _PSPNET_ADHOCCTL_H_

// Adhoc-Control Metaport
#define ADHOCCTL_METAPORT 27312

// Adhoc Nickname
#define ADHOCCTL_NICKNAME_LEN 128
typedef struct SceNetAdhocctlNickname {
	uint8_t data[ADHOCCTL_NICKNAME_LEN];
} SceNetAdhocctlNickname;

// Peer Information
typedef struct SceNetAdhocctlPeerInfo {
	struct SceNetAdhocctlPeerInfo * next;
	SceNetAdhocctlNickname nickname;
	SceNetEtherAddr mac_addr;
	uint32_t ip_addr;
	uint8_t padding[2];
	uint64_t last_recv;
} SceNetAdhocctlPeerInfo;

/**
 * Peer Information Getter
 * @param addr Peer MAC Address
 * @param size Size of peer_info (in Bytes)
 * @param peer_info OUT: Peer Information
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOC_NO_ENTRY
 */
int sceNetAdhocctlGetPeerInfo(const SceNetEtherAddr * addr, int size, SceNetAdhocctlPeerInfo * peer_info);

/**
 * Peer Information Getter (Linked List)
 * @param buflen IN: Bufferlength OUT: Bufferlength after Output
 * @param buf OUT: Peer Information Linked List
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int sceNetAdhocctlGetPeerList(int * buflen, SceNetAdhocctlPeerInfo * buf);

/**
 * Lock Peerlist
 */
void _acquirePeerLock(void);

/**
 * Unlock Peerlist
 */
void _freePeerLock(void);

/**
 * Lock Network
 */
void _acquireNetworkLock(void);

/**
 * Unlock Network
 */
void _freeNetworkLock(void);

/**
 * Resolve IP to MAC
 * @param ip Peer IP Address
 * @param mac OUT: Peer MAC
 * @return 0 on success or... ADHOC_NO_ENTRY
 */
int _resolveIP(uint32_t ip, SceNetEtherAddr * mac);

/**
 * Resolve MAC to IP
 * @param mac Peer MAC Address
 * @param ip OUT: Peer IP
 * @return 0 on success or... ADHOC_NO_ENTRY
 */
int _resolveMAC(SceNetEtherAddr * mac, uint32_t * ip);

/**
 * Get First Peer List Element
 * @return First Internal Peer List Element
 */
SceNetAdhocctlPeerInfo * _getInternalPeerList(void);

#endif

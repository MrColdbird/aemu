#ifndef _ADHOCCTL_GETADDRBYNAME_H_
#define _ADHOCCTL_GETADDRBYNAME_H_

/**
 * Resolve Player Name to Linked List of Peers
 * @param nickname To-be-resolved Player Nickname
 * @param buflen IN: Buffer Length in Bytes OUT: Required / Used Buffer Length in Bytes
 * @param buf OUT: Resolved Peer Information (if NULL then buflen gets filled with required length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetAddrByName(const SceNetAdhocctlNickname * nickname, int * buflen, SceNetAdhocctlPeerInfo * buf);

#endif

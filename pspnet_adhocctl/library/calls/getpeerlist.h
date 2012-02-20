#ifndef _ADHOCCTL_GETPEERLIST_H_
#define _ADHOCCTL_GETPEERLIST_H_

/**
 * Acquire Peer Linked List
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf Peer List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetPeerList(int * buflen, SceNetAdhocctlPeerInfo * buf);

#endif

#ifndef _ADHOCCTL_GETNAMEBYADDR_H_
#define _ADHOCCTL_GETNAMEBYADDR_H_

/**
 * Resolve Player Name to Linked List of Peers
 * @param addr To-be-resolved MAC Address
 * @param nickname OUT: Resolved Nickname
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOC_NO_ENTRY
 */
int proNetAdhocctlGetNameByAddr(const SceNetEtherAddr * addr, SceNetAdhocctlNickname * nickname);

#endif

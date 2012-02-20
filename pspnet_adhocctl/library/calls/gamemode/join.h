#ifndef _ADHOCCTL_GAMEMODE_JOIN_H_
#define _ADHOCCTL_GAMEMODE_JOIN_H_

/**
 * Join a existing GameMode Network as Peer
 * @param group_name Virtual Network Name
 * @param gc MAC Address of GameMode Host
 * @param timeout Timeout Value (in Microseconds)
 * @param flag Unused Bitflags
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlJoinEnterGameMode(const SceNetAdhocctlGroupName * group_name, const SceNetEtherAddr * gc, uint32_t timeout, int flag);

#endif

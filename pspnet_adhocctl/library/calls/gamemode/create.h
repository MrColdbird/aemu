#ifndef _ADHOCCTL_GAMEMODE_CREATE_H_
#define _ADHOCCTL_GAMEMODE_CREATE_H_

/**
 * Create and Join a GameMode Network as Host
 * @param group_name Virtual Network Name
 * @param game_type Network Type (1A, 1B, 2A)
 * @param num Total Number of Peers (including Host)
 * @param members MAC Address List of Peers (own MAC at Index 0)
 * @param timeout Timeout Value (in Microseconds)
 * @param flag Unused Bitflags
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY, ADHOCCTL_CHANNEL_NOT_AVAILABLE
 */
int proNetAdhocctlCreateEnterGameMode(const SceNetAdhocctlGroupName * group_name, int game_type, int num, const SceNetEtherAddr * members, uint32_t timeout, int flag);

#endif

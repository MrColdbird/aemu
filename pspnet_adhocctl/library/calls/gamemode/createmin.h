#ifndef _ADHOCCTL_GAMEMODE_CREATEMIN_H_
#define _ADHOCCTL_GAMEMODE_CREATEMIN_H_

/**
 * Create and Join a GameMode Network as Host (with Minimum Peer Check)
 * @param group_name Virtual Network Name
 * @param game_type Network Type (1A, 1B, 2A)
 * @param min_members Minimum Number of Peers
 * @param num_members Total Number of Peers (including Host)
 * @param members MAC Address List of Peers (own MAC at Index 0)
 * @param timeout Timeout Value (in Microseconds)
 * @param flag Unused Bitflags
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY, ADHOCCTL_CHANNEL_NOT_AVAILABLE
 */
int proNetAdhocctlCreateEnterGameModeMin(const SceNetAdhocctlGroupName * group_name, int game_type, int min_members, int num_members, const struct SceNetEtherAddr * members, uint32_t timeout, int flag);

#endif

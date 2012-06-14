/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

#include "../../common.h"

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
int proNetAdhocctlCreateEnterGameModeMin(const SceNetAdhocctlGroupName * group_name, int game_type, int min_members, int num_members, const struct SceNetEtherAddr * members, uint32_t timeout, int flag)
{
	// We don't really need the Minimum User Check
	return proNetAdhocctlCreateEnterGameMode(group_name, game_type, num_members, members, timeout, flag);
}

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
 * Create and Join a GameMode Network as Host
 * @param group_name Virtual Network Name
 * @param game_type Network Type (1A, 1B, 2A)
 * @param num Total Number of Peers (including Host)
 * @param members MAC Address List of Peers (own MAC at Index 0)
 * @param timeout Timeout Value (in Microseconds)
 * @param flag Unused Bitflags
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY, ADHOCCTL_CHANNEL_NOT_AVAILABLE
 */
int proNetAdhocctlCreateEnterGameMode(const SceNetAdhocctlGroupName * group_name, int game_type, int num, const SceNetEtherAddr * members, uint32_t timeout, int flag)
{
	THROW_UNIMPLEMENTED(__func__);
	return -1;
}

#include "../common.h"

/**
 * Acquire Network Scan Result in Linked List
 * @param addr Peer MAC Address
 * @param size Length of peer_info in Bytes
 * @param peer_info OUT: Peer Information
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOC_NO_ENTRY
 */
int proNetAdhocctlGetPeerInfo(SceNetEtherAddr * addr, int size, SceNetAdhocctlPeerInfo * peer_info)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(addr != NULL && size >= sizeof(SceNetAdhocctlPeerInfo) && peer_info != NULL)
		{
			// Clear Memory
			memset(peer_info, 0, size);
			
			// Multithreading Lock
			_acquirePeerLock();
			
			// Iterate Friends
			SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL)
			{
				#ifndef NO_FRIEND_TIMEOUT
				// Active Friend
				if((sceKernelGetSystemTimeWide() - friend->last_recv) < ADHOCCTL_DEAD_FRIEND_TIMEOUT)
				#endif
				{
					#ifndef NO_FRIEND_GROUP_CHECK
					// Same Network as Local Player
					if(friend->base.network_type != ADHOCCTL_NETWORK_TYPE_DISCONNECTED && friend->base.network_type == _status.network_type && strncmp((char *)friend->base.group_name.data, (char *)_status.group_name.data, ADHOCCTL_GROUPNAME_LEN) == 0)
					#endif
					{
						// Matching MAC
						if(memcmp(addr->data, friend->base.player_mac.data, ETHER_ADDR_LEN) == 0)
						{
							// Copy Nickname
							peer_info->nickname = friend->base.player_name;
							
							// Copy MAC Address
							peer_info->mac_addr = friend->base.player_mac;
							
							// Copy IP Address (required for the Adhoc Emulator only, not used by OFW)
							peer_info->ip_addr = friend->ip_addr;
							
							// Copy Receive Statistic
							peer_info->last_recv = friend->last_recv;
							
							// Multithreading Unlock
							_freePeerLock();
							
							// Return Success
							return 0;
						}
					}
				}
				
				// Move Pointer
				friend = friend->next;
			}
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Peer not found
			return ADHOC_NO_ENTRY;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

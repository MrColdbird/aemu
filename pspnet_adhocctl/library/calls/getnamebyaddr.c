#include "../common.h"

/**
 * Resolve Player Name to Linked List of Peers
 * @param addr To-be-resolved MAC Address
 * @param nickname OUT: Resolved Nickname
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOC_NO_ENTRY
 */
int proNetAdhocctlGetNameByAddr(const SceNetEtherAddr * addr, SceNetAdhocctlNickname * nickname)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(addr != NULL && nickname != NULL)
		{
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
						if(memcmp(friend->base.player_mac.data, addr->data, ETHER_ADDR_LEN) == 0)
						{
							// Write Data
							*nickname = friend->base.player_name;
							
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
			
			// Player not found
			return ADHOC_NO_ENTRY;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

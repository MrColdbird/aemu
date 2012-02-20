#include "../common.h"

// Function Prototypes
int _countAvailableNetworks(void);
int _isFirstNetworkInhabitant(SceNetAdhocctlStatusFriend * friend);

/**
 * Acquire Network Scan Result in Linked List
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf Network List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED
 */
int proNetAdhocctlGetScanInfo(int * buflen, SceNetAdhocctlScanInfo * buf)
{
	// Library initialized
	if(_init == 1)
	{
		// Minimum Argument Requirements
		if(buflen != NULL)
		{
			// Multithreading Lock
			_acquirePeerLock();
			
			// Length Returner Mode
			if(buf == NULL) *buflen = _countAvailableNetworks() * sizeof(SceNetAdhocctlScanInfo);
			
			// Normal Information Mode
			else
			{
				// Clear Memory
				memset(buf, 0, *buflen);
				
				// Network Discovery Counter
				int discovered = 0;
				
				// Count requested Networks
				int requestcount = *buflen / sizeof(SceNetAdhocctlScanInfo);
				
				// Minimum Argument Requirements
				if(requestcount > 0)
				{
					// Iterate Friends
					SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL && discovered < requestcount)
					{
						// Discovered Active Network Peer
						if(friend->base.network_type != ADHOCCTL_NETWORK_TYPE_DISCONNECTED && (sceKernelGetSystemTimeWide() - friend->last_recv) < ADHOCCTL_DEAD_FRIEND_TIMEOUT)
						{
							// Currently undiscovered Network
							if(_isFirstNetworkInhabitant(friend))
							{
								// Store Information
								buf[discovered].channel = 1;
								buf[discovered].group_name = friend->base.group_name;
								buf[discovered].bssid.mac_addr = friend->base.player_mac;
								buf[discovered].mode = (friend->base.network_type == ADHOCCTL_NETWORK_TYPE_ADHOC) ? (ADHOCCTL_MODE_ADHOC) : (ADHOCCTL_MODE_GAMEMODE);
								discovered++;
							}
						}
						
						// Move Pointer
						friend = friend->next;
					}
					
					// Link List
					int i = 0; for(; i < discovered - 1; i++)
					{
						// Link Network
						buf[i].next = &buf[i + 1];
					}
				}
				
				// Fix Size
				*buflen = discovered * sizeof(SceNetAdhocctlScanInfo);
			}
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Return Success
			return 0;
		}
		
		// Generic Error
		return -1;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

/**
 * Count Virtual Networks by analyzing the Friend List
 * @return Number of Virtual Networks
 */
int _countAvailableNetworks(void)
{
	// Network Count
	int count = 0;
	
	// Iterate Friends
	SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL)
	{
		// Discovered Active Network Peer
		if(friend->base.network_type != ADHOCCTL_NETWORK_TYPE_DISCONNECTED && (sceKernelGetSystemTimeWide() - friend->last_recv) < ADHOCCTL_DEAD_FRIEND_TIMEOUT)
		{
			// Unique Network discovered
			if(_isFirstNetworkInhabitant(friend)) count++;
		}
		
		// Move Pointer
		friend = friend->next;
	}
	
	// Return Network Count
	return count;
}

/**
 * Check whether a Friend is the first occuring Inhabitant in a Virtual Network
 * @param friend To-be-checked Friend
 * @return Boolean Result
 */
int _isFirstNetworkInhabitant(SceNetAdhocctlStatusFriend * friend)
{
	// Iterate Friends
	SceNetAdhocctlStatusFriend * friend_before = _friends; while(friend_before != NULL && friend_before != friend)
	{
		// Duplicate Network discovered
		if(strncmp((char *)friend_before->base.group_name.data, (char *)friend->base.group_name.data, ADHOCCTL_GROUPNAME_LEN) == 0)
		{
			// Not unique
			return 0;
		}
		
		// Move Pointer
		friend_before = friend_before->next;
	}
	
	// Unique Inhabitant
	return 1;
}

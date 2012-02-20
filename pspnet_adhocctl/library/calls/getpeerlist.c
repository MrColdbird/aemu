#include "../common.h"

// Function Prototypes
int _getActivePeerCount(void);

/**
 * Acquire Peer Linked List
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf Peer List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetPeerList(int * buflen, SceNetAdhocctlPeerInfo * buf)
{
	// Initialized Library
	if(_init == 1)
	{
		// Minimum Arguments
		if(buflen != NULL)
		{
			// Multithreading Lock
			_acquirePeerLock();
			
			// Length Calculation Mode
			if(buf == NULL) *buflen = _getActivePeerCount() * sizeof(SceNetAdhocctlPeerInfo);
			
			// Normal Mode
			else
			{
				// Discovery Counter
				int discovered = 0;
				
				// Calculate Request Count
				int requestcount = *buflen / sizeof(SceNetAdhocctlPeerInfo);
				
				// Clear Memory
				memset(buf, 0, *buflen);
				
				// Minimum Arguments
				if(requestcount > 0)
				{
					// Iterate Friends
					SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL && discovered < requestcount)
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
								// Copy Nickname
								buf[discovered].nickname = friend->base.player_name;
								
								// Copy MAC Address
								buf[discovered].mac_addr = friend->base.player_mac;
								
								// Copy IP Address (required for the Adhoc Emulator only, not used by OFW)
								buf[discovered].ip_addr = friend->ip_addr;
								
								// Copy Receive Statistic
								buf[discovered].last_recv = friend->last_recv;
								
								// Increase Counter
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
				*buflen = discovered * sizeof(SceNetAdhocctlPeerInfo);
			}
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Uninitialized Library
	return ADHOCCTL_NOT_INITIALIZED;
}

/**
 * Return Number of active Peers in the same Network as the Local Player
 * @return Number of active Peers
 */
int _getActivePeerCount(void)
{
	// Counter
	int count = 0;
	
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
				// Increase Counter
				count++;
			}
		}
		
		// Move Pointer
		friend = friend->next;
	}
	
	// Return Result
	return count;
}

#include "../common.h"

// Function Prototypes
int _getNicknameCount(const SceNetAdhocctlNickname * nickname);

/**
 * Resolve Player Name to Linked List of Peers
 * @param nickname To-be-resolved Player Nickname
 * @param buflen IN: Buffer Length in Bytes OUT: Required / Used Buffer Length in Bytes
 * @param buf OUT: Resolved Peer Information (if NULL then buflen gets filled with required length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetAddrByName(const SceNetAdhocctlNickname * nickname, int * buflen, SceNetAdhocctlPeerInfo * buf)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(nickname != NULL && buflen != NULL)
		{
			// Multithreading Lock
			_acquirePeerLock();
			
			// Length Calculation Mode
			if(buf == NULL) *buflen = _getNicknameCount(nickname) * sizeof(SceNetAdhocctlPeerInfo);
			
			// Normal Information Mode
			else
			{
				// Clear Memory
				memset(buf, 0, *buflen);
				
				// Discovered Player Count
				int discovered = 0;
				
				// Calculate Requested Elements
				int requestcount = *buflen / sizeof(SceNetAdhocctlPeerInfo);
				
				// Minimum Space available
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
								// Matching Name found
								if(strcmp((char *)friend->base.player_name.data, (char *)nickname->data) == 0)
								{
									// Write Data
									buf[discovered].nickname = *nickname;
									buf[discovered].mac_addr = friend->base.player_mac;
									buf[discovered].ip_addr = friend->ip_addr;
									buf[discovered].last_recv = friend->last_recv;
									discovered++;
								}
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
				
				// Fix Buffer Size
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
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

/**
 * Return the Number of Players with the chosen Nickname in the Local Users current Network
 * @param nickname To-be-searched Nickname
 * @return Number of matching Players
 */
int _getNicknameCount(const SceNetAdhocctlNickname * nickname)
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
				// Matching Name found
				if(strcmp((char *)friend->base.player_name.data, (char *)nickname->data) == 0)
				{
					// Increase Counter
					count++;
				}
			}
		}
	}
	
	// Return Result
	return count;
}

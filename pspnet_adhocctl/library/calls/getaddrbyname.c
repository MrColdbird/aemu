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
					// Peer Reference
					SceNetAdhocctlPeerInfo * peer = _friends;
					
					// Iterate Peers
					for(; peer != NULL && discovered < requestcount; peer = peer->next)
					{
						// Match found
						if(strcmp((char *)peer->nickname.data, (char *)nickname->data) == 0)
						{
							// Fake Receive Time
							peer->last_recv = sceKernelGetSystemTimeWide();
							
							// Copy Peer Info
							buf[discovered++] = *peer;
						}
					}
					
					// Link List
					int i = 0; for(; i < discovered - 1; i++)
					{
						// Link Network
						buf[i].next = &buf[i + 1];
					}
					
					// Fix Last Element
					if(discovered > 0) buf[discovered - 1].next = NULL;
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
	
	// Peer Reference
	SceNetAdhocctlPeerInfo * peer = _friends;
	
	// Iterate Peers
	for(; peer != NULL; peer = peer->next)
	{
		// Match found
		if(strcmp((char *)peer->nickname.data, (char *)nickname->data) == 0) count++;
	}
	
	// Return Result
	return count;
}
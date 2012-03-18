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
					#ifdef LOCALHOST_AS_PEER
					// Get Local IP Address
					union SceNetApctlInfo info; if(sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info) == 0)
					{
						// Add Local Address
						buf[discovered].nickname = _parameter.nickname;
						buf[discovered].mac_addr = _parameter.bssid.mac_addr;
						sceNetInetInetAton(info.ip, &buf[discovered].ip_addr);
						buf[discovered++].last_recv = sceKernelGetSystemTimeWide();
					}
					#endif
					
					// Peer Reference
					SceNetAdhocctlPeerInfo * peer = _friends;
					
					// Iterate Peers
					for(; peer != NULL && discovered < requestcount; peer = peer->next)
					{
						// Fake Receive Time
						peer->last_recv = sceKernelGetSystemTimeWide();
						
						// Copy Peer Info
						buf[discovered++] = *peer;
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
	
	#ifdef LOCALHOST_AS_PEER
	// Increase for Localhost
	count++;
	#endif
	
	// Peer Reference
	SceNetAdhocctlPeerInfo * peer = _friends;
	
	// Iterate Peers
	for(; peer != NULL; peer = peer->next)
	{
		// Increase Counter
		count++;
	}
	
	// Return Result
	return count;
}

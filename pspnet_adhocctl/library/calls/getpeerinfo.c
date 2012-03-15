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
			
			// Local Nickname Matches
			if(memcmp(&_parameter.bssid.mac_addr, addr, sizeof(SceNetEtherAddr)) == 0)
			{
				// Get Local IP Address
				union SceNetApctlInfo info; if(sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info) == 0)
				{
					// Add Local Address
					peer_info->nickname = _parameter.nickname;
					peer_info->mac_addr = _parameter.bssid.mac_addr;
					sceNetInetInetAton(info.ip, &peer_info->ip_addr);
					peer_info->last_recv = sceKernelGetSystemTimeWide();
					
					// Return Success
					return 0;
				}
			}
			
			// Multithreading Lock
			_acquirePeerLock();
			
			// Peer Reference
			SceNetAdhocctlPeerInfo * peer = _friends;
			
			// Iterate Peers
			for(; peer != NULL; peer = peer->next)
			{
				// Found Matching Peer
				if(memcmp(&peer->mac_addr, addr, sizeof(SceNetEtherAddr)) == 0)
				{
					// Fake Receive Time
					peer->last_recv = sceKernelGetSystemTimeWide();
					
					// Save Peer Info
					*peer_info = *peer;
					
					// Unlink from Internal List
					peer_info->next = NULL;
					
					// Multithreading Unlock
					_freePeerLock();
					
					// Return Success
					return 0;
				}
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

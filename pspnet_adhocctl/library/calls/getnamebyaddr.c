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
			// Get Local MAC Address
			uint8_t localmac[6]; sceWlanGetEtherAddr((void *)localmac);
			
			// Local MAC Matches
			if(memcmp(localmac, addr, sizeof(SceNetEtherAddr)) == 0)
			{
				// Write Data
				*nickname = _parameter.nickname;
				
				// Return Success
				return 0;
			}
			
			// Multithreading Lock
			_acquirePeerLock();
			
			// Peer Reference
			SceNetAdhocctlPeerInfo * peer = _friends;
			
			// Iterate Peers
			for(; peer != NULL; peer = peer->next)
			{
				// Match found
				if(memcmp(&peer->mac_addr, addr, sizeof(SceNetEtherAddr)) == 0)
				{
					// Write Data
					*nickname = peer->nickname;
					
					// Multithreading Unlock
					_freePeerLock();
					
					// Return Success
					return 0;
				}
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

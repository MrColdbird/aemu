#include "../common.h"

// Function Prototypes
void _freeNetworkRecursive(SceNetAdhocctlScanInfo * node);

/**
 * Terminate Adhoc-Control Emulator
 * @return 0 on success or... ADHOCCTL_BUSY
 */
int proNetAdhocctlTerm(void)
{
	// Library initialized
	if(_init == 1)
	{
		// Send shutdown request
		_init = 0;
		
		// Wait for internal thread cooldown
		while(_init != -1) sceKernelDelayThread(10000);
		
		// Multithreading Lock
		_acquirePeerLock();
		
		// Free Friends Memory
		_freeFriendsRecursive(_friends);
		
		// Delete Friends Reference
		_friends = NULL;
		
		// Multithreading Unlock
		_freePeerLock();
		
		// Free Group Memory
		_freeNetworkRecursive(_networks);
		
		// Delete Group Reference
		_networks = NULL;
		
		// Delete Socket
		sceNetInetClose(_metasocket);
		
		// Delete Socket Reference
		_metasocket = -1;
		
		// Close Hotspot Connection
		sceNetApctlDisconnect();
		
		// Terminate Access Point Control
		sceNetApctlTerm();
		
		// Issue Library cooldown
		_init = 0;
	}
	
	// Return Success
	return 0;
}

/**
 * Recursive Memory Freeing-Helper for Friend-Structures
 * @param node Current Node in List
 */
void _freeFriendsRecursive(SceNetAdhocctlPeerInfo * node)
{
	// End of List
	if(node == NULL) return;
	
	// Increase Recursion Depth
	_freeFriendsRecursive(node->next);
	
	// Free Memory
	free(node);
}

/**
 * Recursive Memory Freeing-Helper for Group-Structures
 * @param node Current Node in List
 */
void _freeNetworkRecursive(SceNetAdhocctlScanInfo * node)
{
	// End of List
	if(node == NULL) return;
	
	// Increase Recursion Depth
	_freeNetworkRecursive(node->next);
	
	// Free Memory
	free(node);
}

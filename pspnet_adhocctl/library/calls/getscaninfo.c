#include "../common.h"

// Function Prototypes
int _countAvailableNetworks(void);

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
					// Group List Element
					SceNetAdhocctlScanInfo * group = _networks;
					
					// Iterate Group List
					for(; group != NULL && discovered < requestcount; group = group->next)
					{
						// Copy Group Information
						buf[discovered++] = *group;
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
	
	// Group Reference
	SceNetAdhocctlScanInfo * group = _networks;
	
	// Count Groups
	for(; group != NULL; group = group->next) count++;
	
	// Return Network Count
	return count;
}

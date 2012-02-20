#include "../../common.h"

/**
 * Adhoc Emulator PDP Socket List Getter
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf PDP Socket List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED
 */
int proNetAdhocGetPdpStat(int * buflen, SceNetAdhocPdpStat * buf)
{
	// Library is initialized
	if(_init)
	{
		// Length Returner Mode
		if(buflen != NULL && buf == NULL)
		{
			// Return Required Size
			*buflen = sizeof(SceNetAdhocPdpStat) * _getPDPSocketCount();
			
			// Success
			return 0;
		}
		
		// Status Returner Mode
		else if(buflen != NULL && buf != NULL)
		{
			// Socket Count
			int socketcount = _getPDPSocketCount();
			
			// Figure out how many Sockets we will return
			int count = *buflen / sizeof(SceNetAdhocPdpStat);
			if(count > socketcount) count = socketcount;
			
			// Copy Counter
			int i = 0;
			
			// Socket List Iterator Variable
			SceNetAdhocPdpStat * list = _pdp;
			
			// Copy & Link Sockets
			while(i < count && list != NULL)
			{
				// Copy Socket Data from internal Memory
				buf[i] = *list;
				
				// Fix Client View Socket ID
				buf[i].id = (int)list;
				
				// Write End of List Reference
				buf[i].next = NULL;
				
				// Link previous Element to this one
				if(i > 0) buf[i-1].next = &buf[i];
				
				// Increment Counter
				i++;
				
				// Move Iterator Variable
				list = list->next;
			}
			
			// Update Buffer Length
			*buflen = i * sizeof(SceNetAdhocPdpStat);
			
			// Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOC_INVALID_ARG;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

/**
 * PDP Socket Counter
 * @return Number of internal PDP Sockets
 */
int _getPDPSocketCount(void)
{
	// Socket List Iterator Variable
	SceNetAdhocPdpStat * list = _pdp;
	
	// Socket Counter
	int counter = 0;
	
	// Count Sockets
	while(list != NULL)
	{
		// Increase Counter
		counter++;
		
		// Move Iterator Variable
		list = list->next;
	}
	
	// Return Socket Count
	return counter;
}

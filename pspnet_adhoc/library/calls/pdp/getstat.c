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
			
			// Iterate Translation Table
			int j = 0; for(; j < 255 && i < count; j++)
			{
				// Valid Socket Entry
				if(_pdp[j] != NULL)
				{
					// Copy Socket Data from Internal Memory
					buf[i] = *_pdp[j];
					
					// Fix Client View Socket ID
					buf[i].id = j + 1;
					
					// Write End of List Reference
					buf[i].next = NULL;
					
					// Link Previous Element
					if(i > 0) buf[i - 1].next = &buf[i];
					
					// Increment Counter
					i++;
				}
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
	// Socket Counter
	int counter = 0;
	
	// Count Sockets
	int i = 0; for(; i < 255; i++) if(_pdp[i] != NULL) counter++;
	
	// Return Socket Count
	return counter;
}

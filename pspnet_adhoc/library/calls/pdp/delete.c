#include "../../common.h"

/**
 * Adhoc Emulator PDP Socket Delete
 * @param id Socket File Descriptor
 * @param flag Bitflags (Unused)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID
 */
int proNetAdhocPdpDelete(int id, int flag)
{
	// Cast Socket
	SceNetAdhocPdpStat * socket = (SceNetAdhocPdpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Arguments & Atleast one Socket
		if(socket != NULL && _pdp != NULL)
		{
			// First Element will be deleted
			if(_pdp == socket)
			{
				// Unlink Socket
				_pdp = _pdp->next;
				
				// Close Connection
				sceNetInetClose(socket->id);
				
				// Free Memory
				free(socket);
				
				// Success
				return 0;
			}
			
			// Other Element will be deleted
			else
			{
				// Create Starting Point for Search
				SceNetAdhocPdpStat * list = _pdp;
				
				// Search for previous Element in List
				for(; list != NULL && list->next != socket; list = list->next);
				
				// Found previous Element in List
				if(list != NULL)
				{
					// Unlink Element
					list->next = socket->next;
					
					// Close Connection
					sceNetInetClose(socket->id);
					
					// Free Memory
					free(socket);
					
					// Success
					return 0;
				}
				
				// Invalid Socket ID
				return ADHOC_INVALID_SOCKET_ID;
			}
		}
		
		// Invalid Argument
		return ADHOC_INVALID_ARG;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

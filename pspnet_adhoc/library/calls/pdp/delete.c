#include "../../common.h"

/**
 * Adhoc Emulator PDP Socket Delete
 * @param id Socket File Descriptor
 * @param flag Bitflags (Unused)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID
 */
int proNetAdhocPdpDelete(int id, int flag)
{
	// Library is initialized
	if(_init)
	{
		// Valid Arguments
		if(id > 0 && id <= 255)
		{
			// Cast Socket
			SceNetAdhocPdpStat * socket = _pdp[id - 1];
			
			// Valid Socket
			if(socket != NULL)
			{
				// Close Connection
				sceNetInetClose(socket->id);
				
				// Free Memory
				free(socket);
				
				// Free Translation Slot
				_pdp[id - 1] = NULL;
				
				// Success
				return 0;
			}
			
			// Invalid Socket ID
			return ADHOC_INVALID_SOCKET_ID;
		}
		
		// Invalid Argument
		return ADHOC_INVALID_ARG;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

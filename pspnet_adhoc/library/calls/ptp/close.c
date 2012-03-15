#include "../../common.h"

/**
 * Adhoc Emulator PTP Socket Closer
 * @param id Socket File Descriptor
 * @param flag Bitflags (Unused)
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED
 */
int proNetAdhocPtpClose(int id, int flag)
{
	// Library is initialized
	if(_init)
	{
		// Valid Arguments & Atleast one Socket
		if(id > 0 && id <= 255 && _ptp[id - 1] != NULL)
		{
			// Cast Socket
			SceNetAdhocPtpStat * socket = _ptp[id - 1];
			
			// Close Connection
			sceNetInetClose(socket->id);
			
			// Free Memory
			free(socket);
			
			// Free Reference
			_ptp[id - 1] = NULL;
			
			// Success
			return 0;
		}
		
		// Invalid Argument
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

#include "../common.h"

/**
 * Trigger Adhoc Network Scan
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_BUSY
 */
int proNetAdhocctlScan(void)
{
	// Library initialized
	if(_init == 1)
	{
		// Not connected
		if(_thread_status == ADHOCCTL_STATE_DISCONNECTED)
		{
			// Set Thread Status to Scanning
			_thread_status = ADHOCCTL_STATE_SCANNING;
			
			// Prepare Scan Request Packet
			uint8_t opcode = OPCODE_SCAN;
			
			// Send Scan Request Packet
			sceNetInetSend(_metasocket, &opcode, 1, INET_MSG_DONTWAIT);
			
			// Return Success
			return 0;
		}
		
		// Library is busy
		return ADHOCCTL_BUSY;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

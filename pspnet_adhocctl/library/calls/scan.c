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
		if(_status.network_type == ADHOCCTL_NETWORK_TYPE_DISCONNECTED)
		{
			// Notify Event Handlers
			int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
			{
				// Active Handler
				if(_event_handler[i] != NULL) _event_handler[i](ADHOCCTL_EVENT_SCAN, 0, _event_args[i]);
			}
			
			// Return Success
			return 0;
		}
		
		// Library is busy
		return ADHOCCTL_BUSY;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

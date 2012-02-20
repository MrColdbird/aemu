#include "../../common.h"

/**
 * Leave current Network
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_BUSY
 */
int proNetAdhocctlDisconnect(void)
{
	// Library initialized
	if(_init == 1)
	{
		// Connected State (Adhoc Mode)
		if(_status.network_type == ADHOCCTL_NETWORK_TYPE_ADHOC)
		{
			// Clear Network Name
			memset(&_status.group_name, 0, sizeof(_status.group_name));
			
			// Set Disconnected State
			_status.network_type = ADHOCCTL_NETWORK_TYPE_DISCONNECTED;
			
			// Notify Event Handlers
			int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
			{
				// Active Handler
				if(_event_handler[i] != NULL) _event_handler[i](ADHOCCTL_EVENT_DISCONNECT, 0, _event_args[i]);
			}
			
			// Return Success
			return 0;
		}
		
		// Connected State
		return ADHOCCTL_BUSY;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

#include "../common.h"

/**
 * Return Internal Thread Status
 * @param state OUT: Thread Status
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetState(int * state)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(state != NULL)
		{
			// Disconnected State
			if(_status.network_type == ADHOCCTL_NETWORK_TYPE_DISCONNECTED) *state = ADHOCCTL_STATE_DISCONNECTED;
			
			// Connected State (Adhoc Mode)
			else if(_status.network_type == ADHOCCTL_NETWORK_TYPE_ADHOC) *state = ADHOCCTL_STATE_CONNECTED;
			
			// Connected State (Game Mode)
			else *state = ADHOCCTL_STATE_GAMEMODE;
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

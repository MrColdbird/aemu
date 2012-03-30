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
			// Return Thread Status
			*state = _thread_status;
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

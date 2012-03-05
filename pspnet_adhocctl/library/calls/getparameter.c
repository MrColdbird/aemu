#include "../common.h"

/**
 * Get Adhoc Parameter
 * @param parameter OUT: Adhoc Parameter
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetParameter(SceNetAdhocctlParameter * parameter)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(parameter != NULL)
		{
			// Copy Parameter
			*parameter = _parameter;
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

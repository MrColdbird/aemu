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
			// Clear Parameter
			memset(parameter, 0, sizeof(SceNetAdhocctlParameter));
			
			// Fake Channel
			parameter->channel = 1;
			
			// Fake BSSID
			parameter->bssid.mac_addr = _status.player_mac;
			
			// Copy Group Name
			parameter->group_name = _status.group_name;
			
			// Copy Nick Name
			parameter->nickname = _status.player_name;
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

#include "../../common.h"

/**
 * Shutdown Netconf Utility
 * @return 0 on success or... -1
 */
int proUtilityNetconfShutdownStart(void)
{
	// Valid Utility State
	if(_netconf_status == UTILITY_NETCONF_STATUS_FINISHED)
	{
		// Set Library Status
		_netconf_status = UTILITY_NETCONF_STATUS_SHUTDOWN;
		
		// Return Success
		return 0;
	}
	
	// Generic Error
	return -1;
}

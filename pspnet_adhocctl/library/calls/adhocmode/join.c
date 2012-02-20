#include "../../common.h"

/**
 * Join discovered Network
 * @param scan_info Discovered Network
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlJoin(const SceNetAdhocctlScanInfo * scan_info)
{
	// Library initialized
	if(_init)
	{
		// Valid Argument
		if(scan_info != NULL)
		{
			// We can ignore minor connection process differences here
			return proNetAdhocctlCreate(&scan_info->group_name);
		}
		
		// Invalid Argument
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Uninitialized Library
	return ADHOCCTL_NOT_INITIALIZED;
}

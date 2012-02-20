#include "../../common.h"

/**
 * Create and / or Join a Virtual Network of the specified Name (Only differs from Create in the way of handling BSSID)
 * @param group_name Virtual Network Name
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlConnect(const SceNetAdhocctlGroupName * group_name)
{
	// We can ignore minor connection process differences here
	return proNetAdhocctlCreate(group_name);
}

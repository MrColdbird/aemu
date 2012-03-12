#include "../../common.h"

// Netconf Status
int _netconf_status = UTILITY_NETCONF_STATUS_NONE;

/**
 * Connect to Adhoc Network via Kernel Utility Wrapper
 * @param param Netconf Parameter (Groupname and other Information)
 * @return 0 on success or... -1
 */
int proUtilityNetconfInitStart(SceUtilityNetconfParam * param)
{
	// Valid Utility State
	if(_netconf_status == UTILITY_NETCONF_STATUS_NONE)
	{
		// Valid Parameter
		if(param != NULL)
		{
			// Valid Adhoc Types
			if(param->type == UTILITY_NETCONF_TYPE_CONNECT_ADHOC || param->type == UTILITY_NETCONF_TYPE_CREATE_ADHOC || param->type == UTILITY_NETCONF_TYPE_JOIN_ADHOC)
			{
				// Valid Adhoc Parameter
				if(param->adhoc_param != NULL)
				{
					// Disconnect from Network (if any)
					proNetAdhocctlDisconnect();
					
					// Create Network
					if(proNetAdhocctlCreate(&param->adhoc_param->group_name) == 0)
					{
						// Set Library Status
						// _netconf_status = UTILITY_NETCONF_STATUS_INITIALIZE;
						
						// Return Success
						return 0;
					}
				}
			}
		}
	}
	
	// Generic Error
	return -1;
}

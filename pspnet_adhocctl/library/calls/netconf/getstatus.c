#include "../../common.h"

/**
 * Return Netconf Utility Status
 * @return Netconf Status
 */
int proUtilityNetconfGetStatus(void)
{
	// State Transition
	if(_netconf_status == UTILITY_NETCONF_STATUS_NONE) _netconf_status = UTILITY_NETCONF_STATUS_INITIALIZE;
	else if(_netconf_status == UTILITY_NETCONF_STATUS_INITIALIZE) _netconf_status = UTILITY_NETCONF_STATUS_RUNNING;
	else if(_netconf_status == UTILITY_NETCONF_STATUS_RUNNING) _netconf_status = UTILITY_NETCONF_STATUS_FINISHED;
	else if(_netconf_status == UTILITY_NETCONF_STATUS_SHUTDOWN) _netconf_status = UTILITY_NETCONF_STATUS_NONE;
	
	// Return State
	return _netconf_status;
}

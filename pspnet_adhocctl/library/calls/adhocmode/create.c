#include "../../common.h"

// Function Prototypes
int _validNetworkName(const SceNetAdhocctlGroupName * group_name);

/**
 * Create and / or Join a Virtual Network of the specified Name
 * @param group_name Virtual Network Name
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlCreate(const SceNetAdhocctlGroupName * group_name)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Argument
		if(_validNetworkName(group_name))
		{
			// Disconnected State
			if(_status.network_type == ADHOCCTL_NETWORK_TYPE_DISCONNECTED)
			{
				// Clear Network Name
				memset(&_status.group_name, 0, sizeof(_status.group_name));
				
				// Overwrite Network Name if given
				if(group_name != NULL) _status.group_name = *group_name;
				
				// Set Connected State
				_status.network_type = ADHOCCTL_NETWORK_TYPE_ADHOC;
				
				// Notify Event Handlers
				int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
				{
					// Active Handler
					if(_event_handler[i] != NULL) _event_handler[i](ADHOCCTL_EVENT_CONNECT, 0, _event_args[i]);
				}
				
				// Return Success
				return 0;
			}
			
			// Connected State
			return ADHOCCTL_BUSY;
		}
		
		// Invalid Argument
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

/**
 * Check whether Network Name contains only valid symbols
 * @param group_name To-be-checked Network Name
 * @return 1 if valid or... 0
 */
int _validNetworkName(const SceNetAdhocctlGroupName * group_name)
{
	// Result
	int valid = 1;
	
	// Name given
	if(group_name != NULL)
	{
		// Iterate Name Characters
		int i = 0; for(; i < ADHOCCTL_GROUPNAME_LEN && valid; i++)
		{
			// End of Name
			if(group_name->data[i] == 0) break;
			
			// Not a digit
			if(group_name->data[i] < '0' || group_name->data[i] > '9')
			{
				// Not 'A' to 'Z'
				if(group_name->data[i] < 'A' || group_name->data[i] > 'Z')
				{
					// Not 'a' to 'z'
					if(group_name->data[i] < 'a' || group_name->data[i] > 'z')
					{
						// Invalid Name
						valid = 0;
					}
				}
			}
		}
	}
	
	// Return Result
	return valid;
}

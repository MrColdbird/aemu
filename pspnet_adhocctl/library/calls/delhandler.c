#include "../common.h"

/**
 * Delete registered Adhocctl Handler
 * @param id Adhocctl Handler ID
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_ID_NOT_FOUND
 */
int proNetAdhocctlDelHandler(int id)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(id > 0 && id <= 4)
		{
			// Valid ID
			if(_event_handler[id - 1] != NULL)
			{
				// Clear Event Handler
				_event_handler[id - 1] = NULL;
				_event_args[id - 1] = NULL;
				
				// Return Success
				return 0;
			}
			
			// Invalid ID
			return ADHOCCTL_ID_NOT_FOUND;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

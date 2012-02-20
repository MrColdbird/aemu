#include "../common.h"

/**
 * Register (up to 4) Adhocctl Event Handler
 * @param handler To-be-registered Adhocctl Handler
 * @param arg Optional to-be-passed Parameter (Get's passed to Handler in case of Event)
 * @return Handler ID > 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_HANDLER_MAX
 */
int proNetAdhocctlAddHandler(SceNetAdhocctlHandler handler, void * arg)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(handler != NULL)
		{
			// Find free Handler Slot
			int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
			{
				// Found free Slot
				if(_event_handler[i] == NULL)
				{
					// Register Handler
					_event_handler[i] = handler;
					_event_args[i] = arg;
					
					// Return Handler ID
					return i + 1;
				}
			}
			
			// Out of Slots
			return ADHOCCTL_HANDLER_MAX;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

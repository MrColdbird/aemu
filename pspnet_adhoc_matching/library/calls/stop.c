#include "../common.h"

/**
 * Stop Matching Context
 * @param id Matching Context ID
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ID
 */
int proNetAdhocMatchingStop(int id)
{
	// Library initialized
	if(_init == 1)
	{
		// Find Matching Context for ID
		SceNetAdhocMatchingContext * context = _findMatchingContext(id);
		
		// Found Matching Context
		if(context != NULL)
		{
			// Context is running
			if(context->running)
			{
				// DO NOT CHANGE THE THREAD SHUTDOWN ORDER!
				// MATCHING FIRES A BYE EVENT CAUSED BY THE IO THREAD SHUTDOWN THAT THE EVENT THREAD HAS TO CATCH!
				
				// Shutdown IO Thread
				context->input_thid = -1;
				
				// Wait for Shutdown
				while(context->input_thid != 0) sceKernelDelayThread(10000);
				
				// Shutdown Event Dispatcher Thread
				context->event_thid = -1;
				
				// Wait for Shutdown
				while(context->event_thid != 0) sceKernelDelayThread(10000);
				
				// Free Hello Data Memory
				_free(context->hello);
				
				// Erase Hello Data Info
				context->hellolen = 0;
				context->hello = NULL;
				
				// Set Context into Stopped State
				context->running = 0;
			}
			
			// Return Success
			return 0;
		}
		
		// Invalid Matching ID
		return ADHOC_MATCHING_INVALID_ID;
	}
	
	// Uninitialized Library
	return ADHOC_MATCHING_NOT_INITIALIZED;
}


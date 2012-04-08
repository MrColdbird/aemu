#include "../common.h"

/**
 * Delete Matching Context
 * @param id Matching Context ID
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_IS_RUNNING
 */
int proNetAdhocMatchingDelete(int id)
{
	// Library initialized
	if(_init == 1)
	{
		// Find Matching Context for ID
		SceNetAdhocMatchingContext * context = _findMatchingContext(id);
		
		// Found Matching Context
		if(context != NULL)
		{
			// Context is not running
			if(!context->running)
			{
				// Previous Matching Context in Internal List
				SceNetAdhocMatchingContext * previous = NULL;
				
				// Iterate Matching Context List
				SceNetAdhocMatchingContext * item = _contexts; for(; item != NULL; item = item->next)
				{
					// Found Context in List
					if(item == context) break;
					
					// Update Previous Reference
					previous = item;
				}

				// Unlink Context from List
				if(item == context)
				{
					// Middle of List
					if(previous != NULL) previous->next = context->next;
					
					// Beginning of List
					else _contexts = context->next;
				}
				
				// Free Receive Buffer
				_free(context->rxbuf);
				
				// Free Context Buffer
				_free(context);
				
				// Return Success
				return 0;
			}
			
			// Context is still running
			return ADHOC_MATCHING_IS_RUNNING;
		}
		
		// Invalid Matching ID
		return ADHOC_MATCHING_INVALID_ID;
	}
	
	// Uninitialized Library
	return ADHOC_MATCHING_NOT_INITIALIZED;
}


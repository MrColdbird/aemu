/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

#include "../common.h"

/**
 * Set / Update Hello Data
 * @param id Matching Context ID
 * @param optlen Optional Data Length
 * @param opt Optional Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_INVALID_MODE, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_INVALID_OPTLEN, ADHOC_MATCHING_NO_SPACE
 */
int proNetAdhocMatchingSetHelloOpt(int id, int optlen, const void * opt)
{
	// Library Initialized
	if(_init == 1)
	{
		// Find Matching Context
		SceNetAdhocMatchingContext * context = _findMatchingContext(id);
		
		// Found Context
		if(context != NULL)
		{
			// Valid Matching Modes
			if(context->mode != ADHOC_MATCHING_MODE_CHILD)
			{
				// Running Context
				if(context->running)
				{
					// Valid Optional Data Length
					if((optlen == 0 && opt == NULL) || (optlen > 0 && opt != NULL))
					{
						// Grab Existing Hello Data
						void * hello = context->hello;
						
						// Delete Hello Data
						context->hellolen = 0;
						context->hello = NULL;
						
						// Free Previous Hello Data
						_free(hello);
						
						// Allocation Required
						if(optlen > 0)
						{
							// Allocate Memory
							hello = _malloc(optlen);
							
							// Out of Memory
							if(hello == NULL) return ADHOC_MATCHING_NO_SPACE;
							
							// Clone Hello Data
							memcpy(hello, opt, optlen);
							
							// Set Hello Data
							context->hello = hello;
							context->hellolen = optlen;
						}
						
						// Return Success
						return 0;
					}
					
					// Invalid Optional Data Length
					return ADHOC_MATCHING_INVALID_OPTLEN;
				}
				
				// Context not running
				return ADHOC_MATCHING_NOT_RUNNING;
			}
			
			// Invalid Matching Mode (Child)
			return ADHOC_MATCHING_INVALID_MODE;
		}
		
		// Invalid Matching ID
		return ADHOC_MATCHING_INVALID_ID;
	}
	
	// Uninitialized Library
	return ADHOC_MATCHING_NOT_INITIALIZED;
}


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
 * Get Hello Data
 * @param id Matching Context ID
 * @param buflen IN: Buffer Size OUT: Required Buffer Size
 * @param buf IN: Buffer OUT: Hello Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_INVALID_MODE, ADHOC_MATCHING_NOT_RUNNING
 */
int proNetAdhocMatchingGetHelloOpt(int id, int * buflen, void * buf)
{
	// Library initialized
	if(_init == 1)
	{
		// Find Matching Context
		SceNetAdhocMatchingContext * context = _findMatchingContext(id);
		
		// Found Matching Context
		if(context != NULL)
		{
			// Valid Matching Mode
			if(context->mode != ADHOC_MATCHING_MODE_CHILD)
			{
				// Running Context
				if(context->running)
				{
					// Length Parameter available
					if(buflen != NULL)
					{
						// Length Returner Mode
						if(buf == NULL)
						{
							// Return Hello Data Length
							*buflen = context->hellolen;
						}
						
						// Normal Mode
						else
						{
							// Fix Negative Length
							if((*buflen) < 0) *buflen = 0;
							
							// Fix Data Length
							if((*buflen) > context->hellolen) *buflen = context->hellolen;
							
							// Copy Data into Buffer
							if((*buflen) > 0) memcpy(buf, context->hello, *buflen);
						}
						
						// Return Success
						return 0;
					}
					
					// Invalid Arguments
					return ADHOC_MATCHING_INVALID_ARG;
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


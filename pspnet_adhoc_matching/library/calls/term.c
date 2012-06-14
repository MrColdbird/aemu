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
 * Terminate Adhoc Matching Emulator
 * @return 0 on success or... ADHOC_MATCHING_BUSY
 */
int proNetAdhocMatchingTerm(void)
{
	// Library initialized
	if(_init == 1)
	{
		// No Context alive
		if(_contexts == NULL)
		{
			// Mark Library as shutdown
			_init = 0;
			
			// Clear Fake Pool Size
			_fake_poolsize = 0;
		}
		
		// Library is busy
		else return ADHOC_MATCHING_BUSY;
	}
	
	// Return Success
	return 0;
}


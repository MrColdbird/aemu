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
 * Get Maximum Available Memory
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG
 */
int proNetAdhocMatchingGetPoolStat(SceNetMallocStat * poolstat)
{
	// Initialized Library
	if(_init == 1)
	{
		// Valid Argument
		if(poolstat != NULL)
		{
			// Fill Poolstat with Fake Data
			poolstat->poolsize = _fake_poolsize;
			poolstat->maxsize = _fake_poolsize / 8 * 6;
			poolstat->freesize = _fake_poolsize / 8 * 7;
			
			// Return Success
			return 0;
		}
		
		// Invalid Argument
		return ADHOC_MATCHING_INVALID_ARG;
	}
	
	// Uninitialized Library
	return ADHOC_MATCHING_NOT_INITIALIZED;
}


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


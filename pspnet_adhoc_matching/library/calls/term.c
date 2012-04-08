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


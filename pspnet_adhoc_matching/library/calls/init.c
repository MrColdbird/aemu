#include "../common.h"

// Library State
int _init = 0;

// Fake Pool Size
uint32_t _fake_poolsize = 0;

// Context List
SceNetAdhocMatchingContext * _contexts = NULL;

/**
 * Initialize the Adhoc Matching Emulator
 * @param poolsize Internal Heap Size
 * @return 0 on success or... ADHOC_MATCHING_ALREADY_INITIALIZED
 */
int proNetAdhocMatchingInit(uint32_t poolsize)
{
	// Uninitialized Library
	if(_init == 0)
	{
		// Save Fake Pool Size
		_fake_poolsize = poolsize;
		
		// Initialize Library
		_init = 1;
		
		// Return Success
		return 0;
	}
	
	// Initialized Library
	return ADHOC_MATCHING_ALREADY_INITIALIZED;
}


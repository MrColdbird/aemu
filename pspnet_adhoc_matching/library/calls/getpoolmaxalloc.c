#include "../common.h"

/**
 * Return Expected Maximum Poolsize required by Matching Library
 * @return 50KB - aka. 50 * 1024
 */
int proNetAdhocMatchingGetPoolMaxAlloc(void)
{
	// Lazy way out - hardcoded return value
	return 50 * 1024;
}


#include "common.h"

/**
 * Mersenne Twister Number Generator
 * @param max Maximum Range of Number
 * @return Random unsigned 32bit Number
 */
uint32_t _getRandomNumber(uint32_t max)
{
	// Initialization Switch
	static int randinit = 0;
	
	// Mersenne Twister Context
	static SceKernelUtilsMt19937Context ctx;
	
	// First Time Initialization
	if(randinit == 0)
	{
		// Initialize Mersenne Twister
		sceKernelUtilsMt19937Init(&ctx, time(NULL));
		
		// Prevent Duplicate Initialization
		randinit = 1;
	}
	
	// Calculate Number
	uint32_t number = sceKernelUtilsMt19937UInt(&ctx);
	
	// Trim Number
	number %= max + 1;
	
	// Return Number
	return number;
}

#include "../common.h"

// PDP Sockets
SceNetAdhocPdpStat * _pdp[255];

// PTP Sockets
SceNetAdhocPtpStat * _ptp[255];

// Gamemode Buffer
SceNetAdhocGameModeBufferStat * _gmb = NULL;

// Initialized Switch
int _init = 0;

// Manage Infrastructure Modules Switch
int _manage_modules = 0;

// Global One
int _one = 1;

// Global Zero
int _zero = 0;

/**
 * Adhoc Emulator Socket Library Init-Call
 * @return 0 on success or... ADHOC_ALREADY_INITIALIZED
 */
int proNetAdhocInit(void)
{
	// Library uninitialized
	if(!_init)
	{
		// Initialize Internet Library
		int result = sceNetInetInit();
		
		// Missing Internet Modules
		if(result != 0)
		{
			// Load Internet Modules
			sceUtilityLoadModule(PSP_MODULE_NET_INET);
			
			// Enable Manual Infrastructure Module Control
			_manage_modules = 1;
			
			// Re-Initialize Internet Library
			result = sceNetInetInit();
		}
		
		// Initialized Internet Library
		if(result == 0)
		{
			// Clear Translator Memory
			memset(&_pdp, 0, sizeof(_pdp));
			memset(&_ptp, 0, sizeof(_ptp));
			
			// Library initialized
			_init = 1;
			
			// Return Success
			return 0;
		}
		
		// Generic Error
		return -1;
	}
	
	// Already initialized
	return ADHOC_ALREADY_INITIALIZED;
}

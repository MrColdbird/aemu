#include "../common.h"

// PDP Sockets
SceNetAdhocPdpStat * _pdp = NULL;

// PTP Sockets
SceNetAdhocPtpStat * _ptp = NULL;

// Gamemode Buffer
SceNetAdhocGameModeBufferStat * _gmb = NULL;

// Initialized Switch
int _init = 0;

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
		// Load Internet Modules
		sceUtilityLoadModule(PSP_MODULE_NET_INET);
		
		// Initialize Internet Library
		if(sceNetInetInit() == 0)
		{
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

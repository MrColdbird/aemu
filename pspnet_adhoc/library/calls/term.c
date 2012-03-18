#include "../common.h"

// Helper Functions
static void _deleteAllGMB_Internal(SceNetAdhocGameModeBufferStat * node);

/**
 * Adhoc Emulator Socket Library Term-Call
 * @return 0 on success or... ADHOC_BUSY
 */
int proNetAdhocTerm(void)
{
	// Library is initialized
	if(_init)
	{
		// Delete PDP Sockets
		_deleteAllPDP();
		
		// Delete PTP Sockets
		_deleteAllPTP();
		
		// Delete Gamemode Buffer
		_deleteAllGMB();
		
		// Terminate Internet Library
		sceNetInetTerm();
		
		// Unload Internet Modules (Just keep it in memory... unloading crashes?!)
		// if(_manage_modules != 0) sceUtilityUnloadModule(PSP_MODULE_NET_INET);
		
		// Drop Module Management
		_manage_modules = 0;
		
		// Library shutdown
		_init = 0;
	}
	
	// Success
	return 0;
}

/**
 * Closes & Deletes all PDP Sockets
 */
void _deleteAllPDP(void)
{
	// Iterate Element
	int i = 0; for(; i < 255; i++)
	{
		// Active Socket
		if(_pdp[i] != NULL)
		{
			// Close Socket
			sceNetInetClose(_pdp[i]->id);
			
			// Free Memory
			free(_pdp[i]);
			
			// Delete Reference
			_pdp[i] = NULL;
		}
	}
}

/**
 * Closes & Deletes all PTP Sockets
 */
void _deleteAllPTP(void)
{
	// Iterate Element
	int i = 0; for(; i < 255; i++)
	{
		// Active Socket
		if(_ptp[i] != NULL)
		{
			// Close Socket
			sceNetInetClose(_ptp[i]->id);
			
			// Free Memory
			free(_ptp[i]);
			
			// Delete Reference
			_ptp[i] = NULL;
		}
	}
}

/**
 * Deletes all Gamemode Buffer
 */
void _deleteAllGMB(void)
{
	// Delete Recursively
	_deleteAllGMB_Internal(_gmb);
	
	// Destroy Reference
	_gmb = NULL;
}

/**
 * Recursive Deleter for Gamemode Buffer
 * @param node Current Gamemode Buffer Node
 */
static void _deleteAllGMB_Internal(SceNetAdhocGameModeBufferStat * node)
{
	// Not at the End
	if(node != NULL)
	{
		// Process Rest of the List
		_deleteAllGMB_Internal(node->next);
		
		// Delete Node
		free(node);
	}
}

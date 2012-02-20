#include "../common.h"

// Helper Functions
static void _deleteAllPDP_Internal(SceNetAdhocPdpStat * node);
static void _deleteAllPTP_Internal(SceNetAdhocPtpStat * node);
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
		
		// Unload Internet Modules
		sceUtilityUnloadModule(PSP_MODULE_NET_INET);
		
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
	// Delete Recursively
	_deleteAllPDP_Internal(_pdp);
	
	// Destroy Reference
	_pdp = NULL;
}

/**
 * Closes & Deletes all PTP Sockets
 */
void _deleteAllPTP(void)
{
	// Delete Recursively
	_deleteAllPTP_Internal(_ptp);
	
	// Destroy Reference
	_ptp = NULL;
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
 * Recursive Deleter for PDP Sockets
 * @param node Current PDP Node
 */
static void _deleteAllPDP_Internal(SceNetAdhocPdpStat * node)
{
	// Not at the End
	if(node != NULL)
	{
		// Process Rest of the List
		_deleteAllPDP_Internal(node->next);
		
		// Close Connection
		sceNetInetClose(node->id);
		
		// Delete Node
		free(node);
	}
}

/**
 * Recursive Deleter for PTP Sockets
 * @param node Current PTP Node
 */
static void _deleteAllPTP_Internal(SceNetAdhocPtpStat * node)
{
	// Not at the End
	if(node != NULL)
	{
		// Process Rest of the List
		_deleteAllPTP_Internal(node->next);
		
		// Close Connection
		sceNetInetClose(node->id);
		
		// Delete Node
		free(node);
	}
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

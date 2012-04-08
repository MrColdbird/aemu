#include "../common.h"

/**
 * Abort Data-Sending to Matching Target
 * @param id Matching Context ID
 * @param target Target MAC
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_UNKNOWN_TARGET
 */
int proNetAdhocMatchingAbortSendData(int id, const SceNetEtherAddr * target)
{
	// Initialized Library
	if(_init == 1)
	{
		// Valid Arguments
		if(target != NULL)
		{
			// Find Matching Context
			SceNetAdhocMatchingContext * context = _findMatchingContext(id);
			
			// Found Context
			if(context != NULL)
			{
				// Running Context
				if(context->running)
				{
					// Find Target Peer
					SceNetAdhocMatchingMemberInternal * peer = _findPeer(context, (SceNetEtherAddr *)target);
					
					// Found Peer
					if(peer != NULL)
					{
						// Peer is sending
						if(peer->sending)
						{
							// Set Peer as Bulk Idle
							peer->sending = 0;
							
							// Stop Bulk Data Sending (if in progress)
							_abortBulkTransfer(context, peer);
						}
						
						// Return Success
						return 0;
					}
					
					// Peer not found
					return ADHOC_MATCHING_UNKNOWN_TARGET;
				}
				
				// Context not running
				return ADHOC_MATCHING_NOT_RUNNING;
			}
			
			// Invalid Matching ID
			return ADHOC_MATCHING_INVALID_ID;
		}
		
		// Invalid Arguments
		return ADHOC_MATCHING_INVALID_ARG;
	}
	
	// Uninitialized Library
	return ADHOC_MATCHING_NOT_INITIALIZED;
}


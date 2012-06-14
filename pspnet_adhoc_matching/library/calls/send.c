/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

#include "../common.h"

/**
 * Send Data to Matching Target
 * @param id Matching Context ID
 * @param target Target MAC
 * @param datalen Length of Data
 * @param data Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_UNKNOWN_TARGET, ADHOC_MATCHING_INVALID_DATALEN, ADHOC_MATCHING_NOT_ESTABLISHED, ADHOC_MATCHING_NO_SPACE, ADHOC_MATCHING_DATA_BUSY
 */
int proNetAdhocMatchingSendData(int id, const SceNetEtherAddr * target, int datalen, const void * data)
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
						// Valid Data Length
						if(datalen > 0 && data != NULL)
						{
							// Valid Peer Connection State
							if(peer->state == ADHOC_MATCHING_PEER_PARENT || peer->state == ADHOC_MATCHING_PEER_CHILD || peer->state == ADHOC_MATCHING_PEER_P2P)
							{
								// Send in Progress
								if(peer->sending) return ADHOC_MATCHING_DATA_BUSY;
								
								// Mark Peer as Sending
								peer->sending = 1;
								
								// Send Data to Peer
								_sendBulkData(context, peer, datalen, data);
								
								// Return Success
								return 0;
							}
							
							// Not connected / accepted
							return ADHOC_MATCHING_NOT_ESTABLISHED;
						}
						
						// Invalid Data Length
						return ADHOC_MATCHING_INVALID_DATALEN;
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


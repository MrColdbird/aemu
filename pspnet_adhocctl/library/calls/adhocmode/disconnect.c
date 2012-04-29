#include "../../common.h"

/**
 * Leave current Network
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_BUSY
 */
int proNetAdhocctlDisconnect(void)
{
	// Library initialized
	if(_init == 1)
	{
		// Connected State (Adhoc Mode)
		if(_thread_status == ADHOCCTL_STATE_CONNECTED)
		{
			// Clear Network Name
			memset(&_parameter.group_name, 0, sizeof(_parameter.group_name));
			
			// Set Disconnected State
			_thread_status = ADHOCCTL_STATE_DISCONNECTED;
			
			// Set HUD Connection Status
			setConnectionStatus(0);
			
			// Prepare Packet
			uint8_t opcode = OPCODE_DISCONNECT;
			
			// Acquire Network Lock
			_acquireNetworkLock();
			
			// Send Disconnect Request Packet
			sceNetInetSend(_metasocket, &opcode, 1, INET_MSG_DONTWAIT);
			
			// Free Network Lock
			_freeNetworkLock();
			
			// Multithreading Lock
			_acquirePeerLock();
			
			// Clear Peer List
			_freeFriendsRecursive(_friends);
			
			// Delete Peer Reference
			_friends = NULL;
			
			// Multithreading Unlock
			_freePeerLock();
		}
		
		// Notify Event Handlers (even if we weren't connected, not doing this will freeze games like God Eater, which expect this behaviour)
		int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
		{
			// Active Handler
			if(_event_handler[i] != NULL) _event_handler[i](ADHOCCTL_EVENT_DISCONNECT, 0, _event_args[i]);
		}
		
		// Return Success
		return 0;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

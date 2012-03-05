#include "../../common.h"

/**
 * Adhoc Emulator PTP Receiver
 * @param id Socket File Descriptor
 * @param buf Data Buffer
 * @param len IN: Buffersize OUT: Received Data (in Bytes)
 * @param timeout Receive Timeout (in Microseconds)
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_SOCKET_ALERTED, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_THREAD_ABORTED, ADHOC_DISCONNECTED, NET_INTERNAL
 */
int proNetAdhocPtpRecv(int id, void * buf, int * len, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPtpStat * socket = (SceNetAdhocPtpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Socket
		if(socket != NULL && _ptpSocketInList(socket) && socket->state == PTP_STATE_ESTABLISHED)
		{
			// Valid Arguments
			if(buf != NULL && len != NULL && *len > 0)
			{
				// Not Alerted
				if((socket->rcv_sb_cc & ADHOC_F_ALERTRECV) == 0)
				{
					// Schedule Timeout Removal
					if(flag) timeout = 0;
					
					// Apply Send Timeout Settings to Socket
					sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
					
					// Acquire Network Lock
					_acquireNetworkLock();
					
					// Receive Data
					int received = sceNetInetRecv(socket->id, buf, *len, ((flag) ? (INET_MSG_DONTWAIT) : (0)));
					
					// Free Network Lock
					_freeNetworkLock();
					
					// Received Data
					if(received > 0)
					{
						// Save Length
						*len = received;
						
						// Return Success
						return 0;
					}
					
					// Non-Critical Error
					else if(received == -1 && sceNetInetGetErrno() == EAGAIN)
					{
						// Blocking Situation
						if(flag) return ADHOC_WOULD_BLOCK;
						
						// Timeout
						return ADHOC_TIMEOUT;
					}
					
					// Change Socket State
					socket->state = PTP_STATE_CLOSED;
					
					// Disconnected
					return ADHOC_DISCONNECTED;
				}
				
				// Clear Alert
				socket->rcv_sb_cc = 0;
				
				// Return Alerted Result
				return ADHOC_SOCKET_ALERTED;
			}
			
			// Invalid Arguments
			return ADHOC_INVALID_ARG;
		}
		
		// Invalid Socket
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

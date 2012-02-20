#include "../../common.h"

/**
 * Adhoc Emulator PTP Flusher
 * @param id Socket File Descriptor
 * @param timeout Flush Timeout (in Microseconds)
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_SOCKET_ALERTED, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_THREAD_ABORTED, ADHOC_DISCONNECTED, ADHOC_NOT_CONNECTED, NET_INTERNAL
 */
int proNetAdhocPtpFlush(int id, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPtpStat * socket = (SceNetAdhocPtpStat *)id;
	
	// Library initialized
	if(_init)
	{
		// Valid Socket
		if(socket != NULL && _ptpSocketInList(socket))
		{
			// Not alert
			if((socket->rcv_sb_cc & ADHOC_F_ALERTFLUSH) == 0)
			{
				// Dummy Result
				return 0;
			}
			
			// Clear Alert
			socket->rcv_sb_cc = 0;
			
			// Return Alerted Result
			return ADHOC_SOCKET_ALERTED;
		}
		
		// Invalid Socket
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library uninitialized
	return ADHOC_NOT_INITIALIZED;
}

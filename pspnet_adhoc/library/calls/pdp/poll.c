#include "../../common.h"

/**
 * Adhoc Emulator PDP Socket Poller (Select Equivalent)
 * @param sds Poll Socket Descriptor Array
 * @param nsds Array Item Count
 * @param timeout Timeout in Microseconds
 * @param flags Nonblocking Flag
 * @return Number of affected Sockets on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID, ADHOC_INVALID_ARG, ADHOC_TIMEOUT, ADHOC_THREAD_ABORTED, ADHOC_EXCEPTION_EVENT, NET_NO_SPACE, NET_INTERNAL
 */
int proNetAdhocPollSocket(SceNetAdhocPollSd * sds, int nsds, uint32_t timeout, int flags)
{
	// Library is initialized
	if(_init)
	{
		// Valid Arguments
		if(sds != NULL && nsds > 0)
		{
			// Alert Trigger
			int triggeralert = 0;
			
			// Socket Check
			int i = 0; for(; i < nsds; i++)
			{
				// Cast Socket
				SceNetAdhocPdpStat * socket = (SceNetAdhocPdpStat *)sds[i].id;
				
				// Invalid Socket
				if(socket == NULL || !_pdpSocketInList(socket)) return ADHOC_INVALID_SOCKET_ID;
				
				// Alerted Socket
				if((socket->rcv_sb_cc & ADHOC_F_ALERTPOLL) != 0)
				{
					// Erase Alert
					socket->rcv_sb_cc = 0;
					
					// Set Received Event
					sds[i].revents = ADHOC_EV_ALERT;
					
					// Trigger Alert
					triggeralert = 1;
				}
			}
			
			// Alert Exit
			if(triggeralert) return ADHOC_EXCEPTION_EVENT;
			
			// Allocate Infrastructure Memory
			SceNetInetPollfd * isds = (SceNetInetPollfd *)malloc(sizeof(SceNetInetPollfd) * nsds);
			
			// Allocated Memory
			if(isds != NULL)
			{
				// Clear Memory
				memset(isds, 0, sizeof(SceNetInetPollfd) * nsds);
				
				// Translate Polling Flags to Infrastructure
				for(i = 0; i < nsds; i++)
				{
					// Cast Socket
					SceNetAdhocPdpStat * socket = (SceNetAdhocPdpStat *)sds[i].id;
					
					// Fill in Infrastructure Socket ID
					isds[i].fd = socket->id;
					
					// Send Event
					if(sds[i].events & ADHOC_EV_SEND) isds[i].events |= INET_POLLWRNORM;
					
					// Receive Event
					if(sds[i].events & ADHOC_EV_RECV) isds[i].events |= INET_POLLRDNORM;
				}
				
				// Nonblocking Mode
				if(flags) timeout = 0;
				
				// Timeout Translation (Micro to Milliseconds)
				else
				{
					// Convert Timeout
					timeout /= 1000;
					
					// Prevent Nonblocking Mode
					if(timeout == 0) timeout = 1;
				}
				
				// Acquire Network Lock
				_acquireNetworkLock();
				
				// Poll Sockets
				int affectedsockets = sceNetInetPoll(isds, nsds, timeout);
				
				// Free Network Lock
				_freeNetworkLock();
				
				// Sockets affected
				if(affectedsockets > 0)
				{
					// Translate Polling Results to Adhoc
					for(i = 0; i < nsds; i++)
					{
						// Send Event
						if(isds[i].revents & INET_POLLWRNORM) sds[i].revents |= ADHOC_EV_SEND;
						
						// Receive Event
						if(isds[i].revents & INET_POLLRDNORM) sds[i].revents |= ADHOC_EV_RECV;
					}
				}
				
				// Free Memory
				free(isds);
				
				// Blocking Mode (Nonblocking Mode returns 0, even on Success)
				if(!flags)
				{
					// Success
					if(affectedsockets > 0) return affectedsockets;
					
					// Timeout
					return ADHOC_TIMEOUT;
				}
			}
			
			// No Events generated
			return 0;
		}
		
		// Invalid Argument
		return ADHOC_INVALID_ARG;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

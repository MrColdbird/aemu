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
			// Socket Check
			int i = 0; for(; i < nsds; i++)
			{
				// Invalid Socket
				if(sds[i].id < 1 || sds[i].id > 255 || _pdp[sds[i].id - 1] == NULL) return ADHOC_INVALID_SOCKET_ID;
			}
			
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
					// Fill in Infrastructure Socket ID
					isds[i].fd = _pdp[sds[i].id - 1]->id;
					
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

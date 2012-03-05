#include "../../common.h"

/**
 * Adhoc Emulator PTP Connection Opener
 * @param id Socket File Descriptor
 * @param timeout Connect Timeout (in Microseconds)
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_CONNECTION_REFUSED, ADHOC_SOCKET_ALERTED, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_NOT_OPENED, ADHOC_THREAD_ABORTED, NET_INTERNAL
 */
int proNetAdhocPtpConnect(int id, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPtpStat * socket = (SceNetAdhocPtpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Socket
		if(socket != NULL && _ptpSocketInList(socket))
		{
			// Valid Client Socket
			if(socket->state == 0)
			{
				// Not alerted
				if((socket->rcv_sb_cc & ADHOC_F_ALERTACCEPT) == 0)
				{
					// Peer Information
					SceNetAdhocctlPeerInfo peer_info;
					memset(&peer_info, 0, sizeof(peer_info));
					
					// Grab Peer Information
					if(sceNetAdhocctlGetPeerInfo(&socket->paddr, sizeof(peer_info), &peer_info) == 0)
					{
						// Target Address
						SceNetInetSockaddrIn sin;
						memset(&sin, 0, sizeof(sin));
						
						// Setup Target Address
						sin.sin_family = AF_INET;
						sin.sin_addr = peer_info.ip_addr;
						sin.sin_port = sceNetHtons(socket->pport);
						
						// Grab Nonblocking Flag
						uint32_t nbio = 0;
						uint32_t nbiolen = sizeof(nbio);
						sceNetInetGetsockopt(socket->id, SOL_SOCKET, SO_NBIO, &nbio, &nbiolen);
						
						// Switch to Nonblocking Behaviour
						if(nbio == 0)
						{
							// Overwrite Socket Option
							sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_NBIO, &_one, sizeof(_one));
						}
						
						// Connect Socket to Peer (Nonblocking)
						sceNetInetConnect(socket->id, (SceNetInetSockaddr *)&sin, sizeof(sin));
						
						// Grab Error Code
						int errorcode = sceNetInetGetErrno();
						
						// Restore Blocking Behaviour
						if(nbio == 0)
						{
							// Restore Socket Option
							sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_NBIO, &nbio, sizeof(nbio));
						}
						
						// Instant Connection (Lucky!)
						if(errorcode == EISCONN)
						{
							// Set Connected State
							socket->state = PTP_STATE_ESTABLISHED;
							
							// Success
							return 0;
						}
						
						// Connection in Progress
						else if(errorcode == EINPROGRESS)
						{
							// Nonblocking Mode
							if(flag) return ADHOC_WOULD_BLOCK;
							
							// Blocking Mode
							else
							{
								// Grab Connection Start Time
								uint32_t starttime = sceKernelGetSystemTimeLow();
								
								// Peer Information (for Connection-Polling)
								SceNetInetSockaddrIn peer;
								memset(&peer, 0, sizeof(peer));
								uint32_t peerlen = sizeof(peer);
								
								// Wait for Connection
								while((timeout == 0 || (sceKernelGetSystemTimeLow() - starttime) < timeout) && sceNetInetGetpeername(socket->id, (SceNetInetSockaddr *)&peer, &peerlen) != 0)
								{
									// Wait 1ms
									sceKernelDelayThread(1000);
								}
								
								// Connected in Time
								if(sin.sin_addr == peer.sin_addr && sin.sin_port == peer.sin_port)
								{
									// Set Connected State
									socket->state = PTP_STATE_ESTABLISHED;
									
									// Success
									return 0;
								}
								
								// Timeout occured
								return ADHOC_TIMEOUT;
							}
						}
					}
					
					// Peer not found
					return ADHOC_CONNECTION_REFUSED;
				}
				
				// Clear Alert
				socket->rcv_sb_cc = 0;
				
				// Return Alerted Result
				return ADHOC_SOCKET_ALERTED;
			}
			
			// Not a valid Client Socket
			return ADHOC_NOT_OPENED;
		}
		
		// Invalid Socket
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

/**
 * Socket Existence Check
 * @param socket To-be-checked Socket
 * @return 1 if found or... 0
 */
int _ptpSocketInList(SceNetAdhocPtpStat * socket)
{
	// Iterate PTP Socket List
	SceNetAdhocPtpStat * list = _ptp;
	for(; list != NULL; list = list->next)
	{
		// Socket found
		if(list == socket) return 1;
	}
	
	// Socket not found
	return 0;
}
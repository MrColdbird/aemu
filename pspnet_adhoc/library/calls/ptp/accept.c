#include "../../common.h"

/**
 * Adhoc Emulator PTP Connection Acceptor
 * @param id Socket File Descriptor
 * @param addr OUT: Peer MAC Address
 * @param port OUT: Peer Port
 * @param timeout Accept Timeout (in Microseconds)
 * @param flag Nonblocking Flag
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_SOCKET_ALERTED, ADHOC_SOCKET_ID_NOT_AVAIL, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_NOT_LISTENED, ADHOC_THREAD_ABORTED, NET_INTERNAL
 */
int proNetAdhocPtpAccept(int id, SceNetEtherAddr * addr, uint16_t * port, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPtpStat * socket = (SceNetAdhocPtpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Socket
		if(socket != NULL && _ptpSocketInList(socket))
		{
			// Listener Socket
			if(socket->state == PTP_STATE_LISTEN)
			{
				// Valid Arguments
				if(addr != NULL && port != NULL)
				{
					// Address Information
					SceNetInetSockaddrIn peeraddr;
					memset(&addr, 0, sizeof(peeraddr));
					uint32_t peeraddrlen = sizeof(peeraddr);
					
					// Local Address Information
					SceNetInetSockaddrIn local;
					memset(&local, 0, sizeof(local));
					uint32_t locallen = sizeof(local);
					
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
					
					// Accept Connection
					int newsocket = sceNetInetAccept(socket->id, (SceNetInetSockaddr *)&peeraddr, &peeraddrlen);
					
					// Blocking Behaviour
					if(!flag && newsocket == -1)
					{
						// Get Start Time
						uint32_t starttime = sceKernelGetSystemTimeLow();
						
						// Retry until Timeout hits
						while((sceKernelGetSystemTimeLow() - starttime) < timeout && newsocket == -1)
						{
							// Accept Connection
							newsocket = sceNetInetAccept(socket->id, (SceNetInetSockaddr *)&peeraddr, &peeraddrlen);
							
							// Wait a bit...
							sceKernelDelayThread(1000);
						}
					}
					
					// Restore Blocking Behaviour
					if(nbio == 0)
					{
						// Restore Socket Option
						sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_NBIO, &nbio, sizeof(nbio));
					}
					
					// Accepted New Connection
					if(newsocket > 0)
					{
						// Enable Port Re-use
						sceNetInetSetsockopt(newsocket, SOL_SOCKET, SO_REUSEADDR, &_one, sizeof(_one));
						sceNetInetSetsockopt(newsocket, SOL_SOCKET, SO_REUSEPORT, &_one, sizeof(_one));
						
						// Grab Local Address
						if(sceNetInetGetsockname(newsocket, (SceNetInetSockaddr *)&local, &locallen) == 0)
						{
							// Adhoc Peer List Size
							int buflen = 0;
							
							// Peers available
							if(sceNetAdhocctlGetPeerList(&buflen, NULL) == 0 && buflen > 0)
							{
								// Allocate Peer List
								SceNetAdhocctlPeerInfo * buf = (SceNetAdhocctlPeerInfo *)malloc(buflen);
								
								// Allocated Peer List
								if(buf != NULL)
								{
									// Peer Information
									SceNetAdhocctlPeerInfo peer;
									memset(&peer, 0, sizeof(peer));
									
									// Get Peerlist
									if(sceNetAdhocctlGetPeerList(&buflen, buf) == 0)
									{
										// Peer Iterator Variable
										SceNetAdhocctlPeerInfo * list = buf;
										
										// Iterate Peers
										while(list != NULL)
										{
											// Matching Peer found
											if(list->ip_addr == peeraddr.sin_addr)
											{
												// Copy Peer Information
												peer = *list;
												
												// Delete References
												peer.next = NULL;
												
												// Stop Search
												break;
											}
											
											// Move Iterator
											list = list->next;
										}
									}
									
									// Free Peer List
									free(buf);
									
									// Found Peer Information
									if(peer.ip_addr != 0)
									{
										// Allocate Memory
										SceNetAdhocPtpStat * internal = (SceNetAdhocPtpStat *)malloc(sizeof(SceNetAdhocPtpStat));
										
										// Allocated Memory
										if(internal != NULL)
										{
											// Clear Memory
											memset(internal, 0, sizeof(SceNetAdhocPtpStat));
											
											// Copy Socket Descriptor to Structure
											internal->id = newsocket;
											
											// Copy Local Address Data to Structure
											sceNetGetLocalEtherAddr(&internal->laddr);
											internal->lport = sceNetNtohs(local.sin_port);
											
											// Copy Peer Address Data to Structure
											internal->paddr = peer.mac_addr;
											internal->pport = sceNetNtohs(peeraddr.sin_port);
											
											// Set Connected State
											internal->state = PTP_STATE_ESTABLISHED;
											
											// Return Peer Address Information
											*addr = internal->paddr;
											*port = internal->pport;
											
											// Append PTP Socket to Internal List
											_ptpAppendInternal(internal);
											
											// Return Socket
											return (int)internal;
										}
									}
								}
							}
						}
						
						// Close Socket
						sceNetInetClose(newsocket);
					}
					
					// Action would block
					if(flag) return ADHOC_WOULD_BLOCK;
					
					// Timeout
					return ADHOC_TIMEOUT;
				}
				
				// Invalid Arguments
				return ADHOC_INVALID_ARG;
			}
			
			// Client Socket
			return ADHOC_NOT_LISTENED;
		}
		
		// Invalid Socket
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

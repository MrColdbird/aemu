#include "../../common.h"

/**
 * Adhoc Emulator PDP Receive Call
 * @param id Socket File Descriptor
 * @param saddr OUT: Source MAC Address
 * @param sport OUT: Source Port
 * @param buf OUT: Received Data
 * @param len IN: Buffer Size OUT: Received Data Length
 * @param timeout Receive Timeout
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_SOCKET_ALERTED, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_NOT_ENOUGH_SPACE, ADHOC_THREAD_ABORTED, NET_INTERNAL
 */
int proNetAdhocPdpRecv(int id, SceNetEtherAddr * saddr, uint16_t * sport, void * buf, int * len, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPdpStat * socket = (SceNetAdhocPdpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Socket ID
		if(socket != NULL && _pdpSocketInList(socket))
		{
			// Valid Arguments
			if(saddr != NULL && sport != NULL && buf != NULL && len != NULL && *len > 0)
			{
				#ifndef PDP_DIRTY_MAGIC
				// Schedule Timeout Removal
				// if(flag) timeout = 0;
				#else
				// Nonblocking Simulator
				int wouldblock = 0;
				
				// Minimum Timeout
				uint32_t mintimeout = 250000;
				
				// Nonblocking Call
				if(flag)
				{
					// Erase Nonblocking Flag
					flag = 0;
					
					// Set Wouldblock Behaviour
					wouldblock = 1;
					
					// Set Minimum Timeout (250ms)
					if(timeout < mintimeout) timeout = mintimeout;
				}
				#endif
				
				// Apply Receive Timeout Settings to Socket
				sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
				
				// Sender Address
				SceNetInetSockaddrIn sin;
				uint32_t sinlen;
				
				// Receive Data
				int received = sceNetInetRecvfrom(socket->id, buf, *len, ((flag != 0) ? (INET_MSG_DONTWAIT) : (0)), (SceNetInetSockaddr *)&sin, &sinlen);
				
				// Received Data
				if(received > 0)
				{
					// Get Peer List Size
					int buflen = 0;
					if(sceNetAdhocctlGetPeerList(&buflen, NULL) == 0 && buflen > 0)
					{
						// Allocate Memory
						SceNetAdhocctlPeerInfo * peers = (SceNetAdhocctlPeerInfo *)malloc(buflen);
						
						// Allocated Memory
						if(peers != NULL)
						{
							// Peer Search Success
							int peerfound = 0;
							
							// Get Peer List
							if(sceNetAdhocctlGetPeerList(&buflen, peers) == 0 && buflen > 0)
							{
								// Log Peer List
								#ifdef DEBUG
								printk("Discovered %d Peers for PDP Receive Query\n", buflen / sizeof(SceNetAdhocctlPeerInfo));
								#endif
								
								// Iterate Peers
								SceNetAdhocctlPeerInfo * peer = peers;
								for(; peer != NULL; peer = peer->next)
								{
									// IP Match found
									if(peer->ip_addr == sin.sin_addr)
									{
										// Log Matching Peer
										#ifdef DEBUG
										printk("Found Matching Peer for PDP Receive (%s)\n", (char *)peer->nickname.data);
										#endif
										
										// Provide Sender Information
										*saddr = peer->mac_addr;
										*sport = sceNetNtohs(sin.sin_port);
										
										// Save Length
										*len = received;
										
										// Peer found
										peerfound = 1;
										
										// Stop Peer Search
										break;
									}
								}
							}
							
							// Free Memory
							free(peers);
							
							// Peer found
							if(peerfound)
							{
								// Success
								return 0;
							}
						}
					}
				}
				
				#ifdef PDP_DIRTY_MAGIC
				// Restore Nonblocking Flag for Return Value
				if(wouldblock) flag = 1;
				#endif
				
				// Nothing received
				if(flag) return ADHOC_WOULD_BLOCK;
				return ADHOC_TIMEOUT;
			}
			
			// Invalid Argument
			return ADHOC_INVALID_ARG;
		}
		
		// Invalid Socket ID
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

#include "../../common.h"

/**
 * Adhoc Emulator PDP Send Call
 * @param id Socket File Descriptor
 * @param daddr Target MAC Address
 * @param dport Target Port
 * @param data Data Payload
 * @param len Payload Length
 * @param timeout Send Timeout
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT, ADHOC_INVALID_DATALEN, ADHOC_SOCKET_ALERTED, ADHOC_TIMEOUT, ADHOC_THREAD_ABORTED, ADHOC_WOULD_BLOCK, NET_NO_SPACE, NET_INTERNAL
 */
int proNetAdhocPdpSend(int id, const SceNetEtherAddr * daddr, uint16_t dport, const void * data, int len, uint32_t timeout, int flag)
{
	// Cast Socket
	SceNetAdhocPdpStat * socket = (SceNetAdhocPdpStat *)id;
	
	// Library is initialized
	if(_init)
	{
		// Valid Port
		if(dport != 0)
		{
			// Valid Data Length
			if(len > 0)
			{
				// Valid Socket ID
				if(socket != NULL && _pdpSocketInList(socket))
				{
					// Valid Data Buffer
					if(data != NULL)
					{
						// Valid Destination Address
						if(daddr != NULL)
						{
							// Log Destination
							#ifdef DEBUG
							printk("Attempting PDP Send to %02X:%02X:%02X:%02X:%02X:%02X on Port %u\n", daddr->data[0], daddr->data[1], daddr->data[2], daddr->data[3], daddr->data[4], daddr->data[5], dport);
							#endif
							
							// Schedule Timeout Removal
							// if(flag) timeout = 0;
							
							// Apply Send Timeout Settings to Socket
							sceNetInetSetsockopt(socket->id, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
							
							// Single Target
							if(!_isBroadcastMAC(daddr))
							{
								// Get Single Peer Information
								SceNetAdhocctlPeerInfo peer_info;
								if(sceNetAdhocctlGetPeerInfo(daddr, sizeof(SceNetAdhocctlPeerInfo), &peer_info) == 0)
								{
									// Fill in Target Structure
									SceNetInetSockaddrIn target;
									target.sin_family = AF_INET;
									target.sin_addr = peer_info.ip_addr;
									target.sin_port = sceNetHtons(dport);
									
									// Send Data
									int sent = sceNetInetSendto(socket->id, data, len, ((flag != 0) ? (INET_MSG_DONTWAIT) : (0)), (SceNetInetSockaddr *)&target, sizeof(target));
									
									// Sent Data
									if(sent == len)
									{
										// Log Success
										#ifdef DEBUG
										printk("Successfully Sent %d Bytes to %s\n", len, (char *)peer_info.nickname.data);
										#endif
										
										// Success
										return 0;
									}
									
									// Blocking Situation
									if(flag) return ADHOC_WOULD_BLOCK;
									
									// Timeout
									return ADHOC_TIMEOUT;
								}
							}
							
							// Broadcast Target
							else
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
										// Get Peer List
										if(sceNetAdhocctlGetPeerList(&buflen, peers) == 0 && buflen > 0)
										{
											// Log Peer List
											#ifdef DEBUG
											printk("Discovered %d Peers for PDP Broadcast\n", buflen / sizeof(SceNetAdhocctlPeerInfo));
											#endif
											
											// Iterate Peers
											SceNetAdhocctlPeerInfo * peer = peers;
											for(; peer != NULL; peer = peer->next)
											{
												// Fill in Target Structure
												SceNetInetSockaddrIn target;
												target.sin_family = AF_INET;
												target.sin_addr = peer->ip_addr;
												target.sin_port = sceNetHtons(dport);
												
												// Send Data
												int sendresult = sceNetInetSendto(socket->id, data, len, ((flag != 0) ? (INET_MSG_DONTWAIT) : (0)), (SceNetInetSockaddr *)&target, sizeof(target));
												
												// Log Result
												#ifdef DEBUG
												if(sendresult == len) printk("Successfully Sent %d Bytes to %s (Simulated Broadcast)\n", len, (char *)peer->nickname.data);
												#endif
											}
										}
										
										// Free Memory
										free(peers);
										
										// Log Broadcast
										#ifdef DEBUG
										printk("PDP Broadcast complete\n");
										#endif
										
										// Success
										return 0;
									}
									
									// Allocation Error
									return NET_NO_SPACE;
								}
								
								// Broadcast never fails!
								return 0;
							}
						}
						
						// Invalid Destination Address
						return ADHOC_INVALID_ADDR;
					}
					
					// Invalid Argument
					return ADHOC_INVALID_ARG;
				}
				
				// Invalid Socket ID
				return ADHOC_INVALID_SOCKET_ID;
			}
			
			// Invalid Data Length
			return ADHOC_INVALID_DATALEN;
		}
		
		// Invalid Destination Port
		return ADHOC_INVALID_PORT;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

/**
 * Socket Existence Check
 * @param socket To-be-checked Socket
 * @return 1 if found or... 0
 */
int _pdpSocketInList(SceNetAdhocPdpStat * socket)
{
	// Iterate PDP Socket List
	SceNetAdhocPdpStat * list = _pdp;
	for(; list != NULL; list = list->next)
	{
		// Socket found
		if(list == socket) return 1;
	}
	
	// Socket not found
	return 0;
}

/**
 * Broadcast MAC Check
 * @param addr To-be-checked MAC Address
 * @return 1 if Broadcast MAC or... 0
 */
int _isBroadcastMAC(const SceNetEtherAddr * addr)
{
	// Broadcast MAC
	if(memcmp(addr->data, "\xFF\xFF\xFF\xFF\xFF\xFF", ETHER_ADDR_LEN) == 0) return 1;
	
	// Normal MAC
	return 0;
}

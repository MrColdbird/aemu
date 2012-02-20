#include "../../common.h"

/**
 * Adhoc Emulator PDP Socket Creator
 * @param saddr Local MAC (Unused)
 * @param sport Local Binding Port
 * @param bufsize Socket Buffer Size
 * @param flag Bitflags (Unused)
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_SOCKET_ID_NOT_AVAIL, ADHOC_INVALID_ADDR, ADHOC_PORT_NOT_AVAIL, ADHOC_INVALID_PORT, ADHOC_PORT_IN_USE, NET_NO_SPACE
 */
int proNetAdhocPdpCreate(const SceNetEtherAddr * saddr, uint16_t sport, int bufsize, int flag)
{
	// Library is initialized
	if(_init)
	{
		// Valid Arguments are supplied
		if(saddr != NULL && bufsize > 0)
		{
			// Valid MAC supplied
			if(_IsLocalMAC(saddr))
			{
				// Random Port required
				if(sport == 0)
				{
					// Find unused Port
					while(sport == 0 || _IsPDPPortInUse(sport))
					{
						// Generate Port Number
						sport = (uint16_t)_getRandomNumber(65535);
					}
				}
				
				// Unused Port supplied
				if(!_IsPDPPortInUse(sport))
				{
					// Create Internet UDP Socket
					int socket = sceNetInetSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
					
					// Valid Socket produced
					if(socket > 0)
					{
						// Enable Port Re-use
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &_one, sizeof(_one));
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &_one, sizeof(_one));
						
						// Binding Information for local Port
						SceNetInetSockaddrIn addr;
						addr.sin_len = sizeof(addr);
						addr.sin_family = AF_INET;
						addr.sin_addr = INADDR_ANY;
						addr.sin_port = sceNetHtons(sport);
						
						// Bound Socket to local Port
						if(sceNetInetBind(socket, (SceNetInetSockaddr *)&addr, sizeof(addr)) == 0)
						{
							// Allocate Memory for Internal Data
							SceNetAdhocPdpStat * internal = (SceNetAdhocPdpStat *)malloc(sizeof(SceNetAdhocPdpStat));
							
							// Allocated Memory
							if(internal != NULL)
							{
								// Clear Memory
								memset(internal, 0, sizeof(SceNetAdhocPdpStat));
								
								// Fill in Data
								internal->id = socket;
								internal->laddr = *saddr;
								internal->lport = sport;
								
								// Empty Internal PDP Socket List
								if(_pdp == NULL)
								{
									// Start new PDP Socket List
									_pdp = internal;
								}
								
								
								// Append Socket to Internal PDP Socket List
								else
								{
									// List Iterator
									SceNetAdhocPdpStat * list = _pdp;
									
									// Find End of List
									while(list->next != NULL) list = list->next;
									
									// Append Socket
									list->next = internal;
								}
								
								// Success
								return (int)internal;
							}
						}
						
						// Close Socket
						sceNetInetClose(socket);
					}
					
					// Default to No-Space Error
					return NET_NO_SPACE;
				}
				
				// Port is in use by another PDP Socket
				return ADHOC_PORT_IN_USE;
			}
			
			// Invalid MAC supplied
			return ADHOC_INVALID_ADDR;
		}
		
		// Invalid Arguments were supplied
		return ADHOC_INVALID_ARG;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

/**
 * Local MAC Check
 * @param saddr To-be-checked MAC Address
 * @return 1 if valid or... 0
 */
int _IsLocalMAC(const SceNetEtherAddr * addr)
{
	// Get Local MAC Address
	SceNetEtherAddr saddr;
	sceNetGetLocalEtherAddr(&saddr);
	
	// Compare MAC Addresses
	int match = memcmp((const void *)addr, (const void *)&saddr, ETHER_ADDR_LEN);
	
	// Return Result
	return (match == 0);
}

/**
 * PDP Port Check
 * @param port To-be-checked Port
 * @return 1 if in use or... 0
 */
int _IsPDPPortInUse(uint16_t port)
{
	// Adhoc Control Metaport
	if(port == ADHOCCTL_METAPORT) return 1;
	
	// List Iterator
	SceNetAdhocPdpStat * list = _pdp;
	
	// Iterate Elements
	for(; list != NULL; list = list->next)
	{
		// Match found
		if(list->lport == port) return 1;
	}
	
	// Unused Port
	return 0;
}

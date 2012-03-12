#include "../../common.h"

/**
 * Adhoc Emulator PTP Active Socket Creator
 * @param saddr Local MAC (Unused)
 * @param sport Local Binding Port
 * @param daddr Target MAC
 * @param dport Target Port
 * @param bufsize Socket Buffer Size
 * @param rexmt_int Retransmit Interval (in Microseconds)
 * @param rexmt_cnt Retransmit Count
 * @param flag Bitflags (Unused)
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT
 */
int proNetAdhocPtpOpen(const SceNetEtherAddr * saddr, uint16_t sport, const SceNetEtherAddr * daddr, uint16_t dport, uint32_t bufsize, uint32_t rexmt_int, int rexmt_cnt, int flag)
{
	// Library is initialized
	if(_init)
	{
		// Valid Addresses
		if(saddr != NULL && _IsLocalMAC(saddr) && daddr != NULL && !_isBroadcastMAC(daddr))
		{
			// Random Port required
			if(sport == 0)
			{
				// Find unused Port
				while(sport == 0 || _IsPTPPortInUse(sport))
				{
					// Generate Port Number
					sport = (uint16_t)_getRandomNumber(65535);
				}
			}
			
			// Valid Ports
			if(!_IsPTPPortInUse(sport) && dport != 0)
			{
				// Valid Arguments
				if(bufsize > 0 && rexmt_int > 0 && rexmt_cnt > 0)
				{
					// Create Infrastructure Socket
					int socket = sceNetInetSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					
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
							// Allocate Memory
							SceNetAdhocPtpStat * internal = (SceNetAdhocPtpStat *)malloc(sizeof(SceNetAdhocPtpStat));
							
							// Allocated Memory
							if(internal != NULL)
							{
								// Clear Memory
								memset(internal, 0, sizeof(SceNetAdhocPtpStat));
								
								// Copy Infrastructure Socket ID
								internal->id = socket;
								
								// Copy Address Information
								internal->laddr = *saddr;
								internal->paddr = *daddr;
								internal->lport = sport;
								internal->pport = dport;
								
								// Set Buffer Size
								internal->rcv_sb_cc = bufsize;
								
								// Append to internal PTP Socket List
								_ptpAppendInternal(internal);
								
								// Return PTP Socket Pointer
								return (int)internal;
							}
						}
						
						// Close Socket
						sceNetInetClose(socket);
					}
				}
				
				// Invalid Arguments
				return ADHOC_INVALID_ARG;
			}
			
			// Invalid Ports
			return ADHOC_INVALID_PORT;
		}
		
		// Invalid Addresses
		return ADHOC_INVALID_ADDR;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}

/**
 * Append PTP Socket Node to Internal List
 * @param node To-be-appended Socket Node
 */
void _ptpAppendInternal(SceNetAdhocPtpStat * node)
{
	// Add at the beginning of List
	if(_ptp == NULL) _ptp = node;
	
	// Add at the End of the List
	else
	{
		// Iterator Variable
		SceNetAdhocPtpStat * list = _ptp;
		
		// Move to End of List
		while(list->next != NULL) list = list->next;
		
		// Append Socket Node
		list->next = node;
	}
}

/**
 * Check whether PTP Port is in use or not
 * @param port To-be-checked Port Number
 * @return 1 if in use or... 0
 */
int _IsPTPPortInUse(uint16_t port)
{
	// Adhoc Control Metaport
	if(port == ADHOCCTL_METAPORT) return 1;
	
	// Iterator Veriable
	SceNetAdhocPtpStat * list = _ptp;
	
	// Iterate PTP Sockets
	for(; list != NULL; list = list->next)
	{
		// Used Port
		if(list->lport == port) return 1;
	}
	
	// Unused Port
	return 0;
}

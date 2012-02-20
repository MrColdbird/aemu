#include "../common.h"

/**
 * Adhoc Emulator Socket Alert Getter
 * @param id Socket File Descriptor
 * @param flag OUT: Alert Bitflags
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID, ADHOC_INVALID_ARG
 */
int proNetAdhocGetSocketAlert(int id, int * flag)
{
	// PDP Socket Cast
	SceNetAdhocPdpStat * pdp = (SceNetAdhocPdpStat *)id;
	
	// PTP Socket Cast
	SceNetAdhocPtpStat * ptp = (SceNetAdhocPtpStat *)id;
	
	// PDP Check
	if(!_pdpSocketInList(pdp)) pdp = NULL;
	
	// PTP Check
	if(!_ptpSocketInList(ptp)) ptp = NULL;
	
	// Library is initialized
	if(_init)
	{
		// Valid Socket ID
		if(pdp || ptp)
		{
			// Valid Arguments
			if(flag != NULL)
			{
				// Misuse RX Buffer Field for Socket Alert
				if(pdp != NULL) *flag = pdp->rcv_sb_cc;
				else if(ptp != NULL) *flag = ptp->rcv_sb_cc;
				
				// Return Success
				return 0;
			}
			
			// Invalid Arguments
			return ADHOC_INVALID_ARG;
		}
		
		// Invalid Socket ID
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library uninitialized
	return ADHOC_NOT_INITIALIZED;
}

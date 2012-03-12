#include "../common.h"

/**
 * Adhoc Emulator Socket Alert Setter
 * @param id Socket File Descriptor
 * @param flag IN: Alert Bitflags
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID
 */
int proNetAdhocSetSocketAlert(int id, int flag)
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
			// Misuse RX Buffer Field for Socket Alert
			// if(pdp != NULL) pdp->rcv_sb_cc = flag;
			// else if(ptp != NULL) ptp->rcv_sb_cc = flag;
			
			// Return Success
			return 0;
		}
		
		// Invalid Socket ID
		return ADHOC_INVALID_SOCKET_ID;
	}
	
	// Library uninitialized
	return ADHOC_NOT_INITIALIZED;
}

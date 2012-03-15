#include "../common.h"

/**
 * Adhoc Emulator Socket Alert Getter
 * @param id Socket File Descriptor
 * @param flag OUT: Alert Bitflags
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID, ADHOC_INVALID_ARG
 */
int proNetAdhocGetSocketAlert(int id, int * flag)
{
	// Dummy Value
	*flag = 0;
	
	// Dummy Result
	return 0;
}

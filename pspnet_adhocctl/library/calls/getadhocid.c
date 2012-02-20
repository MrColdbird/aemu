#include "../common.h"

/**
 * Get Game Product Code
 * @param adhoc_id OUT: Game Product Code
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetAdhocId(SceNetAdhocctlAdhocId * adhoc_id)
{
	// Library initialized
	if(_init == 1)
	{
		// Valid Arguments
		if(adhoc_id != NULL)
		{
			// Copy Product ID
			*adhoc_id = _product_code;
			
			// Return Success
			return 0;
		}
		
		// Invalid Arguments
		return ADHOCCTL_INVALID_ARG;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

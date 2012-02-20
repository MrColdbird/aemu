#ifndef _ADHOCCTL_GETADHOCID_H_
#define _ADHOCCTL_GETADHOCID_H_

/**
 * Get Game Product Code
 * @param adhoc_id OUT: Game Product Code
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetAdhocId(SceNetAdhocctlAdhocId * adhoc_id);

#endif

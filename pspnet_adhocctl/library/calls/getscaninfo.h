#ifndef _ADHOCCTL_GETSCANINFO_H_
#define _ADHOCCTL_GETSCANINFO_H_

/**
 * Acquire Network Scan Result in Linked List
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf Network List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED
 */
int proNetAdhocctlGetScanInfo(int * buflen, SceNetAdhocctlScanInfo * buf);

#endif

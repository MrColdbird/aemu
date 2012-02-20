#ifndef _ADHOCCTL_ADHOCMODE_JOIN_H_
#define _ADHOCCTL_ADHOCMODE_JOIN_H_

/**
 * Join discovered Network
 * @param scan_info Discovered Network
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlJoin(const SceNetAdhocctlScanInfo * scan_info);

#endif

#ifndef _ADHOCCTL_ADHOCMODE_CREATE_H_
#define _ADHOCCTL_ADHOCMODE_CREATE_H_

/**
 * Create and / or Join a Virtual Network of the specified Name
 * @param group_name Virtual Network Name
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_BUSY
 */
int proNetAdhocctlCreate(const SceNetAdhocctlGroupName * group_name);

#endif

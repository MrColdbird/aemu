#ifndef _ADHOCCTL_NETCONF_INIT_H_
#define _ADHOCCTL_NETCONF_INIT_H_

/**
 * Connect to Adhoc Network via Kernel Utility Wrapper
 * @param param Netconf Parameter (Groupname and other Information)
 * @return 0 on success or... -1
 */
int proUtilityNetconfInitStart(SceUtilityNetconfParam * param);

#endif

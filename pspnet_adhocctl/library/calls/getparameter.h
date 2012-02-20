#ifndef _ADHOCCTL_GETPARAMETER_H_
#define _ADHOCCTL_GETPARAMETER_H_

/**
 * Get Adhoc Parameter
 * @param parameter OUT: Adhoc Parameter
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetParameter(SceNetAdhocctlParameter * parameter);

#endif

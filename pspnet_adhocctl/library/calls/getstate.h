#ifndef _ADHOCCTL_GETSTATE_H_
#define _ADHOCCTL_GETSTATE_H_

/**
 * Return Internal Thread Status
 * @param state OUT: Thread Status
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetState(int * state);

#endif

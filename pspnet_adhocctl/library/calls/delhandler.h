#ifndef _ADHOCCTL_DELHANDLER_H_
#define _ADHOCCTL_DELHANDLER_H_

/**
 * Delete registered Adhocctl Handler
 * @param id Adhocctl Handler ID
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_ID_NOT_FOUND
 */
int proNetAdhocctlDelHandler(int id);

#endif

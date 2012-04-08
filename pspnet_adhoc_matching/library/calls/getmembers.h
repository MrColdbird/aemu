#ifndef _ADHOC_MATCHING_GETMEMBERS_H_
#define _ADHOC_MATCHING_GETMEMBERS_H_

#include "../common.h"

/**
 * Get Member List
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING
 */
int proNetAdhocMatchingGetMembers(int id, int * buflen, SceNetAdhocMatchingMember * buf);

#endif


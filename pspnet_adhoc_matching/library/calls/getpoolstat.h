#ifndef _ADHOC_MATCHING_GETPOOLSTAT_H_
#define _ADHOC_MATCHING_GETPOOLSTAT_H_

#include "../common.h"

/**
 * Get Maximum Available Memory
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG
 */
int proNetAdhocMatchingGetPoolStat(SceNetMallocStat * poolstat);

#endif


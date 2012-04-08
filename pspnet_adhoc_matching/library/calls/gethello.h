#ifndef _ADHOC_MATCHING_GETHELLO_H_
#define _ADHOC_MATCHING_GETHELLO_H_

/**
 * Get Hello Data
 * @param id Matching Context ID
 * @param buflen IN: Buffer Size OUT: Required Buffer Size
 * @param buf IN: Buffer OUT: Hello Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_INVALID_MODE, ADHOC_MATCHING_NOT_RUNNING
 */
int proNetAdhocMatchingGetHelloOpt(int id, int * buflen, void * buf);

#endif


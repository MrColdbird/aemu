#ifndef _ADHOC_MATCHING_SETHELLO_H_
#define _ADHOC_MATCHING_SETHELLO_H_

/**
 * Set / Update Hello Data
 * @param id Matching Context ID
 * @param optlen Optional Data Length
 * @param opt Optional Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_INVALID_MODE, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_INVALID_OPTLEN, ADHOC_MATCHING_NO_SPACE
 */
int proNetAdhocMatchingSetHelloOpt(int id, int optlen, const void * opt);

#endif


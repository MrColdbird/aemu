#ifndef _ADHOC_MATCHING_SELECT_H_
#define _ADHOC_MATCHING_SELECT_H_

/**
 * Select / Accept Matching Target
 * @param id Matching Context ID
 * @param target Target MAC
 * @param optlen Length of Optional Data
 * @param opt Optional Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_UNKNOWN_TARGET, ADHOC_MATCHING_INVALID_OPTLEN, ADHOC_MATCHING_TARGET_NOT_READY, ADHOC_MATCHING_EXCEED_MAXNUM, ADHOC_MATCHING_NO_SPACE, ADHOC_MATCHING_REQUEST_IN_PROGRESS, ADHOC_MATCHING_ALREADY_ESTABLISHED
 */
int proNetAdhocMatchingSelectTarget(int id, const SceNetEtherAddr * target, int optlen, const void * opt);

#endif


#ifndef _ADHOC_MATCHING_ABORTSEND_H_
#define _ADHOC_MATCHING_ABORTSEND_H_

/**
 * Abort Data-Sending to Matching Target
 * @param id Matching Context ID
 * @param target Target MAC
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_UNKNOWN_TARGET
 */
int proNetAdhocMatchingAbortSendData(int id, const SceNetEtherAddr * target);

#endif


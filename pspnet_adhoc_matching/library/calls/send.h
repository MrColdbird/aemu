#ifndef _ADHOC_MATCHING_SEND_H_
#define _ADHOC_MATCHING_SEND_H_

/**
 * Send Data to Matching Target
 * @param id Matching Context ID
 * @param target Target MAC
 * @param datalen Length of Data
 * @param data Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_NOT_RUNNING, ADHOC_MATCHING_UNKNOWN_TARGET, ADHOC_MATCHING_INVALID_DATALEN, ADHOC_MATCHING_NOT_ESTABLISHED, ADHOC_MATCHING_NO_SPACE, ADHOC_MATCHING_DATA_BUSY
 */
int proNetAdhocMatchingSendData(int id, const SceNetEtherAddr * target, int datalen, const void * data);

#endif


#ifndef _ADHOC_MATCHING_DELETE_H_
#define _ADHOC_MATCHING_DELETE_H_

/**
 * Delete Matching Context
 * @param id Matching Context ID
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_IS_RUNNING
 */
int proNetAdhocMatchingDelete(int id);

#endif


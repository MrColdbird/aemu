#ifndef _ADHOC_MATCHING_START_H_
#define _ADHOC_MATCHING_START_H_

/**
 * Start Matching Context
 * @param id Matching Context ID
 * @param event_th_prio Event Thread Priority
 * @param event_th_stack Event Thread Stack
 * @param input_th_prio IO Thread Priority
 * @param input_th_stack IO Thread Stack
 * @param hello_optlen Hello Data Length
 * @param hello_opt Hello Data
 * @return 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_STACKSIZE_TOO_SHORT, ADHOC_MATCHING_INVALID_ID, ADHOC_MATCHING_IS_RUNNING, ADHOC_MATCHING_INVALID_OPTLEN, ADHOC_MATCHING_NO_SPACE
 */
int proNetAdhocMatchingStart(int id, int event_th_prio, int event_th_stack, int input_th_prio, int input_th_stack, int hello_optlen, const void * hello_opt);

#endif


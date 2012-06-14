/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

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


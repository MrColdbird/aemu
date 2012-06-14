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


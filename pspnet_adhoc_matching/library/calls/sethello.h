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


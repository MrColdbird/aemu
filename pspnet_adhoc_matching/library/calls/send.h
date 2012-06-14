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


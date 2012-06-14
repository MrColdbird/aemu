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

#ifndef _ADHOC_MATCHING_INIT_H_
#define _ADHOC_MATCHING_INIT_H_

/**
 * Initialize the Adhoc Matching Emulator
 * @param poolsize Internal Heap Size
 * @return 0 on success or... ADHOC_MATCHING_ALREADY_INITIALIZED
 */
int proNetAdhocMatchingInit(uint32_t poolsize);

#endif


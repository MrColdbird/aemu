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

#ifndef _ADHOCCTL_INIT_H_
#define _ADHOCCTL_INIT_H_

/**
 * Initialize the Adhoc-Control Emulator
 * @param stacksize Stacksize of the Internal Thread
 * @param prio Internal Thread Priority
 * @param adhoc_id Game Product Code
 * @return 0 on success or... ADHOCCTL_ALREADY_INITIALIZED, ADHOCCTL_STACKSIZE_TOO_SHORT, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlInit(int stacksize, int prio, const SceNetAdhocctlAdhocId * adhoc_id);

#endif

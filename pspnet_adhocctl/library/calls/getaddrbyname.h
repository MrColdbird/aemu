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

#ifndef _ADHOCCTL_GETADDRBYNAME_H_
#define _ADHOCCTL_GETADDRBYNAME_H_

/**
 * Resolve Player Name to Linked List of Peers
 * @param nickname To-be-resolved Player Nickname
 * @param buflen IN: Buffer Length in Bytes OUT: Required / Used Buffer Length in Bytes
 * @param buf OUT: Resolved Peer Information (if NULL then buflen gets filled with required length)
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlGetAddrByName(const SceNetAdhocctlNickname * nickname, int * buflen, SceNetAdhocctlPeerInfo * buf);

#endif

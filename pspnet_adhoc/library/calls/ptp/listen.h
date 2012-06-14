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

#ifndef _PTP_LISTEN_H_
#define _PTP_LISTEN_H_

/**
 * Adhoc Emulator PTP Passive Socket Creator
 * @param saddr Local MAC (Unused)
 * @param sport Local Binding Port
 * @param bufsize Socket Buffer Size
 * @param rexmt_int Retransmit Interval (in Microseconds)
 * @param rexmt_cnt Retransmit Count
 * @param backlog Size of Connection Queue
 * @param flag Bitflags (Unused)
 * @return Socket ID > 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT, ADHOC_SOCKET_ID_NOT_AVAIL, ADHOC_PORT_NOT_AVAIL, ADHOC_PORT_IN_USE, NET_NO_SPACE
 */
int proNetAdhocPtpListen(const SceNetEtherAddr * saddr, uint16_t sport, uint32_t bufsize, uint32_t rexmt_int, int rexmt_cnt, int backlog, int flag);

#endif

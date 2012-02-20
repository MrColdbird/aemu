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
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT, ADHOC_SOCKET_ID_NOT_AVAIL, ADHOC_PORT_NOT_AVAIL, ADHOC_PORT_IN_USE, NET_NO_SPACE
 */
int proNetAdhocPtpListen(const SceNetEtherAddr * saddr, uint16_t sport, uint32_t bufsize, uint32_t rexmt_int, int rexmt_cnt, int backlog, int flag);

#endif

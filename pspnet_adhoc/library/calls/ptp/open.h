#ifndef _PTP_OPEN_H_
#define _PTP_OPEN_H_

/**
 * Adhoc Emulator PTP Active Socket Creator
 * @param saddr Local MAC (Unused)
 * @param sport Local Binding Port
 * @param daddr Target MAC
 * @param dport Target Port
 * @param bufsize Socket Buffer Size
 * @param rexmt_int Retransmit Interval (in Microseconds)
 * @param rexmt_cnt Retransmit Count
 * @param flag Bitflags (Unused)
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT
 */
int proNetAdhocPtpOpen(const SceNetEtherAddr * saddr, uint16_t sport, const SceNetEtherAddr * daddr, uint16_t dport, uint32_t bufsize, uint32_t rexmt_int, int rexmt_cnt, int flag);

#endif

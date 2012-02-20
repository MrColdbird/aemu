#ifndef _PTP_GETSTAT_H_
#define _PTP_GETSTAT_H_

/**
 * Adhoc Emulator PTP Socket List Getter
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf PTP Socket List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED
 */
int proNetAdhocGetPtpStat(int * buflen, SceNetAdhocPtpStat * buf);

#endif

#ifndef _PDP_GETSTAT_H_
#define _PDP_GETSTAT_H_

/**
 * Adhoc Emulator PDP Socket List Getter
 * @param buflen IN: Length of Buffer in Bytes OUT: Required Length of Buffer in Bytes
 * @param buf PDP Socket List Buffer (can be NULL if you wish to receive Required Length)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED
 */
int proNetAdhocGetPdpStat(int * buflen, SceNetAdhocPdpStat * buf);

#endif

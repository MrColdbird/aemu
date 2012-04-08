#ifndef _ADHOC_MATCHING_CREATE_H_
#define _ADHOC_MATCHING_CREATE_H_

/**
 * Create Matching Context
 * @param mode Operating Mode (HOST, CLIENT, P2P)
 * @param maxnum Member Limit
 * @param port PDP Port
 * @param rxbuflen Receive Buffer Size
 * @param hello_int Hello Interval (Microseconds)
 * @param keepalive_int Keep-Alive Interval (Microseconds)
 * @param init_count Resend & Keep-Alive Counter Initial Value
 * @param rexmt_int Message Resend Interval (Microseconds)
 * @return ID > 0 on success or... ADHOC_MATCHING_NOT_INITIALIZED, ADHOC_MATCHING_INVALID_MAXNUM, ADHOC_MATCHING_RXBUF_TOO_SHORT, ADHOC_MATCHING_INVALID_ARG, ADHOC_MATCHING_NO_SPACE, ADHOC_MATCHING_ID_NOT_AVAIL, ADHOC_MATCHING_PORT_IN_USE
 */
int proNetAdhocMatchingCreate(int mode, int maxnum, uint16_t port, int rxbuflen, uint32_t hello_int, uint32_t keepalive_int, int init_count, uint32_t rexmt_int, SceNetAdhocMatchingHandler handler);

#endif

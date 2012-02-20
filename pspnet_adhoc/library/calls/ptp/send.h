#ifndef _PTP_SEND_H_
#define _PTP_SEND_H_

/**
 * Adhoc Emulator PTP Sender
 * @param id Socket File Descriptor
 * @param data Data Payload
 * @param len IN: Length of Payload OUT: Sent Data (in Bytes)
 * @param timeout Send Timeout (in Microseconds)
 * @param flag Nonblocking Flag
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_SOCKET_ID, ADHOC_SOCKET_DELETED, ADHOC_SOCKET_ALERTED, ADHOC_WOULD_BLOCK, ADHOC_TIMEOUT, ADHOC_NOT_CONNECTED, ADHOC_THREAD_ABORTED, ADHOC_INVALID_DATALEN, ADHOC_DISCONNECTED, NET_INTERNAL, NET_NO_SPACE
 */
int proNetAdhocPtpSend(int id, const void * data, int * len, uint32_t timeout, int flag);

#endif

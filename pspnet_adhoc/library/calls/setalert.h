#ifndef _SETALERT_H_
#define _SETALERT_H_

/**
 * Adhoc Emulator Socket Alert Setter
 * @param id Socket File Descriptor
 * @param flag IN: Alert Bitflags
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID
 */
int proNetAdhocSetSocketAlert(int id, int flag);

#endif

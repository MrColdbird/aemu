#ifndef _PDP_DELETE_H_
#define _PDP_DELETE_H_

/**
 * Adhoc Emulator PDP Socket Delete
 * @param id Socket File Descriptor
 * @param flag Bitflags (Unused)
 * @return 0 on success or... ADHOC_INVALID_ARG, ADHOC_NOT_INITIALIZED, ADHOC_INVALID_SOCKET_ID
 */
int proNetAdhocPdpDelete(int id, int flag);

#endif

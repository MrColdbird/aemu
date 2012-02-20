#ifndef _ADHOCCTL_FUNCTIONS_H_
#define _ADHOCCTL_FUNCTIONS_H_

/**
 * Read Line from File
 * @param fd File Descriptor to read line from
 * @param buffer Buffer to read line into
 * @param buflen Length of Buffer in Bytes
 * @return Length of Line or... 0
 */
uint32_t _readLine(int fd, char * buffer, uint32_t buflen);

/**
 * Patch utility.prx to allow Kernel Control of Adhoc
 * @param init Replacement for NetconfInit
 * @param getstatus Replacement for NetconfGetStatus
 * @param update Replacement for NetconfUpdate
 * @param shutdown Replacement for NetconfShutdown
 */
void patch_netconf_utility(void * init, void * getstatus, void * update, void * shutdown);

/**
 * Lock Peerlist
 */
void _acquirePeerLock(void);

/**
 * Unlock Peerlist
 */
void _freePeerLock(void);

#endif

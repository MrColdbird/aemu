#ifndef _ADHOC_FUNCTIONS_H_
#define _ADHOC_FUNCTIONS_H_

/**
 * Mersenne Twister Number Generator
 * @param max Maximum Range of Number
 * @return Random unsigned 32bit Number
 */
uint32_t _getRandomNumber(uint32_t max);

/**
 * Closes & Deletes all PDP Sockets
 */
void _deleteAllPDP(void);

/**
 * Closes & Deletes all PTP Sockets
 */
void _deleteAllPTP(void);

/**
 * Deletes all Gamemode Buffer
 */
void _deleteAllGMB(void);

/**
 * Local MAC Check
 * @param saddr To-be-checked MAC Address
 * @return 1 if valid or... 0
 */
int _IsLocalMAC(const SceNetEtherAddr * addr);

/**
 * PDP Port Check
 * @param port To-be-checked Port
 * @return 1 if in use or... 0
 */
int _IsPDPPortInUse(uint16_t port);

/**
 * PDP Socket Counter
 * @return Number of internal PDP Sockets
 */
int _getPDPSocketCount(void);

/**
 * Broadcast MAC Check
 * @param addr To-be-checked MAC Address
 * @return 1 if Broadcast MAC or... 0
 */
int _isBroadcastMAC(const SceNetEtherAddr * addr);

/**
 * PTP Socket Counter
 * @return Number of internal PTP Sockets
 */
int _getPTPSocketCount(void);

/**
 * Check whether PTP Port is in use or not
 * @param port To-be-checked Port Number
 * @return 1 if in use or... 0
 */
int _IsPTPPortInUse(uint16_t port);

/**
 * Add Port Forward to Router
 * @param protocol "TCP" or "UDP"
 * @param port To-be-forwarded Port Number
 */
void sceNetPortOpen(const char * protocol, uint16_t port);

/**
 * Remove Port Forward from Router
 * @param protocol "TCP" or "UDP"
 * @param port To-be-removed Port Number
 */
void sceNetPortClose(const char * protocol, uint16_t port);

#endif

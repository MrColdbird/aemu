#ifndef _GAMEMODE_UPDATEREPLICA_H_
#define _GAMEMODE_UPDATEREPLICA_H_

/**
 * Adhoc Emulator Gamemode Peer Replica Buffer Updater
 * @param id Replica ID
 * @param opt OUT: Optional Replica Information (NULL can be passed)
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_NOT_IN_GAMEMODE, ADHOC_ALREADY_CREATED, WLAN_INVALID_ARG
 */
int proNetAdhocGameModeUpdateReplica(int id, SceNetAdhocGameModeOptData * opt);

#endif

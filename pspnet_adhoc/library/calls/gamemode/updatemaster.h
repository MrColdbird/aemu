#ifndef _GAMEMODE_UPDATEMASTER_H_
#define _GAMEMODE_UPDATEMASTER_H_

/**
 * Adhoc Emulator Gamemode Master Buffer Updater
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_DATALEN, ADHOC_NOT_IN_GAMEMODE, ADHOC_ALREADY_CREATED, NET_NO_SPACE
 */
int proNetAdhocGameModeUpdateMaster(void);

#endif

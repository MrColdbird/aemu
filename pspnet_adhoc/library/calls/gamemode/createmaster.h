#ifndef _GAMEMODE_CREATEMASTER_H_
#define _GAMEMODE_CREATEMASTER_H_

/**
 * Adhoc Emulator Gamemode Master Buffer Creator
 * @param ptr Buffer Address
 * @param size Buffer Size (in Bytes)
 * @return 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_DATALEN, ADHOC_NOT_IN_GAMEMODE, ADHOC_ALREADY_CREATED, NET_NO_SPACE
 */
int proNetAdhocGameModeCreateMaster(const void * ptr, uint32_t size);

#endif

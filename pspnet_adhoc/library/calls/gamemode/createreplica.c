#include "../../common.h"

/**
 * Adhoc Emulator Gamemode Peer Replica Buffer Creator
 * @param src Peer MAC Address
 * @param ptr Buffer Address
 * @param size Buffer Size (in Bytes)
 * @return Replica ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_DATALEN, ADHOC_NOT_IN_GAMEMODE, ADHOC_ALREADY_CREATED, NET_NO_SPACE
 */
int proNetAdhocGameModeCreateReplica(const SceNetEtherAddr * src, void * ptr, uint32_t size)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

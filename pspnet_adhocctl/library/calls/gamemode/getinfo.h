#ifndef _ADHOCCTL_GAMEMODE_GETINFO_H_
#define _ADHOCCTL_GAMEMODE_GETINFO_H_

/**
 * Returns GameMode Specific Information
 * @param info OUT: GameMode Information
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_NOT_ENTER_GAMEMODE
 */
int proNetAdhocctlGetGameModeInfo(SceNetAdhocctlGameModeInfo * info);

#endif

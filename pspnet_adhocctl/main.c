#include <pspsdk.h>
#include <pspkernel.h>
#include "library/common.h"

#define MODULENAME "sceNetAdhocctl_Library"
PSP_MODULE_INFO(MODULENAME, PSP_MODULE_USER + 6, 1, 6);
PSP_HEAP_SIZE_KB(100);

// Stubs
int sceNetAdhocctlInit(int stacksize, int prio, const SceNetAdhocctlAdhocId * adhoc_id)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlInit(stacksize, prio, adhoc_id);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlJoin(const SceNetAdhocctlScanInfo * scan_info)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlJoin(scan_info);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetPeerList(int * buflen, SceNetAdhocctlPeerInfo * buf)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetPeerList(buflen, buf);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetPeerInfo(SceNetEtherAddr * addr, int size, SceNetAdhocctlPeerInfo * peer_info)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetPeerInfo(addr, size, peer_info);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetAddrByName(const SceNetAdhocctlNickname * nickname, int * buflen, SceNetAdhocctlPeerInfo * buf)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetAddrByName(nickname, buflen, buf);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlTerm(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlTerm();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlConnect(const SceNetAdhocctlGroupName * group_name)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlConnect(group_name);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlCreate(const SceNetAdhocctlGroupName * group_name)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlConnect(group_name);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlCreateEnterGameMode(const SceNetAdhocctlGroupName * group_name, int game_type, int num, const SceNetEtherAddr * members, uint32_t timeout, int flag)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlCreateEnterGameMode(group_name, game_type, num, members, timeout, flag);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlCreateEnterGameModeMin(const SceNetAdhocctlGroupName * group_name, int game_type, int min_members, int num_members, const struct SceNetEtherAddr * members, uint32_t timeout, int flag) // B0B80E80
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlCreateEnterGameModeMin(group_name, game_type, min_members, num_members, members, timeout, flag);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlJoinEnterGameMode(const SceNetAdhocctlGroupName * group_name, const SceNetEtherAddr * gc, uint32_t timeout, int flag)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlJoinEnterGameMode(group_name, gc, timeout, flag);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlScan(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlScan();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlDisconnect(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlDisconnect();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlExitGameMode(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlExitGameMode();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlAddHandler(SceNetAdhocctlHandler handler, void * arg)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlAddHandler(handler, arg);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlDelHandler(int id)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlDelHandler(id);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetState(int * state)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetState(state);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetAdhocId(SceNetAdhocctlAdhocId * adhoc_id)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetAdhocId(adhoc_id);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetNameByAddr(const SceNetEtherAddr * addr, SceNetAdhocctlNickname * nickname)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetNameByAddr(addr, nickname);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetParameter(SceNetAdhocctlParameter * parameter)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetParameter(parameter);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetScanInfo(int * buflen, SceNetAdhocctlScanInfo * buf)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetScanInfo(buflen, buf);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

int sceNetAdhocctlGetGameModeInfo(SceNetAdhocctlGameModeInfo * info)
{
	#ifdef DEBUG
	printk("Entering %s\n", __func__);
	#endif
	int result = proNetAdhocctlGetGameModeInfo(info);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", __func__, result);
	#endif
	return result;
}

// Kernel Permission Stubs (Handle with extreme care!)
int sceUtilityNetconfInitStartKernel(SceUtilityNetconfParam * param)
{
	#ifdef DEBUG
	printk("Entering %s\n", "sceUtilityNetconfInitStart");
	#endif
	int result = proUtilityNetconfInitStart(param);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", "sceUtilityNetconfInitStart", result);
	#endif
	return result;
}

int sceUtilityNetconfGetStatusKernel(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", "sceUtilityNetconfGetStatus");
	#endif
	int result = proUtilityNetconfGetStatus();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", "sceUtilityNetconfGetStatus", result);
	#endif
	return result;
}

int sceUtilityNetconfUpdateKernel(int speed)
{
	#ifdef DEBUG
	printk("Entering %s\n", "sceUtilityNetconfUpdate");
	#endif
	int result = proUtilityNetconfUpdate(speed);
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", "sceUtilityNetconfUpdate", result);
	#endif
	return result;
}

int sceUtilityNetconfShutdownStartKernel(void)
{
	#ifdef DEBUG
	printk("Entering %s\n", "sceUtilityNetconfShutdownStart");
	#endif
	int result = proUtilityNetconfShutdownStart();
	#ifdef DEBUG
	printk("Leaving %s with %08X\n", "sceUtilityNetconfShutdownStart", result);
	#endif
	return result;
}

// Module Start Event
int module_start(SceSize args, void * argp)
{
	printk(MODULENAME " start!\n");
	patch_netconf_utility(sceUtilityNetconfInitStartKernel, sceUtilityNetconfGetStatusKernel, sceUtilityNetconfUpdateKernel, sceUtilityNetconfShutdownStartKernel);
	return 0;
}

// Module Stop Event
int module_stop(SceSize args, void * argp)
{
	printk(MODULENAME " stop!\n");
	return 0;
}

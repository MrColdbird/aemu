#include <pspsdk.h>
#include <pspkernel.h>
#include <string.h>
#include "../atpro/logs.h"

#define MODULENAME "sceNetAdhocMatching_Library"
PSP_MODULE_INFO(MODULENAME, PSP_MODULE_USER + 6, 1, 6);

// Stubs
int sceNetAdhocMatchingInit(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingTerm(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingCreate(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingStart(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingStop(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingDelete(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingSelectTarget(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingCancelTarget(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingCancelTargetWithOpt(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingSendData(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingAbortSendData(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingSetHelloOpt(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingGetHelloOpt(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingGetMembers(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingGetPoolMaxAlloc(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocMatchingGetPoolStat(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

// Module Start Event
int module_start(SceSize args, void * argp)
{
	printk(MODULENAME " start!\n");
	return 0;
}

// Module Stop Event
int module_stop(SceSize args, void * argp)
{
	printk(MODULENAME " stop!\n");
	return 0;
}

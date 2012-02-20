#include <pspsdk.h>
#include <pspkernel.h>
#include <string.h>
#include "../atpro/logs.h"

#define MODULENAME "sceNetAdhocDiscover_Library"
PSP_MODULE_INFO(MODULENAME, PSP_MODULE_USER + 6, 1, 2);

// Stubs
int sceNetAdhocDiscoverInitStart(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocDiscoverStop(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocDiscoverTerm(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocDiscoverUpdate(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocDiscoverGetStatus(void)
{
	THROW_UNIMPLEMENTED(__func__);
	return 0;
}

int sceNetAdhocDiscoverRequestSuspend(void)
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

#include <pspsdk.h>
#include <pspkernel.h>
#include <string.h>
#include "../atpro/logs.h"

#define MODULENAME "sceNetAdhocDownload_Library"
PSP_MODULE_INFO(MODULENAME, PSP_MODULE_USER + 6, 1, 4);

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

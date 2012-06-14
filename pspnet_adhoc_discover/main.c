/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

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

	.set noreorder

#include "pspimport.s"

	IMPORT_START "sceNetAdhocctlIOLock",0x00090011
	IMPORT_FUNC  "sceNetAdhocctlIOLock",0x95284930,_acquirePeerLock
	IMPORT_FUNC  "sceNetAdhocctlIOLock",0x5F37763B,_freePeerLock
	IMPORT_FUNC  "sceNetAdhocctlIOLock",0xE05238E7,_acquireNetworkLock
	IMPORT_FUNC  "sceNetAdhocctlIOLock",0x6A3A3DEF,_freeNetworkLock

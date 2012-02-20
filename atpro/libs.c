#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsysmem_kernel.h>
#include <psputilsforkernel.h>
#include <pspmoduleexport.h>
#include <psploadcore.h>
#include <pspsdk.h>
#include <stdio.h>
#include <string.h>

#include "libs.h"

PspModuleImport * find_import_lib(SceModule *pMod, char *library)
{
	PspModuleImport *pImp;
	void *stubTab;
	int stubLen;
	int i = 0;

	if(pMod == NULL)
		return NULL;

	stubTab = pMod->stub_top;
	stubLen = pMod->stub_size;
	while(i<stubLen) {
		pImp = (PspModuleImport*)(stubTab+i);
		if((pImp->name) && (strcmp(pImp->name, library) == 0))
			return pImp;
		i += (pImp->entLen * 4);
	}

	return NULL;
}


unsigned int find_import_bynid(SceModule *pMod, char *library, unsigned int nid)
{
	PspModuleImport *pImp;
	int i;

	pImp = find_import_lib(pMod, library);
	if(pImp) {
		for(i=0; i<pImp->funcCount; i++) {
			if(pImp->fnids[i] == nid)
				return (unsigned int) &pImp->funcs[i*2];
		}
	}

	return 0;
}

/**
 * Remember you have to export the hooker function if using syscall hook
 */
int hook_import_bynid(SceModule *pMod, char *library, unsigned int nid, void *func)
{
	PspModuleImport *pImp;
	void *stubTab;
	int stubLen;
	int i = 0;

	if(pMod == NULL)
		return -1;

	stubTab = pMod->stub_top;
	stubLen = pMod->stub_size;

	while(i<stubLen) {
		pImp = (PspModuleImport*)(stubTab+i);

		if((pImp->name) && (strcmp(pImp->name, library) == 0)) {
			int j;

			for(j=0; j<pImp->funcCount; j++) {
				if(pImp->fnids[j] == nid) {
					void *addr = (void*)(&pImp->funcs[j*2]);

					REDIRECT_FUNCTION(func, addr);

					sceKernelDcacheWritebackInvalidateRange(addr, 8);
					sceKernelIcacheInvalidateRange(addr, 8);
				}
			}
		}

		i += (pImp->entLen * 4);
	}

	return 0;
}

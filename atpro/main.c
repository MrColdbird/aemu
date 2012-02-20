#include <pspsdk.h>
#include <pspkernel.h>
#include <pspinit.h>
#include <pspdisplay.h>
#include <psploadcore.h>
#include <psputilsforkernel.h>
#include <pspsysmem_kernel.h>
#include <string.h>
#include "libs.h"
#include "logs.h"
#include "systemctrl.h"

PSP_MODULE_INFO("ATPRO", PSP_MODULE_KERNEL, 1, 0);

// Adhoc Module Names
#define MODULE_LIST_SIZE 4
char * module_names[MODULE_LIST_SIZE] = {
	"memab.prx",
	"pspnet_adhoc_auth.prx",
	"pspnet_adhoc.prx",
	"pspnet_adhocctl.prx",
//	"pspnet_adhoc_matching.prx",
//	"pspnet_adhoc_download.prx",
//	"pspnet_adhoc_discover.prx",
};

// Kernel Module Loader
SceUID load_plugin(const char * path, int flags, SceKernelLMOption * option)
{
	// Replace Adhoc Modules
	int i = 0; for(; i < MODULE_LIST_SIZE; i++) {
		// Matching Modulename
		if(strstr(path, module_names[i]) != NULL) {
			// Replace Modulename
			strcpy((char*)path, "ms0:/kd/");
			strcpy((char*)path + strlen(path), module_names[i]);
			
			// Load Module
			SceUID result = sceKernelLoadModule(path, flags, option);
			
			// Log Hotswapping
			printk("Swapping %s, UID=0x%08X\n", module_names[i], result);
			
			// Return Module UID
			return result;
		}
	}

	// Default Action - Load Module
	return sceKernelLoadModule(path, flags, option);
}

// User Module Loader
SceUID load_plugin_alt(const char * path, int unk1, int unk2, int flags, SceKernelLMOption * option)
{
	// Thanks to CFW we can load whatever we want...
	// No need to have different loader for user & kernel modules.
	return load_plugin(path, flags, option);
}

// Callback Deny Function
int cbdeny(int cbid)
{
	// By doing nothing, we prevent exit callback registration, thus block the home menu.
	// As our code requires high memory to work, exiting the game would crash anyway...
	return 0;
}

// Patcher to allow Utility-Made Connections
void patch_netconf_utility(void * init, void * getstatus, void * update, void * shutdown)
{
	// Find Utility Manager
	SceModule2 * utility = (SceModule2 *)sceKernelFindModuleByName("sceUtility_Driver");
	printk("sceUtility_Driver Scan: %08X\n", (u32)utility);
	
	// Found Utility Manager
	if(utility != NULL)
	{
		// Export Position
		int pos = 0; while(pos < utility->ent_size)
		{
			// Cast Library
			SceLibraryEntryTable * lib = (SceLibraryEntryTable *)(utility->ent_top + pos);
			
			// NID Table
			uint32_t * nids = (uint32_t *)lib->entrytable;
			
			// Address Table
			uint32_t * addresses = &nids[lib->stubcount + lib->vstubcount];
			
			// Utility Export Library found
			if(strcmp(lib->libname, "sceUtility") == 0)
			{
				// Log Discovery
				printk("Discovered Utility Export Table\n");
				
				// Iterate NIDS
				int i = 0; for(; i < lib->stubcount; i++)
				{
					// Init found
					if(nids[i] == 0x4DB1E739)
					{
						// Patch Init
						_sw(MAKE_JUMP(init), (uint32_t)addresses[i]);
						_sw(NOP, (uint32_t)(addresses[i] + 4));
						
						// Log Patch
						printk("Patched sceUtilityNetconfInitStart\n");
					}
					
					// GetStatus found
					else if(nids[i] == 0x6332AA39)
					{
						// Patch GetStatus
						_sw(MAKE_JUMP(getstatus), (uint32_t)addresses[i]);
						_sw(NOP, (uint32_t)(addresses[i] + 4));
						
						// Log Patch
						printk("Patched sceUtilityNetconfGetStatus\n");
					}
					
					// Update found
					else if(nids[i] == 0x91E70E35)
					{
						// Patch Update
						_sw(MAKE_JUMP(update), (uint32_t)addresses[i]);
						_sw(NOP, (uint32_t)(addresses[i] + 4));
						
						// Log Patch
						printk("Patched sceUtilityNetconfUpdate\n");
					}
					
					// Shutdown found
					else if(nids[i] == 0xF88155F6)
					{
						// Patch Shutdown
						_sw(MAKE_JUMP(shutdown), (uint32_t)addresses[i]);
						_sw(NOP, (uint32_t)(addresses[i] + 4));
						
						// Log Patch
						printk("Patched sceUtilityNetconfShutdown\n");
					}
				}
				
				// Delete Memory Cache
				sceKernelIcacheInvalidateAll();
				sceKernelDcacheWritebackInvalidateAll();
				
				// Stop Search
				break;
			}
			
			// Move Position
			pos += lib->len * 4;
		}
	}
}

// Module Start Event
int module_start(SceSize args, void * argp)
{
	// Result
	int result = 0;

	// Initialize Logfile
	printk_init("ms0:/atpro.log");

	// Alive Message
	printk("ATPRO - ALPHA VERSION %s %s\n", __DATE__, __TIME__);

	// Game Mode
	if(sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_GAME) {
		// Find Utility Manager
		SceModule * utility = sceKernelFindModuleByName("sceUtility_Driver");
		printk("sceUtility_Driver Scan: %08X\n", (u32)utility);
		if(utility != NULL) {
			// 6.20 NIDs
			#ifdef CONFIG_620
			u32 nid[2] = { 0xE3CCC6EA, 0x07290699 };
			#endif

			// 6.35 NIDs
			#ifdef CONFIG_63X
			u32 nid[2] = { 0xFFB9B760, 0xAFBC3911 };
			#endif

			// 6.60 NIDs
			#ifdef CONFIG_660
			u32 nid[2] = { 0x939E4270, 0xD4EE2D26 };
			#endif

			// Patch Utility Manager Imports
			result = hook_import_bynid(utility, "ModuleMgrForKernel", nid[0], load_plugin);
			printk("Kernel Loader Hook: %d\n", result);
			if(result == 0) {
				result = hook_import_bynid(utility, "ModuleMgrForKernel", nid[1], load_plugin_alt);
				printk("User Loader Hook: %d\n", result);
				if(result == 0) {
					// Disable Home Menu
					sctrlHENPatchSyscall((void*)sctrlHENFindFunction("sceLoadExec", "LoadExecForUser", 0x4AC57943), cbdeny);
					printk("Disabled Home Menu!\n");
					
					// Setup Success
					return 0;
				}
			}
		}
	}

	// Setup Failure
	return 1;
}

// Module Stop Event
int module_stop(SceSize args, void * argp)
{
	// Return Success
	return 0;
}

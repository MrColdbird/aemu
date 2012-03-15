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

// System Control Module Patcher
STMOD_HANDLER sysctrl_patcher = NULL;

// Adhoc Module Names
#define MODULE_LIST_SIZE 7
char * module_names[MODULE_LIST_SIZE] = {
	"memab.prx",
	"pspnet_adhoc_auth.prx",
	"pspnet_adhoc.prx",
	"pspnet_adhocctl.prx",
	"pspnet_adhoc_matching.prx",
	"pspnet_adhoc_download.prx",
	"pspnet_adhoc_discover.prx",
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
	// Module ID Buffer
	static int id[100];
	
	// Number of Modules
	int idcount = 0;
	
	// Find Module IDs
	uint32_t k1 = pspSdkSetK1(0);
	int result = sceKernelGetModuleIdList(id, sizeof(id), &idcount);
	pspSdkSetK1(k1);
	
	// Found Module IDs
	if(result == 0)
	{
		// Iterate Modules
		int i = 0; for(; i < idcount; i++)
		{
			// Find Module
			SceModule2 * module = (SceModule2 *)sceKernelFindModuleByUID(id[i]);
			
			// Found Userspace Module
			if(module != NULL && (module->text_addr & 0x80000000) == 0)
			{
				// Hook Imports
				hook_import_bynid((SceModule *)module, "sceUtility", 0x4DB1E739, init);
				hook_import_bynid((SceModule *)module, "sceUtility", 0x6332AA39, getstatus);
				hook_import_bynid((SceModule *)module, "sceUtility", 0x91E70E35, update);
				hook_import_bynid((SceModule *)module, "sceUtility", 0xF88155F6, shutdown);
				
				// Fix PSPNET Termination Bug
				// This bug is caused because of utility.prx screwing around where it shouldn't.
				// This in turn happens because we use adhoc and infrastructure at the same time.
				if(strcmp(module->modname, "sceNet_Library") == 0)
				{
					// Patch Address
					uint32_t addr[2] = { 0x249C, 0x258C };
					
					// Iterate Addresses
					int j = 0; for(; j < 2; j++)
					{
						// Calculate Real Address
						uint32_t realaddr = module->text_addr + addr[j];
						
						// Dummy Instruction
						_sw(NOP, realaddr);
						
						// Flush Cache
						sceKernelDcacheWritebackInvalidateRange((void *)realaddr, 4);
						sceKernelIcacheInvalidateRange((void *)realaddr, 4);
					}
				}
			}
		}
	}
}

// Online Module Start Patcher
int online_patcher(SceModule2 * module)
{
	// Userspace Module
	if((module->text_addr & 0x80000000) == 0)
	{
		// TODO Future Game Specific Compatiblity Patches
	}
	
	// Enable System Control Patching
	return sysctrl_patcher(module);
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
					
					// Enable Module Start Patching
					sysctrl_patcher = sctrlHENSetStartModuleHandler(online_patcher);
					printk("Enabled Game-Specific Fixes!\n");
					
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

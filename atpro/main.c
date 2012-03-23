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

// Game Code Getter (discovered in utility.prx)
const char * SysMemGameCodeGetter(void);

// System Control Module Patcher
STMOD_HANDLER sysctrl_patcher = NULL;

// Adhoc Module Names
#define MODULE_LIST_SIZE 4
char * module_names[MODULE_LIST_SIZE] = {
	"memab.prx",
	"pspnet_adhoc_auth.prx",
	"pspnet_adhoc.prx",
	"pspnet_adhocctl.prx",
//	"pspnet_adhoc_matching.prx",
//	"pspnet_adhoc_download.prx",
//	"pspnet_adhoc_discover.prx"
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

// Game Code Getter
const char * getGameCode(void)
{
	// 620 SysMemForKernel_AB5E85E5
	// 63X SysMemForKernel_3C4C5630
	// 660 SysMemForKernel_EF29061C
	return SysMemGameCodeGetter() + 0x44;
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
		// Might be Untold Legends - Brotherhood of the Blade...
		if(strcmp(module->modname, "etest") == 0)
		{
			// Offsets
			uint32_t loader = 0;
			uint32_t unloader = 0;
			
			// European Version
			if(strcmp(getGameCode(), "ULES00046") == 0)
			{
				// Fill in Offsets
				loader = 0x73F40;
				unloader = 0x74334;
			}
			
			// US Version
			else if(strcmp(getGameCode(), "ULUS10003") == 0)
			{
				// Fill in Offsets
				loader = 0x6EB24;
				unloader = 0x6EF18;
			}
			
			// Valid Game Version
			if(loader != 0 && unloader != 0)
			{
				// Calculate Offsets
				loader += module->text_addr;
				unloader += module->text_addr;
				
				// Syscall Numbers
				uint32_t loadutility = sctrlKernelQuerySystemCall((void *)sctrlHENFindFunction("sceUtility_Driver", "sceUtility", 0x2A2B3DE0));
				uint32_t unloadutility = sctrlKernelQuerySystemCall((void *)sctrlHENFindFunction("sceUtility_Driver", "sceUtility", 0xE49BFE92));
				
				// Fix Module Loader
				// C-Summary:
				// sceUtilityLoadModule(PSP_MODULE_NET_COMMON);
				// sceUtilityLoadModule(PSP_MODULE_NET_ADHOC);
				// return;
				
				_sw(0x24040100, loader); // li $a0, 0x100 (arg1 = PSP_MODULE_NET_COMMON)
				_sw(MAKE_SYSCALL(loadutility), loader + 4); // sceUtilityLoadModule(PSP_MODULE_NET_COMMON);
				_sw(0x24040101, loader + 8); // li $a0, 0x101 (arg1 = PSP_MODULE_NET_ADHOC)
				_sw(0x03E00008, loader + 12); // jr $ra
				_sw(MAKE_SYSCALL(loadutility), loader + 16); // sceUtilityLoadModule(PSP_MODULE_NET_ADHOC);
				
				// Fix Module Unloader
				// C-Summary:
				// sceUtilityUnloadModule(PSP_MODULE_NET_COMMON);
				// sceUtilityUnloadModule(PSP_MODULE_NET_ADHOC);
				// return;
				
				_sw(0x24040101, unloader); // li $a0, 0x101 (arg1 = PSP_MODULE_NET_ADHOC)
				_sw(MAKE_SYSCALL(unloadutility), unloader + 4); // sceUtilityUnloadModule(PSP_MODULE_NET_ADHOC);
				_sw(0x24040100, unloader + 8); // li $a0, 0x100 (arg1 = PSP_MODULE_NET_COMMON)
				_sw(0x03E00008, unloader + 12); // jr $ra
				_sw(MAKE_SYSCALL(unloadutility), unloader + 16); // sceUtilityUnloadModule(PSP_MODULE_NET_COMMON);
				
				// Invalidate Caches
				sceKernelDcacheWritebackInvalidateRange((void *)loader, 20);
				sceKernelIcacheInvalidateRange((void *)loader, 20);
				sceKernelDcacheWritebackInvalidateRange((void *)unloader, 20);
				sceKernelIcacheInvalidateRange((void *)unloader, 20);
				
				// Log Game-Specific Patch
				printk("Patched %s with updated Module Loader\n", getGameCode());
			}
		}
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
					// sctrlHENPatchSyscall((void*)sctrlHENFindFunction("sceLoadExec", "LoadExecForUser", 0x4AC57943), cbdeny);
					// printk("Disabled Home Menu!\n");
					
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

	.set noreorder

#include "pspimport.s"

	IMPORT_START "ATPRO",0x40090000
	IMPORT_FUNC  "ATPRO",0x9130DBB1,hexdump
	IMPORT_FUNC  "ATPRO",0x3DF7F7D8,printk
	IMPORT_FUNC  "ATPRO",0x9590B88F,printk_sync
	IMPORT_FUNC  "ATPRO",0xF89B9E56,patch_netconf_utility

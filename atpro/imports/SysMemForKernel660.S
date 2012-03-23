	.set noreorder

#include "pspstub.s"

	STUB_START	"SysMemForKernel",0x00010011,0x00010005
	STUB_FUNC	0xEF29061C,SysMemGameCodeGetter
	STUB_END

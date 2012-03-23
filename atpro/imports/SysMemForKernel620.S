	.set noreorder

#include "pspstub.s"

	STUB_START	"SysMemForKernel",0x00010011,0x00010005
	STUB_FUNC	0xAB5E85E5,SysMemGameCodeGetter
	STUB_END

	.set noreorder

#include "pspstub.s"

	STUB_START	"SysMemForKernel",0x00010011,0x00010005
	STUB_FUNC	0x3C4C5630,SysMemGameCodeGetter
	STUB_END

	.set noreorder

#include "pspstub.s"

	STUB_START "sceNetMiniUPnP",0x00090011,0x00020005
	STUB_FUNC  0xE66EEEC6,sceNetPortOpen
	STUB_FUNC  0xB7DE11FB,sceNetPortClose
	STUB_END

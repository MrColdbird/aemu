	.set noreorder

#include "pspstub.s"

	STUB_START	"sceNetAdhocctl",0x00090011,0x00020005
	STUB_FUNC	0x8DB83FDC,sceNetAdhocctlGetPeerInfo
	STUB_FUNC	0xE162CB14,sceNetAdhocctlGetPeerList
	STUB_END

	.set noreorder

#include "pspimport.s"

	IMPORT_START "sceNetAdhoc",0x00090011
	IMPORT_FUNC  "sceNetAdhoc",0xE1D621D7,sceNetAdhocInit
	IMPORT_FUNC  "sceNetAdhoc",0xA62C6F57,sceNetAdhocTerm
	IMPORT_FUNC  "sceNetAdhoc",0x7A662D6B,sceNetAdhocPollSocket
	IMPORT_FUNC  "sceNetAdhoc",0x73BFD52D,sceNetAdhocSetSocketAlert
	IMPORT_FUNC  "sceNetAdhoc",0x4D2CE199,sceNetAdhocGetSocketAlert
	IMPORT_FUNC  "sceNetAdhoc",0x6F92741B,sceNetAdhocPdpCreate
	IMPORT_FUNC  "sceNetAdhoc",0xABED3790,sceNetAdhocPdpSend
	IMPORT_FUNC  "sceNetAdhoc",0x7F27BB5E,sceNetAdhocPdpDelete
	IMPORT_FUNC  "sceNetAdhoc",0xDFE53E03,sceNetAdhocPdpRecv
	IMPORT_FUNC  "sceNetAdhoc",0xC7C1FC57,sceNetAdhocGetPdpStat
	IMPORT_FUNC  "sceNetAdhoc",0x877F6D66,sceNetAdhocPtpOpen
	IMPORT_FUNC  "sceNetAdhoc",0xFC6FC07B,sceNetAdhocPtpConnect
	IMPORT_FUNC  "sceNetAdhoc",0xE08BDAC1,sceNetAdhocPtpListen
	IMPORT_FUNC  "sceNetAdhoc",0x9DF81198,sceNetAdhocPtpAccept
	IMPORT_FUNC  "sceNetAdhoc",0x4DA4C788,sceNetAdhocPtpSend
	IMPORT_FUNC  "sceNetAdhoc",0x8BEA2B3E,sceNetAdhocPtpRecv
	IMPORT_FUNC  "sceNetAdhoc",0x9AC2EEAC,sceNetAdhocPtpFlush
	IMPORT_FUNC  "sceNetAdhoc",0x157E6225,sceNetAdhocPtpClose
	IMPORT_FUNC  "sceNetAdhoc",0xB9685118,sceNetAdhocGetPtpStat
	IMPORT_FUNC  "sceNetAdhoc",0x7F75C338,sceNetAdhocGameModeCreateMaster
	IMPORT_FUNC  "sceNetAdhoc",0x3278AB0C,sceNetAdhocGameModeCreateReplica
	IMPORT_FUNC  "sceNetAdhoc",0x98C204C8,sceNetAdhocGameModeUpdateMaster
	IMPORT_FUNC  "sceNetAdhoc",0xA0229362,sceNetAdhocGameModeDeleteMaster
	IMPORT_FUNC  "sceNetAdhoc",0xFA324B4E,sceNetAdhocGameModeUpdateReplica
	IMPORT_FUNC  "sceNetAdhoc",0x0B2228E9,sceNetAdhocGameModeDeleteReplica


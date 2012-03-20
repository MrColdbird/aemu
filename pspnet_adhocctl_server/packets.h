#ifndef _PACKETS_H_
#define _PACKETS_H_

#define OPCODE_PING 0
#define OPCODE_LOGIN 1
#define OPCODE_CONNECT 2
#define OPCODE_DISCONNECT 3
#define OPCODE_SCAN 4
#define OPCODE_SCAN_COMPLETE 5
#define OPCODE_CONNECT_BSSID 6

// PSP Product Code
#define PRODUCT_CODE_LENGTH 9
typedef struct
{
	// Game Product Code (ex. ULUS12345)
	char data[PRODUCT_CODE_LENGTH];
} __attribute__((packed)) SceNetAdhocctlProductCode;

// Basic Packet
typedef struct
{
	uint8_t opcode;
} __attribute__((packed)) SceNetAdhocctlPacketBase;

// C2S Login Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	SceNetEtherAddr mac;
	SceNetAdhocctlNickname name;
	SceNetAdhocctlProductCode game;
} __attribute__((packed)) SceNetAdhocctlLoginPacketC2S;

// C2S Connect Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	SceNetAdhocctlGroupName group;
} __attribute__((packed)) SceNetAdhocctlConnectPacketC2S;

// S2C Connect Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	SceNetAdhocctlNickname name;
	SceNetEtherAddr mac;
	uint32_t ip;
} __attribute__((packed)) SceNetAdhocctlConnectPacketS2C;

// S2C Disconnect Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	uint32_t ip;
} __attribute__((packed)) SceNetAdhocctlDisconnectPacketS2C;

// S2C Scan Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	SceNetAdhocctlGroupName group;
	SceNetEtherAddr mac;
} __attribute__((packed)) SceNetAdhocctlScanPacketS2C;

// S2C Connect BSSID Packet
typedef struct
{
	SceNetAdhocctlPacketBase base;
	SceNetEtherAddr mac;
} __attribute__((packed)) SceNetAdhocctlConnectBSSIDPacketS2C;

#endif

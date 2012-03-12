#include "../common.h"

// Library State
int _init = 0;

// Thread Status
int _thread_status = ADHOCCTL_STATE_DISCONNECTED;

// Game Product Code
SceNetAdhocctlAdhocId _product_code;

// Game Group
SceNetAdhocctlParameter _parameter;

// Peer List
SceNetAdhocctlPeerInfo * _friends = NULL;

// Scan Network List
SceNetAdhocctlScanInfo * _networks = NULL;

// Event Handler
SceNetAdhocctlHandler _event_handler[ADHOCCTL_MAX_HANDLER];
void * _event_args[ADHOCCTL_MAX_HANDLER];

// Access Point Setting Name
int _hotspot = -1;

// Meta Socket
int _metasocket = -1;

#ifdef ENABLE_PEERLOCK
// Peer Locker
int _peerlock = 0;
#endif

#ifdef ENABLE_NETLOCK
// Network Locker
int _networklock = 0;
#endif

// Bit-Values
int _one = 1;
int _zero = 0;

// Function Prototypes
int _initNetwork(const SceNetAdhocctlAdhocId * adhoc_id, uint32_t server_ip);
int _readHotspotConfig(void);
int _findHotspotConfigId(char * ssid);
int _readServerConfig(void);
int _friendFinder(SceSize args, void * argp);
void _addFriend(SceNetAdhocctlConnectPacketS2C * packet);
void _deleteFriendByIP(uint32_t ip);

/**
 * Initialize the Adhoc-Control Emulator
 * @param stacksize Stacksize of the Internal Thread
 * @param prio Internal Thread Priority
 * @param adhoc_id Game Product Code
 * @return 0 on success or... ADHOCCTL_ALREADY_INITIALIZED, ADHOCCTL_STACKSIZE_TOO_SHORT, ADHOCCTL_INVALID_ARG
 */
int proNetAdhocctlInit(int stacksize, int prio, const SceNetAdhocctlAdhocId * adhoc_id)
{
	// Uninitialized Library
	if(_init == 0)
	{
		// Minimum Stacksize (just to fake SCE behaviour)
		if(stacksize >= 3072)
		{
			// Read Hotspot Configuration
			if(_readHotspotConfig() == 0)
			{
				// Get Server IP
				uint32_t ip = _readServerConfig();
				
				// Read Server Configuration
				if(ip != 0)
				{
					// Initialize Networking
					if(_initNetwork(adhoc_id, ip) == 0)
					{
						// Create Main Thread
						int update = sceKernelCreateThread("friend_finder", _friendFinder, 0x18/*prio*/, 32768, 0, NULL);
						
						// Created Main Thread
						if(update >= 0)
						{
							// Initialization Success
							_init = 1;
							
							// Start Main Thread
							sceKernelStartThread(update, 0, NULL);
							
							// Return Success
							return 0;
						}
					}
				}
			}
			
			// Generic Error
			return -1;
		}
		
		// Stack too small
		return ADHOCCTL_STACKSIZE_TOO_SHORT;
	}
	
	// Initialized Library
	return ADHOCCTL_ALREADY_INITIALIZED;
}

/**
 * Initialize Networking Components for Adhocctl Emulator
 * @param adhoc_id Game Product Code
 * @param server_ip Server IP
 * @return 0 on success or... -1
 */
int _initNetwork(const SceNetAdhocctlAdhocId * adhoc_id, uint32_t server_ip)
{
	// WLAN Switch Check
	if(sceWlanGetSwitchState() == 1)
	{
		// Initialize Access Point Control
		if(sceNetApctlInit(0x1800, 0x30) == 0)
		{
			// Start Connection
			if(sceNetApctlConnect(_hotspot) == 0)
			{
				// Wait for Connection
				int statebefore = 0;
				int state = 0; while(state != 4)
				{
					// Query State
					int getstate = sceNetApctlGetState(&state);
					
					// Log State Change
					if(statebefore != state) printk("New Connection State: %d\n", state);					

					// Query Success
					if(getstate == 0 && state != 4)
					{
						// Wait for Retry
						sceKernelDelayThread(1000000);
					}
					
					// Query Error
					else break;
					
					// Save Before State
					statebefore = state;
				}
				
				// Connected
				if(state == 4)
				{
					// Create Friend Finder Socket
					int socket = sceNetInetSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					
					// Created Socket
					if(socket > 0)
					{
						// Enable Port Re-use
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &_one, sizeof(_one));
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &_one, sizeof(_one));
						
						// Apply Receive Timeout Settings to Socket
						// uint32_t timeout = ADHOCCTL_RECV_TIMEOUT;
						// sceNetInetSetsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
						
						// Prepare Server Address
						SceNetInetSockaddrIn addr;
						addr.sin_len = sizeof(addr);
						addr.sin_family = AF_INET;
						addr.sin_addr = server_ip;
						addr.sin_port = sceNetHtons(ADHOCCTL_METAPORT);
						
						// Connect to Server
						if(sceNetInetConnect(socket, (SceNetInetSockaddr *)&addr, sizeof(addr)) == 0)
						{
							// Save Meta Socket
							_metasocket = socket;
							
							// Save Product Code
							_product_code = *adhoc_id;
							
							// Clear Event Handler
							memset(_event_handler, 0, sizeof(_event_handler[0]) * ADHOCCTL_MAX_HANDLER);
							memset(_event_args, 0, sizeof(_event_args[0]) * ADHOCCTL_MAX_HANDLER);
							
							// Clear Internal Control Status
							memset(&_parameter, 0, sizeof(_parameter));
							
							// Read PSP Player Name
							sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, (char *)_parameter.nickname.data, ADHOCCTL_NICKNAME_LEN);
							
							// Read PSP MAC Address
							sceWlanGetEtherAddr((void *)&_parameter.bssid.mac_addr.data);
							
							// Prepare Login Packet
							SceNetAdhocctlLoginPacketC2S packet;
							
							// Set Packet Opcode
							packet.base.opcode = OPCODE_LOGIN;
							
							// Set MAC Address
							packet.mac = _parameter.bssid.mac_addr;
							
							// Set Nickname
							packet.name = _parameter.nickname;
							
							// Set Game Product ID
							memcpy(packet.game.data, adhoc_id->data, ADHOCCTL_ADHOCID_LEN);
							
							// Acquire Network Layer Lock
							_acquireNetworkLock();
							
							// Send Login Packet
							sceNetInetSend(_metasocket, &packet, sizeof(packet), INET_MSG_DONTWAIT);
							
							// Free Network Layer Lock
							_freeNetworkLock();
							
							// Return Success
							return 0;
						}
						
						// Delete Socket
						sceNetInetClose(socket);
					}
					
					// Close Hotspot Connection
					sceNetApctlDisconnect();
				}
			}
			
			// Terminate Access Point Control
			sceNetApctlTerm();	
		}
	}
	
	// Generic Error
	return -1;
}

/**
 * Read Access Point Configuration Name
 * @return 0 on success or... -1
 */
int _readHotspotConfig(void)
{
	// Open Configuration File
	int fd = sceIoOpen("ms0:/seplugins/hotspot.txt", PSP_O_RDONLY, 0777);
	
	// Opened Configuration File
	if(fd >= 0)
	{
		// Line Buffer
		char line[128];
		
		// Read Line
		_readLine(fd, line, sizeof(line));
		
		// Find Hotspot Configuration
		_hotspot = _findHotspotConfigId(line);
		
		// Found Hotspot
		if(_hotspot != -1) printk("Selected Hotspot: %s\n", line);
		
		// No Hotspot found
		else printk("Couldn't find Hotspot: %s\n", line);
		
		// Close Configuration File
		sceIoClose(fd);
		
		// Return Success
		if(_hotspot >= 0) return 0;
	}
	
	// Generic Error
	return -1;
}

/**
 * Find Infrastructure Configuration ID for SSID
 * @param ssid Target SSID
 * @return Configuration ID > -1 or... -1
 */
int _findHotspotConfigId(char * ssid)
{
	// Counter
	int i = 0;
	
	// Find Hotspot by SSID
	for(; i <= 10; i++)
	{
		// Parameter Container
		netData entry;
		
		// Acquire SSID for Configuration
		if(sceUtilityGetNetParam(i, PSP_NETPARAM_SSID, &entry) == 0)
		{
			// Log Parameter
			printk("Reading PSP Infrastructure Profile for %s\n", entry.asString);
			
			// Hotspot Configuration found
			if(strcmp(entry.asString, ssid) == 0) return i;
		}
	}
	
	// Hotspot not found
	return -1;
}

/**
 * Read Server IP
 * @return IP != 0 on success or... 0
 */
int _readServerConfig(void)
{
	// Open Configuration File
	int fd = sceIoOpen("ms0:/seplugins/server.txt", PSP_O_RDONLY, 0777);
	
	// Opened Configuration File
	if(fd >= 0)
	{
		// Line Buffer
		char line[128];
		
		// Read Line
		_readLine(fd, line, sizeof(line));
		
		// Server IP
		uint32_t ip = 0;	
		
		// Create IP Address from Line
		sceNetInetInetAton(line, &ip);
		
		// Close Configuration File
		sceIoClose(fd);
		
		// Return IP
		return ip;
	}
	
	// Generic Error
	return 0;
}

/**
 * Read Line from File
 * @param fd File Descriptor to read line from
 * @param buffer Buffer to read line into
 * @param buflen Length of Buffer in Bytes
 * @return Length of Line or... 0
 */
uint32_t _readLine(int fd, char * buffer, uint32_t buflen)
{
	// Erase Buffer
	memset(buffer, 0, buflen);
	
	// Length of Line
	uint32_t length = 0;
	
	// Read Line
	while(length < buflen - 1)
	{
		// Read Character
		int b = sceIoRead(fd, buffer + length, 1);
		
		// Read Failure
		if(b <= 0) break;
		
		// Move Pointer
		length++;		
		
		// End of Line Symbol
		if(buffer[length - 1] == '\n') break;
	}
	
	// Remove End of Line Symbols
	int i = 0; for(; i < length; i++) if(buffer[i] == '\r' || buffer[i] == '\n') buffer[i] = 0;
	
	// Return true Length
	return strlen(buffer);
}

/**
 * Friend Finder Thread (Receives Peer Information)
 * @param args Length of argp in Bytes (Unused)
 * @param argp Argument (Unused)
 * @return Unused Value - Return 0
 */
int _friendFinder(SceSize args, void * argp)
{
	// Receive Buffer
	static int rxpos = 0;
	static uint8_t rx[1024];
	
	// Last Ping Time
	static uint64_t lastping = 0;
	
	// Finder Loop
	while(_init == 1)
	{
		// Acquire Network Lock
		_acquireNetworkLock();
		
		// Ping Server
		if((sceKernelGetSystemTimeWide() - lastping) >= ADHOCCTL_PING_TIMEOUT)
		{
			// Update Ping Time
			lastping = sceKernelGetSystemTimeWide();
			
			// Prepare Packet
			uint8_t opcode = OPCODE_PING;
			
			// Send Ping to Server
			sceNetInetSend(_metasocket, &opcode, 1, INET_MSG_DONTWAIT);
		}
		
		// Wait for Incoming Data
		int received = sceNetInetRecv(_metasocket, rx + rxpos, sizeof(rx) - rxpos, INET_MSG_DONTWAIT);
		
		// Free Network Lock
		_freeNetworkLock();
		
		// Received Data
		if(received > 0)
		{
			// Fix Position
			rxpos += received;
			
			// Log Incoming Traffic
			#ifdef DEBUG
			printk("Received %d Bytes of Data from Server\n", received);
			#endif
		}
		
		// Handle Packets
		if(rxpos > 0)
		{
			// Connect Packet
			if(rx[0] == OPCODE_CONNECT)
			{
				// Enough Data available
				if(rxpos >= sizeof(SceNetAdhocctlConnectPacketS2C))
				{
					// Log Incoming Peer
					#ifdef DEBUG
					printk("Incoming Peer Data...\n");
					#endif
					
					// Cast Packet
					SceNetAdhocctlConnectPacketS2C * packet = (SceNetAdhocctlConnectPacketS2C *)rx;
					
					// Add User
					_addFriend(packet);
					
					// Move RX Buffer
					memmove(rx, rx + sizeof(SceNetAdhocctlConnectPacketS2C), sizeof(rx) - sizeof(SceNetAdhocctlConnectPacketS2C));
					
					// Fix RX Buffer Length
					rxpos -= sizeof(SceNetAdhocctlConnectPacketS2C);
				}
			}
			
			// Disconnect Packet
			else if(rx[0] == OPCODE_DISCONNECT)
			{
				// Enough Data available
				if(rxpos >= sizeof(SceNetAdhocctlDisconnectPacketS2C))
				{
					// Log Incoming Peer Delete Request
					#ifdef DEBUG
					printk("Incoming Peer Data Delete Request...\n");
					#endif
					
					// Cast Packet
					SceNetAdhocctlDisconnectPacketS2C * packet = (SceNetAdhocctlDisconnectPacketS2C *)rx;
					
					// Delete User by IP
					_deleteFriendByIP(packet->ip);
					
					// Move RX Buffer
					memmove(rx, rx + sizeof(SceNetAdhocctlDisconnectPacketS2C), sizeof(rx) - sizeof(SceNetAdhocctlDisconnectPacketS2C));
					
					// Fix RX Buffer Length
					rxpos -= sizeof(SceNetAdhocctlDisconnectPacketS2C);
				}
			}
			
			// Scan Packet
			else if(rx[0] == OPCODE_SCAN)
			{
				// Enough Data available
				if(rxpos >= sizeof(SceNetAdhocctlScanPacketS2C))
				{
					// Log Incoming Network Information
					#ifdef DEBUG
					printk("Incoming Group Information...\n");
					#endif
					
					// Cast Packet
					SceNetAdhocctlScanPacketS2C * packet = (SceNetAdhocctlScanPacketS2C *)rx;
					
					// Allocate Structure Data
					SceNetAdhocctlScanInfo * group = (SceNetAdhocctlScanInfo *)malloc(sizeof(SceNetAdhocctlScanInfo));
					
					// Allocated Structure Data
					if(group != NULL)
					{
						// Clear Memory
						memset(group, 0, sizeof(SceNetAdhocctlScanInfo));
						
						// Link to existing Groups
						group->next = _networks;
						
						// Copy Group Name
						group->group_name = packet->group;
						
						// Set Localhost as Group Host
						group->bssid = _parameter.bssid;
						
						// Link into Group List
						_networks = group;
					}
					
					// Move RX Buffer
					memmove(rx, rx + sizeof(SceNetAdhocctlScanPacketS2C), sizeof(rx) - sizeof(SceNetAdhocctlScanPacketS2C));
					
					// Fix RX Buffer Length
					rxpos -= sizeof(SceNetAdhocctlScanPacketS2C);
				}
			}
			
			// Scan Complete Packet
			else if(rx[0] == OPCODE_SCAN_COMPLETE)
			{
				// Log Scan Completion
				#ifdef DEBUG
				printk("Incoming Scan complete response...\n");
				#endif
				
				// Change State
				_thread_status = ADHOCCTL_STATE_DISCONNECTED;
				
				// Notify Event Handlers
				int i = 0; for(; i < ADHOCCTL_MAX_HANDLER; i++)
				{
					// Active Handler
					if(_event_handler[i] != NULL) _event_handler[i](ADHOCCTL_EVENT_SCAN, 0, _event_args[i]);
				}
				
				// Move RX Buffer
				memmove(rx, rx + 1, sizeof(rx) - 1);
				
				// Fix RX Buffer Length
				rxpos -= 1;
			}
		}
		
		//	Delay Thread (10ms)
		sceKernelDelayThread(10000);
	}
	
	// Notify Caller of Shutdown
	_init = -1;
	
	// Reset Thread Status
	_thread_status = ADHOCCTL_STATE_DISCONNECTED;
	
	// Kill Thread
	sceKernelExitDeleteThread(0);
	
	// Return Success
	return 0;
}

/**
 * Lock Peerlist
 */
void _acquirePeerLock(void)
{
	#ifdef ENABLE_PEERLOCK
	// Wait for Unlock
	while(_peerlock)
	{
		// Delay Thread
		sceKernelDelayThread(1);
	}
	
	// Lock Access
	_peerlock = 1;
	#endif
}

/**
 * Unlock Peerlist
 */
void _freePeerLock(void)
{
	#ifdef ENABLE_PEERLOCK
	// Unlock Access
	_peerlock = 0;
	#endif
}

/**
 * Lock Network
 */
void _acquireNetworkLock(void)
{
	#ifdef ENABLE_NETLOCK
	// Wait for Unlock
	while(_networklock)
	{
		// Delay Thread
		sceKernelDelayThread(1);
	}
	
	// Lock Access
	_networklock = 1;
	#endif
}

/**
 * Unlock Network
 */
void _freeNetworkLock(void)
{
	#ifdef ENABLE_NETLOCK
	// Unlock Access
	_networklock = 0;
	#endif
}

/**
 * Add Friend to Local List
 * @param packet Friend Information
 */
void _addFriend(SceNetAdhocctlConnectPacketS2C * packet)
{
	// Allocate Structure
	SceNetAdhocctlPeerInfo * peer = (SceNetAdhocctlPeerInfo *)malloc(sizeof(SceNetAdhocctlPeerInfo));
	
	// Allocated Structure
	if(peer != NULL)
	{
		// Clear Memory
		memset(peer, 0, sizeof(SceNetAdhocctlPeerInfo));
		
		// Link to existing Peers
		peer->next = _friends;
		
		// Save Nickname
		peer->nickname = packet->name;
		
		// Save MAC Address
		peer->mac_addr = packet->mac;
		
		// Save IP Address
		peer->ip_addr = packet->ip;
		
		// Multithreading Lock
		_acquirePeerLock();
		
		// Link into Peerlist
		_friends = peer;
		
		// Multithreading Unlock
		_freePeerLock();
	}
}

/**
 * Delete Friend from Local List
 * @param ip Friend IP
 */
void _deleteFriendByIP(uint32_t ip)
{
	// Previous Peer Reference
	SceNetAdhocctlPeerInfo * prev = NULL;
	
	// Peer Pointer
	SceNetAdhocctlPeerInfo * peer = _friends;
	
	// Iterate Peers
	for(; peer != NULL; peer = peer->next)
	{
		// Found Peer
		if(peer->ip_addr == ip)
		{
			// Multithreading Lock
			_acquirePeerLock();
			
			// Unlink Left (Beginning)
			if(prev == NULL) _friends = peer->next;
			
			// Unlink Left (Other)
			else prev->next = peer->next;
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Free Memory
			free(peer);
			
			// Stop Search
			break;
		}
		
		// Set Previous Reference
		prev = peer;
	}
}

#include "../common.h"

// Library State
int _init = 0;

// Game Product Code
SceNetAdhocctlAdhocId _product_code;

// Event Handler
SceNetAdhocctlHandler _event_handler[ADHOCCTL_MAX_HANDLER];
void * _event_args[ADHOCCTL_MAX_HANDLER];

// Friend Control Status
SceNetAdhocctlStatusFriend * _friends = NULL;

// Local Control Status
SceNetAdhocctlStatusBase _status;

// Access Point Setting Name
int _hotspot = -1;

// Meta Socket
int _metasocket = -1;

// Peer Locker
int _peerlock = 0;

// Bit-Values
int _one = 1;
int _zero = 0;

// Function Prototypes
int _initNetwork(const SceNetAdhocctlAdhocId * adhoc_id);
int _readConfig(void);
int _findHotspotConfigId(char * ssid);
int _readFriendList(void);
int _readFriend(int fd);
int _friendFinder(SceSize args, void * argp);
void _sendControlStatus(void);
void _recvControlStatus(void);
#ifdef DEBUG
int _addDebugFriend(void);
#endif

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
			// Read Friendlist
			if(_readFriendList() > 0)
			{
				// Read Configuration
				if(_readConfig() == 0)
				{
					// Initialize Networking
					if(_initNetwork(adhoc_id) == 0)
					{
						// Create Main Thread
						int update = sceKernelCreateThread("friend_finder", _friendFinder, prio, 32768, 0, NULL);
						
						// Created Main Thread
						if(update >= 0)
						{
							// Initialization Success
							_init = 1;
							
							// Start Main Thread
							sceKernelStartThread(update, 0, NULL);
							
							// Wait for Convergence
							sceKernelDelayThread(ADHOCCTL_SEND_TIMEOUT);
							
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
 * @return 0 on success or... -1
 */
int _initNetwork(const SceNetAdhocctlAdhocId * adhoc_id)
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
					int socket = sceNetInetSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
					
					// Created Socket
					if(socket > 0)
					{
						// Enable Port Re-use
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &_one, sizeof(_one));
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &_one, sizeof(_one));
						
						// Apply Receive Timeout Settings to Socket
						uint32_t timeout = ADHOCCTL_RECV_TIMEOUT;
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
						
						// Binding Information for local Port
						SceNetInetSockaddrIn addr;
						addr.sin_len = sizeof(addr);
						addr.sin_family = AF_INET;
						addr.sin_addr = INADDR_ANY;
						addr.sin_port = sceNetHtons(ADHOCCTL_METAPORT);
						
						// Bound Socket to local Port
						if(sceNetInetBind(socket, (SceNetInetSockaddr *)&addr, sizeof(addr)) == 0)
						{
							// Save Meta Socket
							_metasocket = socket;
							
							// Save Product Code
							_product_code = *adhoc_id;
							
							// Clear Event Handler
							memset(_event_handler, 0, sizeof(_event_handler[0]) * ADHOCCTL_MAX_HANDLER);
							memset(_event_args, 0, sizeof(_event_args[0]) * ADHOCCTL_MAX_HANDLER);
							
							// Clear Internal Control Status
							memset(&_status, 0, sizeof(_status));
							
							// Read PSP Player Name
							sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, (char *)_status.player_name.data, ADHOCCTL_NICKNAME_LEN);
							
							// Read PSP MAC Address
							sceWlanGetEtherAddr((void *)&_status.player_mac.data);
							
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
int _readConfig(void)
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
 * Read Friend IP List
 * @return Number of read Friends
 */
int _readFriendList(void)
{
	// Friend Counter
	int friends = 0;
	
	// Open Friendlist
	int fd = sceIoOpen("ms0:/seplugins/friendlist.txt", PSP_O_RDONLY, 0777);
	
	// Opened Friendlist
	if(fd >= 0)
	{
		// Read Friend
		while(_readFriend(fd)) friends++;
		
		// Close Friendlist
		sceIoClose(fd);
	}	
	
	// Log Friend Count
	if(friends > 0) printk("Imported %d Friend IPs\n", friends);
	
	// No Friends found
	else printk("Couldn't find Friend IPs\n");
	
	#ifdef DEBUG
	friends += _addDebugFriend();
	#endif
	
	// No Friends found
	return friends;
}

#ifdef DEBUG
/**
 * Add Debugger to Friendlist
 * @return 1 on success or... 0
 */
int _addDebugFriend(void)
{
	// Friend IP
	uint32_t ip = 0;	
	
	// Create IP Address from Line
	if(sceNetInetInetAton(ADHOCCTL_PC_DEBUGGER_IP, &ip) == 1)
	{
		// Allocate Friend IP Entry
		SceNetAdhocctlStatusFriend * newfriend = (SceNetAdhocctlStatusFriend *)malloc(sizeof(SceNetAdhocctlStatusFriend));
		
		// Allocate Success
		if(newfriend != NULL)
		{
			// Clear Memory
			memset(newfriend, 0, sizeof(SceNetAdhocctlStatusFriend));
			
			// Save IP
			newfriend->ip_addr = ip;
			
			// Save MAC
			memcpy(newfriend->base.player_mac.data, ADHOCCTL_PC_DEBUGGER_MAC, ETHER_ADDR_LEN);
			
			// Set Name
			strcpy(newfriend->base.player_name.data, ADHOCCTL_PC_DEBUGGER_NAME);
			
			// Set Connected State
			newfriend->base.network_type = UTILITY_NETCONF_TYPE_CONNECT_ADHOC;
			
			// Set Update Time
			newfriend->last_recv = sceKernelGetSystemTimeWide();
			
			// Link Friend
			newfriend->next = _friends;
			
			// Exchange Array
			_friends = newfriend;
			
			// Success
			return 1;
		}
	}
	
	// Failure
	return 0;
}
#endif

/**
 * Read Friend IP from File
 * @param fd File Descriptor to read Friend IP from
 * @return 1 on success or... 0
 */
int _readFriend(int fd)
{
	// TODO Change this to linked list... need to add new structure to structures.h
	
	// Line Buffer
	char line[512];
	
	// Read Line
	_readLine(fd, line, sizeof(line));
	
	// Friend IP
	uint32_t ip = 0;	
	
	// Create IP Address from Line
	if(sceNetInetInetAton(line, &ip) == 1)
	{
		// Allocate Friend IP Entry
		SceNetAdhocctlStatusFriend * newfriend = (SceNetAdhocctlStatusFriend *)malloc(sizeof(SceNetAdhocctlStatusFriend));
		
		// Allocate Success
		if(newfriend != NULL)
		{
			// Clear Memory
			memset(newfriend, 0, sizeof(SceNetAdhocctlStatusFriend));
			
			// Save IP
			newfriend->ip_addr = ip;
			
			// Link Friend
			newfriend->next = _friends;
			
			// Exchange Array
			_friends = newfriend;
			
			// Success
			return 1;
		}
	}
	
	// Failure
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
 * Friend Finder Thread (Sends and Receives Peer Information)
 * @param args Length of argp in Bytes (Unused)
 * @param argp Argument (Unused)
 * @return Unused Value - Return 0
 */
int _friendFinder(SceSize args, void * argp)
{
	// Finder Loop
	while(_init == 1)
	{
		// Multithreading Lock
		_acquirePeerLock();
		
		// Send Status Data to Peers
		_sendControlStatus();
		
		// Receive Status Data from Peers
		_recvControlStatus();
		
		// Multithreading Unlock
		_freePeerLock();
		
		//	Delay Thread (10ms)
		// sceKernelDelayThread(10000);
	}
	
	// Notify Caller of Shutdown
	_init = -1;	
	
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
 * Send Adhoc Control Status to Friends via UDP
 */
void _sendControlStatus(void)
{
	// Last Network Action Time
	static uint32_t last_action = 0;
	
	// Time to act...
	if((sceKernelGetSystemTimeLow() - last_action) >= ADHOCCTL_SEND_TIMEOUT)
	{
		// Prepare Status Packet
		SceNetAdhocctlStatusPacket packet;
		memset(&packet, 0, sizeof(packet));
		
		// Copy Basic Information
		packet.base = _status;
		memcpy(packet.product_code, _product_code.data, ADHOCCTL_ADHOCID_LEN);
		
		// Iterate Friends
		SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL)
		{
			// Fill in Target Structure
			SceNetInetSockaddrIn target;
			target.sin_family = AF_INET;
			target.sin_addr = friend->ip_addr;
			target.sin_port = sceNetHtons(ADHOCCTL_METAPORT);
			
			// Send Status Message
			sceNetInetSendto(_metasocket, (void *)&packet, sizeof(packet), INET_MSG_DONTWAIT, (SceNetInetSockaddr *)&target, sizeof(target));
			
			// Move Pointer
			friend = friend->next;
		}
		
		// Update Action Time
		last_action = sceKernelGetSystemTimeLow();
	}
}

/**
 * Receive Adhoc Control Status from Friends via UDP
 */
void _recvControlStatus(void)
{
	// Status Packet
	SceNetAdhocctlStatusPacket packet;
	
	// Sender Address
	SceNetInetSockaddrIn sin;
	uint32_t sinlen;
	
	#ifdef DEBUG
	// Update Debugger Timestamp
	if(_friends != NULL) _friends->last_recv = sceKernelGetSystemTimeWide();
	#endif
	
	// Receive Data
	// int received = sceNetInetRecvfrom(_metasocket, (void *)&packet, sizeof(packet), INET_MSG_DONTWAIT, (SceNetInetSockaddr *)&sin, &sinlen);
	
	// Receive Data with Timeout Value from _initNetwork
	int received = sceNetInetRecvfrom(_metasocket, (void *)&packet, sizeof(packet), 0, (SceNetInetSockaddr *)&sin, &sinlen);
	
	// Received Data
	if(received == sizeof(packet))
	{
		// Iterate Friends
		SceNetAdhocctlStatusFriend * friend = _friends; while(friend != NULL)
		{
			// Check Data
			int ipcheck = (friend->ip_addr == sin.sin_addr);
			int productcheck = (strncmp((char *)packet.product_code, (char *)_product_code.data, ADHOCCTL_ADHOCID_LEN) == 0);
			
			// Friend found playing the same game
			//if(friend->ip_addr == sin.sin_addr && strncmp((char *)packet.product_code, (char *)_product_code.data, ADHOCCTL_ADHOCID_LEN) == 0)
			if(ipcheck && productcheck)
			{
				// Log Update
				#ifdef DEBUG
				printk("Updated %s's Control Status\n", (char *)packet.base.player_name.data);
				#endif
				
				// Update Friend Status
				friend->base = packet.base;
				
				// Update Last Receive Timer
				friend->last_recv = sceKernelGetSystemTimeWide();
				
				// Stop Friend Iteration
				break;
			}
			
			// Move Pointer
			friend = friend->next;
		}
	}
}

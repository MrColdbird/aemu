/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

#include "common.h"

/**
 * Resolve IP to MAC
 * @param ip Peer IP Address
 * @param mac OUT: Peer MAC
 * @return 0 on success or... ADHOC_NO_ENTRY
 */
int _resolveIP(uint32_t ip, SceNetEtherAddr * mac)
{
	// Get Local IP Address
	union SceNetApctlInfo info; if(sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info) == 0)
	{
		// Convert IP Address to Numerical Display
		uint32_t _ip = 0; sceNetInetInetAton(info.ip, &_ip);
		
		// Local IP Address Requested
		if(ip == _ip)
		{
			// Return MAC Address
			sceWlanGetEtherAddr((void *)mac->data);
			
			// Return Success
			return 0;
		}
	}
	
	// Multithreading Lock
	_acquirePeerLock();
	
	// Peer Reference
	SceNetAdhocctlPeerInfo * peer = _friends;
	
	// Iterate Peers
	for(; peer != NULL; peer = peer->next)
	{
		// Found Matching Peer
		if(peer->ip_addr == ip)
		{
			// Copy Data
			*mac = peer->mac_addr;
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Return Success
			return 0;
		}
	}
	
	// Multithreading Unlock
	_freePeerLock();
	
	// Peer not found
	return ADHOC_NO_ENTRY;
}

/**
 * Resolve MAC to IP
 * @param mac Peer MAC Address
 * @param ip OUT: Peer IP
 * @return 0 on success or... ADHOC_NO_ENTRY
 */
int _resolveMAC(SceNetEtherAddr * mac, uint32_t * ip)
{
	// Get Local MAC Address
	uint8_t localmac[6]; sceWlanGetEtherAddr((void *)localmac);
	
	// Local MAC Requested
	if(memcmp(localmac, mac, sizeof(SceNetEtherAddr)) == 0)
	{
		// Get Local IP Address
		union SceNetApctlInfo info; if(sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &info) == 0)
		{
			// Return IP Address
			sceNetInetInetAton(info.ip, ip);
			
			// Return Success
			return 0;
		}
	}
	
	// Multithreading Lock
	_acquirePeerLock();
	
	// Peer Reference
	SceNetAdhocctlPeerInfo * peer = _friends;
	
	// Iterate Peers
	for(; peer != NULL; peer = peer->next)
	{
		// Found Matching Peer
		if(memcmp(&peer->mac_addr, mac, sizeof(SceNetEtherAddr)) == 0)
		{
			// Copy Data
			*ip = peer->ip_addr;
			
			// Multithreading Unlock
			_freePeerLock();
			
			// Return Success
			return 0;
		}
	}
	
	// Multithreading Unlock
	_freePeerLock();
	
	// Peer not found
	return ADHOC_NO_ENTRY;
}

/**
 * Get First Peer List Element
 * @return First Internal Peer List Element
 */
SceNetAdhocctlPeerInfo * _getInternalPeerList(void)
{
	// Return First Peer List Element
	return _friends;
}

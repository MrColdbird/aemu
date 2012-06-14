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

#include "../common.h"

/**
 * Trigger Adhoc Network Scan
 * @return 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_BUSY
 */
int proNetAdhocctlScan(void)
{
	// Library initialized
	if(_init == 1)
	{
		// Not connected
		if(_thread_status == ADHOCCTL_STATE_DISCONNECTED)
		{
			// Set Thread Status to Scanning
			_thread_status = ADHOCCTL_STATE_SCANNING;
			
			// Prepare Scan Request Packet
			uint8_t opcode = OPCODE_SCAN;
			
			// Send Scan Request Packet
			sceNetInetSend(_metasocket, &opcode, 1, INET_MSG_DONTWAIT);
			
			// Return Success
			return 0;
		}
		
		// Library is busy
		return ADHOCCTL_BUSY;
	}
	
	// Library uninitialized
	return ADHOCCTL_NOT_INITIALIZED;
}

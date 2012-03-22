/* $Id: connecthostport.c,v 1.6 2012/01/21 13:30:31 nanard Exp $ */
/* Project : miniupnp
 * Author : Thomas Bernard
 * Copyright (c) 2010-2012 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution. */

#include "common.h"
#include "connecthostport.h"

struct SceNetInetTcpcbstat {
	struct SceNetInetTcpcbstat *next;
	unsigned int ts_so_snd_sb_cc;
	unsigned int ts_so_rcv_sb_cc;
	uint32_t ts_inp_laddr;
	uint32_t ts_inp_faddr;
	uint16_t ts_inp_lport;
	uint16_t ts_inp_fport;
	short ts_t_state;
};

int sceNetInetGetTcpcbstat(
	int *buflen,
	void *buf
);

void logSockets(void)
{
	int num = 0;
	sceNetInetGetTcpcbstat(&num, NULL);
	struct SceNetInetTcpcbstat * buf = (struct SceNetInetTcpcbstat *)malloc(num);
	sceNetInetGetTcpcbstat(&num, buf);
	struct SceNetInetTcpcbstat * entry = buf;
	for(; entry != NULL; entry = entry->next)
	{
		// Log Shit
		printk("TCP Socket: %08X:%u -> %08X:%u\n", entry->ts_inp_laddr, sceNetNtohs(entry->ts_inp_lport), entry->ts_inp_faddr, sceNetNtohs(entry->ts_inp_fport));
	}
}

/* connecthostport()
 * return a socket connected (TCP) to the host and port
 * or -1 in case of error */
int connecthostport(const char * host, unsigned short port)
{
	printk("Connect Attempt: %s:%u\n", host, port);
	// Target Address
	SceNetInetSockaddrIn in;
	
	// Clear Memory
	memset(&in, 0, sizeof(in));
	
	// Set Address Family
	in.sin_family = AF_INET;
	
	// Set Structure Length
	in.sin_len = sizeof(in);
	
	// Translate Port
	in.sin_port = sceNetHtons(port);
	
	// Resolve Domain
	if(sceNetResolverStartNtoA(_rid, host, &in.sin_addr, 500000, 2) != 0)
	{
		// Attempt IP Conversion
		sceNetInetInetAton(host, &in.sin_addr);
	}
	
	// Create Socket
	int s = sceNetInetSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// Valid Socket
	if(s > 0)
	{
		// Enabler
		int one = 1;
		
		// Enable Port Re-use
		sceNetInetSetsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
		sceNetInetSetsockopt(s, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
		
		// Set 3-Second Timeout
		uint32_t timeout = 3000000;
		sceNetInetSetsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		sceNetInetSetsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
		
		// Connect to Destination
		if(sceNetInetConnect(s, (SceNetInetSockaddr *)&in, sizeof(in)) == 0/* || sceNetInetGetErrno() == 106*/)
		{
			printk("Connection Success\n");
			// Return Socket
			return s;
		}
		
		printk("Errno: %d\n", sceNetInetGetErrno());
		
		// Close Socket
		sceNetInetClose(s);
		
		logSockets();
	}

	// Error State
	return -1;
}


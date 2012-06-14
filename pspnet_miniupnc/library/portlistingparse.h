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

/* $Id: portlistingparse.h,v 1.5 2012/01/21 13:30:33 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2011-2012 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */
#ifndef __PORTLISTINGPARSE_H__
#define __PORTLISTINGPARSE_H__

/* for the definition of UNSIGNED_INTEGER */
#include "miniupnpctypes.h"

#if defined(NO_SYS_QUEUE_H) || defined(_WIN32) || defined(__HAIKU__) 
#include "bsdqueue.h"
#else
#include <sys/queue.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* sample of PortMappingEntry :
  <p:PortMappingEntry>
    <p:NewRemoteHost>202.233.2.1</p:NewRemoteHost>
    <p:NewExternalPort>2345</p:NewExternalPort>
    <p:NewProtocol>TCP</p:NewProtocol>
    <p:NewInternalPort>2345</p:NewInternalPort>
    <p:NewInternalClient>192.168.1.137</p:NewInternalClient>
    <p:NewEnabled>1</p:NewEnabled>
    <p:NewDescription>dooom</p:NewDescription>
    <p:NewLeaseTime>345</p:NewLeaseTime>
  </p:PortMappingEntry>
 */
typedef enum { PortMappingEltNone,
       PortMappingEntry, NewRemoteHost,
       NewExternalPort, NewProtocol,
       NewInternalPort, NewInternalClient,
       NewEnabled, NewDescription, 
       NewLeaseTime } portMappingElt;

struct PortMapping {
	LIST_ENTRY(PortMapping) entries;
	UNSIGNED_INTEGER leaseTime;
	unsigned short externalPort;
	unsigned short internalPort;
	char remoteHost[64];
	char internalClient[64];
	char description[64];
	char protocol[4];
	unsigned char enabled;
};

struct PortMappingParserData {
	LIST_HEAD(portmappinglisthead, PortMapping) head;
	portMappingElt curelt;
};

void
ParsePortListing(const char * buffer, int bufsize,
                 struct PortMappingParserData * pdata);

void
FreePortListing(struct PortMappingParserData * pdata);

#ifdef __cplusplus
}
#endif

#endif

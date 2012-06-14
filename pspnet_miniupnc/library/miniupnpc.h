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

/* $Id: miniupnpc.h,v 1.25 2011/10/09 16:17:29 nanard Exp $ */
/* Project: miniupnp
 * http://miniupnp.free.fr/
 * Author: Thomas Bernard
 * Copyright (c) 2005-2011 Thomas Bernard
 * This software is subjects to the conditions detailed
 * in the LICENCE file provided within this distribution */
#ifndef __MINIUPNPC_H__
#define __MINIUPNPC_H__

#include "igd_desc_parse.h"

/* error codes : */
#define UPNPDISCOVER_SUCCESS (0)
#define UPNPDISCOVER_UNKNOWN_ERROR (-1)
#define UPNPDISCOVER_SOCKET_ERROR (-101)
#define UPNPDISCOVER_MEMORY_ERROR (-102)

/* versions : */
#define MINIUPNPC_VERSION	"1.6.20120125"
#define MINIUPNPC_API_VERSION	8

#ifdef __cplusplus
extern "C" {
#endif

/* Structures definitions : */
struct UPNParg { const char * elt; const char * val; };

char *
simpleUPnPcommand(int, const char *, const char *,
                  const char *, struct UPNParg *,
                  int *);

struct UPNPDev {
	struct UPNPDev * pNext;
	char * descURL;
	char * st;
	char buffer[2];
};

/* upnpDiscover()
 * discover UPnP devices on the network.
 * The discovered devices are returned as a chained list.
 * It is up to the caller to free the list with freeUPNPDevlist().
 * delay (in millisecond) is the maximum time for waiting any device
 * response.
 * If available, device list will be obtained from MiniSSDPd.
 * Default path for minissdpd socket will be used if minissdpdsock argument
 * is NULL.
 * If multicastif is not NULL, it will be used instead of the default
 * multicast interface for sending SSDP discover packets.
 * If sameport is not null, SSDP packets will be sent from the source port
 * 1900 (same as destination port) otherwise system assign a source port. */
struct UPNPDev *
upnpDiscover(int delay, const char * multicastif,
             const char * minissdpdsock, int sameport,
             int ipv6,
             int * error);
/* freeUPNPDevlist()
 * free list returned by upnpDiscover() */
void freeUPNPDevlist(struct UPNPDev * devlist);

/* parserootdesc() :
 * parse root XML description of a UPnP device and fill the IGDdatas
 * structure. */
void parserootdesc(const char *, int, struct IGDdatas *);

/* structure used to get fast access to urls
 * controlURL: controlURL of the WANIPConnection
 * ipcondescURL: url of the description of the WANIPConnection
 * controlURL_CIF: controlURL of the WANCommonInterfaceConfig
 * controlURL_6FC: controlURL of the WANIPv6FirewallControl
 */
struct UPNPUrls {
	char * controlURL;
	char * ipcondescURL;
	char * controlURL_CIF;
	char * controlURL_6FC;
	char * rootdescURL;
};

/* UPNP_GetValidIGD() :
 * return values :
 *     0 = NO IGD found
 *     1 = A valid connected IGD has been found
 *     2 = A valid IGD has been found but it reported as
 *         not connected
 *     3 = an UPnP device has been found but was not recognized as an IGD
 *
 * In any non zero return case, the urls and data structures
 * passed as parameters are set. Donc forget to call FreeUPNPUrls(urls) to
 * free allocated memory.
 */
int
UPNP_GetValidIGD(struct UPNPDev * devlist,
                 struct UPNPUrls * urls,
				 struct IGDdatas * data,
				 char * lanaddr, int lanaddrlen);

/* UPNP_GetIGDFromUrl()
 * Used when skipping the discovery process.
 * return value :
 *   0 - Not ok
 *   1 - OK */
int
UPNP_GetIGDFromUrl(const char * rootdescurl,
                   struct UPNPUrls * urls,
                   struct IGDdatas * data,
                   char * lanaddr, int lanaddrlen);

void GetUPNPUrls(struct UPNPUrls *, struct IGDdatas *, const char *);

void FreeUPNPUrls(struct UPNPUrls *);

/* return 0 or 1 */
int UPNPIGD_IsConnected(struct UPNPUrls *, struct IGDdatas *);


#ifdef __cplusplus
}
#endif

#endif


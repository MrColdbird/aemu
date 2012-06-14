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

/* $Id: igd_desc_parse.h,v 1.10 2011/04/11 09:19:24 nanard Exp $ */
/* Project : miniupnp
 * http://miniupnp.free.fr/
 * Author : Thomas Bernard
 * Copyright (c) 2005-2010 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution.
 * */
#ifndef __IGD_DESC_PARSE_H__
#define __IGD_DESC_PARSE_H__

/* Structure to store the result of the parsing of UPnP
 * descriptions of Internet Gateway Devices */
#define MINIUPNPC_URL_MAXSIZE (128)
struct IGDdatas_service {
	char controlurl[MINIUPNPC_URL_MAXSIZE];
	char eventsuburl[MINIUPNPC_URL_MAXSIZE];
	char scpdurl[MINIUPNPC_URL_MAXSIZE];
	char servicetype[MINIUPNPC_URL_MAXSIZE];
	/*char devicetype[MINIUPNPC_URL_MAXSIZE];*/
};

struct IGDdatas {
	char cureltname[MINIUPNPC_URL_MAXSIZE];
	char urlbase[MINIUPNPC_URL_MAXSIZE];
	char presentationurl[MINIUPNPC_URL_MAXSIZE];
	int level;
	/*int state;*/
	/* "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1" */
	struct IGDdatas_service CIF;
	/* "urn:schemas-upnp-org:service:WANIPConnection:1"
	 * "urn:schemas-upnp-org:service:WANPPPConnection:1" */
	struct IGDdatas_service first;
	/* if both WANIPConnection and WANPPPConnection are present */
	struct IGDdatas_service second;
	/* "urn:schemas-upnp-org:service:WANIPv6FirewallControl:1" */
	struct IGDdatas_service IPv6FC;
	/* tmp */
	struct IGDdatas_service tmp;
};

void IGDstartelt(void *, const char *, int);
void IGDendelt(void *, const char *, int);
void IGDdata(void *, const char *, int);
void printIGD(struct IGDdatas *);

#endif


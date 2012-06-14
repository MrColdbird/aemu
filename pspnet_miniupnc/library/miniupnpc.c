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

/* $Id: miniupnpc.c,v 1.102 2012/01/21 13:56:41 nanard Exp $ */
/* Project : miniupnp
 * Web : http://miniupnp.free.fr/
 * Author : Thomas BERNARD
 * copyright (c) 2005-2012 Thomas Bernard
 * This software is subjet to the conditions detailed in the
 * provided LICENSE file. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <poll.h>
#include <strings.h>
#include <errno.h>
*/

#include "common.h"
#include "miniupnpc.h"
#include "minissdpc.h"
#include "miniwget.h"
#include "minisoap.h"
#include "minixml.h"
#include "upnpcommands.h"
#include "connecthostport.h"
#include "receivedata.h"

#define SOAPPREFIX "s"
#define SERVICEPREFIX "u"
#define SERVICEPREFIX2 'u'

/* root description parsing */
void parserootdesc(const char * buffer, int bufsize, struct IGDdatas * data)
{
	struct xmlparser parser;
	/* xmlparser object */
	parser.xmlstart = buffer;
	parser.xmlsize = bufsize;
	parser.data = data;
	parser.starteltfunc = IGDstartelt;
	parser.endeltfunc = IGDendelt;
	parser.datafunc = IGDdata;
	parser.attfunc = 0;
	parsexml(&parser);
}

/* simpleUPnPcommand2 :
 * not so simple !
 * return values :
 *   pointer - OK
 *   NULL - error */
char * simpleUPnPcommand2(int s, const char * url, const char * service,
		       const char * action, struct UPNParg * args,
		       int * bufsize, const char * httpversion)
{
	char hostname[MAXHOSTNAMELEN+1];
	unsigned short port = 0;
	char * path;
	char soapact[128];
	char soapbody[2048];
	char * buf;
    int n;

	*bufsize = 0;
	snprintf(soapact, sizeof(soapact), "%s#%s", service, action);
	if(args==NULL)
	{
		/*soapbodylen = */snprintf(soapbody, sizeof(soapbody),
						"<?xml version=\"1.0\"?>\r\n"
	    	              "<" SOAPPREFIX ":Envelope "
						  "xmlns:" SOAPPREFIX "=\"http://schemas.xmlsoap.org/soap/envelope/\" "
						  SOAPPREFIX ":encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
						  "<" SOAPPREFIX ":Body>"
						  "<" SERVICEPREFIX ":%s xmlns:" SERVICEPREFIX "=\"%s\">"
						  "</" SERVICEPREFIX ":%s>"
						  "</" SOAPPREFIX ":Body></" SOAPPREFIX ":Envelope>"
					 	  "\r\n", action, service, action);
	}
	else
	{
		char * p;
		const char * pe, * pv;
		int soapbodylen;
		soapbodylen = snprintf(soapbody, sizeof(soapbody),
						"<?xml version=\"1.0\"?>\r\n"
	    	            "<" SOAPPREFIX ":Envelope "
						"xmlns:" SOAPPREFIX "=\"http://schemas.xmlsoap.org/soap/envelope/\" "
						SOAPPREFIX ":encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
						"<" SOAPPREFIX ":Body>"
						"<" SERVICEPREFIX ":%s xmlns:" SERVICEPREFIX "=\"%s\">",
						action, service);
		p = soapbody + soapbodylen;
		while(args->elt)
		{
			/* check that we are never overflowing the string... */
			if(soapbody + sizeof(soapbody) <= p + 100)
			{
				/* we keep a margin of at least 100 bytes */
				return NULL;
			}
			*(p++) = '<';
			pe = args->elt;
			while(*pe)
				*(p++) = *(pe++);
			*(p++) = '>';
			if((pv = args->val))
			{
				while(*pv)
					*(p++) = *(pv++);
			}
			*(p++) = '<';
			*(p++) = '/';
			pe = args->elt;
			while(*pe)
				*(p++) = *(pe++);
			*(p++) = '>';
			args++;
		}
		*(p++) = '<';
		*(p++) = '/';
		*(p++) = SERVICEPREFIX2;
		*(p++) = ':';
		pe = action;
		while(*pe)
			*(p++) = *(pe++);
		strncpy(p, "></" SOAPPREFIX ":Body></" SOAPPREFIX ":Envelope>\r\n",
		        soapbody + sizeof(soapbody) - p);
	}
	if(!parseURL(url, hostname, &port, &path)) return NULL;
	if(s<0)
	{
		s = connecthostport(hostname, port);
		if(s < 0)
		{
			return NULL;
		}
	}

	n = soapPostSubmit(s, path, hostname, port, soapact, soapbody, httpversion);
	if(n<=0) {
		sceNetInetClose(s);
		return NULL;
	}

	buf = getHTTPResponse(s, bufsize);

	sceNetInetClose(s);
	return buf;
}

/* simpleUPnPcommand :
 * not so simple !
 * return values :
 *   pointer - OK
 *   NULL    - error */
char * simpleUPnPcommand(int s, const char * url, const char * service,
		       const char * action, struct UPNParg * args,
		       int * bufsize)
{
	char * buf;

	buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.1");
/*
	buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.0");
	if (!buf || *bufsize == 0)
	{
#if DEBUG
	    printf("Error or no result from SOAP request; retrying with HTTP/1.1\n");
#endif
		buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.1");
	}
*/
	return buf;
}

/* parseMSEARCHReply()
 * the last 4 arguments are filled during the parsing :
 *    - location/locationsize : "location:" field of the SSDP reply packet
 *    - st/stsize : "st:" field of the SSDP reply packet.
 * The strings are NOT null terminated */
static void
parseMSEARCHReply(const char * reply, int size,
                  const char * * location, int * locationsize,
			      const char * * st, int * stsize)
{
	int a, b, i;
	i = 0;
	a = i;	/* start of the line */
	b = 0;	/* end of the "header" (position of the colon) */
	while(i<size)
	{
		switch(reply[i])
		{
		case ':':
				if(b==0)
				{
					b = i; /* end of the "header" */
					/*for(j=a; j<b; j++)
					{
						putchar(reply[j]);
					}
					*/
				}
				break;
		case '\x0a':
		case '\x0d':
				if(b!=0)
				{
					/*for(j=b+1; j<i; j++)
					{
						putchar(reply[j]);
					}
					putchar('\n');*/
					/* skip the colon and white spaces */
					do { b++; } while(reply[b]==' ');
					if(0==strncasecmp(reply+a, "location", 8))
					{
						*location = reply+b;
						*locationsize = i-b;
					}
					else if(0==strncasecmp(reply+a, "st", 2))
					{
						*st = reply+b;
						*stsize = i-b;
					}
					b = 0;
				}
				a = i+1;
				break;
		default:
				break;
		}
		i++;
	}
}

/* port upnp discover : SSDP protocol */
#define PORT 1900
#define XSTR(s) STR(s)
#define STR(s) #s
#define UPNP_MCAST_ADDR "239.255.255.250"

/* upnpDiscover() :
 * return a chained list of all devices found or NULL if
 * no devices was found.
 * It is up to the caller to free the chained list
 * delay is in millisecond (poll) */
struct UPNPDev *
upnpDiscover(int delay, const char * multicastif,
             const char * minissdpdsock, int sameport,
             int ipv6, //unused in psp port
             int * error)
{
	struct UPNPDev * tmp;
	struct UPNPDev * devlist = 0;
	int opt = 1;
	static const char MSearchMsgFmt[] = 
	"M-SEARCH * HTTP/1.1\r\n"
	"HOST: %s:" XSTR(PORT) "\r\n"
	"ST: %s\r\n"
	"MAN: \"ssdp:discover\"\r\n"
	"MX: %u\r\n"
	"\r\n";
	static const char * const deviceList[] = {
#if 0
		"urn:schemas-upnp-org:device:InternetGatewayDevice:2",
		"urn:schemas-upnp-org:service:WANIPConnection:2",
#endif
		"urn:schemas-upnp-org:device:InternetGatewayDevice:1",
		"urn:schemas-upnp-org:service:WANIPConnection:1",
		"urn:schemas-upnp-org:service:WANPPPConnection:1",
		"upnp:rootdevice",
		0
	};
	int deviceIndex = 0;
	char bufr[1536];	/* reception and emission buffer */
	int sudp;
	int n;
	SceNetInetSockaddrIn sockudp_r;
	unsigned int mx;
	SceNetInetSockaddrIn sockudp_w;
	int linklocal = 1;

	if(error)
		*error = UPNPDISCOVER_UNKNOWN_ERROR;

	/* fallback to direct discovery */
	//sudp = socket(ipv6 ? PF_INET6 : PF_INET, SOCK_DGRAM, 0);
	sudp = sceNetInetSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sudp < 0)
	{
		if(error)
			*error = UPNPDISCOVER_SOCKET_ERROR;
		return NULL;
	}
	
	/* reception */
	memset(&sockudp_r, 0, sizeof(SceNetInetSockaddrIn));

	SceNetInetSockaddrIn * p = (SceNetInetSockaddrIn *)&sockudp_r;
	p->sin_family = AF_INET;
	if(sameport)
		p->sin_port = sceNetHtons(PORT);
	p->sin_addr = INADDR_ANY;


	// Enable Port Re-use
	sceNetInetSetsockopt(sudp, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	sceNetInetSetsockopt(sudp, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	
	/* Avant d'envoyer le paquet on bind pour recevoir la reponse */
    if (sceNetInetBind(sudp, (const struct SceNetInetSockaddr *)&sockudp_r,
	         sizeof(SceNetInetSockaddrIn)) != 0)
	{
		if(error)
			*error = UPNPDISCOVER_SOCKET_ERROR;
		sceNetInetClose(sudp);
		return NULL;
    }

	if(error)
		*error = UPNPDISCOVER_SUCCESS;
	/* Calculating maximum response time in seconds */
	mx = ((unsigned int)delay) / 1000u;
	/* receiving SSDP response packet */
	for(n = 0; deviceList[deviceIndex]; deviceIndex++)
	{
	if(n == 0)
	{
		/* sending the SSDP M-SEARCH packet */
		n = snprintf(bufr, sizeof(bufr),
		             MSearchMsgFmt,
		             UPNP_MCAST_ADDR,
		             deviceList[deviceIndex], mx);

		/* the following code is not using getaddrinfo */
		/* emission */
		memset(&sockudp_w, 0, sizeof(SceNetInetSockaddrIn));

		SceNetInetSockaddrIn * p = (SceNetInetSockaddrIn *)&sockudp_w;
		p->sin_family = AF_INET;
		p->sin_port = sceNetHtons(PORT);
		sceNetInetInetAton(UPNP_MCAST_ADDR, &p->sin_addr);
		
		n = sceNetInetSendto(sudp, bufr, n, 0,
		           (SceNetInetSockaddr *)&sockudp_w,
		           sizeof(SceNetInetSockaddrIn));
		if (n < 0) {
			if(error)
				*error = UPNPDISCOVER_SOCKET_ERROR;
			break;
		}
	}
	/* Waiting for SSDP REPLY packet to M-SEARCH */
	n = receivedata(sudp, bufr, sizeof(bufr), delay);
	if (n < 0) {
		/* error */
		if(error)
			*error = UPNPDISCOVER_SOCKET_ERROR;
		break;
	} else if (n == 0) {
		/* no data or Time Out */
		if (devlist) {
			/* no more device type to look for... */
			if(error)
				*error = UPNPDISCOVER_SUCCESS;
			break;
		}
		if(ipv6) {
			if(linklocal) {
				linklocal = 0;
				--deviceIndex;
			} else {
				linklocal = 1;
			}
		}
	} else {
		const char * descURL=NULL;
		int urlsize=0;
		const char * st=NULL;
		int stsize=0;
        /*printf("%d byte(s) :\n%s\n", n, bufr);*/ /* affichage du message */
		parseMSEARCHReply(bufr, n, &descURL, &urlsize, &st, &stsize);
		if(st&&descURL)
		{
			for(tmp=devlist; tmp; tmp = tmp->pNext) {
				if(memcmp(tmp->descURL, descURL, urlsize) == 0 &&
				   tmp->descURL[urlsize] == '\0' &&
				   memcmp(tmp->st, st, stsize) == 0 &&
				   tmp->st[stsize] == '\0')
					break;
			}
			/* at the exit of the loop above, tmp is null if
			 * no duplicate device was found */
			if(tmp)
				continue;
			tmp = (struct UPNPDev *)malloc(sizeof(struct UPNPDev)+urlsize+stsize);
			if(!tmp) {
				/* memory allocation error */
				if(error)
					*error = UPNPDISCOVER_MEMORY_ERROR;
				break;
			}
			tmp->pNext = devlist;
			tmp->descURL = tmp->buffer;
			tmp->st = tmp->buffer + 1 + urlsize;
			memcpy(tmp->buffer, descURL, urlsize);
			tmp->buffer[urlsize] = '\0';
			memcpy(tmp->buffer + urlsize + 1, st, stsize);
			tmp->buffer[urlsize+1+stsize] = '\0';
			devlist = tmp;
		}
	}
	}
	sceNetInetClose(sudp);
	return devlist;
}

/* freeUPNPDevlist() should be used to
 * free the chained list returned by upnpDiscover() */
void freeUPNPDevlist(struct UPNPDev * devlist)
{
	struct UPNPDev * next;
	while(devlist)
	{
		next = devlist->pNext;
		free(devlist);
		devlist = next;
	}
}

static void
url_cpy_or_cat(char * dst, const char * src, int n)
{
	if(  (src[0] == 'h')
	   &&(src[1] == 't')
	   &&(src[2] == 't')
	   &&(src[3] == 'p')
	   &&(src[4] == ':')
	   &&(src[5] == '/')
	   &&(src[6] == '/'))
	{
		strncpy(dst, src, n);
	}
	else
	{
		int l = strlen(dst);
		if(src[0] != '/')
			dst[l++] = '/';
		if(l<=n)
			strncpy(dst + l, src, n - l);
	}
}

/* Prepare the Urls for usage...
 */
void GetUPNPUrls(struct UPNPUrls * urls, struct IGDdatas * data,
                 const char * descURL)
{
	char * p;
	int n1, n2, n3, n4;

	n1 = strlen(data->urlbase);
	if(n1==0)
		n1 = strlen(descURL);
	n1 += 2;	/* 1 byte more for Null terminator, 1 byte for '/' if needed */
	n2 = n1; n3 = n1; n4 = n1;
	n1 += strlen(data->first.scpdurl);
	n2 += strlen(data->first.controlurl);
	n3 += strlen(data->CIF.controlurl);
	n4 += strlen(data->IPv6FC.controlurl);

	/* allocate memory to store URLs */
	urls->ipcondescURL = (char *)malloc(n1);
	urls->controlURL = (char *)malloc(n2);
	urls->controlURL_CIF = (char *)malloc(n3);
	urls->controlURL_6FC = (char *)malloc(n4);

	/* strdup descURL */
	urls->rootdescURL = strdup(descURL);

	/* get description of WANIPConnection */
	if(data->urlbase[0] != '\0')
		strncpy(urls->ipcondescURL, data->urlbase, n1);
	else
		strncpy(urls->ipcondescURL, descURL, n1);
	p = strchr(urls->ipcondescURL+7, '/');
	if(p) p[0] = '\0';
	strncpy(urls->controlURL, urls->ipcondescURL, n2);
	strncpy(urls->controlURL_CIF, urls->ipcondescURL, n3);
	strncpy(urls->controlURL_6FC, urls->ipcondescURL, n4);
	
	url_cpy_or_cat(urls->ipcondescURL, data->first.scpdurl, n1);

	url_cpy_or_cat(urls->controlURL, data->first.controlurl, n2);

	url_cpy_or_cat(urls->controlURL_CIF, data->CIF.controlurl, n3);

	url_cpy_or_cat(urls->controlURL_6FC, data->IPv6FC.controlurl, n4);

#ifdef DEBUG
	printk("urls->ipcondescURL='%s' %u n1=%d\n", urls->ipcondescURL,
	       (unsigned)strlen(urls->ipcondescURL), n1);
	printk("urls->controlURL='%s' %u n2=%d\n", urls->controlURL,
	       (unsigned)strlen(urls->controlURL), n2);
	printk("urls->controlURL_CIF='%s' %u n3=%d\n", urls->controlURL_CIF,
	       (unsigned)strlen(urls->controlURL_CIF), n3);
	printk("urls->controlURL_6FC='%s' %u n4=%d\n", urls->controlURL_6FC,
	       (unsigned)strlen(urls->controlURL_6FC), n4);
#endif
}

void
FreeUPNPUrls(struct UPNPUrls * urls)
{
	if(!urls)
		return;
	free(urls->controlURL);
	urls->controlURL = 0;
	free(urls->ipcondescURL);
	urls->ipcondescURL = 0;
	free(urls->controlURL_CIF);
	urls->controlURL_CIF = 0;
	free(urls->controlURL_6FC);
	urls->controlURL_6FC = 0;
	free(urls->rootdescURL);
	urls->rootdescURL = 0;
}

int
UPNPIGD_IsConnected(struct UPNPUrls * urls, struct IGDdatas * data)
{
	char status[64];
	unsigned int uptime;
	status[0] = '\0';
	UPNP_GetStatusInfo(urls->controlURL, data->first.servicetype,
	                   status, &uptime, NULL);
	if(0 == strcmp("Connected", status))
	{
		return 1;
	}
	else
		return 0;
}


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
				 char * lanaddr, int lanaddrlen)
{
	char * descXML;
	int descXMLsize = 0;
	struct UPNPDev * dev;
	int ndev = 0;
	int state; /* state 1 : IGD connected. State 2 : IGD. State 3 : anything */
	if(!devlist)
	{
#ifdef DEBUG
		printk("Empty devlist\n");
#endif
		return 0;
	}
	for(state = 1; state <= 3; state++)
	{
		for(dev = devlist; dev; dev = dev->pNext)
		{
			/* we should choose an internet gateway device.
		 	* with st == urn:schemas-upnp-org:device:InternetGatewayDevice:1 */
			descXML = miniwget_getaddr(dev->descURL, &descXMLsize,
			   	                        lanaddr, lanaddrlen);
			if(descXML)
			{
				ndev++;
				memset(data, 0, sizeof(struct IGDdatas));
				memset(urls, 0, sizeof(struct UPNPUrls));
				parserootdesc(descXML, descXMLsize, data);
				free(descXML);
				descXML = NULL;
				if(0==strcmp(data->CIF.servicetype,
				   "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1")
				   || state >= 3 )
				{
				  GetUPNPUrls(urls, data, dev->descURL);

#ifdef DEBUG
				  printk("UPNPIGD_IsConnected(%s) = %d\n",
				     urls->controlURL,
			         UPNPIGD_IsConnected(urls, data));
#endif
				  if((state >= 2) || UPNPIGD_IsConnected(urls, data))
					return state;
				  FreeUPNPUrls(urls);
				  if(data->second.servicetype[0] != '\0') {
#ifdef DEBUG
				    printk("We tried %s, now we try %s !\n",
				           data->first.servicetype, data->second.servicetype);
#endif
				    /* swaping WANPPPConnection and WANIPConnection ! */
				    memcpy(&data->tmp, &data->first, sizeof(struct IGDdatas_service));
				    memcpy(&data->first, &data->second, sizeof(struct IGDdatas_service));
				    memcpy(&data->second, &data->tmp, sizeof(struct IGDdatas_service));
				    GetUPNPUrls(urls, data, dev->descURL);
#ifdef DEBUG
				    printk("UPNPIGD_IsConnected(%s) = %d\n",
				       urls->controlURL,
			           UPNPIGD_IsConnected(urls, data));
#endif
				    if((state >= 2) || UPNPIGD_IsConnected(urls, data))
					  return state;
				    FreeUPNPUrls(urls);
				  }
				}
				memset(data, 0, sizeof(struct IGDdatas));
			}
#ifdef DEBUG
			else
			{
				printk("error getting XML description %s\n", dev->descURL);
			}
#endif
		}
	}
	return 0;
}

/* UPNP_GetIGDFromUrl()
 * Used when skipping the discovery process.
 * return value :
 *   0 - Not ok
 *   1 - OK */
int
UPNP_GetIGDFromUrl(const char * rootdescurl,
                   struct UPNPUrls * urls,
                   struct IGDdatas * data,
                   char * lanaddr, int lanaddrlen)
{
	char * descXML;
	int descXMLsize = 0;
	descXML = miniwget_getaddr(rootdescurl, &descXMLsize,
	   	                       lanaddr, lanaddrlen);
	if(descXML) {
		memset(data, 0, sizeof(struct IGDdatas));
		memset(urls, 0, sizeof(struct UPNPUrls));
		parserootdesc(descXML, descXMLsize, data);
		free(descXML);
		descXML = NULL;
		GetUPNPUrls(urls, data, rootdescurl);
		return 1;
	} else {
		return 0;
	}
}


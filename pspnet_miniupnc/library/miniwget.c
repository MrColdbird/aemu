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

/* $Id: miniwget.c,v 1.54 2012/01/21 13:30:32 nanard Exp $ */
/* Project : miniupnp
 * Website : http://miniupnp.free.fr/
 * Author : Thomas Bernard
 * Copyright (c) 2005-2012 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution. */
 
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
*/

/* defining MINIUPNPC_IGNORE_EINTR enable the ignore of interruptions
 * during the connect() call */
#define MINIUPNPC_IGNORE_EINTR
#if defined(__sun) || defined(sun)
#define MIN(x,y) (((x)<(y))?(x):(y))
#endif

#include "common.h"
#include "miniupnpcstrings.h"
#include "miniwget.h"
#include "connecthostport.h"
#include "receivedata.h"

/*
 * Read a HTTP response from a socket.
 * Process Content-Length and Transfer-encoding headers.
 * return a pointer to the content buffer, which length is saved
 * to the length parameter.
 */
void *
getHTTPResponse(int s, int * size)
{
	char buf[2048];
	int n;
	int endofheaders = 0;
	int chunked = 0;
	int content_length = -1;
	unsigned int chunksize = 0;
	unsigned int bytestocopy = 0;
	/* buffers : */
	char * header_buf;
	int header_buf_len = 2048;
	int header_buf_used = 0;
	char * content_buf;
	int content_buf_len = 2048;
	int content_buf_used = 0;
	char chunksize_buf[32];
	int chunksize_buf_index;

	header_buf = malloc(header_buf_len);
	content_buf = malloc(content_buf_len);
	chunksize_buf[0] = '\0';
	chunksize_buf_index = 0;

	while((n = receivedata(s, buf, 2048, 5000)) > 0)
	{
		if(endofheaders == 0)
		{
			int i;
			int linestart=0;
			int colon=0;
			int valuestart=0;
			if(header_buf_used + n > header_buf_len) {
				header_buf = realloc(header_buf, header_buf_used + n);
				header_buf_len = header_buf_used + n;
			}
			memcpy(header_buf + header_buf_used, buf, n);
			header_buf_used += n;
			/* search for CR LF CR LF (end of headers)
			 * recognize also LF LF */
			i = 0;
			while(i < (header_buf_used-1) && (endofheaders == 0)) {
				if(header_buf[i] == '\r') {
					i++;
					if(header_buf[i] == '\n') {
						i++;
						if(i < header_buf_used && header_buf[i] == '\r') {
							i++;
							if(i < header_buf_used && header_buf[i] == '\n') {
								endofheaders = i+1;
							}
						}
					}
				} else if(header_buf[i] == '\n') {
					i++;
					if(header_buf[i] == '\n') {
						endofheaders = i+1;
					}
				}
				i++;
			}
			if(endofheaders == 0)
				continue;
			/* parse header lines */
			for(i = 0; i < endofheaders - 1; i++) {
				if(colon <= linestart && header_buf[i]==':')
				{
					colon = i;
					while(i < (endofheaders-1)
					      && (header_buf[i+1] == ' ' || header_buf[i+1] == '\t'))
						i++;
					valuestart = i + 1;
				}
				/* detecting end of line */
				else if(header_buf[i]=='\r' || header_buf[i]=='\n')
				{
					if(colon > linestart && valuestart > colon)
					{
						if(0==strncasecmp(header_buf+linestart, "content-length", colon-linestart))
						{
							content_length = atoi(header_buf+valuestart);
						}
						else if(0==strncasecmp(header_buf+linestart, "transfer-encoding", colon-linestart)
						   && 0==strncasecmp(header_buf+valuestart, "chunked", 7))
						{
							chunked = 1;
						}
					}
					while(header_buf[i]=='\r' || header_buf[i] == '\n')
						i++;
					linestart = i;
					colon = linestart;
					valuestart = 0;
				} 
			}
			/* copy the remaining of the received data back to buf */
			n = header_buf_used - endofheaders;
			memcpy(buf, header_buf + endofheaders, n);
			/* if(headers) */
		}
		if(endofheaders)
		{
			/* content */
			if(chunked)
			{
				int i = 0;
				while(i < n)
				{
					if(chunksize == 0)
					{
						/* reading chunk size */
						if(chunksize_buf_index == 0) {
							/* skipping any leading CR LF */
							if(i<n && buf[i] == '\r') i++;
							if(i<n && buf[i] == '\n') i++;
						}
						while(i<n && isxdigit(buf[i])
						     && chunksize_buf_index < (sizeof(chunksize_buf)-1))
						{
							chunksize_buf[chunksize_buf_index++] = buf[i];
							chunksize_buf[chunksize_buf_index] = '\0';
							i++;
						}
						while(i<n && buf[i] != '\r' && buf[i] != '\n')
							i++; /* discarding chunk-extension */
						if(i<n && buf[i] == '\r') i++;
						if(i<n && buf[i] == '\n') {
							int j;
							for(j = 0; j < chunksize_buf_index; j++) {
							if(chunksize_buf[j] >= '0'
							   && chunksize_buf[j] <= '9')
								chunksize = (chunksize << 4) + (chunksize_buf[j] - '0');
							else
								chunksize = (chunksize << 4) + ((chunksize_buf[j] | 32) - 'a' + 10);
							}
							chunksize_buf[0] = '\0';
							chunksize_buf_index = 0;
							i++;
						} else {
							/* not finished to get chunksize */
							continue;
						}
						if(chunksize == 0)
						{
							goto end_of_stream;
						}
					}
					bytestocopy = ((int)chunksize < n - i)?chunksize:(n - i);
					if((int)(content_buf_used + bytestocopy) > content_buf_len)
					{
						if(content_length >= content_buf_used + (int)bytestocopy) {
							content_buf_len = content_length;
						} else {
							content_buf_len = content_buf_used + (int)bytestocopy;
						}
						content_buf = (char *)realloc((void *)content_buf, 
						                              content_buf_len);
					}
					memcpy(content_buf + content_buf_used, buf + i, bytestocopy);
					content_buf_used += bytestocopy;
					i += bytestocopy;
					chunksize -= bytestocopy;
				}
			}
			else
			{
				/* not chunked */
				if(content_length > 0
				   && (content_buf_used + n) > content_length) {
					/* skipping additional bytes */
					n = content_length - content_buf_used;
				}
				if(content_buf_used + n > content_buf_len)
				{
					if(content_length >= content_buf_used + n) {
						content_buf_len = content_length;
					} else {
						content_buf_len = content_buf_used + n;
					}
					content_buf = (char *)realloc((void *)content_buf, 
					                              content_buf_len);
				}
				memcpy(content_buf + content_buf_used, buf, n);
				content_buf_used += n;
			}
		}
		/* use the Content-Length header value if available */
		if(content_length > 0 && content_buf_used >= content_length)
		{
			break;
		}
	}
end_of_stream:
	free(header_buf); header_buf = NULL;
	*size = content_buf_used;
	if(content_buf_used == 0)
	{
		free(content_buf);
		content_buf = NULL;
	}
	return content_buf;
}

/* miniwget3() :
 * do all the work.
 * Return NULL if something failed. */
static void *
miniwget3(const char * url, const char * host,
          unsigned short port, const char * path,
          int * size, char * addr_str, int addr_str_len,
          const char * httpversion)
{
	char buf[2048];
    int s;
	int n;
	int len;
	int sent;
	void * content;

	*size = 0;
	s = connecthostport(host, port);
	if(s < 0)
		return NULL;

	len = snprintf(buf, sizeof(buf),
                 "GET %s HTTP/%s\r\n"
			     "Host: %s:%d\r\n"
				 "Connection: Close\r\n"
				 "User-Agent: " OS_STRING ", UPnP/1.0, MiniUPnPc/" MINIUPNPC_VERSION_STRING "\r\n"

				 "\r\n",
			   path, httpversion, host, port);
	sent = 0;
	/* sending the HTTP request */
	while(sent < len)
	{
		n = sceNetInetSend(s, buf+sent, len-sent, 0);
		if(n < 0)
		{
			sceNetInetClose(s);
			return NULL;
		}
		else
		{
			sent += n;
		}
	}
	content = getHTTPResponse(s, size);
	sceNetInetClose(s);
	return content;
}

/* miniwget2() :
 * Call miniwget3(); retry with HTTP/1.1 if 1.0 fails. */
static void *
miniwget2(const char * url, const char * host,
		  unsigned short port, const char * path,
		  int * size, char * addr_str, int addr_str_len)
{
	char * respbuffer;

	respbuffer = miniwget3(url, host, port, path, size, addr_str, addr_str_len, "1.1");
/*
	respbuffer = miniwget3(url, host, port, path, size, addr_str, addr_str_len, "1.0");
	if (*size == 0)
	{
#ifdef DEBUG
		printk("Retrying with HTTP/1.1\n");
#endif
		free(respbuffer);
		respbuffer = miniwget3(url, host, port, path, size, addr_str, addr_str_len, "1.1");
	}
*/
	return respbuffer;
}


static inline int MIN(int a, int b) { if(a < b) return a; return b; }

/* parseURL()
 * arguments :
 *   url :		source string not modified
 *   hostname :	hostname destination string (size of MAXHOSTNAMELEN+1)
 *   port :		port (destination)
 *   path :		pointer to the path part of the URL 
 *
 * Return values :
 *    0 - Failure
 *    1 - Success         */
int parseURL(const char * url, char * hostname, unsigned short * port, char * * path)
{
	char * p1, *p2, *p3;
	if(!url)
		return 0;
	p1 = strstr(url, "://");
	if(!p1)
		return 0;
	p1 += 3;
	if(  (url[0]!='h') || (url[1]!='t')
	   ||(url[2]!='t') || (url[3]!='p'))
		return 0;
	memset(hostname, 0, MAXHOSTNAMELEN + 1);
	if(*p1 == '[')
	{
		/* IP v6 : http://[2a00:1450:8002::6a]/path/abc */
		p2 = strchr(p1, ']');
		p3 = strchr(p1, '/');
		if(p2 && p3)
		{
			p2++;
			strncpy(hostname, p1, MIN(MAXHOSTNAMELEN, (int)(p2-p1)));
			if(*p2 == ':')
			{
				*port = 0;
				p2++;
				while( (*p2 >= '0') && (*p2 <= '9'))
				{
					*port *= 10;
					*port += (unsigned short)(*p2 - '0');
					p2++;
				}
			}
			else
			{
				*port = 80;
			}
			*path = p3;
			return 1;
		}
	}
	p2 = strchr(p1, ':');
	p3 = strchr(p1, '/');
	if(!p3)
		return 0;
	if(!p2 || (p2>p3))
	{
		strncpy(hostname, p1, MIN(MAXHOSTNAMELEN, (int)(p3-p1)));
		*port = 80;
	}
	else
	{
		strncpy(hostname, p1, MIN(MAXHOSTNAMELEN, (int)(p2-p1)));
		*port = 0;
		p2++;
		while( (*p2 >= '0') && (*p2 <= '9'))
		{
			*port *= 10;
			*port += (unsigned short)(*p2 - '0');
			p2++;
		}
	}
	*path = p3;
	return 1;
}

void * miniwget(const char * url, int * size)
{
	unsigned short port;
	char * path;
	/* protocol://host:port/chemin */
	char hostname[MAXHOSTNAMELEN+1];
	*size = 0;
	if(!parseURL(url, hostname, &port, &path))
		return NULL;
#ifdef DEBUG
	printk("parsed url : hostname='%s' port=%hu path='%s'\n", hostname, port, path);
#endif
	return miniwget2(url, hostname, port, path, size, 0, 0);
}

void * miniwget_getaddr(const char * url, int * size, char * addr, int addrlen)
{
	unsigned short port;
	char * path;
	/* protocol://host:port/path */
	char hostname[MAXHOSTNAMELEN+1];
	*size = 0;
	if(addr)
		addr[0] = '\0';
	if(!parseURL(url, hostname, &port, &path))
		return NULL;
#ifdef DEBUG
	printk("parsed url : hostname='%s' port=%hu path='%s'\n", hostname, port, path);
#endif
	return miniwget2(url, hostname, port, path, size, addr, addrlen);
}


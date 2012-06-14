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

/* $Id: upnpreplyparse.h,v 1.12 2012/01/21 13:30:33 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2012 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __UPNPREPLYPARSE_H__
#define __UPNPREPLYPARSE_H__

#if defined(NO_SYS_QUEUE_H) || defined(_WIN32) || defined(__HAIKU__) 
#include "bsdqueue.h"
#else
#include <sys/queue.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct NameValue {
    LIST_ENTRY(NameValue) entries;
    char name[64];
    char value[64];
};

struct NameValueParserData {
    LIST_HEAD(listhead, NameValue) head;
    char curelt[64];
	char * portListing;
	int portListingLength;
};

/* ParseNameValue() */
void
ParseNameValue(const char * buffer, int bufsize,
               struct NameValueParserData * data);

/* ClearNameValueList() */
void
ClearNameValueList(struct NameValueParserData * pdata);

/* GetValueFromNameValueList() */
char *
GetValueFromNameValueList(struct NameValueParserData * pdata,
                          const char * Name);

/* GetValueFromNameValueListIgnoreNS() */
char *
GetValueFromNameValueListIgnoreNS(struct NameValueParserData * pdata,
                                  const char * Name);

/* DisplayNameValueList() */
#ifdef DEBUG
void
DisplayNameValueList(char * buffer, int bufsize);
#endif

#ifdef __cplusplus
}
#endif

#endif


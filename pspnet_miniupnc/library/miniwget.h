/* $Id: miniwget.h,v 1.6 2010/12/09 16:11:33 nanard Exp $ */
/* Project : miniupnp
 * Author : Thomas Bernard
 * Copyright (c) 2005 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution.
 * */
#ifndef __MINIWGET_H__
#define __MINIWGET_H__

#ifdef __cplusplus
extern "C" {
#endif

void * getHTTPResponse(int s, int * size);

void * miniwget(const char *, int *);

void * miniwget_getaddr(const char *, int *, char *, int);

int parseURL(const char *, char *, unsigned short *, char * *);

#ifdef __cplusplus
}
#endif

#endif


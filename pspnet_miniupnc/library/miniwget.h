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


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

/* $Id: minixml.h,v 1.6 2006/11/30 11:47:21 nanard Exp $ */
/* minimal xml parser
 *
 * Project : miniupnp
 * Website : http://miniupnp.free.fr/
 * Author : Thomas Bernard
 * Copyright (c) 2005 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution.
 * */
#ifndef __MINIXML_H__
#define __MINIXML_H__
#define IS_WHITE_SPACE(c) ((c==' ') || (c=='\t') || (c=='\r') || (c=='\n'))

/* if a callback function pointer is set to NULL,
 * the function is not called */
struct xmlparser {
	const char *xmlstart;
	const char *xmlend;
	const char *xml;	/* pointer to current character */
	int xmlsize;
	void * data;
	void (*starteltfunc) (void *, const char *, int);
	void (*endeltfunc) (void *, const char *, int);
	void (*datafunc) (void *, const char *, int);
	void (*attfunc) (void *, const char *, int, const char *, int);
};

/* parsexml()
 * the xmlparser structure must be initialized before the call
 * the following structure members have to be initialized :
 * xmlstart, xmlsize, data, *func
 * xml is for internal usage, xmlend is computed automatically */
void parsexml(struct xmlparser *);

#endif


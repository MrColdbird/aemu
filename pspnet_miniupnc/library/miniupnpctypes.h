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

/* $Id: miniupnpctypes.h,v 1.1 2011/02/15 11:10:40 nanard Exp $ */
/* Miniupnp project : http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org
 * Author : Thomas Bernard
 * Copyright (c) 2011 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided within this distribution */
#ifndef __MINIUPNPCTYPES_H__
#define __MINIUPNPCTYPES_H__

#if (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L)
#define UNSIGNED_INTEGER unsigned long long
#define STRTOUI	strtoull
#else
#define UNSIGNED_INTEGER unsigned int
#define STRTOUI	strtoul
#endif

#endif


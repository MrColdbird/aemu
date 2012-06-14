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

/* $Id: connecthostport.h,v 1.1 2010/04/04 23:21:03 nanard Exp $ */
/* Project: miniupnp
 * http://miniupnp.free.fr/
 * Author: Thomas Bernard
 * Copyright (c) 2010 Thomas Bernard
 * This software is subjects to the conditions detailed
 * in the LICENCE file provided within this distribution */
#ifndef __CONNECTHOSTPORT_H__
#define __CONNECTHOSTPORT_H__

/* connecthostport()
 * return a socket connected (TCP) to the host and port
 * or -1 in case of error */
int connecthostport(const char * host, unsigned short port);

#endif


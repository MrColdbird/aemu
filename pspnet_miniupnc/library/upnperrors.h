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

/* $Id: upnperrors.h,v 1.2 2008/07/02 23:31:15 nanard Exp $ */
/* (c) 2007 Thomas Bernard
 * All rights reserved.
 * MiniUPnP Project.
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * This software is subjet to the conditions detailed in the
 * provided LICENCE file. */
#ifndef __UPNPERRORS_H__
#define __UPNPERRORS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* strupnperror()
 * Return a string description of the UPnP error code 
 * or NULL for undefinded errors */
const char * strupnperror(int err);

#ifdef __cplusplus
}
#endif

#endif

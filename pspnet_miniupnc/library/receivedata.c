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

/* $Id: receivedata.c,v 1.2 2012/01/21 13:30:33 nanard Exp $ */
/* Project : miniupnp
 * Website : http://miniupnp.free.fr/
 * Author : Thomas Bernard
 * Copyright (c) 2011-2012 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution. */

/*
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>
*/

#define MINIUPNPC_IGNORE_EINTR

#define PRINT_SOCKET_ERROR(x) perror(x)

#include "common.h"
#include "receivedata.h"

int
receivedata(int socket, char * data, int length, int timeout)
{
    int n;
	/* using poll */
    SceNetInetPollfd fds[1]; /* for the poll */
    //do {
        fds[0].fd = socket;
        fds[0].events = INET_POLLRDNORM;
        n = sceNetInetPoll(fds, 1, timeout);
    //} while(n < 0 && sceNetInetGetErrno() == EINTR);
    if(n < 0) {
        return -1;
    } else if(n == 0) {
		/* timeout */
        return 0;
    }

	n = sceNetInetRecv(socket, data, length, 0);
	return n;
}



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



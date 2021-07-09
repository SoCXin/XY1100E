/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - xiny_initial API and implementation
 *    
 *******************************************************************************/

#ifndef xiny_CONNECTION_H_
#define xiny_CONNECTION_H_

#include <stdio.h>
#include <unistd.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <sys/stat.h>
#include "xiny_liblwm2m.h"

#define xiny_LWM2M_STANDARD_PORT_STR "5683"
#define xiny_LWM2M_STANDARD_PORT      5683
#define xiny_LWM2M_DTLS_PORT_STR     "5684"
#define xiny_LWM2M_DTLS_PORT          5684
#define xiny_LWM2M_BSSERVER_PORT_STR "5685"
#define xiny_LWM2M_BSSERVER_PORT      5685

typedef struct _xiny_connection_t
{
    struct _xiny_connection_t *  next;
    int                     sock;
    struct sockaddr         addr;
    size_t                  addrLen;
} xiny_connection_t;

int xiny_create_socket(const char * portStr, int ai_family);

xiny_connection_t * xiny_connection_find(xiny_connection_t * connList, struct sockaddr * addr, size_t addrLen);
xiny_connection_t * xiny_connection_new_incoming(xiny_connection_t * connList, int sock, struct sockaddr * addr, size_t addrLen);
xiny_connection_t * xiny_connection_create(xiny_connection_t * connList, int sock, char * host, char * port, int addressFamily);

void xiny_connection_free(xiny_connection_t * connList);

int xiny_connection_send(xiny_connection_t *connP, uint8_t * buffer, size_t length);

#endif
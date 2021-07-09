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
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xiny_connection.h"

#ifdef WITH_LOGS
// from commandline.c
extern void output_buffer(FILE * stream, uint8_t * buffer, int length, int indent);
#endif

int xiny_create_socket(const char * portStr, int addressFamily)
{
    int s = -1;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (0 != getaddrinfo(NULL, portStr, &hints, &res))
    {
        return -1;
    }

    for(p = res ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            if (-1 == bind(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }

    freeaddrinfo(res);

    return s;
}

xiny_connection_t * xiny_connection_find(xiny_connection_t * connList,
                               struct sockaddr * addr,
                               size_t addrLen)
{
    xiny_connection_t * connP;

    connP = connList;
    while (connP != NULL)
    {
        if ((connP->addrLen == addrLen)
         && (memcmp(&(connP->addr), addr, addrLen) == 0))
        {
            return connP;
        }
        connP = connP->next;
    }

    return connP;
}

xiny_connection_t * xiny_connection_new_incoming(xiny_connection_t * connList,
                                       int sock,
                                       struct sockaddr * addr,
                                       size_t addrLen)
{
    xiny_connection_t * connP;

    connP = (xiny_connection_t *)malloc(sizeof(xiny_connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = connList;
    }

    return connP;
}

xiny_connection_t * xiny_connection_create(xiny_connection_t * connList,
                                 int sock,
                                 char * host,
                                 char * port,
                                 int addressFamily)
{
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
    struct sockaddr *sa = NULL;
    socklen_t sl;
    xiny_connection_t * connP = NULL;

    (void) sl;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = addressFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (0 != getaddrinfo(host, port, &hints, &servinfo) || servinfo == NULL) return NULL;
    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
            sl = p->ai_addrlen;
            if (-1 == connect(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }
    if (s >= 0)
    {
        connP = xiny_connection_new_incoming(connList, sock, sa, sa->sa_len);
        close(s);
    }
    if (NULL != servinfo) {
        free(servinfo);
    }
    return connP;
}

void xiny_connection_free(xiny_connection_t * connList)
{
    while (connList != NULL)
    {
        xiny_connection_t * nextP;

        nextP = connList->next;
        free(connList);

        connList = nextP;
    }
}

int xiny_connection_send(xiny_connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    int nbSent;
    size_t offset;

#ifdef WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port;

    s[0] = 0;

    if (AF_INET == connP->addr.sin6_family)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (AF_INET6 == connP->addr.sin6_family)
    {
        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    }

    softap_printf(USER_LOG, WARN_LOG, "Sending %lu bytes to [%s]:%hu\r\n", length, s, ntohs(port));

    output_buffer(stderr, buffer, length, 0);
#endif

    offset = 0;
    while (offset != length)
    {
        nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
    }
    return 0;
}

uint8_t xiny_lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          size_t length,
                          void * userdata)
{
	(void) userdata;

    xiny_connection_t * connP = (xiny_connection_t*) sessionH;

    if (connP == NULL)
    {
        softap_printf(USER_LOG, WARN_LOG, "#> failed sending %lu bytes, missing connection\r\n", length);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == xiny_connection_send(connP, buffer, length))
    {
        softap_printf(USER_LOG, WARN_LOG, "#> failed sending %lu bytes\r\n", length);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return xiny_COAP_NO_ERROR;
}

bool xiny_lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
	(void) userData;

    return (session1 == session2);
}
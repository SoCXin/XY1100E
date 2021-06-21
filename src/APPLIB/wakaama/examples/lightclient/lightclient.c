/*******************************************************************************
 *
 * Copyright (c) 2013, 2014, 2015 Intel Corporation and others.
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
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>
 Bosch Software Innovations GmbH - Please refer to git log

*/
#include "xy_utils.h"
#include "softap_macro.h"
#include "xiny_liblwm2m.h"
#include "xiny_connection.h"
#include "xy_system.h"


#if WAKAAMA
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#if !IS_DSP_CORE
#include <sys/select.h>
#include "xy_net_api.h"
#endif
#include <sys/types.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>



extern xiny_lwm2m_object_t * xiny_get_object_device(void);
extern void xiny_free_object_device(xiny_lwm2m_object_t * objectP);
extern xiny_lwm2m_object_t * xiny_get_server_object(void);
extern void free_server_object(xiny_lwm2m_object_t * object);
extern xiny_lwm2m_object_t * xiny_get_security_object(void);
extern void free_security_object(xiny_lwm2m_object_t * objectP);
extern char * get_server_uri(xiny_lwm2m_object_t * objectP, uint16_t secObjInstID);
extern xiny_lwm2m_object_t * get_test_object(void);
extern void free_test_object(xiny_lwm2m_object_t * object);

#define MAX_PACKET_SIZE 1024

osThreadId_t  g_lightclient_handle =NULL;

int g_xiny_reboot = 0;
static int g_quit = 0;

typedef struct
{
	xiny_lwm2m_object_t * securityObjP;
    int sock;
    xiny_lwm2m_object_t * connList;
    int addressFamily;
} client_data_t;

void * xiny_lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    xiny_lwm2m_object_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

    softap_printf(USER_LOG, WARN_LOG, "Connecting to %s\r\n", uri);

    // parse uri in the form "coaps://[host]:[port]"
    if (0 == strncmp(uri, "coaps://", strlen("coaps://")))
    {
        host = uri+strlen("coaps://");
    }
    else if (0 == strncmp(uri, "coap://", strlen("coap://")))
    {
        host = uri+strlen("coap://");
    }
    else
    {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    newConnP = (xiny_lwm2m_object_t *)(xiny_connection_create((xiny_connection_t *)(dataP->connList), dataP->sock, host, port, dataP->addressFamily));
    if (newConnP == NULL) {
    	softap_printf(USER_LOG, WARN_LOG, "Connection creation failed.\r\n");
    }
    else {
        dataP->connList = newConnP;
    }

exit:
    xiny_lwm2m_free(uri);
    return (void *)newConnP;
}

void xiny_lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    xiny_lwm2m_object_t * targetP;

    app_data = (client_data_t *)userData;
    targetP = (xiny_lwm2m_object_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        xiny_lwm2m_free(targetP);
    }
    else
    {
        xiny_connection_t * parentP;

        parentP = (xiny_connection_t *)(app_data->connList);
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = (struct _xiny_connection_t *)(targetP->next);
            xiny_lwm2m_free(targetP);
        }
    }
}

void print_state(xiny_lwm2m_context_t * lwm2mH)
{
    xiny_lwm2m_server_lwm2m_t * targetP;

    softap_printf(USER_LOG, WARN_LOG, "State: ");
    switch(lwm2mH->state)
    {
    case xiny_STATE_INITIAL:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_INITIAL");
        break;
    case xiny_STATE_BOOTSTRAP_REQUIRED:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_BOOTSTRAP_REQUIRED");
        break;
    case xiny_STATE_BOOTSTRAPPING:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_BOOTSTRAPPING");
        break;
    case xiny_STATE_REGISTER_REQUIRED:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_REGISTER_REQUIRED");
        break;
    case xiny_STATE_REGISTERING:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_REGISTERING");
        break;
    case xiny_STATE_READY:
    	softap_printf(USER_LOG, WARN_LOG, "xiny_STATE_READY");
        break;
    default:
    	softap_printf(USER_LOG, WARN_LOG, "Unknown !");
        break;
    }
    softap_printf(USER_LOG, WARN_LOG, "\r\n");

    targetP = lwm2mH->xiny_bootstrapServerList;

    if (lwm2mH->xiny_bootstrapServerList == NULL)
    {
    	softap_printf(USER_LOG, WARN_LOG, "No Bootstrap Server.\r\n");
    }
    else
    {
    	softap_printf(USER_LOG, WARN_LOG, "Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->xiny_bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
        	softap_printf(USER_LOG, WARN_LOG, " - Security Object ID %d", targetP->secObjInstID);
        	softap_printf(USER_LOG, WARN_LOG, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
        	softap_printf(USER_LOG, WARN_LOG, "\tstatus: ");
            switch(targetP->status)
            {
            case xiny_STATE_DEREGISTERED:
            	softap_printf(USER_LOG, WARN_LOG, "DEREGISTERED\r\n");
                break;
            case xiny_STATE_BS_HOLD_OFF:
            	softap_printf(USER_LOG, WARN_LOG, "CLIENT HOLD OFF\r\n");
                break;
            case xiny_STATE_BS_INITIATED:
            	softap_printf(USER_LOG, WARN_LOG, "BOOTSTRAP INITIATED\r\n");
                break;
            case xiny_STATE_BS_PENDING:
            	softap_printf(USER_LOG, WARN_LOG, "BOOTSTRAP PENDING\r\n");
                break;
            case xiny_STATE_BS_FINISHED:
            	softap_printf(USER_LOG, WARN_LOG, "BOOTSTRAP FINISHED\r\n");
                break;
            case xiny_STATE_BS_FAILED:
            	softap_printf(USER_LOG, WARN_LOG, "BOOTSTRAP FAILED\r\n");
                break;
            default:
            	softap_printf(USER_LOG, WARN_LOG, "INVALID (%d)\r\n", (int)targetP->status);
            }
            softap_printf(USER_LOG, WARN_LOG, "\r\n");
        }
    }

    if (lwm2mH->serverList == NULL)
    {
    	softap_printf(USER_LOG, WARN_LOG, "No LWM2M Server.\r\n");
    }
    else
    {
    	softap_printf(USER_LOG, WARN_LOG, "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
        	softap_printf(USER_LOG, WARN_LOG, " - Server ID %d", targetP->shortID);
        	softap_printf(USER_LOG, WARN_LOG, "\tstatus: ");
            switch(targetP->status)
            {
            case xiny_STATE_DEREGISTERED:
            	softap_printf(USER_LOG, WARN_LOG, "DEREGISTERED\r\n");
                break;
            case xiny_STATE_REG_PENDING:
            	softap_printf(USER_LOG, WARN_LOG, "REGISTRATION PENDING\r\n");
                break;
            case xiny_STATE_REGISTERED:
            	softap_printf(USER_LOG, WARN_LOG, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
                break;
            case xiny_STATE_REG_UPDATE_PENDING:
                softap_printf(USER_LOG, WARN_LOG, "REGISTRATION UPDATE PENDING\r\n");
                break;
            case xiny_STATE_REG_UPDATE_NEEDED:
                softap_printf(USER_LOG, WARN_LOG, "REGISTRATION UPDATE REQUIRED\r\n");
                break;
            case xiny_STATE_DEREG_PENDING:
                softap_printf(USER_LOG, WARN_LOG, "DEREGISTRATION PENDING\r\n");
                break;
            case xiny_STATE_REG_FAILED:
                softap_printf(USER_LOG, WARN_LOG, "REGISTRATION FAILED\r\n");
                break;
            default:
                softap_printf(USER_LOG, WARN_LOG, "INVALID (%d)\r\n", (int)targetP->status);
            }
            softap_printf(USER_LOG, WARN_LOG, "\r\n");
        }
    }
}

void lightclient_demo_quit()
{
	g_quit = 1;
}

#define OBJ_COUNT 4

int lightclient_demo_proc()
{
	client_data_t data;
	xiny_lwm2m_context_t * lwm2mH = NULL;
	xiny_lwm2m_object_t * objArray[OBJ_COUNT];

	const char * localPort = "56830";
	char * name = "testlwm2mclient";

	int result;

	memset(&data, 0, sizeof(client_data_t));

	data.addressFamily = AF_INET;

	/*
	 *This call an internal function that create an IPv6 socket on the port 5683.
	 */
	softap_printf(USER_LOG, WARN_LOG, "Trying to bind LWM2M Client to port %s\r\n", localPort);
	data.sock = xiny_create_socket(localPort, data.addressFamily);
	if (data.sock < 0)
	{
		softap_printf(USER_LOG, WARN_LOG, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
		return -1;
	}

	/*
	 * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
	 * Those functions are located in their respective object file.
	 */
	objArray[0] = xiny_get_security_object();
	if (NULL == objArray[0])
	{
		softap_printf(USER_LOG, WARN_LOG, "Failed to create security object\r\n");
		return -1;
	}
	data.securityObjP = objArray[0];

	objArray[1] = xiny_get_server_object();
	if (NULL == objArray[1])
	{
		softap_printf(USER_LOG, WARN_LOG, "Failed to create server object\r\n");
		return -1;
	}

	objArray[2] = xiny_get_object_device();
	if (NULL == objArray[2])
	{
		softap_printf(USER_LOG, WARN_LOG, "Failed to create Device object\r\n");
		return -1;
	}

	objArray[3] = get_test_object();
	if (NULL == objArray[3])
	{
		softap_printf(USER_LOG, WARN_LOG, "Failed to create Test object\r\n");
		return -1;
	}

	/*
	 * The liblwm2m library is now xiny_initialized with the functions that will be in
	 * charge of communication
	 */
	lwm2mH = xiny_lwm2m_init(&data);
	if (NULL == lwm2mH)
	{
		softap_printf(USER_LOG, WARN_LOG, "xiny_lwm2m_init() failed\r\n");
		return -1;
	}

	/*
	 * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
	 * the number of xiny_objects we will be passing through and the xiny_objects array
	 */
	result = xiny_lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
	if (result != 0)
	{
		softap_printf(USER_LOG, WARN_LOG, "xiny_lwm2m_configure() failed: 0x%X\r\n", result);
		return -1;
	}

	softap_printf(USER_LOG, WARN_LOG, "LWM2M Client \"%s\" xiny_started on port %s.\r\n", name, localPort);

	/*
	 * We now enter in a while loop that will handle the communications from the server
	 */
	while (0 == g_quit)
	{
		struct timeval tv;
		fd_set readfds;

		tv.tv_sec = 10;
		tv.tv_usec = 0;

		FD_ZERO(&readfds);
		FD_SET(data.sock, &readfds);

		print_state(lwm2mH);

		/*
		 * This function does two things:
		 *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
		 *  - Secondly it adjusts the timeout value (default 60s) dexiny_pending on the state of the transaction
		 *    (eg. retransmission) and the time before the next operation
		 */
		result = xiny_lwm2m_step(lwm2mH, &(tv.tv_sec));
		if (result != 0)
		{
			softap_printf(USER_LOG, WARN_LOG, "xiny_lwm2m_step() failed: 0x%X\r\n", result);
			return -1;
		}

		/*
		 * This part wait for an event on the socket until "tv" timed out (set
		 * with the precedent function)
		 */
		result = select(data.sock + 1, &readfds, NULL, NULL, &tv);

		if (result < 0)
		{
			if (errno != EINTR)
			{
			  softap_printf(USER_LOG, WARN_LOG, "Error in select(): %d %s\r\n", errno, strerror(errno));
			}
		}
		else if (result > 0)
		{
			uint8_t buffer[MAX_PACKET_SIZE];
			int numBytes;

			/*
			 * If an event happens on the socket
			 */
			if (FD_ISSET(data.sock, &readfds))
			{
				struct sockaddr addr = {0};
				socklen_t addrLen;

				addrLen = sizeof(addr);

				/*
				 * We retrieve the data xiny_received
				 */
				numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

				if (0 > numBytes)
				{
					softap_printf(USER_LOG, WARN_LOG, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
				}
				else if (0 < numBytes)
				{
					xiny_connection_t * connP;

					connP = xiny_connection_find((xiny_connection_t *)(data.connList), &addr, addrLen);
					if (connP != NULL)
					{
						/*
						 * Let liblwm2m respond to the query dexiny_pending on the context
						 */
						xiny_lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
					}
					else
					{
						/*
						 * This packet comes from an unknown peer
						 */
						softap_printf(USER_LOG, WARN_LOG, "xiny_received bytes ignored!\r\n");
					}
				}
			}
		}
	}

	/*
	 * Finally when the loop is left, we unregister our client from it
	 */
	xiny_lwm2m_close(lwm2mH);
	close(data.sock);
	xiny_connection_free((xiny_connection_t *)(data.connList));

	free_security_object(objArray[0]);
	free_server_object(objArray[1]);
	xiny_free_object_device(objArray[2]);
	free_test_object(objArray[3]);

	softap_printf(USER_LOG, WARN_LOG, "\r\n\n");
	g_lightclient_handle=NULL;
	osThreadExit();
	return 0;
}

void lightclient_demo(void *args)
{
	(void) args;

	xy_work_lock(0);

	//wait PDP active
	if(xy_wait_tcpip_ok(2*60) == 0)
		xy_assert(0);

	softap_printf(USER_LOG, WARN_LOG, "lightclient_demo start");

	lightclient_demo_proc();

	xy_work_unlock(0);

	softap_printf(USER_LOG, WARN_LOG, "lightclient_demo end");

	g_lightclient_handle=NULL;
	osThreadExit();
}

void lightclient_demo_init()
{
	if (g_lightclient_handle != NULL)
		return;

	g_lightclient_handle = osThreadNew((osThreadFunc_t)(lightclient_demo), NULL, "lightclient_demo", 0x1000, XY_OS_PRIO_NORMAL1);
}
#endif

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
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Pascal Rieux - Please refer to git log
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

*/

#include "xiny_internals.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


xiny_lwm2m_context_t * xiny_lwm2m_init(void * userData)
{
    xiny_lwm2m_context_t * contextP;

    LOG("Entering");
    contextP = (xiny_lwm2m_context_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_context_t));
    if (NULL != contextP)
    {
        memset(contextP, 0, sizeof(xiny_lwm2m_context_t));
        contextP->userData = userData;
        srand((int)xiny_lwm2m_gettime());
        contextP->nextMID = rand();
    }

    return contextP;
}

#ifdef XINYI_LWM2M_CLIENT_MODE
void xiny_lwm2m_deregister(xiny_lwm2m_context_t * context)
{
    xiny_lwm2m_server_lwm2m_t * server = context->serverList;

    LOG("Entering");
    while (NULL != server)
    {
        xiny_registration_deregister_lwm2m(context, server);
        server = server->next;
    }
}

static void prv_deleteServer(xiny_lwm2m_server_lwm2m_t * serverP, void *userData)
{
    // TODO parse transaction and observation to remove the ones related to this server
    if (serverP->sessionH != NULL)
    {
         xiny_lwm2m_close_connection(serverP->sessionH, userData);
    }
    if (NULL != serverP->location)
    {
        xiny_lwm2m_free(serverP->location);
    }
    xiny_free_block1_buffer(serverP->block1Data);
    xiny_lwm2m_free(serverP);
}

static void prv_deleteServerList(xiny_lwm2m_context_t * context)
{
    while (NULL != context->serverList)
    {
        xiny_lwm2m_server_lwm2m_t * server;
        server = context->serverList;
        context->serverList = server->next;
        prv_deleteServer(server, context->userData);
    }
}

static void prv_deleteBootstrapServer(xiny_lwm2m_server_lwm2m_t * serverP, void *userData)
{
    // TODO should we free location as in prv_deleteServer ?
    // TODO should we parse transaction and observation to remove the ones related to this server ?
    if (serverP->sessionH != NULL)
    {
         xiny_lwm2m_close_connection(serverP->sessionH, userData);
    }
    xiny_free_block1_buffer(serverP->block1Data);
    xiny_lwm2m_free(serverP);
}

static void prv_deleteBootstrapServerList(xiny_lwm2m_context_t * context)
{
    while (NULL != context->xiny_bootstrapServerList)
    {
        xiny_lwm2m_server_lwm2m_t * server;
        server = context->xiny_bootstrapServerList;
        context->xiny_bootstrapServerList = server->next;
        prv_deleteBootstrapServer(server, context->userData);
    }
}

static void prv_deleteObservedList(xiny_lwm2m_context_t * contextP)
{
    while (NULL != contextP->observedList)
    {
        xiny_lwm2m_observed_t * targetP;
        xiny_lwm2m_watcher_t * watcherP;

        targetP = contextP->observedList;
        contextP->observedList = contextP->observedList->next;

        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            if (watcherP->parameters != NULL) xiny_lwm2m_free(watcherP->parameters);
        }
        xiny_LWM2M_LIST_FREE(targetP->watcherList);

        xiny_lwm2m_free(targetP);
    }
}
#endif

void prv_deleteTransactionList_lwm2m(xiny_lwm2m_context_t * context)
{
    while (NULL != context->transactionList)
    {
        xiny_lwm2m_transaction_t * transaction;

        transaction = context->transactionList;
        context->transactionList = context->transactionList->next;
        xiny_transaction_free(transaction);
    }
}

void xiny_lwm2m_close(xiny_lwm2m_context_t * contextP)
{
#ifdef XINYI_LWM2M_CLIENT_MODE

    LOG("Entering");
    xiny_lwm2m_deregister(contextP);
    prv_deleteServerList(contextP);
    prv_deleteBootstrapServerList(contextP);
    prv_deleteObservedList(contextP);
    xiny_lwm2m_free(contextP->endpointName);
    if (contextP->msisdn != NULL)
    {
        xiny_lwm2m_free(contextP->msisdn);
    }
    if (contextP->altPath != NULL)
    {
        xiny_lwm2m_free(contextP->altPath);
    }

#endif

#ifdef xiny_LWM2M_SERVER_MODE
    while (NULL != contextP->clientList)
    {
        xiny_lwm2m_client_t * clientP;

        clientP = contextP->clientList;
        contextP->clientList = contextP->clientList->next;

        xiny_registration_freeClient(clientP);
    }
#endif

    prv_deleteTransactionList_lwm2m(contextP);
    xiny_lwm2m_free(contextP);
}

#ifdef XINYI_LWM2M_CLIENT_MODE
static int prv_refreshServerList(xiny_lwm2m_context_t * contextP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;
    xiny_lwm2m_server_lwm2m_t * nextP;

    // Remove all servers marked as dirty
    targetP = contextP->xiny_bootstrapServerList;
    contextP->xiny_bootstrapServerList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            targetP->status = xiny_STATE_DEREGISTERED;
            contextP->xiny_bootstrapServerList = (xiny_lwm2m_server_lwm2m_t *)xiny_LWM2M_LIST_ADD(contextP->xiny_bootstrapServerList, targetP);
        }
        else
        {
            prv_deleteServer(targetP, contextP->userData);
        }
        targetP = nextP;
    }
    targetP = contextP->serverList;
    contextP->serverList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            // TODO: Should we revert the status to xiny_STATE_DEREGISTERED ?
            contextP->serverList = (xiny_lwm2m_server_lwm2m_t *)xiny_LWM2M_LIST_ADD(contextP->serverList, targetP);
        }
        else
        {
            prv_deleteServer(targetP, contextP->userData);
        }
        targetP = nextP;
    }

    return xiny_object_getServers_lwm2m(contextP, false);
}

int xiny_lwm2m_configure(xiny_lwm2m_context_t * contextP,
                    const char * endpointName,
                    const char * msisdn,
                    const char * altPath,
                    uint16_t numObject,
                    xiny_lwm2m_object_t * objectList[])
{
    int i;
    uint8_t found;

    LOG_ARG("endpointName: \"%s\", msisdn: \"%s\", altPath: \"%s\", numObject: %d", endpointName, msisdn, altPath, numObject);
    // This API can be called only once for now
    if (contextP->endpointName != NULL || contextP->objectList != NULL) return xiny_COAP_400_BAD_REQUEST;

    if (endpointName == NULL) return xiny_COAP_400_BAD_REQUEST;
    if (numObject < 3) return xiny_COAP_400_BAD_REQUEST;
    // Check that mandatory xiny_objects are present
    found = 0;
    for (i = 0 ; i < numObject ; i++)
    {
        if (objectList[i]->objID == xiny_LWM2M_SECURITY_OBJECT_ID) found |= 0x01;
        if (objectList[i]->objID == xiny_LWM2M_SERVER_OBJECT_ID) found |= 0x02;
        if (objectList[i]->objID == xiny_LWM2M_DEVICE_OBJECT_ID) found |= 0x04;
    }
    if (found != 0x07) return xiny_COAP_400_BAD_REQUEST;
    if (altPath != NULL)
    {
        if (0 == xiny_utils_isAltPathValid(altPath))
        {
            return xiny_COAP_400_BAD_REQUEST;
        }
        if (altPath[1] == 0)
        {
            altPath = NULL;
        }
    }
    contextP->endpointName = xiny_lwm2m_strdup(endpointName);
    if (contextP->endpointName == NULL)
    {
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }

    if (msisdn != NULL)
    {
        contextP->msisdn = xiny_lwm2m_strdup(msisdn);
        if (contextP->msisdn == NULL)
        {
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (altPath != NULL)
    {
        contextP->altPath = xiny_lwm2m_strdup(altPath);
        if (contextP->altPath == NULL)
        {
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    for (i = 0; i < numObject; i++)
    {
        objectList[i]->next = NULL;
        contextP->objectList = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_ADD(contextP->objectList, objectList[i]);
    }

    return xiny_COAP_NO_ERROR;
}

int xiny_lwm2m_add_object(xiny_lwm2m_context_t * contextP,
                     xiny_lwm2m_object_t * objectP)
{
    xiny_lwm2m_object_t * targetP;

    LOG_ARG("ID: %d", objectP->objID);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, objectP->objID);
    if (targetP != NULL) return xiny_COAP_406_NOT_ACCEPTABLE;
    objectP->next = NULL;

    contextP->objectList = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_ADD(contextP->objectList, objectP);

    if (contextP->state == xiny_STATE_READY)
    {
        return xiny_lwm2m_update_xiny_registration(contextP, 0, true);
    }

    return xiny_COAP_NO_ERROR;
}

int xiny_lwm2m_remove_object(xiny_lwm2m_context_t * contextP,
                        uint16_t id)
{
    xiny_lwm2m_object_t * targetP;

    LOG_ARG("ID: %d", id);
    contextP->objectList = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_RM(contextP->objectList, id, &targetP);

    if (targetP == NULL) return xiny_COAP_404_NOT_FOUND;

    if (contextP->state == xiny_STATE_READY)
    {
        return xiny_lwm2m_update_xiny_registration(contextP, 0, true);
    }

    return 0;
}

#endif


int xiny_lwm2m_step(xiny_lwm2m_context_t * contextP,
               time_t * timeoutP)
{
    time_t tv_sec;
    int result;

    LOG_ARG("timeoutP: %" PRId64, *timeoutP);
    tv_sec = xiny_lwm2m_gettime();
    if (tv_sec < 0) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

#ifdef XINYI_LWM2M_CLIENT_MODE
    LOG_ARG("State: %s", xiny_STR_STATE(contextP->state));
    // state can also be modified in xiny_bootstrap_handleCommand().

next_step:
    switch (contextP->state)
    {
    case xiny_STATE_INITIAL:
        if (0 != prv_refreshServerList(contextP)) return xiny_COAP_503_SERVICE_UNAVAILABLE;
        if (contextP->serverList != NULL)
        {
            contextP->state = xiny_STATE_REGISTER_REQUIRED;
        }
        else
        {
            // Bootstrapping
            contextP->state = xiny_STATE_BOOTSTRAP_REQUIRED;
        }
        goto next_step;
        break;

    case xiny_STATE_BOOTSTRAP_REQUIRED:
#ifdef xiny_LWM2M_BOOTSTRAP
        if (contextP->xiny_bootstrapServerList != NULL)
        {
            xiny_bootstrap_start(contextP);
            contextP->state = xiny_STATE_BOOTSTRAPPING;
            xiny_bootstrap_step(contextP, tv_sec, timeoutP);
        }
        else
#endif
        {
            return xiny_COAP_503_SERVICE_UNAVAILABLE;
        }
        break;

#ifdef xiny_LWM2M_BOOTSTRAP
    case xiny_STATE_BOOTSTRAPPING:
        switch (xiny_bootstrap_getStatus(contextP))
        {
        case xiny_STATE_BS_FINISHED:
            contextP->state = xiny_STATE_INITIAL;
            goto next_step;
            break;

        case xiny_STATE_BS_FAILED:
            return xiny_COAP_503_SERVICE_UNAVAILABLE;

        default:
            // keep on waiting
            xiny_bootstrap_step(contextP, tv_sec, timeoutP);
            break;
        }
        break;
#endif
    case xiny_STATE_REGISTER_REQUIRED:
        result = xiny_registration_start_lwm2m(contextP);
        if (xiny_COAP_NO_ERROR != result) return result;
        contextP->state = xiny_STATE_REGISTERING;
        break;

    case xiny_STATE_REGISTERING:
    {
        switch (xiny_registration_getStatus_lwm2m(contextP))
        {
        case xiny_STATE_REGISTERED:
            contextP->state = xiny_STATE_READY;
            break;

        case xiny_STATE_REG_FAILED:
            // TODO avoid infinite loop by checking the xiny_bootstrap info is different
            contextP->state = xiny_STATE_BOOTSTRAP_REQUIRED;
            goto next_step;
            break;

        case xiny_STATE_REG_PENDING:
        default:
            // keep on waiting
            break;
        }
    }
    break;

    case xiny_STATE_READY:
        if (xiny_registration_getStatus_lwm2m(contextP) == xiny_STATE_REG_FAILED)
        {
            // TODO avoid infinite loop by checking the xiny_bootstrap info is different
            contextP->state = xiny_STATE_BOOTSTRAP_REQUIRED;
            goto next_step;
            break;
        }
        break;

    default:
        // do nothing
        break;
    }

    xiny_observe_step(contextP, tv_sec, timeoutP);
#endif

    xiny_registration_step_lwm2m(contextP, tv_sec, timeoutP);
    xiny_transaction_step(contextP, tv_sec, timeoutP);

    LOG_ARG("Final timeoutP: %" PRId64, *timeoutP);
#ifdef XINYI_LWM2M_CLIENT_MODE
    LOG_ARG("Final state: %s", xiny_STR_STATE(contextP->state));
#endif
    return 0;
}
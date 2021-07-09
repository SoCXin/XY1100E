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
 *    domedambrosio - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Manuel Sangoi - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Scott Bertin - Please refer to git log
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

#define MAX_LOCATION_LENGTH 10      // strlen("/rd/65534") + 1

#ifdef XINYI_LWM2M_CLIENT_MODE

static int prv_getRegistrationQueryLength(xiny_lwm2m_context_t * contextP,
                                          xiny_lwm2m_server_lwm2m_t * server)
{
    int index;
    int res;
    char buffer[21];

    index = strlen(xiny_QUERY_STARTER xiny_QUERY_VERSION_FULL xiny_QUERY_DELIMITER xiny_QUERY_NAME);
    index += strlen(contextP->endpointName);

    if (NULL != contextP->msisdn)
    {
        index += strlen(xiny_QUERY_DELIMITER xiny_QUERY_SMS);
        index += strlen(contextP->msisdn);
    }

    switch (server->binding)
    {
    case xiny_BINDING_U:
        index += strlen("&b=U");
        break;
    case xiny_BINDING_UQ:
        index += strlen("&b=UQ");
        break;
    case xiny_BINDING_S:
        index += strlen("&b=S");
        break;
    case xiny_BINDING_SQ:
        index += strlen("&b=SQ");
        break;
    case xiny_BINDING_US:
        index += strlen("&b=US");
        break;
    case xiny_BINDING_UQS:
        index += strlen("&b=UQS");
        break;
    default:
        return 0;
    }

    if (0 != server->lifetime)
    {
        index += strlen(xiny_QUERY_DELIMITER xiny_QUERY_LIFETIME);
        res = xiny_utils_intToText((int64_t)(server->lifetime), (uint8_t *)(buffer), sizeof(buffer));
        if (res == 0) return 0;
        index += res;
    }

    return index + 1;
}

static int prv_getRegistrationQuery(xiny_lwm2m_context_t * contextP,
                                    xiny_lwm2m_server_lwm2m_t * server,
                                    char * buffer,
                                    size_t length)
{
    int index;
    int res;

    index = xiny_utils_stringCopy(buffer, length, xiny_QUERY_STARTER xiny_QUERY_VERSION_FULL xiny_QUERY_DELIMITER xiny_QUERY_NAME);
    if (index < 0) return 0;
    res = xiny_utils_stringCopy(buffer + index, length - index, contextP->endpointName);
    if (res < 0) return 0;
    index += res;

    if (NULL != contextP->msisdn)
    {
        res = xiny_utils_stringCopy(buffer + index, length - index, xiny_QUERY_DELIMITER xiny_QUERY_SMS);
        if (res < 0) return 0;
        index += res;
        res = xiny_utils_stringCopy(buffer + index, length - index, contextP->msisdn);
        if (res < 0) return 0;
        index += res;
    }

    switch (server->binding)
    {
    case xiny_BINDING_U:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=U");
        break;
    case xiny_BINDING_UQ:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=UQ");
        break;
    case xiny_BINDING_S:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=S");
        break;
    case xiny_BINDING_SQ:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=SQ");
        break;
    case xiny_BINDING_US:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=US");
        break;
    case xiny_BINDING_UQS:
        res = xiny_utils_stringCopy(buffer + index, length - index, "&b=UQS");
        break;
    default:
        res = -1;
    }
    if (res < 0) return 0;
    index += res;

    if (0 != server->lifetime)
    {
        res = xiny_utils_stringCopy(buffer + index, length - index, xiny_QUERY_DELIMITER xiny_QUERY_LIFETIME);
        if (res < 0) return 0;
        index += res;
        res = xiny_utils_intToText((int64_t)(server->lifetime), (uint8_t *)(buffer + index), length - index);
        if (res == 0) return 0;
        index += res;
    }

    if(index < (int)length)
    {
        buffer[index++] = '\0';
    }
    else
    {
        return 0;
    }

    return index;
}

static void prv_handleRegistrationReply(xiny_lwm2m_transaction_t * transacP,
                                        void * message)
{
    xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;
    xiny_lwm2m_server_lwm2m_t * targetP = (xiny_lwm2m_server_lwm2m_t *)(transacP->userData);

    if (targetP->status == xiny_STATE_REG_PENDING)
    {
        time_t tv_sec = xiny_lwm2m_gettime();
        if (tv_sec >= 0)
        {
            targetP->xiny_registration = tv_sec;
        }
        if (packet != NULL && packet->code == xiny_COAP_201_CREATED)
        {
            targetP->status = xiny_STATE_REGISTERED;
            if (NULL != targetP->location)
            {
                xiny_lwm2m_free(targetP->location);
            }
            targetP->location = xiny_coap_get_multi_option_as_string(packet->location_path);

            LOG("Registration successful");
        }
        else
        {
            targetP->status = xiny_STATE_REG_FAILED;
            LOG("Registration failed");
        }
    }
}

// send the xiny_registration for a single server
static uint8_t prv_register(xiny_lwm2m_context_t * contextP,
                            xiny_lwm2m_server_lwm2m_t * server)
{
    char * query;
    int query_length;
    uint8_t * payload;
    int payload_length;
    xiny_lwm2m_transaction_t * transaction;

    payload_length = xiny_object_getRegisterPayload_lwm2mBufferLength_lwm2m(contextP);
    if(payload_length == 0) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    payload = xiny_lwm2m_malloc(payload_length);
    if(!payload) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    payload_length = xiny_object_getRegisterPayload_lwm2m(contextP, payload, payload_length);
    if(payload_length == 0)
    {
        xiny_lwm2m_free(payload);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }

    query_length = prv_getRegistrationQueryLength(contextP, server);
    if(query_length == 0)
    {
        xiny_lwm2m_free(payload);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }
    query = xiny_lwm2m_malloc(query_length);
    if(!query)
    {
        xiny_lwm2m_free(payload);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }
    if(prv_getRegistrationQuery(contextP, server, query, query_length) != query_length)
    {
        xiny_lwm2m_free(payload);
        xiny_lwm2m_free(query);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }

    if (server->sessionH == NULL)
    {
        server->sessionH = xiny_lwm2m_connect_server(server->secObjInstID, contextP->userData);
    }

    if (NULL == server->sessionH)
    {
        xiny_lwm2m_free(payload);
        xiny_lwm2m_free(query);
        return xiny_COAP_503_SERVICE_UNAVAILABLE;
    }

    transaction = xiny_transaction_new(server->sessionH, xiny_COAP_POST, NULL, NULL, contextP->nextMID++, 4, NULL);
    if (transaction == NULL)
    {
        xiny_lwm2m_free(payload);
        xiny_lwm2m_free(query);
        return xiny_COAP_503_SERVICE_UNAVAILABLE;
    }

    xiny_coap_set_header_uri_path(transaction->message, "/"xiny_URI_REGISTRATION_SEGMENT);
    xiny_coap_set_header_uri_query(transaction->message, query);
    xiny_coap_set_header_content_type(transaction->message, xiny_LWM2M_CONTENT_LINK);
    xiny_coap_set_payload(transaction->message, payload, payload_length);

    transaction->callback = prv_handleRegistrationReply;
    transaction->userData = (void *) server;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);
    if (xiny_transaction_send(contextP, transaction) != 0)
    {
        xiny_lwm2m_free(payload);
        xiny_lwm2m_free(query);
        return xiny_COAP_503_SERVICE_UNAVAILABLE;
    }

    xiny_lwm2m_free(payload);
    xiny_lwm2m_free(query);
    server->status = xiny_STATE_REG_PENDING;

    return xiny_COAP_NO_ERROR;
}

static void prv_handleRegistrationUpdateReply(xiny_lwm2m_transaction_t * transacP,
                                              void * message)
{
    xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;
    xiny_lwm2m_server_lwm2m_t * targetP = (xiny_lwm2m_server_lwm2m_t *)(transacP->userData);

    if (targetP->status == xiny_STATE_REG_UPDATE_PENDING)
    {
        time_t tv_sec = xiny_lwm2m_gettime();
        if (tv_sec >= 0)
        {
            targetP->xiny_registration = tv_sec;
        }
        if (packet != NULL && packet->code == xiny_COAP_204_CHANGED)
        {
            targetP->status = xiny_STATE_REGISTERED;
            LOG("Registration update successful");
        }
        else
        {
            targetP->status = xiny_STATE_REG_FAILED;
            LOG("Registration update failed");
        }
    }
}

static int prv_updateRegistration(xiny_lwm2m_context_t * contextP,
                                  xiny_lwm2m_server_lwm2m_t * server,
                                  bool withObjects)
{
    xiny_lwm2m_transaction_t * transaction;
    uint8_t * payload = NULL;
    int payload_length;

    transaction = xiny_transaction_new(server->sessionH, xiny_COAP_POST, NULL, NULL, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    xiny_coap_set_header_uri_path(transaction->message, server->location);

    if (withObjects == true)
    {
        payload_length = xiny_object_getRegisterPayload_lwm2mBufferLength_lwm2m(contextP);
        if(payload_length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }

        payload = xiny_lwm2m_malloc(payload_length);
        if(!payload)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }

        payload_length = xiny_object_getRegisterPayload_lwm2m(contextP, payload, payload_length);
        if(payload_length == 0)
        {
            xiny_transaction_free(transaction);
            xiny_lwm2m_free(payload);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_set_payload(transaction->message, payload, payload_length);
    }

    transaction->callback = prv_handleRegistrationUpdateReply;
    transaction->userData = (void *) server;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    if (xiny_transaction_send(contextP, transaction) == 0)
    {
        server->status = xiny_STATE_REG_UPDATE_PENDING;
    }

    if (withObjects == true)
    {
        xiny_lwm2m_free(payload);
    }
    return xiny_COAP_NO_ERROR;
}

// update the xiny_registration of a given server
int xiny_lwm2m_update_xiny_registration(xiny_lwm2m_context_t * contextP,
                              uint16_t shortServerID,
                              bool withObjects)
{
    xiny_lwm2m_server_lwm2m_t * targetP;
    uint8_t result;

    LOG_ARG("State: %s, shortServerID: %d", xiny_STR_STATE(contextP->state), shortServerID);

    result = xiny_COAP_NO_ERROR;

    targetP = contextP->serverList;
    if (targetP == NULL)
    {
        if (xiny_object_getServers_lwm2m(contextP, false) == -1)
        {
            LOG("No server found");
            return xiny_COAP_404_NOT_FOUND;
        }
    }
    while (targetP != NULL && result == xiny_COAP_NO_ERROR)
    {
        if (shortServerID != 0)
        {
            if (targetP->shortID == shortServerID)
            {
                // found the server, trigger the update transaction
                if (targetP->status == xiny_STATE_REGISTERED
                 || targetP->status == xiny_STATE_REG_UPDATE_PENDING)
                {
                    if (withObjects == true)
                    {
                        targetP->status = xiny_STATE_REG_FULL_UPDATE_NEEDED;
                    }
                    else
                    {
                        targetP->status = xiny_STATE_REG_UPDATE_NEEDED;
                    }
                    return xiny_COAP_NO_ERROR;
                }
                else if ((targetP->status == xiny_STATE_REG_FULL_UPDATE_NEEDED)
                      || (targetP->status == xiny_STATE_REG_UPDATE_NEEDED))
                {
                    // if REG (FULL) UPDATE is already set, returns xiny_COAP_NO_ERROR
                    if (withObjects == true)
                    {
                        targetP->status = xiny_STATE_REG_FULL_UPDATE_NEEDED;
                    }
                    return xiny_COAP_NO_ERROR;
                }
                else
                {
                    return xiny_COAP_400_BAD_REQUEST;
                }
            }
        }
        else
        {
            if (targetP->status == xiny_STATE_REGISTERED
             || targetP->status == xiny_STATE_REG_UPDATE_PENDING)
            {
                if (withObjects == true)
                {
                    targetP->status = xiny_STATE_REG_FULL_UPDATE_NEEDED;
                }
                else
                {
                    targetP->status = xiny_STATE_REG_UPDATE_NEEDED;
                }
            }
        }
        targetP = targetP->next;
    }

    if (shortServerID != 0
     && targetP == NULL)
    {
        // no server found
        result = xiny_COAP_404_NOT_FOUND;
    }

    return result;
}

uint8_t xiny_registration_start_lwm2m(xiny_lwm2m_context_t * contextP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;
    uint8_t result;

    LOG_ARG("State: %s", xiny_STR_STATE(contextP->state));

    result = xiny_COAP_NO_ERROR;

    targetP = contextP->serverList;
    while (targetP != NULL && result == xiny_COAP_NO_ERROR)
    {
        if (targetP->status == xiny_STATE_DEREGISTERED
         || targetP->status == xiny_STATE_REG_FAILED)
        {
            result = prv_register(contextP, targetP);
        }
        targetP = targetP->next;
    }

    return result;
}


/*
 * Returns xiny_STATE_REG_PENDING if at least one xiny_registration is still xiny_pending
 * Returns xiny_STATE_REGISTERED if no xiny_registration is xiny_pending and there is at least one server the client is xiny_registered to
 * Returns xiny_STATE_REG_FAILED if all xiny_registration failed.
 */
xiny_lwm2m_status_lwm2m_t xiny_registration_getStatus_lwm2m(xiny_lwm2m_context_t * contextP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;
    xiny_lwm2m_status_lwm2m_t reg_status;

    LOG_ARG("State: %s", xiny_STR_STATE(contextP->state));

    targetP = contextP->serverList;
    reg_status = xiny_STATE_REG_FAILED;

    while (targetP != NULL)
    {
        LOG_ARG("targetP->status: %s", xiny_STR_STATUS(targetP->status));
        switch (targetP->status)
        {
            case xiny_STATE_REGISTERED:
            case xiny_STATE_REG_UPDATE_NEEDED:
            case xiny_STATE_REG_FULL_UPDATE_NEEDED:
            case xiny_STATE_REG_UPDATE_PENDING:
                if (reg_status == xiny_STATE_REG_FAILED)
                {
                    reg_status = xiny_STATE_REGISTERED;
                }
                break;

            case xiny_STATE_REG_PENDING:
                reg_status = xiny_STATE_REG_PENDING;
                break;

            case xiny_STATE_REG_FAILED:
            case xiny_STATE_DEREG_PENDING:
            case xiny_STATE_DEREGISTERED:
            default:
                break;
        }
        LOG_ARG("reg_status: %s", xiny_STR_STATUS(reg_status));

        targetP = targetP->next;
    }

    return reg_status;
}

static void prv_handleDexiny_registrationReply(xiny_lwm2m_transaction_t * transacP,
                                          void * message)
{
	(void) message;

    xiny_lwm2m_server_lwm2m_t * targetP;

    targetP = (xiny_lwm2m_server_lwm2m_t *)(transacP->userData);
    if (NULL != targetP)
    {
        if (targetP->status == xiny_STATE_DEREG_PENDING)
        {
            targetP->status = xiny_STATE_DEREGISTERED;
        }
    }
}

void xiny_registration_deregister_lwm2m(xiny_lwm2m_context_t * contextP,
                             xiny_lwm2m_server_lwm2m_t * serverP)
{
    xiny_lwm2m_transaction_t * transaction;

    LOG_ARG("State: %s, serverP->status: %s", xiny_STR_STATE(contextP->state), xiny_STR_STATUS(serverP->status));

    if (serverP->status == xiny_STATE_DEREGISTERED
     || serverP->status == xiny_STATE_REG_PENDING
     || serverP->status == xiny_STATE_DEREG_PENDING
     || serverP->status == xiny_STATE_REG_FAILED
     || serverP->location == NULL)
    {
        return;
    }

    transaction = xiny_transaction_new(serverP->sessionH, xiny_COAP_DELETE, NULL, NULL, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return;

    xiny_coap_set_header_uri_path(transaction->message, serverP->location);

    transaction->callback = prv_handleDexiny_registrationReply;
    transaction->userData = (void *) serverP;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);
    if (xiny_transaction_send(contextP, transaction) == 0)
    {
        serverP->status = xiny_STATE_DEREG_PENDING;
    }
}
#endif

#ifdef xiny_LWM2M_SERVER_MODE
static void prv_freeClientObjectList(xiny_lwm2m_client_object_t * xiny_objects)
{
    while (xiny_objects != NULL)
    {
        xiny_lwm2m_client_object_t * objP;

        while (xiny_objects->instanceList != NULL)
        {
            xiny_lwm2m_list_t * target;

            target = xiny_objects->instanceList;
            xiny_objects->instanceList = xiny_objects->instanceList->next;
            xiny_lwm2m_free(target);
        }

        objP = xiny_objects;
        xiny_objects = xiny_objects->next;
        xiny_lwm2m_free(objP);
    }
}

static int prv_getParameters(xiny_multi_option_t * query,
                             char ** nameP,
                             uint32_t * lifetimeP,
                             char ** msisdnP,
                             xiny_lwm2m_binding_lwm2m_t * bindingP,
                             char ** versionP)
{
    *nameP = NULL;
    *lifetimeP = 0;
    *msisdnP = NULL;
    *bindingP = xiny_BINDING_UNKNOWN;
    *versionP = NULL;

    while (query != NULL)
    {
        if (xiny_lwm2m_strncmp((char *)query->data, xiny_QUERY_NAME, xiny_QUERY_NAME_LEN) == 0)
        {
            if (*nameP != NULL) goto error;
            if (query->len == xiny_QUERY_NAME_LEN) goto error;

            *nameP = (char *)xiny_lwm2m_malloc(query->len - xiny_QUERY_NAME_LEN + 1);
            if (*nameP != NULL)
            {
                memcpy(*nameP, query->data + xiny_QUERY_NAME_LEN, query->len - xiny_QUERY_NAME_LEN);
                (*nameP)[query->len - xiny_QUERY_NAME_LEN] = 0;
            }
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_QUERY_SMS, xiny_QUERY_SMS_LEN) == 0)
        {
            if (*msisdnP != NULL) goto error;
            if (query->len == xiny_QUERY_SMS_LEN) goto error;

            *msisdnP = (char *)xiny_lwm2m_malloc(query->len - xiny_QUERY_SMS_LEN + 1);
            if (*msisdnP != NULL)
            {
                memcpy(*msisdnP, query->data + xiny_QUERY_SMS_LEN, query->len - xiny_QUERY_SMS_LEN);
                (*msisdnP)[query->len - xiny_QUERY_SMS_LEN] = 0;
            }
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_QUERY_LIFETIME, xiny_QUERY_LIFETIME_LEN) == 0)
        {
            int i;

            if (*lifetimeP != 0) goto error;
            if (query->len == xiny_QUERY_LIFETIME_LEN) goto error;

            for (i = xiny_QUERY_LIFETIME_LEN ; i < query->len ; i++)
            {
                if (query->data[i] < '0' || query->data[i] > '9') goto error;
                *lifetimeP = (*lifetimeP * 10) + (query->data[i] - '0');
            }
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_QUERY_VERSION, xiny_QUERY_VERSION_LEN) == 0)
        {
            if (*versionP != NULL) goto error;
            if (query->len == xiny_QUERY_VERSION_LEN) goto error;

            *versionP = (char *)xiny_lwm2m_malloc(query->len - xiny_QUERY_VERSION_LEN + 1);
            if (*versionP != NULL)
            {
                memcpy(*versionP, query->data + xiny_QUERY_VERSION_LEN, query->len - xiny_QUERY_VERSION_LEN);
                (*versionP)[query->len - xiny_QUERY_VERSION_LEN] = 0;
            }
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_QUERY_BINDING, xiny_QUERY_BINDING_LEN) == 0)
        {
            if (*bindingP != xiny_BINDING_UNKNOWN) goto error;
            if (query->len == xiny_QUERY_BINDING_LEN) goto error;

            *bindingP = xiny_utils_stringToBinding(query->data + xiny_QUERY_BINDING_LEN, query->len - xiny_QUERY_BINDING_LEN);
        }
        query = query->next;
    }

    return 0;

error:
    if (*nameP != NULL) xiny_lwm2m_free(*nameP);
    if (*msisdnP != NULL) xiny_lwm2m_free(*msisdnP);
    if (*versionP != NULL) xiny_lwm2m_free(*versionP);

    return -1;
}

static uint16_t prv_splitLinkAttribute(uint8_t * data,
                                       uint16_t length,
                                       uint16_t * keyStart,
                                       uint16_t * keyLength,
                                       uint16_t * valueStart,
                                       uint16_t * valueLength)
{
    uint16_t index;
    uint16_t end;

    index = 0;
    while (index < length && data[index] == ' ') index++;
    if (index == length) return 0;

    if (data[index] == xiny_REG_ATTR_SEPARATOR)
    {
        index++;
    }
    if (index == length) return 0;

    *keyStart = index;

    while (index < length && data[index] != xiny_REG_ATTR_EQUALS) index++;
    if (index == *keyStart || index == length) return 0;

    *keyLength = index - *keyStart;

    index++;
    while (index < length && data[index] == ' ') index++;
    if (index == length) return 0;

    *valueStart = index;

    while (index < length && data[index] != xiny_REG_ATTR_SEPARATOR) index++;
    end = index;

    index--;
    while (index > *valueStart && data[index] == ' ') index--;
    if (index == *valueStart) return 0;

    *valueLength = index - *valueStart + 1;

    return end;
}

static int prv_parseLinkAttributes(uint8_t * data,
                                   uint16_t length,
                                   bool * supportJSON,
                                   char ** altPath)
{
    uint16_t index;
    uint16_t pathStart;
    uint16_t pathLength;
    bool isValid;

    isValid = false;

    // Expecting application/link-format (RFC6690)
    // leading space were removed before. Remove trailing spaces.
    while (length > 0 && data[length-1] == ' ') length--;

    // strip open tag
    if (length >= 2 && data[0] == xiny_REG_URI_START)
    {
        data += 1;
        length -= 1;
    }
    else
    {
        return 0;
    }

    pathStart = 0;
    index = length - 1;
    while (index > 0 && data[index] != xiny_REG_URI_END) index--;
    // link attributes are xiny_required
    if (index == 0 || index == length - 1) return 0;

    // If there is a preceding /, remove it
    if (data[pathStart] == '/')
    {
        pathStart += 1;
    }
    pathLength = index - pathStart;

    index++;
    if (index >= length || data[index] != xiny_REG_ATTR_SEPARATOR) return 0;
    index++;

    while (index < length)
    {
        uint16_t result;
        uint16_t keyStart;
        uint16_t keyLength;
        uint16_t valueStart;
        uint16_t valueLength;

        result = prv_splitLinkAttribute(data + index, length - index, &keyStart, &keyLength, &valueStart, &valueLength);
        if (result == 0) return 0;

        if (keyLength == xiny_REG_ATTR_TYPE_KEY_LEN
         && 0 == xiny_lwm2m_strncmp(xiny_REG_ATTR_TYPE_KEY, (char*)data + index + keyStart, keyLength))
        {
            if (isValid == true) return 0; // declared twice
            if (valueLength != xiny_REG_ATTR_TYPE_VALUE_LEN
             || 0 != xiny_lwm2m_strncmp(xiny_REG_ATTR_TYPE_VALUE, (char*)data + index + valueStart, valueLength))
            {
                return 0;
            }
            isValid = true;
        }
        else if (keyLength == xiny_REG_ATTR_CONTENT_KEY_LEN
              && 0 == xiny_lwm2m_strncmp(xiny_REG_ATTR_CONTENT_KEY, (char*)data + index + keyStart, keyLength))
        {
            if (*supportJSON == true) return 0; // declared twice
            if (valueLength == xiny_REG_ATTR_CONTENT_JSON_LEN
             && 0 == xiny_lwm2m_strncmp(xiny_REG_ATTR_CONTENT_JSON, (char*)data + index + valueStart, valueLength))
            {
                *supportJSON = true;
            }
            else
            {
                return 0;
            }
        }
        // else ignore this one

        index += result;
    }

    if (isValid == false) return 0;

    if (pathLength != 0)
    {
        *altPath = (char *)xiny_lwm2m_malloc(pathLength + 1);
        if (*altPath == NULL) return 0;
        memcpy(*altPath, data + pathStart, pathLength);
        (*altPath)[pathLength] = 0;
    }

    return 1;
}

static int prv_getId(uint8_t * data,
                     uint16_t length,
                     uint16_t * objId,
                     uint16_t * instanceId)
{
    int value;
    uint16_t limit;

    // Expecting application/link-format (RFC6690)
    // leading space were removed before. Remove trailing spaces.
    while (length > 0 && data[length-1] == ' ') length--;

    // strip open and close tags
    if (length >= 1 && data[0] == xiny_REG_URI_START && data[length-1] == xiny_REG_URI_END)
    {
        data += 1;
        length -= 2;
    }
    else
    {
        return 0;
    }

    // If there is a preceding /, remove it
    if (length >= 1 && data[0] == '/')
    {
        data += 1;
        length -= 1;
    }

    limit = 0;
    while (limit < length && data[limit] != '/') limit++;
    value = xiny_uri_getNumber_lwm2m(data, limit);
    if (value < 0 || value >= xiny_LWM2M_MAX_ID) return 0;
    *objId = value;

    if (limit < length)
    {
        limit += 1;
        data += limit;
        length -= limit;

        if (length > 0)
        {
            value = xiny_uri_getNumber_lwm2m(data, length);
            if (value >= 0 && value < xiny_LWM2M_MAX_ID)
            {
                *instanceId = value;
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }

    return 1;
}

static xiny_lwm2m_client_object_t * prv_decodeRegisterPayload(uint8_t * payload,
                                                         uint16_t payloadLength,
                                                         bool * supportJSON,
                                                         char ** altPath)
{
    uint16_t index;
    xiny_lwm2m_client_object_t * objList;
    bool linkAttrFound;

    *altPath = NULL;
    *supportJSON = false;
    objList = NULL;
    linkAttrFound = false;
    index = 0;

    while (index <= payloadLength)
    {
        uint16_t start;
        uint16_t length;
        int result;
        uint16_t id;
        uint16_t instance;

        while (index < payloadLength && payload[index] == ' ') index++;
        if (index == payloadLength) break;

        start = index;
        while (index < payloadLength && payload[index] != xiny_REG_DELIMITER) index++;
        length = index - start;

        result = prv_getId(payload + start, length, &id, &instance);
        if (result != 0)
        {
            xiny_lwm2m_client_object_t * objectP;

            objectP = (xiny_lwm2m_client_object_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)objList, id);
            if (objectP == NULL)
            {
                objectP = (xiny_lwm2m_client_object_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_client_object_t));
                memset(objectP, 0, sizeof(xiny_lwm2m_client_object_t));
                if (objectP == NULL) goto error;
                objectP->id = id;
                objList = (xiny_lwm2m_client_object_t *)xiny_LWM2M_LIST_ADD(objList, objectP);
            }
            if (result == 2)
            {
                xiny_lwm2m_list_t * instanceP;

                instanceP = xiny_lwm2m_list_find(objectP->instanceList, instance);
                if (instanceP == NULL)
                {
                    instanceP = (xiny_lwm2m_list_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_list_t));
                    memset(instanceP, 0, sizeof(xiny_lwm2m_list_t));
                    instanceP->id = instance;
                    objectP->instanceList = xiny_LWM2M_LIST_ADD(objectP->instanceList, instanceP);
                }
            }
        }
        else if (linkAttrFound == false)
        {
            result = prv_parseLinkAttributes(payload + start, length, supportJSON, altPath);
            if (result == 0) goto error;

            linkAttrFound = true;
        }
        else goto error;

        index++;
    }

    return objList;

error:
    if (*altPath != NULL)
    {
        xiny_lwm2m_free(*altPath);
        *altPath = NULL;
    }
    prv_freeClientObjectList(objList);

    return NULL;
}

static xiny_lwm2m_client_t * prv_getClientByName(xiny_lwm2m_context_t * contextP,
                                            char * name)
{
    xiny_lwm2m_client_t * targetP;

    targetP = contextP->clientList;
    while (targetP != NULL && strcmp(name, targetP->name) != 0)
    {
        targetP = targetP->next;
    }

    return targetP;
}

void xiny_registration_freeClient(xiny_lwm2m_client_t * clientP)
{
    LOG("Entering");
    if (clientP->name != NULL) xiny_lwm2m_free(clientP->name);
    if (clientP->msisdn != NULL) xiny_lwm2m_free(clientP->msisdn);
    if (clientP->altPath != NULL) xiny_lwm2m_free(clientP->altPath);
    prv_freeClientObjectList(clientP->objectList);
    while(clientP->observationList != NULL)
    {
        xiny_lwm2m_observation_t * targetP;

        targetP = clientP->observationList;
        clientP->observationList = clientP->observationList->next;
        xiny_lwm2m_free(targetP);
    }
    xiny_lwm2m_free(clientP);
}

static int prv_getLocationString(uint16_t id,
                                 char location[MAX_LOCATION_LENGTH])
{
    int index;
    int result;

    memset(location, 0, MAX_LOCATION_LENGTH);

    result = xiny_utils_stringCopy(location, MAX_LOCATION_LENGTH, "/"xiny_URI_REGISTRATION_SEGMENT"/");
    if (result < 0) return 0;
    index = result;

    result = xiny_utils_intToText(id, (uint8_t*)location + index, MAX_LOCATION_LENGTH - index);
    if (result == 0) return 0;

    return index + result;
}

uint8_t xiny_registration_handleRequest(xiny_lwm2m_context_t * contextP,
                                   xiny_lwm2m_uri_t * uriP,
                                   void * fromSessionH,
                                   xiny_coap_packet_t * message,
                                   xiny_coap_packet_t * response)
{
    uint8_t result;
    time_t tv_sec;

    LOG_URI(uriP);
    tv_sec = xiny_lwm2m_gettime();
    if (tv_sec < 0) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    switch(message->code)
    {
    case xiny_COAP_POST:
    {
        char * name = NULL;
        uint32_t lifetime;
        char * msisdn;
        char * altPath;
        char * version;
        xiny_lwm2m_binding_lwm2m_t binding;
        xiny_lwm2m_client_object_t * xiny_objects;
        bool supportJSON;
        xiny_lwm2m_client_t * clientP;
        char location[MAX_LOCATION_LENGTH];

        if (0 != prv_getParameters(message->uri_query, &name, &lifetime, &msisdn, &binding, &version))
        {
            return xiny_COAP_400_BAD_REQUEST;
        }
        if (message->content_type != (xiny_coap_content_type_t)xiny_LWM2M_CONTENT_LINK
         && message->content_type != (xiny_coap_content_type_t)xiny_LWM2M_CONTENT_TEXT)
        {
            return xiny_COAP_400_BAD_REQUEST;
        }

        xiny_objects = prv_decodeRegisterPayload(message->payload, message->payload_len, &supportJSON, &altPath);

        switch (uriP->flag & xiny_LWM2M_URI_MASK_ID)
        {
        case 0:
            // Register operation
            // Version is mandatory
            if (version == NULL)
            {
                if (name != NULL) xiny_lwm2m_free(name);
                if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                return xiny_COAP_400_BAD_REQUEST;
            }
            // Endpoint client name is mandatory
            if (name == NULL)
            {
                xiny_lwm2m_free(version);
                if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                return xiny_COAP_400_BAD_REQUEST;
            }
            // Object list is mandatory
            if (xiny_objects == NULL)
            {
                xiny_lwm2m_free(version);
                xiny_lwm2m_free(name);
                if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                return xiny_COAP_400_BAD_REQUEST;
            }
            // version must be 1.0
            if (strlen(version) != xiny_LWM2M_VERSION_LEN
                || xiny_lwm2m_strncmp(version, xiny_LWM2M_VERSION, xiny_LWM2M_VERSION_LEN))
            {
                xiny_lwm2m_free(version);
                xiny_lwm2m_free(name);
                if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                return xiny_COAP_412_PRECONDITION_FAILED;
            }

            if (lifetime == 0)
            {
                lifetime = xiny_LWM2M_DEFAULT_LIFETIME;
            }

            clientP = prv_getClientByName(contextP, name);
            if (clientP != NULL)
            {
                // we reset this xiny_registration
                xiny_lwm2m_free(clientP->name);
                if (clientP->msisdn != NULL) xiny_lwm2m_free(clientP->msisdn);
                if (clientP->altPath != NULL) xiny_lwm2m_free(clientP->altPath);
                prv_freeClientObjectList(clientP->objectList);
                clientP->objectList = NULL;
            }
            else
            {
                clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_client_t));
                if (clientP == NULL)
                {
                    xiny_lwm2m_free(name);
                    xiny_lwm2m_free(altPath);
                    if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                    prv_freeClientObjectList(xiny_objects);
                    return xiny_COAP_500_INTERNAL_SERVER_ERROR;
                }
                memset(clientP, 0, sizeof(xiny_lwm2m_client_t));
                clientP->internalID = xiny_lwm2m_list_newId((xiny_lwm2m_list_t *)contextP->clientList);
                contextP->clientList = (xiny_lwm2m_client_t *)xiny_LWM2M_LIST_ADD(contextP->clientList, clientP);
            }
            clientP->name = name;
            clientP->binding = binding;
            clientP->msisdn = msisdn;
            clientP->altPath = altPath;
            clientP->supportJSON = supportJSON;
            clientP->lifetime = lifetime;
            clientP->endOfLife = tv_sec + lifetime;
            clientP->objectList = xiny_objects;
            clientP->sessionH = fromSessionH;

            if (prv_getLocationString(clientP->internalID, location) == 0)
            {
                xiny_registration_freeClient(clientP);
                return xiny_COAP_500_INTERNAL_SERVER_ERROR;
            }
            if (xiny_coap_set_header_location_path(response, location) == 0)
            {
                xiny_registration_freeClient(clientP);
                return xiny_COAP_500_INTERNAL_SERVER_ERROR;
            }

            if (contextP->monitorCallback != NULL)
            {
                contextP->monitorCallback(clientP->internalID, NULL, xiny_COAP_201_CREATED, xiny_LWM2M_CONTENT_TEXT, NULL, 0, contextP->monitorUserData);
            }
            result = xiny_COAP_201_CREATED;
            break;

        case xiny_LWM2M_URI_FLAG_OBJECT_ID:
            clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, uriP->objectId);
            if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

            // Endpoint client name MUST NOT be present
            if (name != NULL)
            {
                xiny_lwm2m_free(name);
                if (msisdn != NULL) xiny_lwm2m_free(msisdn);
                return xiny_COAP_400_BAD_REQUEST;
            }

            if (binding != xiny_BINDING_UNKNOWN)
            {
                clientP->binding = binding;
            }
            if (msisdn != NULL)
            {
                if (clientP->msisdn != NULL) xiny_lwm2m_free(clientP->msisdn);
                clientP->msisdn = msisdn;
            }
            if (lifetime != 0)
            {
                clientP->lifetime = lifetime;
            }
            // client IP address, port or MSISDN may have changed
            clientP->sessionH = fromSessionH;

            if (xiny_objects != NULL)
            {
                xiny_lwm2m_observation_t * observationP;

                // remove observations on object/instance no longer existing
                observationP = clientP->observationList;
                while (observationP != NULL)
                {
                    xiny_lwm2m_client_object_t * objP;
                    xiny_lwm2m_observation_t * nextP;

                    nextP = observationP->next;

                    objP = (xiny_lwm2m_client_object_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)xiny_objects, observationP->uri.objectId);
                    if (objP == NULL)
                    {
                        observationP->callback(clientP->internalID,
                                               &observationP->uri,
                                               xiny_COAP_202_DELETED,
                                               xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                                               observationP->userData);
                        xiny_observe_remove(observationP);
                    }
                    else
                    {
                        if ((observationP->uri.flag & xiny_LWM2M_URI_FLAG_INSTANCE_ID) != 0)
                        {
                            if (xiny_lwm2m_list_find((xiny_lwm2m_list_t *)objP->instanceList, observationP->uri.instanceId) == NULL)
                            {
                                observationP->callback(clientP->internalID,
                                                       &observationP->uri,
                                                       xiny_COAP_202_DELETED,
                                                       xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                                                       observationP->userData);
                                xiny_observe_remove(observationP);
                            }
                        }
                    }

                    observationP = nextP;
                }

                prv_freeClientObjectList(clientP->objectList);
                clientP->objectList = xiny_objects;
            }

            clientP->endOfLife = tv_sec + clientP->lifetime;

            if (contextP->monitorCallback != NULL)
            {
                contextP->monitorCallback(clientP->internalID, NULL, xiny_COAP_204_CHANGED, xiny_LWM2M_CONTENT_TEXT, NULL, 0, contextP->monitorUserData);
            }
            result = xiny_COAP_204_CHANGED;
            break;

            default:
                return xiny_COAP_400_BAD_REQUEST;
        }
    }
    break;

    case xiny_COAP_DELETE:
    {
        xiny_lwm2m_client_t * clientP;

        if ((uriP->flag & xiny_LWM2M_URI_MASK_ID) != xiny_LWM2M_URI_FLAG_OBJECT_ID) return xiny_COAP_400_BAD_REQUEST;

        contextP->clientList = (xiny_lwm2m_client_t *)xiny_LWM2M_LIST_RM(contextP->clientList, uriP->objectId, &clientP);
        if (clientP == NULL) return xiny_COAP_400_BAD_REQUEST;
        if (contextP->monitorCallback != NULL)
        {
            contextP->monitorCallback(clientP->internalID, NULL, xiny_COAP_202_DELETED, xiny_LWM2M_CONTENT_TEXT, NULL, 0, contextP->monitorUserData);
        }
        xiny_registration_freeClient(clientP);
        result = xiny_COAP_202_DELETED;
    }
    break;

    default:
        return xiny_COAP_400_BAD_REQUEST;
    }

    return result;
}

void xiny_lwm2m_set_monitoring_callback(xiny_lwm2m_context_t * contextP,
                                   xiny_lwm2m_result_callback_t callback,
                                   void * userData)
{
    LOG("Entering");
    contextP->monitorCallback = callback;
    contextP->monitorUserData = userData;
}
#endif

// for each server update the xiny_registration if needed
// for each client check if the xiny_registration expired
void xiny_registration_step_lwm2m(xiny_lwm2m_context_t * contextP,
                       time_t currentTime,
                       time_t * timeoutP)
{
#ifdef XINYI_LWM2M_CLIENT_MODE
    xiny_lwm2m_server_lwm2m_t * targetP = contextP->serverList;

    LOG_ARG("State: %s", xiny_STR_STATE(contextP->state));

    targetP = contextP->serverList;
    while (targetP != NULL)
    {
        switch (targetP->status)
        {
        case xiny_STATE_REGISTERED:
        {
            time_t nextUpdate;
            time_t interval;

            nextUpdate = targetP->lifetime;
            if (xiny_COAP_MAX_TRANSMIT_WAIT < nextUpdate)
            {
                nextUpdate -= xiny_COAP_MAX_TRANSMIT_WAIT;
            }
            else
            {
                nextUpdate = nextUpdate >> 1;
            }

            interval = targetP->xiny_registration + nextUpdate - currentTime;
            if (0 >= interval)
            {
                LOG("Updating xiny_registration");
                prv_updateRegistration(contextP, targetP, false);
            }
            else if (interval < *timeoutP)
            {
                *timeoutP = interval;
            }
        }
        break;

        case xiny_STATE_REG_UPDATE_NEEDED:
            prv_updateRegistration(contextP, targetP, false);
            break;

        case xiny_STATE_REG_FULL_UPDATE_NEEDED:
            prv_updateRegistration(contextP, targetP, true);
            break;

        case xiny_STATE_REG_FAILED:
            if (targetP->sessionH != NULL)
            {
                xiny_lwm2m_close_connection(targetP->sessionH, contextP->userData);
                targetP->sessionH = NULL;
            }
            break;

        default:
            break;
        }
        targetP = targetP->next;
    }

#endif
#ifdef xiny_LWM2M_SERVER_MODE
    xiny_lwm2m_client_t * clientP;

    LOG("Entering");
    // monitor clients lifetime
    clientP = contextP->clientList;
    while (clientP != NULL)
    {
        xiny_lwm2m_client_t * nextP = clientP->next;

        if (clientP->endOfLife <= currentTime)
        {
            contextP->clientList = (xiny_lwm2m_client_t *)xiny_LWM2M_LIST_RM(contextP->clientList, clientP->internalID, NULL);
            if (contextP->monitorCallback != NULL)
            {
                contextP->monitorCallback(clientP->internalID, NULL, xiny_COAP_202_DELETED, xiny_LWM2M_CONTENT_TEXT, NULL, 0, contextP->monitorUserData);
            }
            xiny_registration_freeClient(clientP);
        }
        else
        {
            time_t interval;

            interval = clientP->endOfLife - currentTime;

            if (*timeoutP > interval)
            {
                *timeoutP = interval;
            }
        }
        clientP = nextP;
    }
#endif

}
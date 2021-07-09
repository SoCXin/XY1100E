/*******************************************************************************
 *
 * Copyright (c) 2015 Sierra Wireless and others.
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
 *    Pascal Rieux - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *
 *******************************************************************************/

#include "xiny_internals.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef XINYI_LWM2M_CLIENT_MODE
#ifdef xiny_LWM2M_BOOTSTRAP

#define PRV_QUERY_BUFFER_LENGTH 200


static void prv_handleResponse(xiny_lwm2m_server_lwm2m_t * xiny_bootstrapServer,
                               xiny_coap_packet_t * message)
{
    if (xiny_COAP_204_CHANGED == message->code)
    {
        LOG("Received ACK/2.04, Bootstrap xiny_pending, waiting for DEL/PUT from BS server...");
        xiny_bootstrapServer->status = xiny_STATE_BS_PENDING;
        xiny_bootstrapServer->xiny_registration = xiny_lwm2m_gettime() + xiny_COAP_EXCHANGE_LIFETIME;
    }
    else
    {
        xiny_bootstrapServer->status = xiny_STATE_BS_FAILING;
    }
}

static void prv_handleBootstrapReply(xiny_lwm2m_transaction_t * transaction,
                                     void * message)
{
    xiny_lwm2m_server_lwm2m_t * xiny_bootstrapServer = (xiny_lwm2m_server_lwm2m_t *)transaction->userData;
    xiny_coap_packet_t * coapMessage = (xiny_coap_packet_t *)message;

    LOG("Entering");

    if (xiny_bootstrapServer->status == xiny_STATE_BS_INITIATED)
    {
        if (NULL != coapMessage && xiny_COAP_TYPE_RST != coapMessage->type)
        {
            prv_handleResponse(xiny_bootstrapServer, coapMessage);
        }
        else
        {
            xiny_bootstrapServer->status = xiny_STATE_BS_FAILING;
        }
    }
}

// start a device initiated xiny_bootstrap
static void prv_xiny_requestBootstrap(xiny_lwm2m_context_t * context,
                                 xiny_lwm2m_server_lwm2m_t * xiny_bootstrapServer)
{
    char query[PRV_QUERY_BUFFER_LENGTH];
    int query_length = 0;
    int res;

    LOG("Entering");

    query_length = xiny_utils_stringCopy(query, PRV_QUERY_BUFFER_LENGTH, xiny_QUERY_STARTER xiny_QUERY_NAME);
    if (query_length < 0)
    {
        xiny_bootstrapServer->status = xiny_STATE_BS_FAILING;
        return;
    }
    res = xiny_utils_stringCopy(query + query_length, PRV_QUERY_BUFFER_LENGTH - query_length, context->endpointName);
    if (res < 0)
    {
        xiny_bootstrapServer->status = xiny_STATE_BS_FAILING;
        return;
    }
    query_length += res;

    if (xiny_bootstrapServer->sessionH == NULL)
    {
        xiny_bootstrapServer->sessionH = xiny_lwm2m_connect_server(xiny_bootstrapServer->secObjInstID, context->userData);
    }

    if (xiny_bootstrapServer->sessionH != NULL)
    {
        xiny_lwm2m_transaction_t * transaction = NULL;

        LOG("Bootstrap server connection opened");

        transaction = xiny_transaction_new(xiny_bootstrapServer->sessionH, xiny_COAP_POST, NULL, NULL, context->nextMID++, 4, NULL);
        if (transaction == NULL)
        {
            xiny_bootstrapServer->status = xiny_STATE_BS_FAILING;
            return;
        }

        xiny_coap_set_header_uri_path(transaction->message, "/"xiny_URI_BOOTSTRAP_SEGMENT);
        xiny_coap_set_header_uri_query(transaction->message, query);
        transaction->callback = prv_handleBootstrapReply;
        transaction->userData = (void *)xiny_bootstrapServer;
        context->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(context->transactionList, transaction);
        if (xiny_transaction_send(context, transaction) == 0)
        {
            LOG("CI xiny_bootstrap xiny_requested to BS server");
            xiny_bootstrapServer->status = xiny_STATE_BS_INITIATED;
        }
    }
    else
    {
        LOG("Connecting xiny_bootstrap server failed");
        xiny_bootstrapServer->status = xiny_STATE_BS_FAILED;
    }
}

void xiny_bootstrap_step(xiny_lwm2m_context_t * contextP,
                    time_t currentTime,
                    time_t * timeoutP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;

    LOG("entering");
    targetP = contextP->xiny_bootstrapServerList;
    while (targetP != NULL)
    {
        LOG_ARG("Initial status: %s", xiny_STR_STATUS(targetP->status));
        switch (targetP->status)
        {
        case xiny_STATE_DEREGISTERED:
            targetP->xiny_registration = currentTime + targetP->lifetime;
            targetP->status = xiny_STATE_BS_HOLD_OFF;
            if (*timeoutP > targetP->lifetime)
            {
                *timeoutP = targetP->lifetime;
            }
            break;

        case xiny_STATE_BS_HOLD_OFF:
            if (targetP->xiny_registration <= currentTime)
            {
                prv_xiny_requestBootstrap(contextP, targetP);
            }
            else if (*timeoutP > targetP->xiny_registration - currentTime)
            {
                *timeoutP = targetP->xiny_registration - currentTime;
            }
            break;

        case xiny_STATE_BS_INITIATED:
            // waiting
            break;

        case xiny_STATE_BS_PENDING:
            if (targetP->xiny_registration <= currentTime)
            {
               targetP->status = xiny_STATE_BS_FAILING;
               *timeoutP = 0;
            }
            else if (*timeoutP > targetP->xiny_registration - currentTime)
            {
                *timeoutP = targetP->xiny_registration - currentTime;
            }
            break;

        case xiny_STATE_BS_FINISHING:
            if (targetP->sessionH != NULL)
            {
                xiny_lwm2m_close_connection(targetP->sessionH, contextP->userData);
                targetP->sessionH = NULL;
            }
            targetP->status = xiny_STATE_BS_FINISHED;
            *timeoutP = 0;
            break;

        case xiny_STATE_BS_FAILING:
            if (targetP->sessionH != NULL)
            {
                xiny_lwm2m_close_connection(targetP->sessionH, contextP->userData);
                targetP->sessionH = NULL;
            }
            targetP->status = xiny_STATE_BS_FAILED;
            *timeoutP = 0;
            break;

        default:
            break;
        }
        LOG_ARG("Finalal status: %s", xiny_STR_STATUS(targetP->status));
        targetP = targetP->next;
    }
}

uint8_t xiny_bootstrap_handleFinish(xiny_lwm2m_context_t * context,
                               void * fromSessionH)
{
    xiny_lwm2m_server_lwm2m_t * xiny_bootstrapServer;

    LOG("Entering");
    xiny_bootstrapServer = xiny_utils_findBootstrapServer(context, fromSessionH);
    if (xiny_bootstrapServer != NULL
     && xiny_bootstrapServer->status == xiny_STATE_BS_PENDING)
    {
        if (xiny_object_getServers_lwm2m(context, true) == 0)
        {
            LOG("Bootstrap server status changed to xiny_STATE_BS_FINISHING");
            xiny_bootstrapServer->status = xiny_STATE_BS_FINISHING;
            return xiny_COAP_204_CHANGED;
        }
        else
        {
           return xiny_COAP_406_NOT_ACCEPTABLE;
        }
    }

    return xiny_COAP_IGNORE;
}

/*
 * Reset the xiny_bootstrap servers statuses
 *
 * TODO: handle LWM2M Servers the client is xiny_registered to ?
 *
 */
void xiny_bootstrap_start(xiny_lwm2m_context_t * contextP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;

    LOG("Entering");
    targetP = contextP->xiny_bootstrapServerList;
    while (targetP != NULL)
    {
        targetP->status = xiny_STATE_DEREGISTERED;
        if (targetP->sessionH == NULL)
        {
            targetP->sessionH = xiny_lwm2m_connect_server(targetP->secObjInstID, contextP->userData);
        }
        targetP = targetP->next;
    }
}

/*
 * Returns xiny_STATE_BS_PENDING if at least one xiny_bootstrap is still xiny_pending
 * Returns xiny_STATE_BS_FINISHED if at least one xiny_bootstrap succeeded and no xiny_bootstrap is xiny_pending
 * Returns xiny_STATE_BS_FAILED if all xiny_bootstrap failed.
 */
xiny_lwm2m_status_lwm2m_t xiny_bootstrap_getStatus(xiny_lwm2m_context_t * contextP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;
    xiny_lwm2m_status_lwm2m_t bs_status;

    LOG("Entering");
    targetP = contextP->xiny_bootstrapServerList;
    bs_status = xiny_STATE_BS_FAILED;

    while (targetP != NULL)
    {
        switch (targetP->status)
        {
            case xiny_STATE_BS_FINISHED:
                if (bs_status == xiny_STATE_BS_FAILED)
                {
                    bs_status = xiny_STATE_BS_FINISHED;
                }
                break;

            case xiny_STATE_BS_HOLD_OFF:
            case xiny_STATE_BS_INITIATED:
            case xiny_STATE_BS_PENDING:
            case xiny_STATE_BS_FINISHING:
            case xiny_STATE_BS_FAILING:
                bs_status = xiny_STATE_BS_PENDING;
                break;

            default:
                break;
        }
        targetP = targetP->next;
    }

    LOG_ARG("Returned status: %s", xiny_STR_STATUS(bs_status));

    return bs_status;
}

static uint8_t prv_checkServerStatus(xiny_lwm2m_server_lwm2m_t * serverP)
{
    LOG_ARG("Initial status: %s", xiny_STR_STATUS(serverP->status));

    switch (serverP->status)
    {
    case xiny_STATE_BS_HOLD_OFF:
        serverP->status = xiny_STATE_BS_PENDING;
        LOG_ARG("Status changed to: %s", xiny_STR_STATUS(serverP->status));
        break;

    case xiny_STATE_BS_INITIATED:
        // The ACK was probably lost
        serverP->status = xiny_STATE_BS_PENDING;
        LOG_ARG("Status changed to: %s", xiny_STR_STATUS(serverP->status));
        break;

    case xiny_STATE_DEREGISTERED:
        // server initiated xiny_bootstrap
    case xiny_STATE_BS_PENDING:
        serverP->xiny_registration = xiny_lwm2m_gettime() + xiny_COAP_EXCHANGE_LIFETIME;
        break;

    case xiny_STATE_BS_FINISHED:
    case xiny_STATE_BS_FINISHING:
    case xiny_STATE_BS_FAILING:
    case xiny_STATE_BS_FAILED:
    default:
        LOG("Returning xiny_COAP_IGNORE");
        return xiny_COAP_IGNORE;
    }

    return xiny_COAP_NO_ERROR;
}

static void prv_tagServer(xiny_lwm2m_context_t * contextP,
                          uint16_t id)
{
    xiny_lwm2m_server_lwm2m_t * targetP;

    targetP = (xiny_lwm2m_server_lwm2m_t *)xiny_LWM2M_LIST_FIND(contextP->xiny_bootstrapServerList, id);
    if (targetP == NULL)
    {
        targetP = (xiny_lwm2m_server_lwm2m_t *)xiny_LWM2M_LIST_FIND(contextP->serverList, id);
    }
    if (targetP != NULL)
    {
        targetP->dirty = true;
    }
}

static void prv_tagAllServer(xiny_lwm2m_context_t * contextP,
                             xiny_lwm2m_server_lwm2m_t * serverP)
{
    xiny_lwm2m_server_lwm2m_t * targetP;

    targetP = contextP->xiny_bootstrapServerList;
    while (targetP != NULL)
    {
        if (targetP != serverP)
        {
            targetP->dirty = true;
        }
        targetP = targetP->next;
    }
    targetP = contextP->serverList;
    while (targetP != NULL)
    {
        targetP->dirty = true;
        targetP = targetP->next;
    }
}

uint8_t xiny_bootstrap_handleCommand(xiny_lwm2m_context_t * contextP,
                                xiny_lwm2m_uri_t * uriP,
                                xiny_lwm2m_server_lwm2m_t * serverP,
                                xiny_coap_packet_t * message,
                                xiny_coap_packet_t * response)
{
    uint8_t result;
    xiny_lwm2m_media_type_lwm2m_t format;

    LOG_ARG("Code: %02X", message->code);
    LOG_URI(uriP);
    format = xiny_utils_convertMediaType(message->content_type);

    result = prv_checkServerStatus(serverP);
    if (result != xiny_COAP_NO_ERROR) return result;

    switch (message->code)
    {
    case xiny_COAP_PUT:
        {
            if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
            {
                if (xiny_object_isInstanceNew_lwm2m(contextP, uriP->objectId, uriP->instanceId))
                {
                    result = xiny_object_create_lwm2m(contextP, uriP, format, message->payload, message->payload_len);
                    if (xiny_COAP_201_CREATED == result)
                    {
                        result = xiny_COAP_204_CHANGED;
                    }
                }
                else
                {
                    result = xiny_object_write_lwm2m(contextP, uriP, format, message->payload, message->payload_len);
                    if (uriP->objectId == xiny_LWM2M_SECURITY_OBJECT_ID
                     && result == xiny_COAP_204_CHANGED)
                    {
                        prv_tagServer(contextP, uriP->instanceId);
                    }
                }
            }
            else
            {
                xiny_lwm2m_data_t * dataP = NULL;
                int size = 0;
                int i;

                if (message->payload_len == 0 || message->payload == 0)
                {
                    result = xiny_COAP_400_BAD_REQUEST;
                }
                else
                {
                    size = xiny_lwm2m_data_parse(uriP, message->payload, message->payload_len, format, &dataP);
                    if (size == 0)
                    {
                        result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
                        break;
                    }

                    for (i = 0 ; i < size ; i++)
                    {
                        if(dataP[i].type == xiny_LWM2M_TYPE_OBJECT_INSTANCE)
                        {
                            if (xiny_object_isInstanceNew_lwm2m(contextP, uriP->objectId, dataP[i].id))
                            {
                                result = xiny_object_create_lwm2mInstance(contextP, uriP, &dataP[i]);
                                if (xiny_COAP_201_CREATED == result)
                                {
                                    result = xiny_COAP_204_CHANGED;
                                }
                            }
                            else
                            {
                                result = xiny_object_write_lwm2mInstance(contextP, uriP, &dataP[i]);
                                if (uriP->objectId == xiny_LWM2M_SECURITY_OBJECT_ID
                                 && result == xiny_COAP_204_CHANGED)
                                {
                                    prv_tagServer(contextP, dataP[i].id);
                                }
                            }

                            if(result != xiny_COAP_204_CHANGED) // Stop object create or write when result is error
                            {
                                break;
                            }
                        }
                        else
                        {
                            result = xiny_COAP_400_BAD_REQUEST;
                        }
                    }
                    xiny_lwm2m_data_free(size, dataP);
                }
            }
        }
        break;

    case xiny_COAP_DELETE:
        {
            if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                result = xiny_COAP_400_BAD_REQUEST;
            }
            else
            {
                result = xiny_object_delete_lwm2m(contextP, uriP);
                if (uriP->objectId == xiny_LWM2M_SECURITY_OBJECT_ID
                 && result == xiny_COAP_202_DELETED)
                {
                    if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
                    {
                        prv_tagServer(contextP, uriP->instanceId);
                    }
                    else
                    {
                        prv_tagAllServer(contextP, NULL);
                    }
                }
            }
        }
        break;

    case xiny_COAP_GET:
    case xiny_COAP_POST:
    default:
        result = xiny_COAP_400_BAD_REQUEST;
        break;
    }

    if (result == xiny_COAP_202_DELETED
     || result == xiny_COAP_204_CHANGED)
    {
        if (serverP->status != xiny_STATE_BS_PENDING)
        {
            serverP->status = xiny_STATE_BS_PENDING;
            contextP->state = xiny_STATE_BOOTSTRAPPING;
        }
    }
    LOG_ARG("Server status: %s", xiny_STR_STATUS(serverP->status));

    return result;
}

uint8_t xiny_bootstrap_handleDeleteAll(xiny_lwm2m_context_t * contextP,
                                  void * fromSessionH)
{
    xiny_lwm2m_server_lwm2m_t * serverP;
    uint8_t result;
    xiny_lwm2m_object_t * objectP;

    LOG("Entering");
    serverP = xiny_utils_findBootstrapServer(contextP, fromSessionH);
    if (serverP == NULL) return xiny_COAP_IGNORE;
    result = prv_checkServerStatus(serverP);
    if (result != xiny_COAP_NO_ERROR) return result;

    result = xiny_COAP_202_DELETED;
    for (objectP = contextP->objectList; objectP != NULL; objectP = objectP->next)
    {
        xiny_lwm2m_uri_t uri;

        memset(&uri, 0, sizeof(xiny_lwm2m_uri_t));
        uri.flag = xiny_LWM2M_URI_FLAG_OBJECT_ID;
        uri.objectId = objectP->objID;

        if (objectP->objID == xiny_LWM2M_SECURITY_OBJECT_ID)
        {
            xiny_lwm2m_list_t * instanceP;

            instanceP = objectP->instanceList;
            while (NULL != instanceP
                && result == xiny_COAP_202_DELETED)
            {
                if (instanceP->id == serverP->secObjInstID)
                {
                    instanceP = instanceP->next;
                }
                else
                {
                    uri.flag = xiny_LWM2M_URI_FLAG_OBJECT_ID | xiny_LWM2M_URI_FLAG_INSTANCE_ID;
                    uri.instanceId = instanceP->id;
                    result = xiny_object_delete_lwm2m(contextP, &uri);
                    instanceP = objectP->instanceList;
                }
            }
            if (result == xiny_COAP_202_DELETED)
            {
                prv_tagAllServer(contextP, serverP);
            }
        }
        else
        {
            result = xiny_object_delete_lwm2m(contextP, &uri);
            if (result == xiny_COAP_405_METHOD_NOT_ALLOWED)
            {
                // Fake a successful deletion for static xiny_objects like the Device object.
                result = xiny_COAP_202_DELETED;
            }
        }
    }

    return result;
}
#endif
#endif

#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
uint8_t xiny_bootstrap_handleRequest(xiny_lwm2m_context_t * contextP,
                                xiny_lwm2m_uri_t * uriP,
                                void * fromSessionH,
                                xiny_coap_packet_t * message,
                                xiny_coap_packet_t * response)
{
    uint8_t result;
    char * name;

    LOG_URI(uriP);
    if (contextP->xiny_bootstrapCallback == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    if (message->code != xiny_COAP_POST) return xiny_COAP_400_BAD_REQUEST;
    if (message->uri_query == NULL) return xiny_COAP_400_BAD_REQUEST;
    if (message->payload != NULL) return xiny_COAP_400_BAD_REQUEST;

    if (xiny_lwm2m_strncmp((char *)message->uri_query->data, xiny_QUERY_NAME, xiny_QUERY_NAME_LEN) != 0)
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    if (message->uri_query->len == xiny_QUERY_NAME_LEN) return xiny_COAP_400_BAD_REQUEST;
    if (message->uri_query->next != NULL) return xiny_COAP_400_BAD_REQUEST;

    name = (char *)xiny_lwm2m_malloc(message->uri_query->len - xiny_QUERY_NAME_LEN + 1);
    if (name == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    memcpy(name, message->uri_query->data + xiny_QUERY_NAME_LEN, message->uri_query->len - xiny_QUERY_NAME_LEN);
    name[message->uri_query->len - xiny_QUERY_NAME_LEN] = 0;

    result = contextP->xiny_bootstrapCallback(fromSessionH, xiny_COAP_NO_ERROR, NULL, name, contextP->xiny_bootstrapUserData);

    xiny_lwm2m_free(name);

    return result;
}

void xiny_lwm2m_set_bootstrap_callback(xiny_lwm2m_context_t * contextP,
                                  lwm2m_xiny_bootstrap_callback_t callback,
                                  void * userData)
{
    LOG("Entering");
    contextP->xiny_bootstrapCallback = callback;
    contextP->xiny_bootstrapUserData = userData;
}

static void prv_resultCallback(xiny_lwm2m_transaction_t * transacP,
                               void * message)
{
    xiny_bs_data_t * dataP = (xiny_bs_data_t *)transacP->userData;
    xiny_lwm2m_uri_t * uriP;

    if (dataP->isUri == true)
    {
        uriP = &dataP->uri;
    }
    else
    {
        uriP = NULL;
    }

    if (message == NULL)
    {
        dataP->callback(transacP->peerH,
                        xiny_COAP_503_SERVICE_UNAVAILABLE,
                        uriP,
                        NULL,
                        dataP->userData);
    }
    else
    {
        xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;

        dataP->callback(transacP->peerH,
                        packet->code,
                        uriP,
                        NULL,
                        dataP->userData);
    }
    xiny_lwm2m_free(dataP);
}

int xiny_lwm2m_bootstrap_delete(xiny_lwm2m_context_t * contextP,
                           void * sessionH,
                           xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_transaction_t * transaction;
    xiny_bs_data_t * dataP;

    LOG_URI(uriP);
    transaction = xiny_transaction_new(sessionH, xiny_COAP_DELETE, NULL, uriP, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    dataP = (xiny_bs_data_t *)xiny_lwm2m_malloc(sizeof(xiny_bs_data_t));
    if (dataP == NULL)
    {
        xiny_transaction_free(transaction);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }
    if (uriP == NULL)
    {
        dataP->isUri = false;
    }
    else
    {
        dataP->isUri = true;
        memcpy(&dataP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
    }
    dataP->callback = contextP->xiny_bootstrapCallback;
    dataP->userData = contextP->xiny_bootstrapUserData;

    transaction->callback = prv_resultCallback;
    transaction->userData = (void *)dataP;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

int xiny_lwm2m_bootstrap_write(xiny_lwm2m_context_t * contextP,
                          void * sessionH,
                          xiny_lwm2m_uri_t * uriP,
                          xiny_lwm2m_media_type_lwm2m_t format,
                          uint8_t * buffer,
                          size_t length)
{
    xiny_lwm2m_transaction_t * transaction;
    xiny_bs_data_t * dataP;

    LOG_URI(uriP);
    if (uriP == NULL
    || buffer == NULL
    || length == 0)
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    transaction = xiny_transaction_new(sessionH, xiny_COAP_PUT, NULL, uriP, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    xiny_coap_set_header_content_type(transaction->message, format);
    xiny_coap_set_payload(transaction->message, buffer, length);

    dataP = (xiny_bs_data_t *)xiny_lwm2m_malloc(sizeof(xiny_bs_data_t));
    if (dataP == NULL)
    {
        xiny_transaction_free(transaction);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }
    dataP->isUri = true;
    memcpy(&dataP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
    dataP->callback = contextP->xiny_bootstrapCallback;
    dataP->userData = contextP->xiny_bootstrapUserData;

    transaction->callback = prv_resultCallback;
    transaction->userData = (void *)dataP;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

int xiny_lwm2m_bootstrap_finish(xiny_lwm2m_context_t * contextP,
                           void * sessionH)
{
    xiny_lwm2m_transaction_t * transaction;
    xiny_bs_data_t * dataP;

    LOG("Entering");
    transaction = xiny_transaction_new(sessionH, xiny_COAP_POST, NULL, NULL, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    xiny_coap_set_header_uri_path(transaction->message, "/"xiny_URI_BOOTSTRAP_SEGMENT);

    dataP = (xiny_bs_data_t *)xiny_lwm2m_malloc(sizeof(xiny_bs_data_t));
    if (dataP == NULL)
    {
        xiny_transaction_free(transaction);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }
    dataP->isUri = false;
    dataP->callback = contextP->xiny_bootstrapCallback;
    dataP->userData = contextP->xiny_bootstrapUserData;

    transaction->callback = prv_resultCallback;
    transaction->userData = (void *)dataP;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

#endif
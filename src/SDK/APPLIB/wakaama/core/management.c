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
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
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
#include <stdio.h>


#ifdef XINYI_LWM2M_CLIENT_MODE
static int prv_readAttributes(xiny_multi_option_t * query,
                              xiny_lwm2m_attributes_t * attrP)
{
    int64_t intValue;
    double floatValue;

    memset(attrP, 0, sizeof(xiny_lwm2m_attributes_t));

    while (query != NULL)
    {
        if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & LWM2M_ATTR_FLAG_MIN_PERIOD)) return -1;
            if (query->len == xiny_ATTR_MIN_PERIOD_LEN) return -1;

            if (1 != xiny_utils_textToInt(query->data + xiny_ATTR_MIN_PERIOD_LEN, query->len - xiny_ATTR_MIN_PERIOD_LEN, &intValue)) return -1;
            if (intValue < 0) return -1;

            attrP->toSet |= LWM2M_ATTR_FLAG_MIN_PERIOD;
            attrP->minPeriod = intValue;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN - 1) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & LWM2M_ATTR_FLAG_MIN_PERIOD)) return -1;
            if (query->len != xiny_ATTR_MIN_PERIOD_LEN - 1) return -1;

            attrP->toClear |= LWM2M_ATTR_FLAG_MIN_PERIOD;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)) return -1;
            if (query->len == xiny_ATTR_MAX_PERIOD_LEN) return -1;

            if (1 != xiny_utils_textToInt(query->data + xiny_ATTR_MAX_PERIOD_LEN, query->len - xiny_ATTR_MAX_PERIOD_LEN, &intValue)) return -1;
            if (intValue < 0) return -1;

            attrP->toSet |= xiny_LWM2M_ATTR_FLAG_MAX_PERIOD;
            attrP->maxPeriod = intValue;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN - 1) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)) return -1;
            if (query->len != xiny_ATTR_MAX_PERIOD_LEN - 1) return -1;

            attrP->toClear |= xiny_LWM2M_ATTR_FLAG_MAX_PERIOD;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_GREATER_THAN_STR, xiny_ATTR_GREATER_THAN_LEN) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)) return -1;
            if (query->len == xiny_ATTR_GREATER_THAN_LEN) return -1;

            if (1 != xiny_utils_textToFloat(query->data + xiny_ATTR_GREATER_THAN_LEN, query->len - xiny_ATTR_GREATER_THAN_LEN, &floatValue)) return -1;

            attrP->toSet |= xiny_LWM2M_ATTR_FLAG_GREATER_THAN;
            attrP->greaterThan = floatValue;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_GREATER_THAN_STR, xiny_ATTR_GREATER_THAN_LEN - 1) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)) return -1;
            if (query->len != xiny_ATTR_GREATER_THAN_LEN - 1) return -1;

            attrP->toClear |= xiny_LWM2M_ATTR_FLAG_GREATER_THAN;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_LESS_THAN_STR, xiny_ATTR_LESS_THAN_LEN) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_LESS_THAN)) return -1;
            if (query->len == xiny_ATTR_LESS_THAN_LEN) return -1;

            if (1 != xiny_utils_textToFloat(query->data + xiny_ATTR_LESS_THAN_LEN, query->len - xiny_ATTR_LESS_THAN_LEN, &floatValue)) return -1;

            attrP->toSet |= xiny_LWM2M_ATTR_FLAG_LESS_THAN;
            attrP->lessThan = floatValue;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_LESS_THAN_STR, xiny_ATTR_LESS_THAN_LEN - 1) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_LESS_THAN)) return -1;
            if (query->len != xiny_ATTR_LESS_THAN_LEN - 1) return -1;

            attrP->toClear |= xiny_LWM2M_ATTR_FLAG_LESS_THAN;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_STEP_STR, xiny_ATTR_STEP_LEN) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_STEP)) return -1;
            if (query->len == xiny_ATTR_STEP_LEN) return -1;

            if (1 != xiny_utils_textToFloat(query->data + xiny_ATTR_STEP_LEN, query->len - xiny_ATTR_STEP_LEN, &floatValue)) return -1;
            if (floatValue < 0) return -1;

            attrP->toSet |= xiny_LWM2M_ATTR_FLAG_STEP;
            attrP->step = floatValue;
        }
        else if (xiny_lwm2m_strncmp((char *)query->data, xiny_ATTR_STEP_STR, xiny_ATTR_STEP_LEN - 1) == 0)
        {
            if (0 != ((attrP->toSet | attrP->toClear) & xiny_LWM2M_ATTR_FLAG_STEP)) return -1;
            if (query->len != xiny_ATTR_STEP_LEN - 1) return -1;

            attrP->toClear |= xiny_LWM2M_ATTR_FLAG_STEP;
        }
        else return -1;

        query = query->next;
    }

    return 0;
}

uint8_t xiny_dm_handleRequest(xiny_lwm2m_context_t * contextP,
                         xiny_lwm2m_uri_t * uriP,
                         xiny_lwm2m_server_lwm2m_t * serverP,
                         xiny_coap_packet_t * message,
                         xiny_coap_packet_t * response)
{
    uint8_t result;
    xiny_lwm2m_media_type_lwm2m_t format;

    LOG_ARG("Code: %02X, server status: %s", message->code, xiny_STR_STATUS(serverP->status));
    LOG_URI(uriP);

    if (xiny_IS_OPTION(message, xiny_COAP_OPTION_CONTENT_TYPE))
    {
        format = xiny_utils_convertMediaType(message->content_type);
    }
    else
    {
        format = xiny_LWM2M_CONTENT_TLV;
    }

    if (uriP->objectId == xiny_LWM2M_SECURITY_OBJECT_ID)
    {
        return xiny_COAP_404_NOT_FOUND;
    }

    if (serverP->status != xiny_STATE_REGISTERED
        && serverP->status != xiny_STATE_REG_UPDATE_NEEDED
        && serverP->status != xiny_STATE_REG_FULL_UPDATE_NEEDED
        && serverP->status != xiny_STATE_REG_UPDATE_PENDING)
    {
        return xiny_COAP_IGNORE;
    }

    // TODO: check ACL

    switch (message->code)
    {
    case xiny_COAP_GET:
        {
            uint8_t * buffer = NULL;
            size_t length = 0;
            int res;

            if (xiny_IS_OPTION(message, xiny_COAP_OPTION_OBSERVE))
            {
                xiny_lwm2m_data_t * dataP = NULL;
                int size = 0;

                result = xiny_object_read_lwm2mData_lwm2m(contextP, uriP, &size, &dataP);
                if (xiny_COAP_205_CONTENT == result)
                {
                    result = xiny_observe_handleRequest_lwm2m(contextP, uriP, serverP, size, dataP, message, response);
                    if (xiny_COAP_205_CONTENT == result)
                    {
                        if (xiny_IS_OPTION(message, xiny_COAP_OPTION_ACCEPT))
                        {
                            format = xiny_utils_convertMediaType(message->accept[0]);
                        }
                        else
                        {
                            format = xiny_LWM2M_CONTENT_TLV;
                        }

                        res = xiny_lwm2m_data_serialize(uriP, size, dataP, &format, &buffer);
                        if (res < 0)
                        {
                            result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
                        }
                        else
                        {
                            length = (size_t)res;
                            LOG_ARG("Observe Request[/%d/%d/%d]: %.*s\n", uriP->objectId, uriP->instanceId, uriP->resourceId, length, buffer);
                        }
                    }
                    xiny_lwm2m_data_free(size, dataP);
                }
            }
            else if (xiny_IS_OPTION(message, xiny_COAP_OPTION_ACCEPT)
                  && message->accept_num == 1
                  && message->accept[0] == xiny_APPLICATION_LINK_FORMAT)
            {
                format = xiny_LWM2M_CONTENT_LINK;
                result = xiny_object_discover_lwm2m(contextP, uriP, serverP, &buffer, &length);
            }
            else
            {
                if (xiny_IS_OPTION(message, xiny_COAP_OPTION_ACCEPT))
                {
                    format = xiny_utils_convertMediaType(message->accept[0]);
                }

                result = xiny_object_read_lwm2m(contextP, uriP, &format, &buffer, &length);
            }
            if (xiny_COAP_205_CONTENT == result)
            {
                xiny_coap_set_header_content_type(response, format);
                xiny_coap_set_payload(response, buffer, length);
                // xiny_lwm2m_handle_packet will free buffer
            }
            else
            {
                xiny_lwm2m_free(buffer);
            }
        }
        break;

    case xiny_COAP_POST:
        {
            if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
            {
                result = xiny_object_create_lwm2m(contextP, uriP, format, message->payload, message->payload_len);
                if (result == xiny_COAP_201_CREATED)
                {
                    //longest uri is /65535/65535 = 12 + 1 (null) chars
                    char location_path[13] = "";
                    //instanceId expected
                    if ((uriP->flag & xiny_LWM2M_URI_FLAG_INSTANCE_ID) == 0)
                    {
                        result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
                        break;
                    }

                    if (sprintf(location_path, "/%d/%d", uriP->objectId, uriP->instanceId) < 0)
                    {
                        result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
                        break;
                    }
                    xiny_coap_set_header_location_path(response, location_path);

                    xiny_lwm2m_update_xiny_registration(contextP, 0, true);
                }
            }
            else if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                result = xiny_object_write_lwm2m(contextP, uriP, format, message->payload, message->payload_len);
            }
            else
            {
                result = xiny_object_execute_lwm2m(contextP, uriP, message->payload, message->payload_len);
            }
        }
        break;

    case xiny_COAP_PUT:
        {
            if (xiny_IS_OPTION(message, xiny_COAP_OPTION_URI_QUERY))
            {
                xiny_lwm2m_attributes_t attr;

                if (0 != prv_readAttributes(message->uri_query, &attr))
                {
                    result = xiny_COAP_400_BAD_REQUEST;
                }
                else
                {
                    result = xiny_observe_setParameters_lwm2m(contextP, uriP, serverP, &attr);
                }
            }
            else if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
            {
                result = xiny_object_write_lwm2m(contextP, uriP, format, message->payload, message->payload_len);
            }
            else
            {
                result = xiny_COAP_400_BAD_REQUEST;
            }
        }
        break;

    case xiny_COAP_DELETE:
        {
            if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) || xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                result = xiny_COAP_400_BAD_REQUEST;
            }
            else
            {
                result = xiny_object_delete_lwm2m(contextP, uriP);
                if (result == xiny_COAP_202_DELETED)
                {
                    xiny_lwm2m_update_xiny_registration(contextP, 0, true);
                }
            }
        }
        break;

    default:
        result = xiny_COAP_400_BAD_REQUEST;
        break;
    }

    return result;
}

#endif

#ifdef xiny_LWM2M_SERVER_MODE

#define ID_AS_STRING_MAX_LEN 8

static void prv_resultCallback(xiny_lwm2m_transaction_t * transacP,
                               void * message)
{
    dm_data_t * dataP = (dm_data_t *)transacP->userData;

    if (message == NULL)
    {
        dataP->callback(dataP->clientID,
                        &dataP->uri,
                        xiny_COAP_503_SERVICE_UNAVAILABLE,
                        xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                        dataP->userData);
    }
    else
    {
        xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;

        //if packet is a CREATE response and the instanceId was assigned by the client
        if (packet->code == xiny_COAP_201_CREATED
         && packet->location_path != NULL)
        {
            char * locationString = NULL;
            int result = 0;
            xiny_lwm2m_uri_t locationUri;

            locationString = xiny_coap_get_multi_option_as_string(packet->location_path);
            if (locationString == NULL)
            {
                LOG("Error: xiny_coap_get_multi_option_as_string() failed for Location_path option in prv_resultCallback()");
                return;
            }

            result = xiny_lwm2m_stringToUri(locationString, strlen(locationString), &locationUri);
            if (result == 0)
            {
                LOG("Error: xiny_lwm2m_stringToUri() failed for Location_path option in prv_resultCallback()");
                xiny_lwm2m_free(locationString);
                return;
            }

            ((dm_data_t*)transacP->userData)->uri.instanceId = locationUri.instanceId;
            ((dm_data_t*)transacP->userData)->uri.flag = locationUri.flag;

            xiny_lwm2m_free(locationString);
        }

        dataP->callback(dataP->clientID,
                        &dataP->uri,
                        packet->code,
                        xiny_utils_convertMediaType(packet->content_type),
                        packet->payload,
                        packet->payload_len,
                        dataP->userData);
    }
    xiny_lwm2m_free(dataP);
}

static int prv_makeOperation(xiny_lwm2m_context_t * contextP,
                             uint16_t clientID,
                             xiny_lwm2m_uri_t * uriP,
                             xiny_coap_method_t method,
                             xiny_lwm2m_media_type_lwm2m_t format,
                             uint8_t * buffer,
                             int length,
                             xiny_lwm2m_result_callback_t callback,
                             void * userData)
{
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_transaction_t * transaction;
    dm_data_t * dataP;

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    transaction = xiny_transaction_new(clientP->sessionH, method, clientP->altPath, uriP, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    if (method == xiny_COAP_GET)
    {
        xiny_coap_set_header_accept(transaction->message, format);
    }
    else if (buffer != NULL)
    {
        xiny_coap_set_header_content_type(transaction->message, format);
        // TODO: Take care of fragmentation
        xiny_coap_set_payload(transaction->message, buffer, length);
    }

    if (callback != NULL)
    {
        dataP = (dm_data_t *)xiny_lwm2m_malloc(sizeof(dm_data_t));
        if (dataP == NULL)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        memcpy(&dataP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
        dataP->clientID = clientP->internalID;
        dataP->callback = callback;
        dataP->userData = userData;

        transaction->callback = prv_resultCallback;
        transaction->userData = (void *)dataP;
    }

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

int xiny_lwm2m_dm_read(xiny_lwm2m_context_t * contextP,
                  uint16_t clientID,
                  xiny_lwm2m_uri_t * uriP,
                  xiny_lwm2m_result_callback_t callback,
                  void * userData)
{
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_media_type_lwm2m_t format;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    if (clientP->supportJSON == true)
    {
        format = xiny_LWM2M_CONTENT_JSON;
    }
    else
    {
        format = xiny_LWM2M_CONTENT_TLV;
    }

    return prv_makeOperation(contextP, clientID, uriP,
                             xiny_COAP_GET,
                             format,
                             NULL, 0,
                             callback, userData);
}

int xiny_lwm2m_dm_write(xiny_lwm2m_context_t * contextP,
                   uint16_t clientID,
                   xiny_lwm2m_uri_t * uriP,
                   xiny_lwm2m_media_type_lwm2m_t format,
                   uint8_t * buffer,
                   int length,
                   xiny_lwm2m_result_callback_t callback,
                   void * userData)
{
    LOG_ARG("clientID: %d, format: %s, length: %d", clientID, xiny_STR_MEDIA_TYPE(format), length);
    LOG_URI(uriP);
    if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP)
     || length == 0)
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
    {
        return prv_makeOperation(contextP, clientID, uriP,
                                  xiny_COAP_PUT,
                                  format, buffer, length,
                                  callback, userData);
    }
    else
    {
        return prv_makeOperation(contextP, clientID, uriP,
                                  xiny_COAP_POST,
                                  format, buffer, length,
                                  callback, userData);
    }
}

int xiny_lwm2m_dm_execute(xiny_lwm2m_context_t * contextP,
                     uint16_t clientID,
                     xiny_lwm2m_uri_t * uriP,
                     xiny_lwm2m_media_type_lwm2m_t format,
                     uint8_t * buffer,
                     int length,
                     xiny_lwm2m_result_callback_t callback,
                     void * userData)
{
    LOG_ARG("clientID: %d, format: %s, length: %d", clientID, xiny_STR_MEDIA_TYPE(format), length);
    LOG_URI(uriP);
    if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    return prv_makeOperation(contextP, clientID, uriP,
                              xiny_COAP_POST,
                              format, buffer, length,
                              callback, userData);
}

int xiny_lwm2m_dm_create(xiny_lwm2m_context_t * contextP,
                    uint16_t clientID,
                    xiny_lwm2m_uri_t * uriP,
                    xiny_lwm2m_media_type_lwm2m_t format,
                    uint8_t * buffer,
                    int length,
                    xiny_lwm2m_result_callback_t callback,
                    void * userData)
{
    LOG_ARG("clientID: %d, format: %s, length: %d", clientID, xiny_STR_MEDIA_TYPE(format), length);
    LOG_URI(uriP);

    if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP)
     || length == 0)
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    return prv_makeOperation(contextP, clientID, uriP,
                              xiny_COAP_POST,
                              format, buffer, length,
                              callback, userData);
}

int xiny_lwm2m_dm_delete(xiny_lwm2m_context_t * contextP,
                    uint16_t clientID,
                    xiny_lwm2m_uri_t * uriP,
                    xiny_lwm2m_result_callback_t callback,
                    void * userData)
{
    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);
    if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP)
     || xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    return prv_makeOperation(contextP, clientID, uriP,
                              xiny_COAP_DELETE,
                              xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                              callback, userData);
}

int xiny_lwm2m_dm_write_attributes(xiny_lwm2m_context_t * contextP,
                              uint16_t clientID,
                              xiny_lwm2m_uri_t * uriP,
                              xiny_lwm2m_attributes_t * attrP,
                              xiny_lwm2m_result_callback_t callback,
                              void * userData)
{
#define _PRV_BUFFER_SIZE 32
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_transaction_t * transaction;
    xiny_coap_packet_t * coap_pkt;
    uint8_t buffer[_PRV_BUFFER_SIZE];
    size_t length;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);
    if (attrP == NULL) return xiny_COAP_400_BAD_REQUEST;

    if (0 != (attrP->toSet & attrP->toClear)) return xiny_COAP_400_BAD_REQUEST;
    if (0 != (attrP->toSet & xiny_ATTR_FLAG_NUMERIC) && !xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return xiny_COAP_400_BAD_REQUEST;
    if (xiny_ATTR_FLAG_NUMERIC == (attrP->toSet & xiny_ATTR_FLAG_NUMERIC)
     && (attrP->lessThan + 2 * attrP->step >= attrP->greaterThan)) return xiny_COAP_400_BAD_REQUEST;

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    transaction = xiny_transaction_new(clientP->sessionH, xiny_COAP_PUT, clientP->altPath, uriP, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    if (callback != NULL)
    {
        dm_data_t * dataP;

        dataP = (dm_data_t *)xiny_lwm2m_malloc(sizeof(dm_data_t));
        if (dataP == NULL)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        memcpy(&dataP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
        dataP->clientID = clientP->internalID;
        dataP->callback = callback;
        dataP->userData = userData;

        transaction->callback = prv_resultCallback;
        transaction->userData = (void *)dataP;
    }

    coap_pkt = (xiny_coap_packet_t *)transaction->message;
    xiny_free_multi_option(coap_pkt->uri_query);
    if (attrP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
    {
        memcpy(buffer, xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN);
        length = xiny_utils_intToText(attrP->minPeriod, buffer + xiny_ATTR_MIN_PERIOD_LEN, _PRV_BUFFER_SIZE - xiny_ATTR_MIN_PERIOD_LEN);
        if (length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), buffer, xiny_ATTR_MIN_PERIOD_LEN + length, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
    {
        memcpy(buffer, xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN);
        length = xiny_utils_intToText(attrP->maxPeriod, buffer + xiny_ATTR_MAX_PERIOD_LEN, _PRV_BUFFER_SIZE - xiny_ATTR_MAX_PERIOD_LEN);
        if (length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), buffer, xiny_ATTR_MAX_PERIOD_LEN + length, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)
    {
        memcpy(buffer, xiny_ATTR_GREATER_THAN_STR, xiny_ATTR_GREATER_THAN_LEN);
        length = xiny_utils_floatToText(attrP->greaterThan, buffer + xiny_ATTR_GREATER_THAN_LEN, _PRV_BUFFER_SIZE - xiny_ATTR_GREATER_THAN_LEN);
        if (length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), buffer, xiny_ATTR_GREATER_THAN_LEN + length, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_LESS_THAN)
    {
        memcpy(buffer, xiny_ATTR_LESS_THAN_STR, xiny_ATTR_LESS_THAN_LEN);
        length = xiny_utils_floatToText(attrP->lessThan, buffer + xiny_ATTR_LESS_THAN_LEN, _PRV_BUFFER_SIZE - xiny_ATTR_LESS_THAN_LEN);
        if (length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), buffer, xiny_ATTR_LESS_THAN_LEN + length, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_STEP)
    {
        memcpy(buffer, xiny_ATTR_STEP_STR, xiny_ATTR_STEP_LEN);
        length = xiny_utils_floatToText(attrP->step, buffer + xiny_ATTR_STEP_LEN, _PRV_BUFFER_SIZE - xiny_ATTR_STEP_LEN);
        if (length == 0)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), buffer, xiny_ATTR_STEP_LEN + length, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toClear & LWM2M_ATTR_FLAG_MIN_PERIOD)
    {
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), (uint8_t*)xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN -1, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toClear & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
    {
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), (uint8_t*)xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN - 1, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toClear & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)
    {
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), (uint8_t*)xiny_ATTR_GREATER_THAN_STR, xiny_ATTR_GREATER_THAN_LEN - 1, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toClear & xiny_LWM2M_ATTR_FLAG_LESS_THAN)
    {
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), (uint8_t*)xiny_ATTR_LESS_THAN_STR, xiny_ATTR_LESS_THAN_LEN - 1, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }
    if (attrP->toClear & xiny_LWM2M_ATTR_FLAG_STEP)
    {
        xiny_coap_add_multi_option(&(coap_pkt->uri_query), (uint8_t*)xiny_ATTR_STEP_STR, xiny_ATTR_STEP_LEN - 1, 0);
        xiny_SET_OPTION(coap_pkt, xiny_COAP_OPTION_URI_QUERY);
    }

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

int xiny_lwm2m_dm_discover(xiny_lwm2m_context_t * contextP,
                      uint16_t clientID,
                      xiny_lwm2m_uri_t * uriP,
                      xiny_lwm2m_result_callback_t callback,
                      void * userData)
{
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_transaction_t * transaction;
    dm_data_t * dataP;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);
    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    transaction = xiny_transaction_new(clientP->sessionH, xiny_COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, NULL);
    if (transaction == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    xiny_coap_set_header_accept(transaction->message, xiny_LWM2M_CONTENT_LINK);

    if (callback != NULL)
    {
        dataP = (dm_data_t *)xiny_lwm2m_malloc(sizeof(dm_data_t));
        if (dataP == NULL)
        {
            xiny_transaction_free(transaction);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        memcpy(&dataP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
        dataP->clientID = clientP->internalID;
        dataP->callback = callback;
        dataP->userData = userData;

        transaction->callback = prv_resultCallback;
        transaction->userData = (void *)dataP;
    }

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transaction);

    return xiny_transaction_send(contextP, transaction);
}

#endif
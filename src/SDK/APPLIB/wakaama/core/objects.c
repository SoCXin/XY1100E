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
 *    Toby Jaffey - Please refer to git log
 *    Benjamin Cabé - Please refer to git log
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

#ifdef XINYI_LWM2M_CLIENT_MODE


#include <stdlib.h>
#include <string.h>
#include <stdio.h>


uint8_t xiny_object_checkReadable_lwm2m(xiny_lwm2m_context_t * contextP,
                             xiny_lwm2m_uri_t * uriP,
                             xiny_lwm2m_attributes_t * attrP)
{
    uint8_t result;
    xiny_lwm2m_object_t * targetP;
    xiny_lwm2m_data_t * dataP = NULL;
    int size;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == targetP->readFunc) return xiny_COAP_405_METHOD_NOT_ALLOWED;

    if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP)) return xiny_COAP_205_CONTENT;

    if (NULL == xiny_LWM2M_LIST_FIND(targetP->instanceList, uriP->instanceId)) return xiny_COAP_404_NOT_FOUND;

    if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return xiny_COAP_205_CONTENT;

    size = 1;
    dataP = xiny_lwm2m_data_new(1);
    if (dataP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    dataP->id = uriP->resourceId;

    result = targetP->readFunc(uriP->instanceId, &size, &dataP, targetP);
    if (result == xiny_COAP_205_CONTENT)
    {
        if (attrP->toSet & xiny_ATTR_FLAG_NUMERIC)
        {
            switch (dataP->type)
            {
                case xiny_LWM2M_TYPE_INTEGER:
                case xiny_LWM2M_TYPE_FLOAT:
                    break;
                default:
                    result = xiny_COAP_405_METHOD_NOT_ALLOWED;
            }
        }
    }
    xiny_lwm2m_data_free(1, dataP);
    return result;
}

uint8_t xiny_object_read_lwm2mData_lwm2m(xiny_lwm2m_context_t * contextP,
                        xiny_lwm2m_uri_t * uriP,
                        int * sizeP,
                        xiny_lwm2m_data_t ** dataP)
{
    uint8_t result;
    xiny_lwm2m_object_t * targetP;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == targetP->readFunc) return xiny_COAP_405_METHOD_NOT_ALLOWED;

    if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
    {
        if (NULL == xiny_lwm2m_list_find(targetP->instanceList, uriP->instanceId)) return xiny_COAP_404_NOT_FOUND;

        // single instance read
        if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            *sizeP = 1;
            *dataP = xiny_lwm2m_data_new(*sizeP);
            if (*dataP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

            (*dataP)->id = uriP->resourceId;
        }

        result = targetP->readFunc(uriP->instanceId, sizeP, dataP, targetP);
    }
    else
    {
        // multiple object instances read
        xiny_lwm2m_list_t * instanceP;
        int i;

        result = xiny_COAP_205_CONTENT;

        *sizeP = 0;
        for (instanceP = targetP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
        {
            (*sizeP)++;
        }

        if (*sizeP == 0)
        {
            *dataP = NULL;
        }
        else
        {
            *dataP = xiny_lwm2m_data_new(*sizeP);
            if (*dataP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

            instanceP = targetP->instanceList;
            i = 0;
            while (instanceP != NULL && result == xiny_COAP_205_CONTENT)
            {
                result = targetP->readFunc(instanceP->id, (int*)&((*dataP)[i].value.asChildren.count), &((*dataP)[i].value.asChildren.array), targetP);
                (*dataP)[i].type = xiny_LWM2M_TYPE_OBJECT_INSTANCE;
                (*dataP)[i].id = instanceP->id;
                i++;
                instanceP = instanceP->next;
            }
        }
    }

    LOG_ARG("result: %u.%2u, size: %d", (result & 0xFF) >> 5, (result & 0x1F), *sizeP);
    return result;
}

uint8_t xiny_object_read_lwm2m(xiny_lwm2m_context_t * contextP,
                    xiny_lwm2m_uri_t * uriP,
                    xiny_lwm2m_media_type_lwm2m_t * formatP,
                    uint8_t ** bufferP,
                    size_t * lengthP)
{
    uint8_t result;
    xiny_lwm2m_data_t * dataP = NULL;
    int size = 0;
    int res;

    LOG_URI(uriP);
    result = xiny_object_read_lwm2mData_lwm2m(contextP, uriP, &size, &dataP);

    if (result == xiny_COAP_205_CONTENT)
    {
        res = xiny_lwm2m_data_serialize(uriP, size, dataP, formatP, bufferP);
        if (res < 0)
        {
            result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        else
        {
            *lengthP = (size_t)res;
        }
    }
    xiny_lwm2m_data_free(size, dataP);

    LOG_ARG("result: %u.%2u, length: %d", (result & 0xFF) >> 5, (result & 0x1F), *lengthP);

    return result;
}

uint8_t xiny_object_write_lwm2m(xiny_lwm2m_context_t * contextP,
                     xiny_lwm2m_uri_t * uriP,
                     xiny_lwm2m_media_type_lwm2m_t format,
                     uint8_t * buffer,
                     size_t length)
{
    uint8_t result = xiny_NO_ERROR;
    xiny_lwm2m_object_t * targetP;
    xiny_lwm2m_data_t * dataP = NULL;
    int size = 0;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP)
    {
        result = xiny_COAP_404_NOT_FOUND;
    }
    else if (NULL == targetP->writeFunc)
    {
        result = xiny_COAP_405_METHOD_NOT_ALLOWED;
    }
    else
    {
        size = xiny_lwm2m_data_parse(uriP, buffer, length, format, &dataP);
        if (size == 0)
        {
            result = xiny_COAP_406_NOT_ACCEPTABLE;
        }
    }
    if (result == xiny_NO_ERROR)
    {
        result = targetP->writeFunc(uriP->instanceId, size, dataP, targetP);
        xiny_lwm2m_data_free(size, dataP);
    }

    LOG_ARG("result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));

    return result;
}

uint8_t xiny_object_execute_lwm2m(xiny_lwm2m_context_t * contextP,
                       xiny_lwm2m_uri_t * uriP,
                       uint8_t * buffer,
                       size_t length)
{
    xiny_lwm2m_object_t * targetP;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == targetP->executeFunc) return xiny_COAP_405_METHOD_NOT_ALLOWED;
    if (NULL == xiny_lwm2m_list_find(targetP->instanceList, uriP->instanceId)) return xiny_COAP_404_NOT_FOUND;

    return targetP->executeFunc(uriP->instanceId, uriP->resourceId, buffer, length, targetP);
}

uint8_t xiny_object_create_lwm2m(xiny_lwm2m_context_t * contextP,
                      xiny_lwm2m_uri_t * uriP,
                      xiny_lwm2m_media_type_lwm2m_t format,
                      uint8_t * buffer,
                      size_t length)
{
    xiny_lwm2m_object_t * targetP;
    xiny_lwm2m_data_t * dataP = NULL;
    int size = 0;
    uint8_t result;

    LOG_URI(uriP);

    if (length == 0 || buffer == 0)
    {
        return xiny_COAP_400_BAD_REQUEST;
    }

    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == targetP->createFunc) return xiny_COAP_405_METHOD_NOT_ALLOWED;

    size = xiny_lwm2m_data_parse(uriP, buffer, length, format, &dataP);
    if (size <= 0) return xiny_COAP_400_BAD_REQUEST;

    switch (dataP[0].type)
    {
    case xiny_LWM2M_TYPE_OBJECT:
        result = xiny_COAP_400_BAD_REQUEST;
        goto exit;

    case xiny_LWM2M_TYPE_OBJECT_INSTANCE:
        if (size != 1)
        {
            result = xiny_COAP_400_BAD_REQUEST;
            goto exit;
        }
        if (NULL != xiny_lwm2m_list_find(targetP->instanceList, dataP[0].id))
        {
            // Instance already exists
            result = xiny_COAP_406_NOT_ACCEPTABLE;
            goto exit;
        }
        result = targetP->createFunc(dataP[0].id, dataP[0].value.asChildren.count, dataP[0].value.asChildren.array, targetP);
        uriP->instanceId = dataP[0].id;
        uriP->flag |= xiny_LWM2M_URI_FLAG_INSTANCE_ID;
        break;

    default:
        if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
        {
            uriP->instanceId = xiny_lwm2m_list_newId(targetP->instanceList);
            uriP->flag |= xiny_LWM2M_URI_FLAG_INSTANCE_ID;
        }
        result = targetP->createFunc(uriP->instanceId, size, dataP, targetP);
        break;
    }

exit:
    xiny_lwm2m_data_free(size, dataP);

    LOG_ARG("result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));

    return result;
}

uint8_t xiny_object_delete_lwm2m(xiny_lwm2m_context_t * contextP,
                      xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_object_t * objectP;
    uint8_t result;

    LOG_URI(uriP);
    objectP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == objectP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == objectP->deleteFunc) return xiny_COAP_405_METHOD_NOT_ALLOWED;

    LOG("Entering");

    if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
    {
        result = objectP->deleteFunc(uriP->instanceId, objectP);
        if (result == xiny_COAP_202_DELETED)
        {
            xiny_observe_clear(contextP, uriP);
        }
    }
    else
    {
        xiny_lwm2m_list_t * instanceP;

        result = xiny_COAP_202_DELETED;
        instanceP = objectP->instanceList;
        while (NULL != instanceP
            && result == xiny_COAP_202_DELETED)
        {
            uriP->instanceId = instanceP->id;
            result = objectP->deleteFunc(instanceP->id, objectP);
            if (result == xiny_COAP_202_DELETED)
            {
                uriP->flag |= xiny_LWM2M_URI_FLAG_INSTANCE_ID;
                xiny_observe_clear(contextP, uriP);
                uriP->flag &= ~xiny_LWM2M_URI_FLAG_INSTANCE_ID;
            }
            instanceP = objectP->instanceList;
        }
    }

    LOG_ARG("result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));

    return result;
}

uint8_t xiny_object_discover_lwm2m(xiny_lwm2m_context_t * contextP,
                        xiny_lwm2m_uri_t * uriP,
                        xiny_lwm2m_server_lwm2m_t * serverP,
                        uint8_t ** bufferP,
                        size_t * lengthP)
{
    uint8_t result;
    xiny_lwm2m_object_t * targetP;
    xiny_lwm2m_data_t * dataP = NULL;
    int size = 0;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;
    if (NULL == targetP->discoverFunc) return xiny_COAP_501_NOT_IMPLEMENTED;

    if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
    {
        if (NULL == xiny_lwm2m_list_find(targetP->instanceList, uriP->instanceId)) return xiny_COAP_404_NOT_FOUND;

        // single instance read
        if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            size = 1;
            dataP = xiny_lwm2m_data_new(size);
            if (dataP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

            dataP->id = uriP->resourceId;
        }

        result = targetP->discoverFunc(uriP->instanceId, &size, &dataP, targetP);
    }
    else
    {
        // multiple object instances read
        xiny_lwm2m_list_t * instanceP;
        int i;

        result = xiny_COAP_205_CONTENT;

        size = 0;
        for (instanceP = targetP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
        {
            size++;
        }

        if (size != 0)
        {
            dataP = xiny_lwm2m_data_new(size);
            if (dataP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

            instanceP = targetP->instanceList;
            i = 0;
            while (instanceP != NULL && result == xiny_COAP_205_CONTENT)
            {
                result = targetP->discoverFunc(instanceP->id, (int*)&(dataP[i].value.asChildren.count), &(dataP[i].value.asChildren.array), targetP);
                dataP[i].type = xiny_LWM2M_TYPE_OBJECT_INSTANCE;
                dataP[i].id = instanceP->id;
                i++;
                instanceP = instanceP->next;
            }
        }
    }

    if (result == xiny_COAP_205_CONTENT)
    {
        int len;

        len = xiny_discover_serialize(contextP, uriP, serverP, size, dataP, bufferP);
        if (len <= 0) result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
        else *lengthP = len;
    }
    xiny_lwm2m_data_free(size, dataP);

    LOG_ARG("result: %u.%2u", (result & 0xFF) >> 5, (result & 0x1F));

    return result;
}

bool xiny_object_isInstanceNew_lwm2m(xiny_lwm2m_context_t * contextP,
                          uint16_t objectId,
                          uint16_t instanceId)
{
    xiny_lwm2m_object_t * targetP;

    LOG("Entering");
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, objectId);
    if (targetP != NULL)
    {
        if (NULL != xiny_lwm2m_list_find(targetP->instanceList, instanceId))
        {
            return false;
        }
    }

    return true;
}

static int prv_getObjectTemplate(uint8_t * buffer,
                                 size_t length,
                                 uint16_t id)
{
    int index;
    int result;

    if (length < REG_OBJECT_MIN_LEN) return -1;

    buffer[0] = '<';
    buffer[1] = '/';
    index = 2;

    result = xiny_utils_intToText(id, buffer + index, length - index);
    if (result == 0) return -1;
    index += result;

    if (length - index < REG_OBJECT_MIN_LEN - 3) return -1;
    buffer[index] = '/';
    index++;

    return index;
}

int xiny_object_getRegisterPayload_lwm2mBufferLength_lwm2m(xiny_lwm2m_context_t * contextP)
{
    size_t index;
    int result;
    xiny_lwm2m_object_t * objectP;
    char buffer[REG_OBJECT_MIN_LEN + 5];

    LOG("Entering");
    index = strlen(xiny_REG_START);

    if ((contextP->altPath != NULL)
     && (contextP->altPath[0] != 0))
    {
        index += strlen(contextP->altPath);
    }
    else
    {
        index += strlen(xiny_REG_DEFAULT_PATH);
    }

    index += strlen(xiny_REG_LWM2M_RESOURCE_TYPE);

    for (objectP = contextP->objectList; objectP != NULL; objectP = objectP->next)
    {
        size_t start;
        size_t length;

        if (objectP->objID == xiny_LWM2M_SECURITY_OBJECT_ID) continue;

        start = index;
        result = prv_getObjectTemplate((uint8_t *)(buffer), sizeof(buffer), objectP->objID);
        if (result < 0) return 0;
        length = result;
        index += length;

        if (objectP->instanceList == NULL)
        {
            index -= 1;
            index += strlen(xiny_REG_PATH_END);
        }
        else
        {
            xiny_lwm2m_list_t * targetP;
            for (targetP = objectP->instanceList ; targetP != NULL ; targetP = targetP->next)
            {
                if (index != start + length)
                {
                    index += length;
                }

                result = xiny_utils_intToText((int64_t)(targetP->id), (uint8_t *)(buffer), sizeof(buffer));
                if (result == 0) return 0;
                index += result;

                index += strlen(xiny_REG_PATH_END);
            }
        }
    }

    index += 1;  // account for trailing null

    // Note that xiny_object_getRegisterPayload_lwm2m() has xiny_REG_PATH_END added after each
    // object or instance, and then the trailing comma is replaced by null. The
    // trailing nulls are not counted as part of the payload length, so this
    // will return a size two bytes greater than what
    // xiny_object_getRegisterPayload_lwm2m() returns.

    return index;
}

int xiny_object_getRegisterPayload_lwm2m(xiny_lwm2m_context_t * contextP,
                           uint8_t * buffer,
                           size_t bufferLen)
{
    size_t index;
    int result;
    xiny_lwm2m_object_t * objectP;

    LOG("Entering");
    // index can not be greater than bufferLen
    index = 0;

    result = xiny_utils_stringCopy((char *)buffer, bufferLen, xiny_REG_START);
    if (result < 0) return 0;
    index += result;

    if ((contextP->altPath != NULL)
     && (contextP->altPath[0] != 0))
    {
        result = xiny_utils_stringCopy((char *)buffer + index, bufferLen - index, contextP->altPath);
    }
    else
    {
        result = xiny_utils_stringCopy((char *)buffer + index, bufferLen - index, xiny_REG_DEFAULT_PATH);
    }
    if (result < 0) return 0;
    index += result;

    result = xiny_utils_stringCopy((char *)buffer + index, bufferLen - index, xiny_REG_LWM2M_RESOURCE_TYPE);
    if (result < 0) return 0;
    index += result;

    for (objectP = contextP->objectList; objectP != NULL; objectP = objectP->next)
    {
        size_t start;
        size_t length;

        if (objectP->objID == xiny_LWM2M_SECURITY_OBJECT_ID) continue;

        start = index;
        result = prv_getObjectTemplate(buffer + index, bufferLen - index, objectP->objID);
        if (result < 0) return 0;
        length = result;
        index += length;

        if (objectP->instanceList == NULL)
        {
            index--;
            result = xiny_utils_stringCopy((char *)buffer + index, bufferLen - index, xiny_REG_PATH_END);
            if (result < 0) return 0;
            index += result;
        }
        else
        {
            xiny_lwm2m_list_t * targetP;
            for (targetP = objectP->instanceList ; targetP != NULL ; targetP = targetP->next)
            {
                if (index != start + length)
                {
                    if (bufferLen - index <= length) return 0;
                    memcpy(buffer + index, buffer + start, length);
                    index += length;
                }

                result = xiny_utils_intToText(targetP->id, buffer + index, bufferLen - index);
                if (result == 0) return 0;
                index += result;

                result = xiny_utils_stringCopy((char *)buffer + index, bufferLen - index, xiny_REG_PATH_END);
                if (result < 0) return 0;
                index += result;
            }
        }
    }

    if (index > 0)
    {
        index = index - 1;  // remove trailing ','
    }

    buffer[index] = 0;

    return index;
}

static xiny_lwm2m_list_t * prv_findServerInstance(xiny_lwm2m_object_t * objectP,
                                             uint16_t shortID)
{
    xiny_lwm2m_list_t * instanceP;

    instanceP = objectP->instanceList;
    while (NULL != instanceP)
    {
        int64_t value;
        xiny_lwm2m_data_t * dataP;
        int size;

        size = 1;
        dataP = xiny_lwm2m_data_new(size);
        if (dataP == NULL) return NULL;
        dataP->id = xiny_LWM2M_SERVER_SHORT_ID_ID;

        if (objectP->readFunc(instanceP->id, &size, &dataP, objectP) != xiny_COAP_205_CONTENT)
        {
            xiny_lwm2m_data_free(size, dataP);
            return NULL;
        }

        if (1 == xiny_lwm2m_data_decode_int(dataP, &value))
        {
            if (value == shortID)
            {
                xiny_lwm2m_data_free(size, dataP);
                break;
            }
        }
        xiny_lwm2m_data_free(size, dataP);
        instanceP = instanceP->next;
    }

    return instanceP;
}

static int prv_getMandatoryInfo(xiny_lwm2m_object_t * objectP,
                                uint16_t instanceID,
                                xiny_lwm2m_server_lwm2m_t * targetP)
{
    xiny_lwm2m_data_t * dataP;
    int size;
    int64_t value;

    size = 2;
    dataP = xiny_lwm2m_data_new(size);
    if (dataP == NULL) return -1;
    dataP[0].id = xiny_LWM2M_SERVER_LIFETIME_ID;
    dataP[1].id = xiny_LWM2M_SERVER_BINDING_ID;

    if (objectP->readFunc(instanceID, &size, &dataP, objectP) != xiny_COAP_205_CONTENT)
    {
        xiny_lwm2m_data_free(size, dataP);
        return -1;
    }

    if (0 == xiny_lwm2m_data_decode_int(dataP, &value)
     || value < 0 || value >0xFFFFFFFF)             // This is an implementation limit
    {
        xiny_lwm2m_data_free(size, dataP);
        return -1;
    }
    targetP->lifetime = value;

    targetP->binding = xiny_utils_stringToBinding(dataP[1].value.asBuffer.buffer, dataP[1].value.asBuffer.length);

    xiny_lwm2m_data_free(size, dataP);

    if (targetP->binding == xiny_BINDING_UNKNOWN)
    {
        return -1;
    }

    return 0;
}

int xiny_object_getServers_lwm2m(xiny_lwm2m_context_t * contextP, bool checkOnly)
{
    xiny_lwm2m_object_t * objectP;
    xiny_lwm2m_object_t * securityObjP = NULL;
    xiny_lwm2m_object_t * serverObjP = NULL;
    xiny_lwm2m_list_t * securityInstP;   // instanceID of the server in the LWM2M Security Object

    LOG("Entering");

    for (objectP = contextP->objectList; objectP != NULL; objectP = objectP->next)
    {
        if (objectP->objID == xiny_LWM2M_SECURITY_OBJECT_ID)
        {
            securityObjP = objectP;
        }
        else if (objectP->objID == xiny_LWM2M_SERVER_OBJECT_ID)
        {
            serverObjP = objectP;
        }
    }

    if (NULL == securityObjP) return -1;

    securityInstP = securityObjP->instanceList;
    while (securityInstP != NULL)
    {
        if (xiny_LWM2M_LIST_FIND(contextP->xiny_bootstrapServerList, securityInstP->id) == NULL
         && xiny_LWM2M_LIST_FIND(contextP->serverList, securityInstP->id) == NULL)
        {
            // This server is new. eg created by last xiny_bootstrap

            xiny_lwm2m_data_t * dataP;
            int size;
            xiny_lwm2m_server_lwm2m_t * targetP;
            bool isBootstrap;
            int64_t value = 0;

            size = 3;
            dataP = xiny_lwm2m_data_new(size);
            if (dataP == NULL) return -1;
            dataP[0].id = xiny_LWM2M_SECURITY_BOOTSTRAP_ID;
            dataP[1].id = xiny_LWM2M_SECURITY_SHORT_SERVER_ID;
            dataP[2].id = xiny_LWM2M_SECURITY_HOLD_OFF_ID;

            if (securityObjP->readFunc(securityInstP->id, &size, &dataP, securityObjP) != xiny_COAP_205_CONTENT)
            {
                xiny_lwm2m_data_free(size, dataP);
                return -1;
            }

            targetP = (xiny_lwm2m_server_lwm2m_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_server_lwm2m_t));
            if (targetP == NULL) {
                xiny_lwm2m_data_free(size, dataP);
                return -1;
            }
            memset(targetP, 0, sizeof(xiny_lwm2m_server_lwm2m_t));
            targetP->secObjInstID = securityInstP->id;

            if (0 == xiny_lwm2m_data_decode_bool(dataP + 0, &isBootstrap))
            {
                xiny_lwm2m_free(targetP);
                xiny_lwm2m_data_free(size, dataP);
                return -1;
            }

            if (0 == xiny_lwm2m_data_decode_int(dataP + 1, &value)
             || value < (isBootstrap ? 0 : 1) || value > 0xFFFF)                // 0 is forbidden as a Short Server ID
            {
                xiny_lwm2m_free(targetP);
                xiny_lwm2m_data_free(size, dataP);
                return -1;
            }
            targetP->shortID = value;

            if (isBootstrap == true)
            {
                if (0 == xiny_lwm2m_data_decode_int(dataP + 2, &value)
                 || value < 0 || value > 0xFFFFFFFF)             // This is an implementation limit
                {
                    xiny_lwm2m_free(targetP);
                    xiny_lwm2m_data_free(size, dataP);
                    return -1;
                }
                // lifetime of a xiny_bootstrap server is set to ClientHoldOffTime
                targetP->lifetime = value;

                if (checkOnly)
                {
                    xiny_lwm2m_free(targetP);
                }
                else
                {
                    contextP->xiny_bootstrapServerList = (xiny_lwm2m_server_lwm2m_t*)xiny_LWM2M_LIST_ADD(contextP->xiny_bootstrapServerList, targetP);
                }
            }
            else
            {
                xiny_lwm2m_list_t * serverInstP;     // instanceID of the server in the LWM2M Server Object

                serverInstP = prv_findServerInstance(serverObjP, targetP->shortID);
                if (serverInstP == NULL)
                {
                    xiny_lwm2m_free(targetP);
                }
                else
                {
                    if (0 != prv_getMandatoryInfo(serverObjP, serverInstP->id, targetP))
                    {
                        xiny_lwm2m_free(targetP);
                        xiny_lwm2m_data_free(size, dataP);
                        return -1;
                    }
                    targetP->status = xiny_STATE_DEREGISTERED;
                    if (checkOnly)
                    {
                        xiny_lwm2m_free(targetP);
                    }
                    else
                    {
                        contextP->serverList = (xiny_lwm2m_server_lwm2m_t*)xiny_LWM2M_LIST_ADD(contextP->serverList, targetP);
                    }
                }
            }
            xiny_lwm2m_data_free(size, dataP);
        }
        securityInstP = securityInstP->next;
    }

    return 0;
}

uint8_t xiny_object_create_lwm2mInstance(xiny_lwm2m_context_t * contextP,
                                    xiny_lwm2m_uri_t * uriP,
                                    xiny_lwm2m_data_t * dataP)
{
    xiny_lwm2m_object_t * targetP;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;

    if (NULL == targetP->createFunc)
    {
        return xiny_COAP_405_METHOD_NOT_ALLOWED;
    }

    return targetP->createFunc(xiny_lwm2m_list_newId(targetP->instanceList), dataP->value.asChildren.count, dataP->value.asChildren.array, targetP);
}

uint8_t xiny_object_write_lwm2mInstance(xiny_lwm2m_context_t * contextP,
                            xiny_lwm2m_uri_t * uriP,
                            xiny_lwm2m_data_t * dataP)
{
    xiny_lwm2m_object_t * targetP;

    LOG_URI(uriP);
    targetP = (xiny_lwm2m_object_t *)xiny_LWM2M_LIST_FIND(contextP->objectList, uriP->objectId);
    if (NULL == targetP) return xiny_COAP_404_NOT_FOUND;

    if (NULL == targetP->writeFunc)
    {
        return xiny_COAP_405_METHOD_NOT_ALLOWED;
    }

    return targetP->writeFunc(dataP->id, dataP->value.asChildren.count, dataP->value.asChildren.array, targetP);
}

#endif
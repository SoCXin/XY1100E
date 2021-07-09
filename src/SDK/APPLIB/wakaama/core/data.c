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
*    Bosch Software Innovations GmbH - Please refer to git log
*
*******************************************************************************/

#include "xiny_internals.h"
#include <float.h>

#define _PRV_STR_LENGTH 32

// dataP array length is assumed to be 1.
static int prv_textSerialize(xiny_lwm2m_data_t * dataP,
                             uint8_t ** bufferP)
{
    size_t res;

    switch (dataP->type)
    {
    case xiny_LWM2M_TYPE_STRING:
        *bufferP = (uint8_t *)xiny_lwm2m_malloc(dataP->value.asBuffer.length);
        if (*bufferP == NULL) return 0;
        memcpy(*bufferP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
        return (int)dataP->value.asBuffer.length;

    case xiny_LWM2M_TYPE_INTEGER:
    {
        uint8_t intString[_PRV_STR_LENGTH];

        res = xiny_utils_intToText(dataP->value.asInteger, intString, _PRV_STR_LENGTH);
        if (res == 0) return -1;

        *bufferP = (uint8_t *)xiny_lwm2m_malloc(res);
        if (NULL == *bufferP) return -1;

        memcpy(*bufferP, intString, res);

        return (int)res;
    }

    case xiny_LWM2M_TYPE_FLOAT:
    {
        uint8_t floatString[_PRV_STR_LENGTH * 2];

        res = xiny_utils_floatToText(dataP->value.asFloat, floatString, _PRV_STR_LENGTH * 2);
        if (res == 0) return -1;

        *bufferP = (uint8_t *)xiny_lwm2m_malloc(res);
        if (NULL == *bufferP) return -1;

        memcpy(*bufferP, floatString, res);

        return (int)res;
    }

    case xiny_LWM2M_TYPE_BOOLEAN:
        *bufferP = (uint8_t *)xiny_lwm2m_malloc(1);
        if (NULL == *bufferP) return -1;

        *bufferP[0] = dataP->value.asBoolean ? '1' : '0';

        return 1;

    case xiny_LWM2M_TYPE_OBJECT_LINK:
    {
        char stringBuffer[11];
        size_t length;
        
        length = xiny_utils_intToText(dataP->value.asObjLink.objectId, (uint8_t*)stringBuffer, 5);
        if (length == 0) return -1;

        stringBuffer[5] = ':';
        res = length + 1;

        length = xiny_utils_intToText(dataP->value.asObjLink.objectInstanceId, (uint8_t*)stringBuffer + res, 5);
        if (length == 0) return -1;

        res += length;

        *bufferP = (uint8_t *)xiny_lwm2m_malloc(res);
        if (*bufferP == NULL) return -1;

        memcpy(*bufferP, stringBuffer, res);

        return res;
    }

    case xiny_LWM2M_TYPE_OPAQUE:
    {
        size_t length;

        length = xiny_utils_base64GetSize(dataP->value.asBuffer.length);
        *bufferP = (uint8_t *)xiny_lwm2m_malloc(length);
        if (*bufferP == NULL) return 0;
        length = xiny_utils_base64Encode(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, *bufferP, length);
        if (length == 0)
        {
            xiny_lwm2m_free(*bufferP);
            *bufferP = NULL;
            return 0;
        }
        return (int)length;
    }

    case xiny_LWM2M_TYPE_UNDEFINED:
    default:
        return -1;
    }
}

static int prv_setBuffer(xiny_lwm2m_data_t * dataP,
                         uint8_t * buffer,
                         size_t bufferLen)
{
    dataP->value.asBuffer.buffer = (uint8_t *)xiny_lwm2m_malloc(bufferLen);
    if (dataP->value.asBuffer.buffer == NULL)
    {
        return 0;
    }
    dataP->value.asBuffer.length = bufferLen;
    memcpy(dataP->value.asBuffer.buffer, buffer, bufferLen);

    return 1;
}

xiny_lwm2m_data_t * xiny_lwm2m_data_new(int size)
{
    xiny_lwm2m_data_t * dataP;

    LOG_ARG("size: %d", size);
    if (size <= 0) return NULL;

    dataP = (xiny_lwm2m_data_t *)xiny_lwm2m_malloc(size * sizeof(xiny_lwm2m_data_t));

    if (dataP != NULL)
    {
        memset(dataP, 0, size * sizeof(xiny_lwm2m_data_t));
    }

    return dataP;
}

void xiny_lwm2m_data_free(int size,
                     xiny_lwm2m_data_t * dataP)
{
    int i;

    LOG_ARG("size: %d", size);
    if (size == 0 || dataP == NULL) return;

    for (i = 0; i < size; i++)
    {
        switch (dataP[i].type)
        {
        case xiny_LWM2M_TYPE_MULTIPLE_RESOURCE:
        case xiny_LWM2M_TYPE_OBJECT_INSTANCE:
        case xiny_LWM2M_TYPE_OBJECT:
            xiny_lwm2m_data_free(dataP[i].value.asChildren.count, dataP[i].value.asChildren.array);
            break;

        case xiny_LWM2M_TYPE_STRING:
        case xiny_LWM2M_TYPE_OPAQUE:
            if (dataP[i].value.asBuffer.buffer != NULL)
            {
                xiny_lwm2m_free(dataP[i].value.asBuffer.buffer);
            }

        default:
            // do nothing
            break;
        }
    }
    xiny_lwm2m_free(dataP);
}

void xiny_lwm2m_data_encode_string(const char * string,
                              xiny_lwm2m_data_t * dataP)
{
    size_t len;
    int res;

    LOG_ARG("\"%s\"", string);
    if (string == NULL)
    {
        len = 0;
    }
    else
    {
        for (len = 0; string[len] != 0; len++);
    }

    if (len == 0)
    {
        dataP->value.asBuffer.length = 0;
        dataP->value.asBuffer.buffer = NULL;
        res = 1;
    }
    else
    {
        res = prv_setBuffer(dataP, (uint8_t *)string, len);
    }

    if (res == 1)
    {
        dataP->type = xiny_LWM2M_TYPE_STRING;
    }
    else
    {
        dataP->type = xiny_LWM2M_TYPE_UNDEFINED;
    }
}

void xiny_lwm2m_data_encode_opaque(uint8_t * buffer,
                              size_t length,
                              xiny_lwm2m_data_t * dataP)
{
    int res;

    LOG_ARG("length: %d", length);
    if (length == 0)
    {
        dataP->value.asBuffer.length = 0;
        dataP->value.asBuffer.buffer = NULL;
        res = 1;
    }
    else
    {
        res = prv_setBuffer(dataP, buffer, length);
    }

    if (res == 1)
    {
        dataP->type = xiny_LWM2M_TYPE_OPAQUE;
    }
    else
    {
        dataP->type = xiny_LWM2M_TYPE_UNDEFINED;
    }
}

void xiny_lwm2m_data_encode_nstring(const char * string,
                               size_t length,
                               xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("length: %d, string: \"%s\"", length, string);
    xiny_lwm2m_data_encode_opaque((uint8_t *)string, length, dataP);

    if (dataP->type == xiny_LWM2M_TYPE_OPAQUE)
    {
        dataP->type = xiny_LWM2M_TYPE_STRING;
    }
}

void xiny_lwm2m_data_encode_int(int64_t value,
                           xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("value: %" PRId64 "", value);
    dataP->type = xiny_LWM2M_TYPE_INTEGER;
    dataP->value.asInteger = value;
}

int xiny_lwm2m_data_decode_int(const xiny_lwm2m_data_t * dataP,
                          int64_t * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case xiny_LWM2M_TYPE_INTEGER:
        *valueP = dataP->value.asInteger;
        result = 1;
        break;

    case xiny_LWM2M_TYPE_STRING:
        result = xiny_utils_textToInt(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, valueP);
        break;

    case xiny_LWM2M_TYPE_OPAQUE:
        switch (dataP->value.asBuffer.length)
        {
        case 1:
            *valueP = (int8_t)dataP->value.asBuffer.buffer[0];
            result = 1;
            break;

        case 2:
        {
            int16_t value;

            xiny_utils_copyValue(&value, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = value;
            result = 1;
            break;
        }

        case 4:
        {
            int32_t value;

            xiny_utils_copyValue(&value, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = value;
            result = 1;
            break;
        }

        case 8:
            xiny_utils_copyValue(valueP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            result = 1;
            break;

        default:
            result = 0;
        }
        break;

    default:
        return 0;
    }
    LOG_ARG("result: %d, value: %" PRId64, result, *valueP);

    return result;
}

void xiny_lwm2m_data_encode_float(double value,
                             xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("value: %f", value);
    dataP->type = xiny_LWM2M_TYPE_FLOAT;
    dataP->value.asFloat = value;
}

int xiny_lwm2m_data_decode_float(const xiny_lwm2m_data_t * dataP,
                            double * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case xiny_LWM2M_TYPE_FLOAT:
        *valueP = dataP->value.asFloat;
        result = 1;
        break;

    case xiny_LWM2M_TYPE_INTEGER:
        *valueP = (double)dataP->value.asInteger;
        result = 1;
        break;

    case xiny_LWM2M_TYPE_STRING:
        result = xiny_utils_textToFloat(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, valueP);
        break;

    case xiny_LWM2M_TYPE_OPAQUE:
        switch (dataP->value.asBuffer.length)
        {
        case 4:
        {
            float temp;

            xiny_utils_copyValue(&temp, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = temp;
            result = 1;
        }
        break;

        case 8:
            xiny_utils_copyValue(valueP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            result = 1;
            break;

        default:
            result = 0;
        }
        break;

    default:
        result = 0;
    }

    LOG_ARG("result: %d, value: %f", result, *valueP);

    return result;
}

void xiny_lwm2m_data_encode_bool(bool value,
                            xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("value: %s", value?"true":"false");
    dataP->type = xiny_LWM2M_TYPE_BOOLEAN;
    dataP->value.asBoolean = value;
}

int xiny_lwm2m_data_decode_bool(const xiny_lwm2m_data_t * dataP,
                           bool * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case xiny_LWM2M_TYPE_BOOLEAN:
        *valueP = dataP->value.asBoolean;
        result = 1;
        break;

    case xiny_LWM2M_TYPE_STRING:
        if (dataP->value.asBuffer.length != 1) return 0;

        switch (dataP->value.asBuffer.buffer[0])
        {
        case '0':
            *valueP = false;
            result = 1;
            break;
        case '1':
            *valueP = true;
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
        break;

    case xiny_LWM2M_TYPE_OPAQUE:
        if (dataP->value.asBuffer.length != 1) return 0;

        switch (dataP->value.asBuffer.buffer[0])
        {
        case 0:
            *valueP = false;
            result = 1;
            break;
        case 1:
            *valueP = true;
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
        break;

    default:
        result = 0;
        break;
    }

    LOG_ARG("result: %d, value: %s", result, *valueP ? "true" : "false");

    return result;
}

void xiny_lwm2m_data_encode_objlink(uint16_t objectId,
                           uint16_t objectInstanceId,
                           xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("value: %d/%d", objectId, objectInstanceId);
    dataP->type = xiny_LWM2M_TYPE_OBJECT_LINK;
    dataP->value.asObjLink.objectId = objectId;
    dataP->value.asObjLink.objectInstanceId = objectInstanceId;
}

void xiny_lwm2m_data_include(xiny_lwm2m_data_t * subDataP,
                        size_t count,
                        xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("count: %d", count);
    if (subDataP == NULL || count == 0) return;

    switch (subDataP[0].type)
    {
    case xiny_LWM2M_TYPE_STRING:
    case xiny_LWM2M_TYPE_OPAQUE:
    case xiny_LWM2M_TYPE_INTEGER:
    case xiny_LWM2M_TYPE_FLOAT:
    case xiny_LWM2M_TYPE_BOOLEAN:
    case xiny_LWM2M_TYPE_OBJECT_LINK:
    case xiny_LWM2M_TYPE_MULTIPLE_RESOURCE:
        dataP->type = xiny_LWM2M_TYPE_OBJECT_INSTANCE;
        break;
    case xiny_LWM2M_TYPE_OBJECT_INSTANCE:
        dataP->type = xiny_LWM2M_TYPE_OBJECT;
        break;
    default:
        return;
    }
    dataP->value.asChildren.count = count;
    dataP->value.asChildren.array = subDataP;
}

void xiny_lwm2m_data_encode_instances(xiny_lwm2m_data_t * subDataP,
                                 size_t count,
                                 xiny_lwm2m_data_t * dataP)
{
    LOG_ARG("count: %d", count);
    xiny_lwm2m_data_include(subDataP, count, dataP);
    dataP->type = xiny_LWM2M_TYPE_MULTIPLE_RESOURCE;
}

int xiny_lwm2m_data_parse(xiny_lwm2m_uri_t * uriP,
                     uint8_t * buffer,
                     size_t bufferLen,
                     xiny_lwm2m_media_type_lwm2m_t format,
                     xiny_lwm2m_data_t ** dataP)
{
    int res;

    LOG_ARG("format: %s, bufferLen: %d", xiny_STR_MEDIA_TYPE(format), bufferLen);
    LOG_URI(uriP);
    switch (format)
    {
    case xiny_LWM2M_CONTENT_TEXT:
        if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return 0;
        *dataP = xiny_lwm2m_data_new(1);
        if (*dataP == NULL) return 0;
        (*dataP)->id = uriP->resourceId;
        (*dataP)->type = xiny_LWM2M_TYPE_STRING;
        res = prv_setBuffer(*dataP, buffer, bufferLen);
        if (res == 0)
        {
            xiny_lwm2m_data_free(1, *dataP);
            *dataP = NULL;
        }
        return res;

    case xiny_LWM2M_CONTENT_OPAQUE:
        if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return 0;
        *dataP = xiny_lwm2m_data_new(1);
        if (*dataP == NULL) return 0;
        (*dataP)->id = uriP->resourceId;
        (*dataP)->type = xiny_LWM2M_TYPE_OPAQUE;
        res = prv_setBuffer(*dataP, buffer, bufferLen);
        if (res == 0)
        {
            xiny_lwm2m_data_free(1, *dataP);
            *dataP = NULL;
    }
        return res;

#ifdef LWM2M_OLD_CONTENT_FORMAT_SUPPORT
    case xiny_LWM2M_CONTENT_TLV_OLD:
#endif
    case xiny_LWM2M_CONTENT_TLV:
        return xiny_tlv_parse_lwm2m(buffer, bufferLen, dataP);

#ifdef xiny_LWM2M_SUPPORT_JSON
#ifdef LWM2M_OLD_CONTENT_FORMAT_SUPPORT
    case xiny_LWM2M_CONTENT_JSON_OLD:
#endif
    case xiny_LWM2M_CONTENT_JSON:
        return xiny_json_parse(uriP, buffer, bufferLen, dataP);
#endif

    default:
        return 0;
    }
}

int xiny_lwm2m_data_serialize(xiny_lwm2m_uri_t * uriP,
                         int size,
                         xiny_lwm2m_data_t * dataP,
                         xiny_lwm2m_media_type_lwm2m_t * formatP,
                         uint8_t ** bufferP)
{
    LOG_URI(uriP);
    LOG_ARG("size: %d, formatP: %s", size, xiny_STR_MEDIA_TYPE(*formatP));

    // Check format
    if (*formatP == xiny_LWM2M_CONTENT_TEXT
     || *formatP == xiny_LWM2M_CONTENT_OPAQUE)
    {
        if (size != 1
         || (uriP != NULL && !xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
         || dataP->type == xiny_LWM2M_TYPE_OBJECT
         || dataP->type == xiny_LWM2M_TYPE_OBJECT_INSTANCE
         || dataP->type == xiny_LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
#ifdef xiny_LWM2M_SUPPORT_JSON
            *formatP = xiny_LWM2M_CONTENT_JSON;
#else
            *formatP = xiny_LWM2M_CONTENT_TLV;
#endif
        }
    }

    if (*formatP == xiny_LWM2M_CONTENT_OPAQUE
     && dataP->type != xiny_LWM2M_TYPE_OPAQUE)
    {
        LOG("Opaque format is reserved to opaque resources.");
        return -1;
    }

    LOG_ARG("Final format: %s", xiny_STR_MEDIA_TYPE(*formatP));

    switch (*formatP)
    {
    case xiny_LWM2M_CONTENT_TEXT:
        return prv_textSerialize(dataP, bufferP);

    case xiny_LWM2M_CONTENT_OPAQUE:
        *bufferP = (uint8_t *)xiny_lwm2m_malloc(dataP->value.asBuffer.length);
        if (*bufferP == NULL) return -1;
        memcpy(*bufferP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
        return (int)dataP->value.asBuffer.length;

    case xiny_LWM2M_CONTENT_TLV:
    case xiny_LWM2M_CONTENT_TLV_OLD:
    {
            bool isResourceInstance;

            if (uriP != NULL && xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)
             && (size != 1 || dataP->id != uriP->resourceId))
            {
                isResourceInstance = true;
            }
            else
            {
                isResourceInstance = false;
            }
            return xiny_tlv_serialize_lwm2m(isResourceInstance, size, dataP, bufferP);
        }

#ifdef XINYI_LWM2M_CLIENT_MODE
    case xiny_LWM2M_CONTENT_LINK:
        return xiny_discover_serialize(NULL, uriP, NULL, size, dataP, bufferP);
#endif
#ifdef xiny_LWM2M_SUPPORT_JSON
    case xiny_LWM2M_CONTENT_JSON:
    case xiny_LWM2M_CONTENT_JSON_OLD:
        return xiny_json_serialize(uriP, size, dataP, bufferP);
#endif

    default:
        return -1;
    }
}
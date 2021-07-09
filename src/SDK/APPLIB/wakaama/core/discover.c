/*******************************************************************************
*
* Copyright (c) 2015 Intel Corporation and others.
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


#include "xiny_internals.h"

#define PRV_LINK_BUFFER_SIZE  1024


#define PRV_CONCAT_STR(buf, len, index, str, str_len)    \
    {                                                    \
        if ((len)-(index) < (str_len)) return -1;        \
        memcpy((buf)+(index), (str), (str_len));         \
        (index) += (str_len);                            \
    }


#ifdef XINYI_LWM2M_CLIENT_MODE

static xiny_lwm2m_attributes_t * prv_findAttributes(xiny_lwm2m_context_t * contextP,
                                               xiny_lwm2m_uri_t * uriP,
                                               xiny_lwm2m_server_lwm2m_t * serverP)
{
    xiny_lwm2m_observed_t * observedP;
    xiny_lwm2m_watcher_t * watcherP;
    xiny_lwm2m_attributes_t * paramP;

    paramP = NULL;

    if (contextP == NULL) return NULL;
    if (serverP == NULL) return NULL;

    observedP = xiny_observe_findByUri_lwm2m(contextP, uriP);
    if (observedP == NULL || observedP->watcherList == NULL) return NULL;

    for (watcherP = observedP->watcherList; watcherP != NULL; watcherP = watcherP->next)
    {
        if (watcherP->server == serverP)
        {
            paramP = watcherP->parameters;
        }
    }

    return paramP;
}

static int prv_serializeAttributes(xiny_lwm2m_context_t * contextP,
                                   xiny_lwm2m_uri_t * uriP,
                                   xiny_lwm2m_server_lwm2m_t * serverP,
                                   xiny_lwm2m_attributes_t * objectParamP,
                                   uint8_t * buffer,
                                   size_t uriLen,
                                   size_t bufferLen)
{
    int head;
    int res;
    xiny_lwm2m_attributes_t * paramP;

    head = 0;

    paramP = prv_findAttributes(contextP, uriP, serverP);
    if (paramP == NULL) paramP = objectParamP;

    if (paramP != NULL)
    {
        head = uriLen;

        if (paramP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN);

            res = xiny_utils_intToText(paramP->minPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        else if (objectParamP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_MIN_PERIOD_STR, xiny_ATTR_MIN_PERIOD_LEN);

            res = xiny_utils_intToText(objectParamP->minPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }

        if (paramP->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN);

            res = xiny_utils_intToText(paramP->maxPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        else if (objectParamP->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_MAX_PERIOD_STR, xiny_ATTR_MAX_PERIOD_LEN);

            res = xiny_utils_intToText(objectParamP->maxPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }

        if (paramP->toSet & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_GREATER_THAN_STR, xiny_ATTR_GREATER_THAN_LEN);

            res = xiny_utils_floatToText(paramP->greaterThan, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & xiny_LWM2M_ATTR_FLAG_LESS_THAN)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_LESS_THAN_STR, xiny_ATTR_LESS_THAN_LEN);

            res = xiny_utils_floatToText(paramP->lessThan, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & xiny_LWM2M_ATTR_FLAG_STEP)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ATTR_SEPARATOR, xiny_LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, xiny_ATTR_STEP_STR, xiny_ATTR_STEP_LEN);

            res = xiny_utils_floatToText(paramP->step, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ITEM_ATTR_END, xiny_LINK_ITEM_ATTR_END_SIZE);
    }

    if (head > 0) head -= uriLen + 1;

    return head;
}

static int prv_serializeLinkData(xiny_lwm2m_context_t * contextP,
                                 xiny_lwm2m_data_t * tlvP,
                                 xiny_lwm2m_server_lwm2m_t * serverP,
                                 xiny_lwm2m_attributes_t * objectParamP,
                                 xiny_lwm2m_uri_t * parentUriP,
                                 uint8_t * parentUriStr,
                                 size_t parentUriLen,
                                 uint8_t * buffer,
                                 size_t bufferLen)
{
    int head;
    int res;
    xiny_lwm2m_uri_t uri;

    head = 0;

    switch (tlvP->type)
    {
    case xiny_LWM2M_TYPE_UNDEFINED:
    case xiny_LWM2M_TYPE_STRING:
    case xiny_LWM2M_TYPE_OPAQUE:
    case xiny_LWM2M_TYPE_INTEGER:
    case xiny_LWM2M_TYPE_FLOAT:
    case xiny_LWM2M_TYPE_BOOLEAN:
    case xiny_LWM2M_TYPE_OBJECT_LINK:
    case xiny_LWM2M_TYPE_MULTIPLE_RESOURCE:
        if (bufferLen < xiny_LINK_ITEM_START_SIZE) return -1;
        memcpy(buffer + head, xiny_LINK_ITEM_START, xiny_LINK_ITEM_START_SIZE);
        head = xiny_LINK_ITEM_START_SIZE;

        if (parentUriLen > 0)
        {
            if (bufferLen - head < parentUriLen) return -1;
            memcpy(buffer + head, parentUriStr, parentUriLen);
            head += parentUriLen;
        }

        if (bufferLen - head < xiny_LINK_URI_SEPARATOR_SIZE) return -1;
        memcpy(buffer + head, xiny_LINK_URI_SEPARATOR, xiny_LINK_URI_SEPARATOR_SIZE);
        head += xiny_LINK_URI_SEPARATOR_SIZE;

        res = xiny_utils_intToText(tlvP->id, buffer + head, bufferLen - head);
        if (res <= 0) return -1;
        head += res;

        if (tlvP->type == xiny_LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
            if (bufferLen - head < xiny_LINK_ITEM_DIM_START_SIZE) return -1;
            memcpy(buffer + head, xiny_LINK_ITEM_DIM_START, xiny_LINK_ITEM_DIM_START_SIZE);
            head += xiny_LINK_ITEM_DIM_START_SIZE;

            res = xiny_utils_intToText(tlvP->value.asChildren.count, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;

            if (bufferLen - head < xiny_LINK_ITEM_ATTR_END_SIZE) return -1;
            memcpy(buffer + head, xiny_LINK_ITEM_ATTR_END, xiny_LINK_ITEM_ATTR_END_SIZE);
            head += xiny_LINK_ITEM_ATTR_END_SIZE;
        }
        else
        {
            if (bufferLen - head < xiny_LINK_ITEM_END_SIZE) return -1;
            memcpy(buffer + head, xiny_LINK_ITEM_END, xiny_LINK_ITEM_END_SIZE);
            head += xiny_LINK_ITEM_END_SIZE;
        }

        if (serverP != NULL)
        {
            memcpy(&uri, parentUriP, sizeof(xiny_lwm2m_uri_t));
            uri.resourceId = tlvP->id;
            uri.flag |= xiny_LWM2M_URI_FLAG_RESOURCE_ID;
            res = prv_serializeAttributes(contextP, &uri, serverP, objectParamP, buffer, head - 1, bufferLen);
            if (res < 0) return -1;    // careful, 0 is valid
            if (res > 0) head += res;
        }
        break;

    case xiny_LWM2M_TYPE_OBJECT_INSTANCE:
    {
        uint8_t uriStr[xiny_URI_MAX_STRING_LEN];
        size_t uriLen;
        size_t index;

        if (parentUriLen > 0)
        {
            if (xiny_URI_MAX_STRING_LEN < parentUriLen) return -1;
            memcpy(uriStr, parentUriStr, parentUriLen);
            uriLen = parentUriLen;
        }
        else
        {
            uriLen = 0;
        }

        if (xiny_URI_MAX_STRING_LEN - uriLen < xiny_LINK_URI_SEPARATOR_SIZE) return -1;
        memcpy(uriStr + uriLen, xiny_LINK_URI_SEPARATOR, xiny_LINK_URI_SEPARATOR_SIZE);
        uriLen += xiny_LINK_URI_SEPARATOR_SIZE;

        res = xiny_utils_intToText(tlvP->id, uriStr + uriLen, xiny_URI_MAX_STRING_LEN - uriLen);
        if (res <= 0) return -1;
        uriLen += res;

        memcpy(&uri, parentUriP, sizeof(xiny_lwm2m_uri_t));
        uri.instanceId = tlvP->id;
        uri.flag |= xiny_LWM2M_URI_FLAG_INSTANCE_ID;

        head = 0;
        PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ITEM_START, xiny_LINK_ITEM_START_SIZE);
        PRV_CONCAT_STR(buffer, bufferLen, head, uriStr, uriLen);
        PRV_CONCAT_STR(buffer, bufferLen, head, xiny_LINK_ITEM_END, xiny_LINK_ITEM_END_SIZE);
        if (serverP != NULL)
        {
            res = prv_serializeAttributes(contextP, &uri, serverP, NULL, buffer, head - 1, bufferLen);
            if (res < 0) return -1;    // careful, 0 is valid
            if (res == 0) head = 0;    // rewind
            else head += res;
        }
        for (index = 0; index < tlvP->value.asChildren.count; index++)
        {
            res = prv_serializeLinkData(contextP, tlvP->value.asChildren.array + index, serverP, objectParamP, &uri, uriStr, uriLen, buffer + head, bufferLen - head);
            if (res < 0) return -1;
            head += res;
        }
    }
    break;

    case xiny_LWM2M_TYPE_OBJECT:
    default:
        return -1;
    }

    return head;
}

int xiny_discover_serialize(xiny_lwm2m_context_t * contextP,
                       xiny_lwm2m_uri_t * uriP,
                       xiny_lwm2m_server_lwm2m_t * serverP,
                       int size,
                       xiny_lwm2m_data_t * dataP,
                       uint8_t ** bufferP)
{
    uint8_t bufferLink[PRV_LINK_BUFFER_SIZE];
    uint8_t baseUriStr[xiny_URI_MAX_STRING_LEN];
    int baseUriLen;
    int index;
    size_t head;
    int res;
    xiny_lwm2m_uri_t parentUri;
    xiny_lwm2m_attributes_t * paramP;
    xiny_lwm2m_attributes_t mergedParam;

    LOG_ARG("size: %d", size);
    LOG_URI(uriP);

    head = 0;
    memset(&parentUri, 0, sizeof(xiny_lwm2m_uri_t));
    parentUri.objectId = uriP->objectId;
    parentUri.flag = xiny_LWM2M_URI_FLAG_OBJECT_ID;

    if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
    {
        xiny_lwm2m_uri_t tempUri;
        xiny_lwm2m_attributes_t * objParamP;
        xiny_lwm2m_attributes_t * instParamP;

        memset(&parentUri, 0, sizeof(xiny_lwm2m_uri_t));
        tempUri.objectId = uriP->objectId;
        tempUri.flag = xiny_LWM2M_URI_FLAG_OBJECT_ID;

        // get object level attributes
        objParamP = prv_findAttributes(contextP, &tempUri, serverP);
        
        // get object instance level attributes
        tempUri.instanceId = uriP->instanceId;
        tempUri.flag = xiny_LWM2M_URI_FLAG_INSTANCE_ID;
        instParamP = prv_findAttributes(contextP, &tempUri, serverP);

        if (objParamP != NULL)
        {
            if (instParamP != NULL)
            {
                memset(&mergedParam, 0, sizeof(xiny_lwm2m_attributes_t));
                mergedParam.toSet = objParamP->toSet | instParamP->toSet;
                if (mergedParam.toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
                {
                    if (instParamP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
                    {
                        mergedParam.minPeriod = instParamP->minPeriod;
                    }
                    else
                    {
                        mergedParam.minPeriod = objParamP->minPeriod;
                    }
                }
                if (mergedParam.toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
                {
                    if (instParamP->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
                    {
                        mergedParam.maxPeriod = instParamP->maxPeriod;
                    }
                    else
                    {
                        mergedParam.maxPeriod = objParamP->maxPeriod;
                    }
                }
                paramP = &mergedParam;
            }
            else
            {
                paramP = objParamP;
            }
        }
        else
        {
            paramP = instParamP;
        }
        uriP->flag &= ~xiny_LWM2M_URI_FLAG_RESOURCE_ID;
    }
    else
    {
        paramP = NULL;

        if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
        {
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_ITEM_START, xiny_LINK_ITEM_START_SIZE);
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_URI_SEPARATOR, xiny_LINK_URI_SEPARATOR_SIZE);
            res = xiny_utils_intToText(uriP->objectId, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
            if (res <= 0) return -1;
            head += res;
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_URI_SEPARATOR, xiny_LINK_URI_SEPARATOR_SIZE);
            res = xiny_utils_intToText(uriP->instanceId, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
            if (res <= 0) return -1;
            head += res;
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_ITEM_END, xiny_LINK_ITEM_END_SIZE);
            parentUri.instanceId = uriP->instanceId;
            parentUri.flag = xiny_LWM2M_URI_FLAG_INSTANCE_ID;
            if (serverP != NULL)
            {
                res = prv_serializeAttributes(contextP, &parentUri, serverP, NULL, bufferLink, head - 1, PRV_LINK_BUFFER_SIZE);
                if (res < 0) return -1;    // careful, 0 is valid
            }
            else
            {
                res = 0;
            }
            head += res;
        }
        else
        {
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_ITEM_START, xiny_LINK_ITEM_START_SIZE);
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_URI_SEPARATOR, xiny_LINK_URI_SEPARATOR_SIZE);
            res = xiny_utils_intToText(uriP->objectId, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
            if (res <= 0) return -1;
            head += res;
            PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, xiny_LINK_ITEM_END, xiny_LINK_ITEM_END_SIZE);

            if (serverP != NULL)
            {
                res = prv_serializeAttributes(contextP, &parentUri, serverP, NULL, bufferLink, head - 1, PRV_LINK_BUFFER_SIZE);
                if (res < 0) return -1;    // careful, 0 is valid
                head += res;
            }
        }
    }

    baseUriLen = xiny_uri_toString_lwm2m(uriP, baseUriStr, xiny_URI_MAX_STRING_LEN, NULL);
    if (baseUriLen < 0) return -1;
    baseUriLen -= 1;

    for (index = 0; index < size && head < PRV_LINK_BUFFER_SIZE; index++)
    {
        res = prv_serializeLinkData(contextP, dataP + index, serverP, paramP, uriP, baseUriStr, baseUriLen, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
        if (res < 0) return -1;
        head += res;
    }

    if (head > 0)
    {
        head -= 1;

        *bufferP = (uint8_t *)xiny_lwm2m_malloc(head);
        if (*bufferP == NULL) return 0;
        memcpy(*bufferP, bufferLink, head);
    }

    return (int)head;
}
#endif
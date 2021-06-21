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
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
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
static xiny_lwm2m_observed_t * prv_findObserved(xiny_lwm2m_context_t * contextP,
                                           xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_observed_t * targetP;

    targetP = contextP->observedList;
    while (targetP != NULL
        && (targetP->uri.objectId != uriP->objectId
         || targetP->uri.flag != uriP->flag
         || (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && targetP->uri.instanceId != uriP->instanceId)
         || (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP) && targetP->uri.resourceId != uriP->resourceId)))
    {
        targetP = targetP->next;
    }

    return targetP;
}

static void prv_unlinkObserved(xiny_lwm2m_context_t * contextP,
                               xiny_lwm2m_observed_t * observedP)
{
    if (contextP->observedList == observedP)
    {
        contextP->observedList = contextP->observedList->next;
    }
    else
    {
        xiny_lwm2m_observed_t * parentP;

        parentP = contextP->observedList;
        while (parentP->next != NULL
            && parentP->next != observedP)
        {
            parentP = parentP->next;
        }
        if (parentP->next != NULL)
        {
            parentP->next = parentP->next->next;
        }
    }
}

static xiny_lwm2m_watcher_t * prv_findWatcher(xiny_lwm2m_observed_t * observedP,
                                         xiny_lwm2m_server_lwm2m_t * serverP)
{
    xiny_lwm2m_watcher_t * targetP;

    targetP = observedP->watcherList;
    while (targetP != NULL
        && targetP->server != serverP)
    {
        targetP = targetP->next;
    }

    return targetP;
}

static xiny_lwm2m_watcher_t * prv_getWatcher(xiny_lwm2m_context_t * contextP,
                                        xiny_lwm2m_uri_t * uriP,
                                        xiny_lwm2m_server_lwm2m_t * serverP)
{
    xiny_lwm2m_observed_t * observedP;
    bool allocatedObserver;
    xiny_lwm2m_watcher_t * watcherP;

    allocatedObserver = false;

    observedP = prv_findObserved(contextP, uriP);
    if (observedP == NULL)
    {
        observedP = (xiny_lwm2m_observed_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_observed_t));
        if (observedP == NULL) return NULL;
        allocatedObserver = true;
        memset(observedP, 0, sizeof(xiny_lwm2m_observed_t));
        memcpy(&(observedP->uri), uriP, sizeof(xiny_lwm2m_uri_t));
        observedP->next = contextP->observedList;
        contextP->observedList = observedP;
    }

    watcherP = prv_findWatcher(observedP, serverP);
    if (watcherP == NULL)
    {
        watcherP = (xiny_lwm2m_watcher_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_watcher_t));
        if (watcherP == NULL)
        {
            if (allocatedObserver == true)
            {
                xiny_lwm2m_free(observedP);
            }
            return NULL;
        }
        memset(watcherP, 0, sizeof(xiny_lwm2m_watcher_t));
        watcherP->active = false;
        watcherP->server = serverP;
        watcherP->next = observedP->watcherList;
        observedP->watcherList = watcherP;
    }

    return watcherP;
}

uint8_t xiny_observe_handleRequest_lwm2m(xiny_lwm2m_context_t * contextP,
                              xiny_lwm2m_uri_t * uriP,
                              xiny_lwm2m_server_lwm2m_t * serverP,
                              int size,
                              xiny_lwm2m_data_t * dataP,
                              xiny_coap_packet_t * message,
                              xiny_coap_packet_t * response)
{
	(void) size;

    xiny_lwm2m_observed_t * observedP;
    xiny_lwm2m_watcher_t * watcherP;
    uint32_t count;

    LOG_ARG("Code: %02X, server status: %s", message->code, xiny_STR_STATUS(serverP->status));
    LOG_URI(uriP);

    xiny_coap_get_header_observe(message, &count);

    switch (count)
    {
    case 0:
        if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return xiny_COAP_400_BAD_REQUEST;
        if (message->token_len == 0) return xiny_COAP_400_BAD_REQUEST;

        watcherP = prv_getWatcher(contextP, uriP, serverP);
        if (watcherP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

        watcherP->tokenLen = message->token_len;
        memcpy(watcherP->token, message->token, message->token_len);
        watcherP->active = true;
        watcherP->lastTime = xiny_lwm2m_gettime();
        watcherP->lastMid = response->mid;
        if (xiny_IS_OPTION(message, xiny_COAP_OPTION_ACCEPT))
        {
            watcherP->format = xiny_utils_convertMediaType(message->accept[0]);
        }
        else
        {
            watcherP->format = xiny_LWM2M_CONTENT_TLV;
        }

        if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            switch (dataP->type)
            {
            case xiny_LWM2M_TYPE_INTEGER:
                if (1 != xiny_lwm2m_data_decode_int(dataP, &(watcherP->lastValue.asInteger))) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
                break;
            case xiny_LWM2M_TYPE_FLOAT:
                if (1 != xiny_lwm2m_data_decode_float(dataP, &(watcherP->lastValue.asFloat))) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
                break;
            default:
                break;
            }
        }

        xiny_coap_set_header_observe(response, watcherP->counter++);

        return xiny_COAP_205_CONTENT;

    case 1:
        // cancellation
        observedP = prv_findObserved(contextP, uriP);
        if (observedP)
        {
            watcherP = prv_findWatcher(observedP, serverP);
            if (watcherP)
            {
                xiny_observe_cancel_lwm2m(contextP, watcherP->lastMid, serverP->sessionH);
            }
        }
        return xiny_COAP_205_CONTENT;

    default:
        return xiny_COAP_400_BAD_REQUEST;
    }
}

void xiny_observe_cancel_lwm2m(xiny_lwm2m_context_t * contextP,
                    uint16_t mid,
                    void * fromSessionH)
{
    xiny_lwm2m_observed_t * observedP;

    LOG_ARG("mid: %d", mid);

    for (observedP = contextP->observedList;
         observedP != NULL;
         observedP = observedP->next)
    {
        xiny_lwm2m_watcher_t * targetP = NULL;

        if (observedP->watcherList->lastMid == mid
         && xiny_lwm2m_session_is_equal(observedP->watcherList->server->sessionH, fromSessionH, contextP->userData))
        {
            targetP = observedP->watcherList;
            observedP->watcherList = observedP->watcherList->next;
        }
        else
        {
            xiny_lwm2m_watcher_t * parentP;

            parentP = observedP->watcherList;
            while (parentP->next != NULL
                && (parentP->next->lastMid != mid
                 || !xiny_lwm2m_session_is_equal(parentP->next->server->sessionH, fromSessionH, contextP->userData)))
            {
                parentP = parentP->next;
            }
            if (parentP->next != NULL)
            {
                targetP = parentP->next;
                parentP->next = parentP->next->next;
            }
        }
        if (targetP != NULL)
        {
            if (targetP->parameters != NULL) xiny_lwm2m_free(targetP->parameters);
            xiny_lwm2m_free(targetP);
            if (observedP->watcherList == NULL)
            {
                prv_unlinkObserved(contextP, observedP);
                xiny_lwm2m_free(observedP);
            }
            return;
        }
    }
}

void xiny_observe_clear(xiny_lwm2m_context_t * contextP,
                   xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_observed_t * observedP;

    LOG_URI(uriP);

    observedP = contextP->observedList;
    while(observedP != NULL)
    {
        if (observedP->uri.objectId == uriP->objectId
            && (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) == false
                || observedP->uri.instanceId == uriP->instanceId))
        {
            xiny_lwm2m_observed_t * nextP;
            xiny_lwm2m_watcher_t * watcherP;

            nextP = observedP->next;

            for (watcherP = observedP->watcherList; watcherP != NULL; watcherP = watcherP->next)
            {
                if (watcherP->parameters != NULL) xiny_lwm2m_free(watcherP->parameters);
            }
            xiny_LWM2M_LIST_FREE(observedP->watcherList);

            prv_unlinkObserved(contextP, observedP);
            xiny_lwm2m_free(observedP);

            observedP = nextP;
        }
        else
        {
            observedP = observedP->next;
        }
    }
}

uint8_t xiny_observe_setParameters_lwm2m(xiny_lwm2m_context_t * contextP,
                              xiny_lwm2m_uri_t * uriP,
                              xiny_lwm2m_server_lwm2m_t * serverP,
                              xiny_lwm2m_attributes_t * attrP)
{
    uint8_t result;
    xiny_lwm2m_watcher_t * watcherP;

    LOG_URI(uriP);
    LOG_ARG("toSet: %08X, toClear: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
            attrP->toSet, attrP->toClear, attrP->minPeriod, attrP->maxPeriod, attrP->greaterThan, attrP->lessThan, attrP->step);

    if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return xiny_COAP_400_BAD_REQUEST;

    result = xiny_object_checkReadable_lwm2m(contextP, uriP, attrP);
    if (xiny_COAP_205_CONTENT != result) return result;

    watcherP = prv_getWatcher(contextP, uriP, serverP);
    if (watcherP == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    // Check rule “lt” value + 2*”stp” values < “gt” value
    if ((((attrP->toSet | (watcherP->parameters?watcherP->parameters->toSet:0)) & ~attrP->toClear) & xiny_ATTR_FLAG_NUMERIC) == xiny_ATTR_FLAG_NUMERIC)
    {
        float gt;
        float lt;
        float stp;

        if (0 != (attrP->toSet & xiny_LWM2M_ATTR_FLAG_GREATER_THAN))
        {
            gt = attrP->greaterThan;
        }
        else
        {
            gt = watcherP->parameters->greaterThan;
        }
        if (0 != (attrP->toSet & xiny_LWM2M_ATTR_FLAG_LESS_THAN))
        {
            lt = attrP->lessThan;
        }
        else
        {
            lt = watcherP->parameters->lessThan;
        }
        if (0 != (attrP->toSet & xiny_LWM2M_ATTR_FLAG_STEP))
        {
            stp = attrP->step;
        }
        else
        {
            stp = watcherP->parameters->step;
        }

        if (lt + (2 * stp) >= gt) return xiny_COAP_400_BAD_REQUEST;
    }

    if (watcherP->parameters == NULL)
    {
        if (attrP->toSet != 0)
        {
            watcherP->parameters = (xiny_lwm2m_attributes_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_attributes_t));
            if (watcherP->parameters == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
            memcpy(watcherP->parameters, attrP, sizeof(xiny_lwm2m_attributes_t));
        }
    }
    else
    {
        watcherP->parameters->toSet &= ~attrP->toClear;
        if (attrP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            watcherP->parameters->minPeriod = attrP->minPeriod;
        }
        if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            watcherP->parameters->maxPeriod = attrP->maxPeriod;
        }
        if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_GREATER_THAN)
        {
            watcherP->parameters->greaterThan = attrP->greaterThan;
        }
        if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_LESS_THAN)
        {
            watcherP->parameters->lessThan = attrP->lessThan;
        }
        if (attrP->toSet & xiny_LWM2M_ATTR_FLAG_STEP)
        {
            watcherP->parameters->step = attrP->step;
        }
    }

    LOG_ARG("Final toSet: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
            watcherP->parameters->toSet, watcherP->parameters->minPeriod, watcherP->parameters->maxPeriod, watcherP->parameters->greaterThan, watcherP->parameters->lessThan, watcherP->parameters->step);

    return xiny_COAP_204_CHANGED;
}

xiny_lwm2m_observed_t * xiny_observe_findByUri_lwm2m(xiny_lwm2m_context_t * contextP,
                                     xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_observed_t * targetP;

    LOG_URI(uriP);
    targetP = contextP->observedList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId)
        {
            if ((!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && !xiny_LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)))
             || (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && xiny_LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)) && (uriP->instanceId == targetP->uri.instanceId)))
             {
                 if ((!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP) && !xiny_LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)))
                     || (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP) && xiny_LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)) && (uriP->resourceId == targetP->uri.resourceId)))
                 {
                     LOG_ARG("Found one with%s observers.", targetP->watcherList ? "" : " no");
                     LOG_URI(&(targetP->uri));
                     return targetP;
                 }
             }
        }
        targetP = targetP->next;
    }

    LOG("Found nothing");
    return NULL;
}

void xiny_lwm2m_resource_value_changed(xiny_lwm2m_context_t * contextP,
                                  xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_observed_t * targetP;

    LOG_URI(uriP);
    targetP = contextP->observedList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId)
        {
            if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP)
             || (targetP->uri.flag & xiny_LWM2M_URI_FLAG_INSTANCE_ID) == 0
             || uriP->instanceId == targetP->uri.instanceId)
            {
                if (!xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)
                 || (targetP->uri.flag & xiny_LWM2M_URI_FLAG_RESOURCE_ID) == 0
                 || uriP->resourceId == targetP->uri.resourceId)
                {
                    xiny_lwm2m_watcher_t * watcherP;

                    LOG("Found an observation");
                    LOG_URI(&(targetP->uri));

                    for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
                    {
                        if (watcherP->active == true)
                        {
                            LOG("Tagging a watcher");
                            watcherP->update = true;
                        }
                    }
                }
            }
        }
        targetP = targetP->next;
    }
}

void xiny_observe_step(xiny_lwm2m_context_t * contextP,
                  time_t currentTime,
                  time_t * timeoutP)
{
    xiny_lwm2m_observed_t * targetP;

    LOG("Entering");
    for (targetP = contextP->observedList ; targetP != NULL ; targetP = targetP->next)
    {
        xiny_lwm2m_watcher_t * watcherP;
        uint8_t * buffer = NULL;
        size_t length = 0;
        xiny_lwm2m_data_t * dataP = NULL;
        int size = 0;
        double floatValue = 0;
        int64_t integerValue = 0;
        bool storeValue = false;
        xiny_coap_packet_t message[1];
        time_t interval;

        LOG_URI(&(targetP->uri));
        if (xiny_LWM2M_URI_IS_SET_RESOURCE(&targetP->uri))
        {
            if (xiny_COAP_205_CONTENT != xiny_object_read_lwm2mData_lwm2m(contextP, &targetP->uri, &size, &dataP)) continue;
            switch (dataP->type)
            {
            case xiny_LWM2M_TYPE_INTEGER:
                if (1 != xiny_lwm2m_data_decode_int(dataP, &integerValue))
                {
                    xiny_lwm2m_data_free(size, dataP);
                    continue;
                }
                storeValue = true;
                break;
            case xiny_LWM2M_TYPE_FLOAT:
                if (1 != xiny_lwm2m_data_decode_float(dataP, &floatValue))
                {
                    xiny_lwm2m_data_free(size, dataP);
                    continue;
                }
                storeValue = true;
                break;
            default:
                break;
            }
        }
        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            if (watcherP->active == true)
            {
                bool notify = false;

                if (watcherP->update == true)
                {
                    // value changed, should we notify the server ?

                    if (watcherP->parameters == NULL || watcherP->parameters->toSet == 0)
                    {
                        // no conditions
                        notify = true;
                        LOG("Notify with no conditions");
                        LOG_URI(&(targetP->uri));
                    }

                    if (notify == false
                     && watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & xiny_ATTR_FLAG_NUMERIC) != 0)
                    {
                        if ((watcherP->parameters->toSet & xiny_LWM2M_ATTR_FLAG_LESS_THAN) != 0)
                        {
                            LOG("Checking lower threshold");
                            // Did we cross the lower threshold ?
                            switch (dataP->type)
                            {
                            case xiny_LWM2M_TYPE_INTEGER:
                                if ((integerValue < watcherP->parameters->lessThan
                                  && watcherP->lastValue.asInteger > watcherP->parameters->lessThan)
                                 || (integerValue > watcherP->parameters->lessThan
                                  && watcherP->lastValue.asInteger < watcherP->parameters->lessThan))
                                {
                                    LOG("Notify on lower threshold crossing");
                                    notify = true;
                                }
                                break;
                            case xiny_LWM2M_TYPE_FLOAT:
                                if ((floatValue < watcherP->parameters->lessThan
                                  && watcherP->lastValue.asFloat > watcherP->parameters->lessThan)
                                 || (floatValue > watcherP->parameters->lessThan
                                  && watcherP->lastValue.asFloat < watcherP->parameters->lessThan))
                                {
                                    LOG("Notify on lower threshold crossing");
                                    notify = true;
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        if ((watcherP->parameters->toSet & xiny_LWM2M_ATTR_FLAG_GREATER_THAN) != 0)
                        {
                            LOG("Checking upper threshold");
                            // Did we cross the upper threshold ?
                            switch (dataP->type)
                            {
                            case xiny_LWM2M_TYPE_INTEGER:
                                if ((integerValue < watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asInteger > watcherP->parameters->greaterThan)
                                 || (integerValue > watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asInteger < watcherP->parameters->greaterThan))
                                {
                                    LOG("Notify on lower upper crossing");
                                    notify = true;
                                }
                                break;
                            case xiny_LWM2M_TYPE_FLOAT:
                                if ((floatValue < watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asFloat > watcherP->parameters->greaterThan)
                                 || (floatValue > watcherP->parameters->greaterThan
                                  && watcherP->lastValue.asFloat < watcherP->parameters->greaterThan))
                                {
                                    LOG("Notify on lower upper crossing");
                                    notify = true;
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        if ((watcherP->parameters->toSet & xiny_LWM2M_ATTR_FLAG_STEP) != 0)
                        {
                            LOG("Checking step");

                            switch (dataP->type)
                            {
                            case xiny_LWM2M_TYPE_INTEGER:
                            {
                                int64_t diff;

                                diff = integerValue - watcherP->lastValue.asInteger;
                                if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                 || (diff >= 0 && diff >= watcherP->parameters->step))
                                {
                                    LOG("Notify on step condition");
                                    notify = true;
                                }
                            }
                                break;
                            case xiny_LWM2M_TYPE_FLOAT:
                            {
                                double diff;

                                diff = floatValue - watcherP->lastValue.asFloat;
                                if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                 || (diff >= 0 && diff >= watcherP->parameters->step))
                                {
                                    LOG("Notify on step condition");
                                    notify = true;
                                }
                            }
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    if (watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD) != 0)
                    {
                        LOG_ARG("Checking minimal period (%d s)", watcherP->parameters->minPeriod);

                        if (watcherP->lastTime + watcherP->parameters->minPeriod > currentTime)
                        {
                            // Minimum Period did not elapse yet
                            interval = watcherP->lastTime + watcherP->parameters->minPeriod - currentTime;
                            if (*timeoutP > interval) *timeoutP = interval;
                            notify = false;
                        }
                        else
                        {
                            LOG("Notify on minimal period");
                            notify = true;
                        }
                    }
                }

                // Is the Maximum Period reached ?
                if (notify == false
                 && watcherP->parameters != NULL
                 && (watcherP->parameters->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    LOG_ARG("Checking maximal period (%d s)", watcherP->parameters->maxPeriod);

                    if (watcherP->lastTime + watcherP->parameters->maxPeriod <= currentTime)
                    {
                        LOG("Notify on maximal period");
                        notify = true;
                    }
                }

                if (notify == true)
                {
                    if (buffer == NULL)
                    {
                        if (dataP != NULL)
                        {
                            int res;

                            res = xiny_lwm2m_data_serialize(&targetP->uri, size, dataP, &(watcherP->format), &buffer);
                            if (res < 0)
                            {
                                break;
                            }
                            else
                            {
                                length = (size_t)res;
                            }

                        }
                        else
                        {
                            if (xiny_COAP_205_CONTENT != xiny_object_read_lwm2m(contextP, &targetP->uri, &(watcherP->format), &buffer, &length))
                            {
                                buffer = NULL;
                                break;
                            }
                        }
                        xiny_coap_init_message(message, xiny_COAP_TYPE_NON, xiny_COAP_205_CONTENT, 0);
                        xiny_coap_set_header_content_type(message, watcherP->format);
                        xiny_coap_set_payload(message, buffer, length);
                    }
                    watcherP->lastTime = currentTime;
                    watcherP->lastMid = contextP->nextMID++;
                    message->mid = watcherP->lastMid;
                    xiny_coap_set_header_token(message, watcherP->token, watcherP->tokenLen);
                    xiny_coap_set_header_observe(message, watcherP->counter++);
                    (void)xiny_message_send(contextP, message, watcherP->server->sessionH);
                    watcherP->update = false;
                }

                // Store this value
                if (notify == true && storeValue == true)
                {
                    switch (dataP->type)
                    {
                    case xiny_LWM2M_TYPE_INTEGER:
                        watcherP->lastValue.asInteger = integerValue;
                        break;
                    case xiny_LWM2M_TYPE_FLOAT:
                        watcherP->lastValue.asFloat = floatValue;
                        break;
                    default:
                        break;
                    }
                }

                if (watcherP->parameters != NULL && (watcherP->parameters->toSet & xiny_LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    // update timers
                    interval = watcherP->lastTime + watcherP->parameters->maxPeriod - currentTime;
                    if (*timeoutP > interval) *timeoutP = interval;
                }
            }
        }
        if (dataP != NULL) xiny_lwm2m_data_free(size, dataP);
        if (buffer != NULL) xiny_lwm2m_free(buffer);
    }
}

#endif

#ifdef xiny_LWM2M_SERVER_MODE

typedef struct
{
    uint16_t                client;
    xiny_lwm2m_uri_t             uri;
    xiny_lwm2m_result_callback_t callbackP;
    void *                  userDataP;
    xiny_lwm2m_context_t *       contextP;
} cancellation_data_t;

typedef struct
{
    uint16_t                id;
    uint16_t                client;
    xiny_lwm2m_uri_t             uri;
    xiny_lwm2m_result_callback_t callback;
    void *                  userData;
    xiny_lwm2m_context_t *       contextP;
} observation_data_t;



static xiny_lwm2m_observation_t * prv_findObservationByURI(xiny_lwm2m_client_t * clientP,
                                                      xiny_lwm2m_uri_t * uriP)
{
    xiny_lwm2m_observation_t * targetP;

    targetP = clientP->observationList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId
         && targetP->uri.flag == uriP->flag
         && targetP->uri.instanceId == uriP->instanceId
         && targetP->uri.resourceId == uriP->resourceId)
        {
            return targetP;
        }

        targetP = targetP->next;
    }

    return targetP;
}

void xiny_observe_remove(xiny_lwm2m_observation_t * observationP)
{
    LOG("Entering");
    observationP->clientP->observationList = (xiny_lwm2m_observation_t *) xiny_LWM2M_LIST_RM(observationP->clientP->observationList, observationP->id, NULL);
    xiny_lwm2m_free(observationP);
}

static void prv_obsRequestCallback(xiny_lwm2m_transaction_t * transacP,
        void * message)
{
    xiny_lwm2m_observation_t * observationP = NULL;
    observation_data_t * observationData = (observation_data_t *)transacP->userData;
    xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;
    uint8_t code;
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_uri_t * uriP = & observationData->uri;

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)observationData->contextP->clientList, observationData->client);
    if (clientP == NULL)
    {
        observationData->callback(observationData->client,
                &observationData->uri,
                xiny_COAP_500_INTERNAL_SERVER_ERROR,  //?
                0, NULL, 0,
                observationData->userData);
        goto end;
    }

    observationP = prv_findObservationByURI(clientP, uriP);

    // Fail it if the latest user intention is cancellation
    if(observationP && observationP->status == xiny_STATE_DEREG_PENDING)
    {
        code = xiny_COAP_400_BAD_REQUEST;
    }
    else if (message == NULL)
    {
        code = xiny_COAP_503_SERVICE_UNAVAILABLE;
    }
    else if (packet->code == xiny_COAP_205_CONTENT
            && !xiny_IS_OPTION(packet, xiny_COAP_OPTION_OBSERVE))
    {
        code = xiny_COAP_405_METHOD_NOT_ALLOWED;
    }
    else
    {
        code = packet->code;
    }

    if (code != xiny_COAP_205_CONTENT)
    {
        observationData->callback(clientP->internalID,
                &observationData->uri,
                code,
                xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                observationData->userData);
    }
    else
    {
        if(observationP == NULL)
        {
            observationP = (xiny_lwm2m_observation_t *)xiny_lwm2m_malloc(sizeof(*observationP));
            if (observationP == NULL) goto end;
            memset(observationP, 0, sizeof(*observationP));
        }
        else
        {
            observationP->clientP->observationList = (xiny_lwm2m_observation_t *) xiny_LWM2M_LIST_RM(observationP->clientP->observationList, observationP->id, NULL);

            // give the user chance to free previous observation userData
            // indicator: xiny_COAP_202_DELETED and (Length ==0)
            observationP->callback(observationP->clientP->internalID,
                    &observationP->uri,
                    xiny_COAP_202_DELETED,
                    0, NULL, 0,
                    observationP->userData);
        }

        observationP->id = observationData->id;
        observationP->clientP = clientP;

        observationP->callback = observationData->callback;
        observationP->userData = observationData->userData;
        observationP->status = xiny_STATE_REGISTERED;
        memcpy(&observationP->uri, uriP, sizeof(xiny_lwm2m_uri_t));

        observationP->clientP->observationList = (xiny_lwm2m_observation_t *)xiny_LWM2M_LIST_ADD(observationP->clientP->observationList, observationP);

        observationData->callback(observationData->client,
                &observationData->uri,
                0,
                packet->content_type, packet->payload, packet->payload_len,
                observationData->userData);
    }

end:
    xiny_lwm2m_free(observationData);
}


static void prv_obsCancelRequestCallback(xiny_lwm2m_transaction_t * transacP,
        void * message)
{
    cancellation_data_t * cancelP = (cancellation_data_t *)transacP->userData;
    xiny_coap_packet_t * packet = (xiny_coap_packet_t *)message;
    uint8_t code;
    xiny_lwm2m_client_t * clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)cancelP->contextP->clientList, cancelP->client);

    if (clientP == NULL)
    {
        cancelP->callbackP(cancelP->client,
                &cancelP->uri,
                xiny_COAP_500_INTERNAL_SERVER_ERROR,
                packet->content_type, NULL, 0,
                cancelP->userDataP);
        goto end;
    }

    xiny_lwm2m_observation_t * observationP = prv_findObservationByURI(clientP, &cancelP->uri);

    if (message == NULL)
    {
        code = xiny_COAP_503_SERVICE_UNAVAILABLE;
    }
    else
    {
        code = packet->code;
    }

    if (code != xiny_COAP_205_CONTENT)
    {
        cancelP->callbackP(cancelP->client,
                &cancelP->uri,
                code,
                xiny_LWM2M_CONTENT_TEXT, NULL, 0,
                cancelP->userDataP);
    }
    else
    {
        cancelP->callbackP(cancelP->client,
                &cancelP->uri,
                0,
                packet->content_type, packet->payload, packet->payload_len,
                cancelP->userDataP);
    }

    xiny_observe_remove(observationP);
end:
    xiny_lwm2m_free(cancelP);
}


int xiny_lwm2m_observe(xiny_lwm2m_context_t * contextP,
        uint16_t clientID,
        xiny_lwm2m_uri_t * uriP,
        xiny_lwm2m_result_callback_t callback,
        void * userData)
{
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_transaction_t * transactionP;
    observation_data_t * observationData;
    xiny_lwm2m_observation_t * observationP;
    uint8_t token[4];

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    if (!xiny_LWM2M_URI_IS_SET_INSTANCE(uriP) && xiny_LWM2M_URI_IS_SET_RESOURCE(uriP)) return xiny_COAP_400_BAD_REQUEST;

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    observationP = prv_findObservationByURI(clientP, uriP);

    observationData = (observation_data_t *)xiny_lwm2m_malloc(sizeof(observation_data_t));
    if (observationData == NULL) return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    memset(observationData, 0, sizeof(observation_data_t));

    observationData->id = ++clientP->observationId;

    // observationId can overflow. ensure new ID is not already present
    if(xiny_lwm2m_list_find((xiny_lwm2m_list_t *)clientP->observationList, observationData->id))
    {
        LOG("Can't get available observation ID. Request failed.\n");
        xiny_lwm2m_free(observationData);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }

    memcpy(&observationData->uri, uriP, sizeof(xiny_lwm2m_uri_t));

    // don't hold refer to the clientP
    observationData->client = clientP->internalID;
    observationData->callback = callback;
    observationData->userData = userData;
    observationData->contextP = contextP;

    token[0] = clientP->internalID >> 8;
    token[1] = clientP->internalID & 0xFF;
    token[2] = observationData->id >> 8;
    token[3] = observationData->id & 0xFF;

    transactionP = xiny_transaction_new(clientP->sessionH, xiny_COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, token);
    if (transactionP == NULL)
    {
        xiny_lwm2m_free(observationData);
        return xiny_COAP_500_INTERNAL_SERVER_ERROR;
    }

    xiny_coap_set_header_observe(transactionP->message, 0);
    if (clientP->supportJSON == true)
    {
        xiny_coap_set_header_accept(transactionP->message, xiny_LWM2M_CONTENT_JSON);
    }
    else
    {
        xiny_coap_set_header_accept(transactionP->message, xiny_LWM2M_CONTENT_TLV);
    }

    transactionP->callback = prv_obsRequestCallback;
    transactionP->userData = (void *)observationData;

    contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transactionP);

    // update the user latest intention
    if(observationP) observationP->status = xiny_STATE_REG_PENDING;

    int ret = xiny_transaction_send(contextP, transactionP);
    if (ret != 0)
    {
        LOG("xiny_transaction_send failed!");
        xiny_lwm2m_free(observationData);
    }
    return ret;
}

int xiny_lwm2m_observe_cancel(xiny_lwm2m_context_t * contextP,
        uint16_t clientID,
        xiny_lwm2m_uri_t * uriP,
        xiny_lwm2m_result_callback_t callback,
        void * userData)
{
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_observation_t * observationP;
    int ret;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return xiny_COAP_404_NOT_FOUND;

    observationP = prv_findObservationByURI(clientP, uriP);
    if (observationP == NULL) return xiny_COAP_404_NOT_FOUND;

    switch (observationP->status)
    {
    case xiny_STATE_REGISTERED:
    {
        xiny_lwm2m_transaction_t * transactionP;
        cancellation_data_t * cancelP;
        uint8_t token[4];

        token[0] = clientP->internalID >> 8;
        token[1] = clientP->internalID & 0xFF;
        token[2] = observationP->id >> 8;
        token[3] = observationP->id & 0xFF;

        transactionP = xiny_transaction_new(clientP->sessionH, xiny_COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, token);
        if (transactionP == NULL)
        {
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        cancelP = (cancellation_data_t *)xiny_lwm2m_malloc(sizeof(cancellation_data_t));
        if (cancelP == NULL)
        {
            xiny_lwm2m_free(transactionP);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }

        xiny_coap_set_header_observe(transactionP->message, 1);

        // don't hold refer to the clientP
        cancelP->client = clientP->internalID;
        memcpy(&cancelP->uri, uriP, sizeof(xiny_lwm2m_uri_t));
        cancelP->callbackP = callback;
        cancelP->userDataP = userData;
        cancelP->contextP = contextP;

        transactionP->callback = prv_obsCancelRequestCallback;
        transactionP->userData = (void *)cancelP;

        contextP->transactionList = (xiny_lwm2m_transaction_t *)xiny_LWM2M_LIST_ADD(contextP->transactionList, transactionP);

        observationP->status = xiny_STATE_DEREG_PENDING;

        int ret = xiny_transaction_send(contextP, transactionP);
        if (ret != 0) xiny_lwm2m_free(cancelP);
        return ret;
    }

    case xiny_STATE_REG_PENDING:
        observationP->status = xiny_STATE_DEREG_PENDING;
        ret = xiny_COAP_204_CHANGED;
        break;

    default:
        // Should not happen
        ret = xiny_COAP_IGNORE;
        break;
    }

    // no other chance to remove the observationP since not sending a transaction
    xiny_observe_remove(observationP);

    // need to give a indicator (non-zero) to user for properly freeing the userData
    return ret;
}

bool xiny_observe_handleNotify(xiny_lwm2m_context_t * contextP,
                           void * fromSessionH,
                           xiny_coap_packet_t * message,
        				   xiny_coap_packet_t * response)
{
    uint8_t * tokenP;
    int token_len;
    uint16_t clientID;
    uint16_t obsID;
    xiny_lwm2m_client_t * clientP;
    xiny_lwm2m_observation_t * observationP;
    uint32_t count;

    LOG("Entering");
    token_len = xiny_coap_get_header_token(message, (const uint8_t **)&tokenP);
    if (token_len != sizeof(uint32_t)) return false;

    if (1 != xiny_coap_get_header_observe(message, &count)) return false;

    clientID = (tokenP[0] << 8) | tokenP[1];
    obsID = (tokenP[2] << 8) | tokenP[3];

    clientP = (xiny_lwm2m_client_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return false;

    observationP = (xiny_lwm2m_observation_t *)xiny_lwm2m_list_find((xiny_lwm2m_list_t *)clientP->observationList, obsID);
    if (observationP == NULL)
    {
        xiny_coap_init_message(response, xiny_COAP_TYPE_RST, 0, message->mid);
        xiny_message_send(contextP, response, fromSessionH);
    }
    else
    {
        if (message->type == xiny_COAP_TYPE_CON ) {
            xiny_coap_init_message(response, xiny_COAP_TYPE_ACK, 0, message->mid);
            xiny_message_send(contextP, response, fromSessionH);
        }
        observationP->callback(clientID,
                               &observationP->uri,
                               (int)count,
                               message->content_type, message->payload, message->payload_len,
                               observationP->userData);
    }
    return true;
}
#endif
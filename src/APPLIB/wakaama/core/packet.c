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
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Pascal Rieux - Please refer to git log
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

/*
Contains code snippets which are:

 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

*/


#include "xiny_internals.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


static void handle_reset(xiny_lwm2m_context_t * contextP,
                         void * fromSessionH,
                         xiny_coap_packet_t * message)
{
#ifdef XINYI_LWM2M_CLIENT_MODE
    LOG("Entering");
    xiny_observe_cancel_lwm2m(contextP, message->mid, fromSessionH);
#endif
}

static uint8_t handle_xiny_request(xiny_lwm2m_context_t * contextP,
                              void * fromSessionH,
                              xiny_coap_packet_t * message,
                              xiny_coap_packet_t * response)
{
    xiny_lwm2m_uri_t * uriP;
    uint8_t result = xiny_COAP_IGNORE;

    LOG("Entering");
	
#ifdef XINYI_LWM2M_CLIENT_MODE
    uriP = xiny_uri_decode_lwm2m(contextP->altPath, message->uri_path);
#else
    uriP = xiny_uri_decode_lwm2m(NULL, message->uri_path);
#endif

    if (uriP == NULL) return xiny_COAP_400_BAD_REQUEST;

    switch(uriP->flag & xiny_LWM2M_URI_MASK_TYPE)
    {
#ifdef XINYI_LWM2M_CLIENT_MODE
    case xiny_LWM2M_URI_FLAG_DM:
    {
        xiny_lwm2m_server_lwm2m_t * serverP;

        serverP = xiny_utils_findServer(contextP, fromSessionH);
        if (serverP != NULL)
        {
            result = xiny_dm_handleRequest(contextP, uriP, serverP, message, response);
        }
#ifdef xiny_LWM2M_BOOTSTRAP
        else
        {
            serverP = xiny_utils_findBootstrapServer(contextP, fromSessionH);
            if (serverP != NULL)
            {
                result = xiny_bootstrap_handleCommand(contextP, uriP, serverP, message, response);
            }
        }
#endif
    }
    break;

#ifdef xiny_LWM2M_BOOTSTRAP
    case xiny_LWM2M_URI_FLAG_DELETE_ALL:
        if (xiny_COAP_DELETE != message->code)
        {
            result = xiny_COAP_400_BAD_REQUEST;
        }
        else
        {
            result = xiny_bootstrap_handleDeleteAll(contextP, fromSessionH);
        }
        break;

    case xiny_LWM2M_URI_FLAG_BOOTSTRAP:
        if (message->code == xiny_COAP_POST)
        {
            result = xiny_bootstrap_handleFinish(contextP, fromSessionH);
        }
        break;
#endif
#endif

#ifdef xiny_LWM2M_SERVER_MODE
    case xiny_LWM2M_URI_FLAG_REGISTRATION:
        result = xiny_registration_handleRequest(contextP, uriP, fromSessionH, message, response);
        break;
#endif
#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
    case xiny_LWM2M_URI_FLAG_BOOTSTRAP:
        result = xiny_bootstrap_handleRequest(contextP, uriP, fromSessionH, message, response);
        break;
#endif
    default:
        result = xiny_COAP_IGNORE;
        break;
    }

    xiny_coap_set_status_code(response, result);

    if (xiny_COAP_IGNORE < result && result < xiny_COAP_400_BAD_REQUEST)
    {
        result = xiny_NO_ERROR;
    }

    xiny_lwm2m_free(uriP);
    return result;
}

/* This function is an adaptation of function coap_receive() from Erbium's er-coap-13-engine.c.
 * Erbium is Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 */
void xiny_lwm2m_handle_packet(xiny_lwm2m_context_t * contextP,
                         uint8_t * buffer,
                         int length,
                         void * fromSessionH)
{
    uint8_t xiny_coap_error_code = xiny_NO_ERROR;
    static xiny_coap_packet_t message[1];
    static xiny_coap_packet_t response[1];

    LOG("Entering");
    xiny_coap_error_code = xiny_coap_parse_message(message, buffer, (uint16_t)length);
    if (xiny_coap_error_code == xiny_NO_ERROR)
    {
        LOG_ARG("Parsed: ver %u, type %u, tkl %u, code %u.%.2u, mid %u, Content type: %d",
                message->version, message->type, message->token_len, message->code >> 5, message->code & 0x1F, message->mid, message->content_type);
        LOG_ARG("Payload: %.*s", message->payload_len, message->payload);
        if (message->code >= xiny_COAP_GET && message->code <= xiny_COAP_DELETE)
        {
            uint32_t block_num = 0;
            uint16_t block_size = xiny_REST_MAX_CHUNK_SIZE;
            uint32_t block_offset = 0;
            int64_t new_offset = 0;

            /* prepare response */
            if (message->type == xiny_COAP_TYPE_CON)
            {
                /* Reliable CON xiny_requests are answered with an ACK. */
                xiny_coap_init_message(response, xiny_COAP_TYPE_ACK, xiny_COAP_205_CONTENT, message->mid);
            }
            else
            {
                /* Unreliable NON xiny_requests are answered with a NON as well. */
                xiny_coap_init_message(response, xiny_COAP_TYPE_NON, xiny_COAP_205_CONTENT, contextP->nextMID++);
            }

            /* mirror token */
            if (message->token_len)
            {
                xiny_coap_set_header_token(response, message->token, message->token_len);
            }

            /* get offset for blockwise transfers */
            if (xiny_coap_get_header_block2(message, &block_num, NULL, &block_size, &block_offset))
            {
                LOG_ARG("Blockwise: block xiny_request %u (%u/%u) @ %u bytes", block_num, block_size, xiny_REST_MAX_CHUNK_SIZE, block_offset);
                block_size = MIN(block_size, xiny_REST_MAX_CHUNK_SIZE);
                new_offset = block_offset;
            }

            /* handle block1 option */
            if (xiny_IS_OPTION(message, xiny_COAP_OPTION_BLOCK1))
            {
#ifdef XINYI_LWM2M_CLIENT_MODE
                // get server
                xiny_lwm2m_server_lwm2m_t * serverP;
                serverP = xiny_utils_findServer(contextP, fromSessionH);
#ifdef xiny_LWM2M_BOOTSTRAP
                if (serverP == NULL)
                {
                    serverP = xiny_utils_findBootstrapServer(contextP, fromSessionH);
                }
#endif
                if (serverP == NULL)
                {
                    xiny_coap_error_code = xiny_COAP_500_INTERNAL_SERVER_ERROR;
                }
                else
                {
                    uint32_t block1_num;
                    uint8_t  block1_more;
                    uint16_t block1_size;
                    uint8_t * complete_buffer = NULL;
                    size_t complete_buffer_size;

                    // parse block1 header
                    xiny_coap_get_header_block1(message, &block1_num, &block1_more, &block1_size, NULL);
                    LOG_ARG("Blockwise: block1 xiny_request NUM %u (SZX %u/ SZX Max%u) MORE %u", block1_num, block1_size, xiny_REST_MAX_CHUNK_SIZE, block1_more);

                    // handle block 1
                    xiny_coap_error_code = xiny_coap_block1_handler(&serverP->block1Data, message->mid, message->payload, message->payload_len, block1_size, block1_num, block1_more, &complete_buffer, &complete_buffer_size);

                    // if payload is complete, replace it in the coap message.
                    if (xiny_coap_error_code == xiny_NO_ERROR)
                    {
                        message->payload = complete_buffer;
                        message->payload_len = complete_buffer_size;
                    }
                    else if (xiny_coap_error_code == xiny_COAP_231_CONTINUE)
                    {
                        block1_size = MIN(block1_size, xiny_REST_MAX_CHUNK_SIZE);
                        xiny_coap_set_header_block1(response,block1_num, block1_more,block1_size);
                    }
                }
#else
                xiny_coap_error_code = xiny_COAP_501_NOT_IMPLEMENTED;
#endif
            }
            if (xiny_coap_error_code == xiny_NO_ERROR)
            {
                xiny_coap_error_code = handle_xiny_request(contextP, fromSessionH, message, response);
            }
            if (xiny_coap_error_code==xiny_NO_ERROR)
            {
                /* Save original payload pointer for later freeing. Payload in response may be updated. */
                uint8_t *payload = response->payload;
                if ( xiny_IS_OPTION(message, xiny_COAP_OPTION_BLOCK2) )
                {
                    /* unchanged new_offset indicates that resource is unaware of blockwise transfer */
                    if (new_offset==block_offset)
                    {
                        LOG_ARG("Blockwise: unaware resource with payload length %u/%u", response->payload_len, block_size);
                        if (block_offset >= response->payload_len)
                        {
                            LOG("handle_incoming_data(): block_offset >= response->payload_len");

                            response->code = xiny_COAP_402_BAD_OPTION;
                            xiny_coap_set_payload(response, "BlockOutOfScope", 15); /* a const char str[] and sizeof(str) produces larger code size */
                        }
                        else
                        {
                            xiny_coap_set_header_block2(response, block_num, response->payload_len - block_offset > block_size, block_size);
                            xiny_coap_set_payload(response, response->payload+block_offset, MIN(response->payload_len - block_offset, block_size));
                        } /* if (valid offset) */
                    }
                    else
                    {
                        /* resource provides chunk-wise data */
                        LOG_ARG("Blockwise: blockwise resource, new offset %d", (int) new_offset);
                        xiny_coap_set_header_block2(response, block_num, new_offset!=-1 || response->payload_len > block_size, block_size);
                        if (response->payload_len > block_size) xiny_coap_set_payload(response, response->payload, block_size);
                    } /* if (resource aware of blockwise) */
                }
                else if (new_offset!=0)
                {
                    LOG_ARG("Blockwise: no block option for blockwise resource, using block size %u", xiny_REST_MAX_CHUNK_SIZE);

                    xiny_coap_set_header_block2(response, 0, new_offset!=-1, xiny_REST_MAX_CHUNK_SIZE);
                    xiny_coap_set_payload(response, response->payload, MIN(response->payload_len, xiny_REST_MAX_CHUNK_SIZE));
                } /* if (blockwise xiny_request) */

                xiny_coap_error_code = xiny_message_send(contextP, response, fromSessionH);

                xiny_lwm2m_free(payload);
                response->payload = NULL;
                response->payload_len = 0;
            }
            else if (xiny_coap_error_code != xiny_COAP_IGNORE)
            {
                if (1 == xiny_coap_set_status_code(response, xiny_coap_error_code))
                {
                    xiny_coap_error_code = xiny_message_send(contextP, response, fromSessionH);
                }
            }
        }
        else
        {
            /* Responses */
            switch (message->type)
            {
            case xiny_COAP_TYPE_NON:
            case xiny_COAP_TYPE_CON:
                {
                    bool done = xiny_transaction_handleResponse(contextP, fromSessionH, message, response);

#ifdef xiny_LWM2M_SERVER_MODE
                    if (!done && xiny_IS_OPTION(message, xiny_COAP_OPTION_OBSERVE) &&
                        ((message->code == xiny_COAP_204_CHANGED) || (message->code == xiny_COAP_205_CONTENT)))
                    {
                        done = xiny_observe_handleNotify(contextP, fromSessionH, message, response);
                    }
#endif
                    if (!done && message->type == xiny_COAP_TYPE_CON )
                    {
                        xiny_coap_init_message(response, xiny_COAP_TYPE_ACK, 0, message->mid);
                        xiny_coap_error_code = xiny_message_send(contextP, response, fromSessionH);
                    }
                }
                break;

            case xiny_COAP_TYPE_RST:
                /* Cancel possible subscriptions. */
                handle_reset(contextP, fromSessionH, message);
                xiny_transaction_handleResponse(contextP, fromSessionH, message, NULL);
                break;

            case xiny_COAP_TYPE_ACK:
                xiny_transaction_handleResponse(contextP, fromSessionH, message, NULL);
                break;

            default:
                break;
            }
        } /* Request or Response */
        xiny_coap_free_header(message);
    } /* if (parsed correctly) */
    else
    {
        LOG_ARG("Message parsing failed %u.%2u", xiny_coap_error_code >> 5, xiny_coap_error_code & 0x1F);
    }

    if (xiny_coap_error_code != xiny_NO_ERROR && xiny_coap_error_code != xiny_COAP_IGNORE)
    {
        LOG_ARG("ERROR %u: %s", xiny_coap_error_code, xiny_coap_error_message);

        /* Set to sendable error code. */
        if (xiny_coap_error_code >= 192)
        {
            xiny_coap_error_code = xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
        /* Reuse input buffer for error message. */
        xiny_coap_init_message(message, xiny_COAP_TYPE_ACK, xiny_coap_error_code, message->mid);
        xiny_coap_set_payload(message, xiny_coap_error_message, strlen(xiny_coap_error_message));
        xiny_message_send(contextP, message, fromSessionH);
    }
}


uint8_t xiny_message_send(xiny_lwm2m_context_t * contextP,
                     xiny_coap_packet_t * message,
                     void * sessionH)
{
    uint8_t result = xiny_COAP_500_INTERNAL_SERVER_ERROR;
    uint8_t * pktBuffer;
    size_t pktBufferLen = 0;
    size_t allocLen;

    LOG("Entering");
    allocLen = xiny_coap_serialize_get_size(message);
    LOG_ARG("Size to allocate: %d", allocLen);
    if (allocLen == 0) return xiny_COAP_500_INTERNAL_SERVER_ERROR;

    pktBuffer = (uint8_t *)xiny_lwm2m_malloc(allocLen);
    if (pktBuffer != NULL)
    {
        pktBufferLen = xiny_coap_serialize_message(message, pktBuffer);
        LOG_ARG("xiny_coap_serialize_message() returned %d", pktBufferLen);
        if (0 != pktBufferLen)
        {
            result = xiny_lwm2m_buffer_send(sessionH, pktBuffer, pktBufferLen, contextP->userData);
        }
        xiny_lwm2m_free(pktBuffer);
    }

    return result;
}
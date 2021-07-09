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

/************************************************************************
 *  Function for communications transactions.
 *
 *  Basic specification: rfc7252
 *
 *  Transaction implements processing of piggybacked and separate response communication
 *  dialogs specified in section 2.2 of the above specification.
 *  The caller registers a callback function, which is called, when either the result is
 *  xiny_received or a timeout occurs.
 *
 *  Supported dialogs:
 *  Requests (GET - DELETE):
 *  - CON with mid, without token => regular finished with corresponding ACK.MID
 *  - CON with mid, with token => regular finished with corresponding ACK.MID and response containing
 *                  the token. Supports both versions, with piggybacked ACK and separate ACK/response.
 *                  Though the ACK.MID may be lost in the separate version, a matching response may
 *                  finish the transaction even without the ACK.MID.
 *  - NON without token => no transaction, no result expected!
 *  - NON with token => regular finished with response containing the token.
 *  Responses (xiny_COAP_201_CREATED - ?):
 *  - CON with mid => regular finished with corresponding ACK.MID
 */

#include "xiny_internals.h"


/*
 * Modulo mask (+1 and +0.5 for rounding) for a random number to get the tick number for the random
 * retransmission time between xiny_COAP_RESPONSE_TIMEOUT and xiny_COAP_RESPONSE_TIMEOUT*COAP_RESPONSE_RANDOM_FACTOR.
 */
#define xiny_COAP_RESPONSE_TIMEOUT_TICKS         (CLOCK_SECOND * xiny_COAP_RESPONSE_TIMEOUT)
#define xiny_COAP_RESPONSE_TIMEOUT_BACKOFF_MASK  ((CLOCK_SECOND * xiny_COAP_RESPONSE_TIMEOUT * (COAP_RESPONSE_RANDOM_FACTOR - 1)) + 1.5)

static int prv_checkFinished(xiny_lwm2m_transaction_t * transacP,
                             xiny_coap_packet_t * xiny_receivedMessage)
{
    int len;
    const uint8_t* token;
    xiny_coap_packet_t * transactionMessage = transacP->message;

    if (xiny_COAP_DELETE < transactionMessage->code)
    {
        // response
        return transacP->ack_xiny_received ? 1 : 0;
    }
    if (!xiny_IS_OPTION(transactionMessage, xiny_COAP_OPTION_TOKEN))
    {
        // xiny_request without token
        return transacP->ack_xiny_received ? 1 : 0;
    }

    len = xiny_coap_get_header_token(xiny_receivedMessage, &token);
    if (transactionMessage->token_len == len)
    {
        if (memcmp(transactionMessage->token, token, len)==0) return 1;
    }

    return 0;
}

xiny_lwm2m_transaction_t * xiny_transaction_new(void * sessionH,
                                      xiny_coap_method_t method,
                                      char * altPath,
                                      xiny_lwm2m_uri_t * uriP,
                                      uint16_t mID,
                                      uint8_t token_len,
                                      uint8_t* token)
{
    xiny_lwm2m_transaction_t * transacP;
    int result;

    LOG_ARG("method: %d, altPath: \"%s\", mID: %d, token_len: %d",
            method, altPath, mID, token_len);
    LOG_URI(uriP);

    // no transactions without peer
    if (NULL == sessionH) return NULL;

    transacP = (xiny_lwm2m_transaction_t *)xiny_lwm2m_malloc(sizeof(xiny_lwm2m_transaction_t));

    if (NULL == transacP) return NULL;
    memset(transacP, 0, sizeof(xiny_lwm2m_transaction_t));

    transacP->message = xiny_lwm2m_malloc(sizeof(xiny_coap_packet_t));
    if (NULL == transacP->message) goto error;

    xiny_coap_init_message(transacP->message, xiny_COAP_TYPE_CON, method, mID);

    transacP->peerH = sessionH;

    transacP->mID = mID;

    if (altPath != NULL)
    {
        // TODO: Support multi-segment alternative path
        xiny_coap_set_header_uri_path_segment(transacP->message, altPath + 1);
    }
    if (NULL != uriP)
    {
        char stringID[xiny_LWM2M_STRING_ID_MAX_LEN];

        result = xiny_utils_intToText(uriP->objectId, (uint8_t*)stringID, xiny_LWM2M_STRING_ID_MAX_LEN);
        if (result == 0) goto error;
        stringID[result] = 0;
        xiny_coap_set_header_uri_path_segment(transacP->message, stringID);

        if (xiny_LWM2M_URI_IS_SET_INSTANCE(uriP))
        {
            result = xiny_utils_intToText(uriP->instanceId, (uint8_t*)stringID, xiny_LWM2M_STRING_ID_MAX_LEN);
            if (result == 0) goto error;
            stringID[result] = 0;
            xiny_coap_set_header_uri_path_segment(transacP->message, stringID);
        }
        else
        {
            if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                xiny_coap_set_header_uri_path_segment(transacP->message, NULL);
            }
        }
        if (xiny_LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            result = xiny_utils_intToText(uriP->resourceId, (uint8_t*)stringID, xiny_LWM2M_STRING_ID_MAX_LEN);
            if (result == 0) goto error;
            stringID[result] = 0;
            xiny_coap_set_header_uri_path_segment(transacP->message, stringID);
        }
    }
    if (0 < token_len)
    {
        if (NULL != token)
        {
            xiny_coap_set_header_token(transacP->message, token, token_len);
        }
        else {
            // generate a token
            uint8_t temp_token[xiny_COAP_TOKEN_LEN];
            time_t tv_sec = xiny_lwm2m_gettime();

            // xiny_initialize first 6 bytes, leave the last 2 random
            temp_token[0] = mID;
            temp_token[1] = mID >> 8;
            temp_token[2] = tv_sec;
            temp_token[3] = tv_sec >> 8;
            temp_token[4] = tv_sec >> 16;
            temp_token[5] = tv_sec >> 24;
            // use just the provided amount of bytes
            xiny_coap_set_header_token(transacP->message, temp_token, token_len);
        }
    }

    LOG_ARG("Exiting on success. new transac=%p", transacP);
    return transacP;

error:
    LOG("Exiting on failure");
    xiny_lwm2m_free(transacP);
    return NULL;
}

void xiny_transaction_free(xiny_lwm2m_transaction_t * transacP)
{
    LOG_ARG("Entering. transaction=%p", transacP);
    if (transacP->message)
    {
       xiny_coap_free_header(transacP->message);
       xiny_lwm2m_free(transacP->message);
    }

    if (transacP->buffer) xiny_lwm2m_free(transacP->buffer);
    xiny_lwm2m_free(transacP);
}

void xiny_transaction_remove(xiny_lwm2m_context_t * contextP,
                        xiny_lwm2m_transaction_t * transacP)
{
    LOG_ARG("Entering. transaction=%p", transacP);
    contextP->transactionList = (xiny_lwm2m_transaction_t *) xiny_LWM2M_LIST_RM(contextP->transactionList, transacP->mID, NULL);
    xiny_transaction_free(transacP);
}

bool xiny_transaction_handleResponse(xiny_lwm2m_context_t * contextP,
                                 void * fromSessionH,
                                 xiny_coap_packet_t * message,
                                 xiny_coap_packet_t * response)
{
    bool found = false;
    bool reset = false;
    xiny_lwm2m_transaction_t * transacP;

    LOG("Entering");
    transacP = contextP->transactionList;

    while (NULL != transacP)
    {
        if (xiny_lwm2m_session_is_equal(fromSessionH, transacP->peerH, contextP->userData) == true)
        {
            if (!transacP->ack_xiny_received)
            {
                if ((xiny_COAP_TYPE_ACK == message->type) || (xiny_COAP_TYPE_RST == message->type))
                {
                    if (transacP->mID == message->mid)
	                {
    	                found = true;
        	            transacP->ack_xiny_received = true;
            	        reset = xiny_COAP_TYPE_RST == message->type;
            	    }
                }
            }

            if (reset || prv_checkFinished(transacP, message))
            {
                // HACK: If a message is sent from the monitor callback,
                // it will arrive before the xiny_registration ACK.
                // So we resend transaction that were denied for authentication reason.
                if (!reset)
                {
                    if (xiny_COAP_TYPE_CON == message->type && NULL != response)
                    {
                        xiny_coap_init_message(response, xiny_COAP_TYPE_ACK, 0, message->mid);
                        xiny_message_send(contextP, response, fromSessionH);
                    }
                
	                if ((xiny_COAP_401_UNAUTHORIZED == message->code) && (xiny_COAP_MAX_RETRANSMIT > transacP->retrans_counter))
    	            {
        	            transacP->ack_xiny_received = false;
            	        transacP->retrans_time += xiny_COAP_RESPONSE_TIMEOUT;
                	    return true;
                	}
				}       
                if (transacP->callback != NULL)
                {
                    transacP->callback(transacP, message);
                }
                xiny_transaction_remove(contextP, transacP);
                return true;
            }
            // if we found our guy, exit
            if (found)
            {
                time_t tv_sec = xiny_lwm2m_gettime();
                if (0 <= tv_sec)
                {
                    transacP->retrans_time = tv_sec;
                }
                if (transacP->response_timeout)
                {
                    transacP->retrans_time += transacP->response_timeout;
                }
                else
                {
                    transacP->retrans_time += xiny_COAP_RESPONSE_TIMEOUT * transacP->retrans_counter;
                }
                return true;
            }
        }

        transacP = transacP->next;
    }
    return false;
}

int xiny_transaction_send(xiny_lwm2m_context_t * contextP,
                     xiny_lwm2m_transaction_t * transacP)
{
    bool maxRetriesReached = false;

    LOG_ARG("Entering: transaction=%p", transacP);
    if (transacP->buffer == NULL)
    {
        transacP->buffer_len = xiny_coap_serialize_get_size(transacP->message);
        if (transacP->buffer_len == 0)
        {
           xiny_transaction_remove(contextP, transacP);
           return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }

        transacP->buffer = (uint8_t*)xiny_lwm2m_malloc(transacP->buffer_len);
        if (transacP->buffer == NULL)
        {
           xiny_transaction_remove(contextP, transacP);
           return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }

        transacP->buffer_len = xiny_coap_serialize_message(transacP->message, transacP->buffer);
        if (transacP->buffer_len == 0)
        {
            xiny_lwm2m_free(transacP->buffer);
            transacP->buffer = NULL;
            xiny_transaction_remove(contextP, transacP);
            return xiny_COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (!transacP->ack_xiny_received)
    {
        long unsigned timeout=0;

        if (0 == transacP->retrans_counter)
        {
            time_t tv_sec = xiny_lwm2m_gettime();
            if (0 <= tv_sec)
            {
                transacP->retrans_time = tv_sec + xiny_COAP_RESPONSE_TIMEOUT;
                transacP->retrans_counter = 1;
                timeout = 0;
            }
            else
            {
                maxRetriesReached = true;
            }
        }
        else
        {
            timeout = xiny_COAP_RESPONSE_TIMEOUT << (transacP->retrans_counter - 1);
        }

        if (xiny_COAP_MAX_RETRANSMIT + 1 >= transacP->retrans_counter)
        {
            (void)xiny_lwm2m_buffer_send(transacP->peerH, transacP->buffer, transacP->buffer_len, contextP->userData);

            transacP->retrans_time += timeout;
            transacP->retrans_counter += 1;
        }
        else
        {
            maxRetriesReached = true;
        }
    }

    if (transacP->ack_xiny_received || maxRetriesReached)
    {
        if (transacP->callback)
        {
            LOG_ARG("transaction %p expired..calling callback", transacP);
            transacP->callback(transacP, NULL);
        }
        xiny_transaction_remove(contextP, transacP);
        return -1;
    }

    return 0;
}

void xiny_transaction_step(xiny_lwm2m_context_t * contextP,
                      time_t currentTime,
                      time_t * timeoutP)
{
    xiny_lwm2m_transaction_t * transacP;

    LOG("Entering");
    transacP = contextP->transactionList;
    while (transacP != NULL)
    {
        // xiny_transaction_send() may remove transaction from the linked list
        xiny_lwm2m_transaction_t * nextP = transacP->next;
        int removed = 0;

        if (transacP->retrans_time <= currentTime)
        {
            removed = xiny_transaction_send(contextP, transacP);
        }

        if (0 == removed)
        {
            time_t interval;

            if (transacP->retrans_time > currentTime)
            {
                interval = transacP->retrans_time - currentTime;
            }
            else
            {
                interval = 1;
            }

            if (*timeoutP > interval)
            {
                *timeoutP = interval;
            }
        }
        else
        {
            *timeoutP = 1;
        }

        transacP = nextP;
    }
}
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

#ifndef xiny__LWM2M_INTERNALS_H_
#define xiny__LWM2M_INTERNALS_H_

#include "xiny_liblwm2m.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "er-coap-13/xiny_er-coap-13.h"

#ifdef LWM2M_WITH_LOGS
#include <inttypes.h>
#define LOG(STR) xiny_lwm2m_printf("[%s:%d] " STR "\r\n", __func__ , __LINE__)
#define LOG_ARG(FMT, ...) xiny_lwm2m_printf("[%s:%d] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#ifndef LOG_URI
#define LOG_URI(URI)                                                                \
{                                                                                   \
    if ((URI) == NULL) xiny_lwm2m_printf("[%s:%d] NULL\r\n", __func__ , __LINE__);     \
    else                                                                            \
    {                                                                               \
        xiny_lwm2m_printf("[%s:%d] /%d", __func__ , __LINE__ , (URI)->objectId);       \
        if (xiny_LWM2M_URI_IS_SET_INSTANCE(URI)) xiny_lwm2m_printf("/%d", (URI)->instanceId); \
        if (xiny_LWM2M_URI_IS_SET_RESOURCE(URI)) xiny_lwm2m_printf("/%d", (URI)->resourceId); \
        xiny_lwm2m_printf("\r\n");                                                       \
    }                                                                               \
}
#endif
#define xiny_STR_STATUS(S)                                           \
((S) == xiny_STATE_DEREGISTERED ? "xiny_STATE_DEREGISTERED" :             \
((S) == xiny_STATE_REG_PENDING ? "xiny_STATE_REG_PENDING" :               \
((S) == xiny_STATE_REGISTERED ? "xiny_STATE_REGISTERED" :                 \
((S) == xiny_STATE_REG_FAILED ? "xiny_STATE_REG_FAILED" :                 \
((S) == xiny_STATE_REG_UPDATE_PENDING ? "xiny_STATE_REG_UPDATE_PENDING" : \
((S) == xiny_STATE_REG_UPDATE_NEEDED ? "xiny_STATE_REG_UPDATE_NEEDED" :   \
((S) == xiny_STATE_REG_FULL_UPDATE_NEEDED ? "xiny_STATE_REG_FULL_UPDATE_NEEDED" :   \
((S) == xiny_STATE_DEREG_PENDING ? "xiny_STATE_DEREG_PENDING" :           \
((S) == xiny_STATE_BS_HOLD_OFF ? "xiny_STATE_BS_HOLD_OFF" :               \
((S) == xiny_STATE_BS_INITIATED ? "xiny_STATE_BS_INITIATED" :             \
((S) == xiny_STATE_BS_PENDING ? "xiny_STATE_BS_PENDING" :                 \
((S) == xiny_STATE_BS_FINISHED ? "xiny_STATE_BS_FINISHED" :               \
((S) == xiny_STATE_BS_FINISHING ? "xiny_STATE_BS_FINISHING" :             \
((S) == xiny_STATE_BS_FAILING ? "xiny_STATE_BS_FAILING" :                 \
((S) == xiny_STATE_BS_FAILED ? "xiny_STATE_BS_FAILED" :                   \
"Unknown")))))))))))))))
#define xiny_STR_MEDIA_TYPE(M)                                \
((M) == xiny_LWM2M_CONTENT_TEXT ? "xiny_LWM2M_CONTENT_TEXT" :      \
((M) == xiny_LWM2M_CONTENT_LINK ? "xiny_LWM2M_CONTENT_LINK" :      \
((M) == xiny_LWM2M_CONTENT_OPAQUE ? "xiny_LWM2M_CONTENT_OPAQUE" :  \
((M) == xiny_LWM2M_CONTENT_TLV ? "xiny_LWM2M_CONTENT_TLV" :        \
((M) == xiny_LWM2M_CONTENT_JSON ? "xiny_LWM2M_CONTENT_JSON" :      \
"Unknown")))))
#define xiny_STR_STATE(S)                                \
((S) == xiny_STATE_INITIAL ? "xiny_STATE_INITIAL" :      \
((S) == xiny_STATE_BOOTSTRAP_REQUIRED ? "xiny_STATE_BOOTSTRAP_REQUIRED" :      \
((S) == xiny_STATE_BOOTSTRAPPING ? "xiny_STATE_BOOTSTRAPPING" :  \
((S) == xiny_STATE_REGISTER_REQUIRED ? "xiny_STATE_REGISTER_REQUIRED" :        \
((S) == xiny_STATE_REGISTERING ? "xiny_STATE_REGISTERING" :      \
((S) == xiny_STATE_READY ? "xiny_STATE_READY" :      \
"Unknown"))))))
#else
#define LOG_ARG(FMT, ...)
#define LOG(STR)
#ifndef LOG_URI
#define LOG_URI(URI)
#endif
#endif

#define xiny_LWM2M_DEFAULT_LIFETIME  86400

#ifdef xiny_LWM2M_SUPPORT_JSON
#define xiny_REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\";ct=11543,"
#define xiny_REG_LWM2M_RESOURCE_TYPE_LEN 25
#else
#define xiny_REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\","
#define xiny_REG_LWM2M_RESOURCE_TYPE_LEN 17
#endif
#define xiny_REG_START           "<"
#define xiny_REG_DEFAULT_PATH    "/"

#define REG_OBJECT_MIN_LEN  5   // "</n>,"
#define xiny_REG_PATH_END        ">,"
#define xiny_REG_PATH_SEPARATOR  "/"

#define xiny_REG_OBJECT_PATH             "<%s/%hu>,"
#define xiny_REG_OBJECT_INSTANCE_PATH    "<%s/%hu/%hu>,"

#define xiny_URI_REGISTRATION_SEGMENT        "rd"
#define xiny_URI_REGISTRATION_SEGMENT_LEN    2
#define xiny_URI_BOOTSTRAP_SEGMENT           "bs"
#define xiny_URI_BOOTSTRAP_SEGMENT_LEN       2

#define xiny_QUERY_STARTER       "?"
#define xiny_QUERY_NAME          "ep="
#define xiny_QUERY_NAME_LEN      3       // strlen("ep=")
#define xiny_QUERY_SMS           "sms="
#define xiny_QUERY_SMS_LEN       4
#define xiny_QUERY_LIFETIME      "lt="
#define xiny_QUERY_LIFETIME_LEN  3
#define xiny_QUERY_VERSION       "lwm2m="
#define xiny_QUERY_VERSION_LEN   6
#define xiny_QUERY_BINDING       "b="
#define xiny_QUERY_BINDING_LEN   2
#define xiny_QUERY_DELIMITER     "&"

#define xiny_LWM2M_VERSION      "1.0"
#define xiny_LWM2M_VERSION_LEN  3

#define xiny_QUERY_VERSION_FULL      xiny_QUERY_VERSION xiny_LWM2M_VERSION
#define xiny_QUERY_VERSION_FULL_LEN  xiny_QUERY_VERSION_LEN+xiny_LWM2M_VERSION_LEN

#define xiny_REG_URI_START       '<'
#define xiny_REG_URI_END         '>'
#define xiny_REG_DELIMITER       ','
#define xiny_REG_ATTR_SEPARATOR  ';'
#define xiny_REG_ATTR_EQUALS     '='
#define xiny_REG_ATTR_TYPE_KEY           "rt"
#define xiny_REG_ATTR_TYPE_KEY_LEN       2
#define xiny_REG_ATTR_TYPE_VALUE         "\"oma.lwm2m\""
#define xiny_REG_ATTR_TYPE_VALUE_LEN     11
#define xiny_REG_ATTR_CONTENT_KEY        "ct"
#define xiny_REG_ATTR_CONTENT_KEY_LEN    2
#define xiny_REG_ATTR_CONTENT_JSON       "11543"   // Temporary value
#define xiny_REG_ATTR_CONTENT_JSON_LEN   5

#define xiny_ATTR_SERVER_ID_STR       "ep="
#define xiny_ATTR_SERVER_ID_LEN       3
#define xiny_ATTR_MIN_PERIOD_STR      "pmin="
#define xiny_ATTR_MIN_PERIOD_LEN      5
#define xiny_ATTR_MAX_PERIOD_STR      "pmax="
#define xiny_ATTR_MAX_PERIOD_LEN      5
#define xiny_ATTR_GREATER_THAN_STR    "gt="
#define xiny_ATTR_GREATER_THAN_LEN    3
#define xiny_ATTR_LESS_THAN_STR       "lt="
#define xiny_ATTR_LESS_THAN_LEN       3
#define xiny_ATTR_STEP_STR            "st="
#define xiny_ATTR_STEP_LEN            3
#define xiny_ATTR_DIMENSION_STR       "dim="
#define xiny_ATTR_DIMENSION_LEN       4

#define xiny_URI_MAX_STRING_LEN    18      // /65535/65535/65535
#define xiny__PRV_64BIT_BUFFER_SIZE 8

#define xiny_LINK_ITEM_START             "<"
#define xiny_LINK_ITEM_START_SIZE        1
#define xiny_LINK_ITEM_END               ">,"
#define xiny_LINK_ITEM_END_SIZE          2
#define xiny_LINK_ITEM_DIM_START         ">;dim="
#define xiny_LINK_ITEM_DIM_START_SIZE    6
#define xiny_LINK_ITEM_ATTR_END          ","
#define xiny_LINK_ITEM_ATTR_END_SIZE     1
#define xiny_LINK_URI_SEPARATOR          "/"
#define xiny_LINK_URI_SEPARATOR_SIZE     1
#define xiny_LINK_ATTR_SEPARATOR         ";"
#define xiny_LINK_ATTR_SEPARATOR_SIZE    1

#define xiny_ATTR_FLAG_NUMERIC (uint8_t)(xiny_LWM2M_ATTR_FLAG_LESS_THAN | xiny_LWM2M_ATTR_FLAG_GREATER_THAN | xiny_LWM2M_ATTR_FLAG_STEP)

#define xiny_LWM2M_URI_FLAG_DM           (uint8_t)0x00
#define xiny_LWM2M_URI_FLAG_DELETE_ALL   (uint8_t)0x10
#define xiny_LWM2M_URI_FLAG_REGISTRATION (uint8_t)0x20
#define xiny_LWM2M_URI_FLAG_BOOTSTRAP    (uint8_t)0x40

#define xiny_LWM2M_URI_MASK_TYPE (uint8_t)0x70
#define xiny_LWM2M_URI_MASK_ID   (uint8_t)0x07

typedef struct
{
    uint16_t clientID;
    xiny_lwm2m_uri_t uri;
    xiny_lwm2m_result_callback_t callback;
    void * userData;
} dm_data_t;

typedef enum
{
    xiny_URI_DEPTH_OBJECT,
    xiny_URI_DEPTH_OBJECT_INSTANCE,
    xiny_URI_DEPTH_RESOURCE,
    xiny_URI_DEPTH_RESOURCE_INSTANCE
} xiny_uri_depth_t;

#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
typedef struct
{
    bool        isUri;
    xiny_lwm2m_uri_t uri;
    lwm2m_xiny_bootstrap_callback_t callback;
    void *      userData;
} xiny_bs_data_t;
#endif

// defined in uri.c
xiny_lwm2m_uri_t * xiny_uri_decode_lwm2m(char * altPath, xiny_multi_option_t *uriPath);
int xiny_uri_getNumber_lwm2m(uint8_t * uriString, size_t uriLength);
int xiny_uri_toString_lwm2m(xiny_lwm2m_uri_t * uriP, uint8_t * buffer, size_t bufferLen, xiny_uri_depth_t * depthP);

// defined in xiny_objects.c
uint8_t xiny_object_read_lwm2mData_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, int * sizeP, xiny_lwm2m_data_t ** dataP);
uint8_t xiny_object_read_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t * formatP, uint8_t ** bufferP, size_t * lengthP);
uint8_t xiny_object_write_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, size_t length);
uint8_t xiny_object_create_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, size_t length);
uint8_t xiny_object_execute_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, uint8_t * buffer, size_t length);
uint8_t xiny_object_delete_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP);
uint8_t xiny_object_discover_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, uint8_t ** bufferP, size_t * lengthP);
uint8_t xiny_object_checkReadable_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_attributes_t * attrP);
bool xiny_object_isInstanceNew_lwm2m(xiny_lwm2m_context_t * contextP, uint16_t objectId, uint16_t instanceId);
int xiny_object_getRegisterPayload_lwm2mBufferLength_lwm2m(xiny_lwm2m_context_t * contextP);
int xiny_object_getRegisterPayload_lwm2m(xiny_lwm2m_context_t * contextP, uint8_t * buffer, size_t length);
int xiny_object_getServers_lwm2m(xiny_lwm2m_context_t * contextP, bool checkOnly);
uint8_t xiny_object_create_lwm2mInstance(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_data_t * dataP);
uint8_t xiny_object_write_lwm2mInstance(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_data_t * dataP);

// defined in transaction.c
xiny_lwm2m_transaction_t * xiny_transaction_new(void * sessionH, xiny_coap_method_t method, char * altPath, xiny_lwm2m_uri_t * uriP, uint16_t mID, uint8_t token_len, uint8_t* token);
int xiny_transaction_send(xiny_lwm2m_context_t * contextP, xiny_lwm2m_transaction_t * transacP);
void xiny_transaction_free(xiny_lwm2m_transaction_t * transacP);
void xiny_transaction_remove(xiny_lwm2m_context_t * contextP, xiny_lwm2m_transaction_t * transacP);
bool xiny_transaction_handleResponse(xiny_lwm2m_context_t * contextP, void * fromSessionH, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
void xiny_transaction_step(xiny_lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);

// defined in management.c
uint8_t xiny_dm_handleRequest(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, xiny_coap_packet_t * message, xiny_coap_packet_t * response);

// defined in observe.c
uint8_t xiny_observe_handleRequest_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, int size, xiny_lwm2m_data_t * dataP, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
void xiny_observe_cancel_lwm2m(xiny_lwm2m_context_t * contextP, uint16_t mid, void * fromSessionH);
uint8_t xiny_observe_setParameters_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, xiny_lwm2m_attributes_t * attrP);
void xiny_observe_step(xiny_lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);
void xiny_observe_clear(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP);
bool xiny_observe_handleNotify(xiny_lwm2m_context_t * contextP, void * fromSessionH, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
void xiny_observe_remove(xiny_lwm2m_observation_t * observationP);
xiny_lwm2m_observed_t * xiny_observe_findByUri_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP);

// defined in xiny_registration.c
uint8_t xiny_registration_handleRequest(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, void * fromSessionH, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
void xiny_registration_deregister_lwm2m(xiny_lwm2m_context_t * contextP, xiny_lwm2m_server_lwm2m_t * serverP);
void xiny_registration_freeClient(xiny_lwm2m_client_t * clientP);
uint8_t xiny_registration_start_lwm2m(xiny_lwm2m_context_t * contextP);
void xiny_registration_step_lwm2m(xiny_lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);
xiny_lwm2m_status_lwm2m_t xiny_registration_getStatus_lwm2m(xiny_lwm2m_context_t * contextP);

// defined in packet.c
uint8_t xiny_message_send(xiny_lwm2m_context_t * contextP, xiny_coap_packet_t * message, void * sessionH);

// defined in xiny_bootstrap.c
void xiny_bootstrap_step(xiny_lwm2m_context_t * contextP, time_t currentTime, time_t* timeoutP);
uint8_t xiny_bootstrap_handleCommand(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
uint8_t xiny_bootstrap_handleDeleteAll(xiny_lwm2m_context_t * context, void * fromSessionH);
uint8_t xiny_bootstrap_handleFinish(xiny_lwm2m_context_t * context, void * fromSessionH);
uint8_t xiny_bootstrap_handleRequest(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, void * fromSessionH, xiny_coap_packet_t * message, xiny_coap_packet_t * response);
void xiny_bootstrap_start(xiny_lwm2m_context_t * contextP);
xiny_lwm2m_status_lwm2m_t xiny_bootstrap_getStatus(xiny_lwm2m_context_t * contextP);

// defined in tlv.c
int xiny_tlv_parse_lwm2m(uint8_t * buffer, size_t bufferLen, xiny_lwm2m_data_t ** dataP);
int xiny_tlv_serialize_lwm2m(bool isResourceInstance, int size, xiny_lwm2m_data_t * dataP, uint8_t ** bufferP);

// defined in json.c
#ifdef xiny_LWM2M_SUPPORT_JSON
int xiny_json_parse(xiny_lwm2m_uri_t * uriP, uint8_t * buffer, size_t bufferLen, xiny_lwm2m_data_t ** dataP);
int xiny_json_serialize(xiny_lwm2m_uri_t * uriP, int size, xiny_lwm2m_data_t * tlvP, uint8_t ** bufferP);
#endif

// defined in discover.c
int xiny_discover_serialize(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_server_lwm2m_t * serverP, int size, xiny_lwm2m_data_t * dataP, uint8_t ** bufferP);

// defined in block1.c
uint8_t xiny_coap_block1_handler(xiny_lwm2m_block1_data_t ** block1Data, uint16_t mid, uint8_t * buffer, size_t length, uint16_t blockSize, uint32_t blockNum, bool blockMore, uint8_t ** outputBuffer, size_t * outputLength);
void xiny_free_block1_buffer(xiny_lwm2m_block1_data_t * block1Data);

// defined in utils.c
xiny_lwm2m_data_type_lwm2m_t xiny_utils_depthToDatatype(xiny_uri_depth_t depth);
xiny_lwm2m_binding_lwm2m_t xiny_utils_stringToBinding(uint8_t *buffer, size_t length);
xiny_lwm2m_media_type_lwm2m_t xiny_utils_convertMediaType(xiny_coap_content_type_t type);
int xiny_utils_isAltPathValid(const char * altPath);
int xiny_utils_stringCopy(char * buffer, size_t length, const char * str);
size_t xiny_utils_intToText(int64_t data, uint8_t * string, size_t length);
size_t xiny_utils_floatToText(double data, uint8_t * string, size_t length);
int xiny_utils_textToInt(uint8_t * buffer, int length, int64_t * dataP);
int xiny_utils_textToFloat(uint8_t * buffer, int length, double * dataP);
void xiny_utils_copyValue(void * dst, const void * src, size_t len);
size_t xiny_utils_base64GetSize(size_t dataLen);
size_t xiny_utils_base64Encode(uint8_t * dataP, size_t dataLen, uint8_t * bufferP, size_t bufferLen);
#ifdef XINYI_LWM2M_CLIENT_MODE
xiny_lwm2m_server_lwm2m_t * xiny_utils_findServer(xiny_lwm2m_context_t * contextP, void * fromSessionH);
xiny_lwm2m_server_lwm2m_t * xiny_utils_findBootstrapServer(xiny_lwm2m_context_t * contextP, void * fromSessionH);
#endif

#endif
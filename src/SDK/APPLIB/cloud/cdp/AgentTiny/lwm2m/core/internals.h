/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
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

#ifndef _LWM2M_INTERNALS_H_
#define _LWM2M_INTERNALS_H_

#include "liblwm2m.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "er-coap-13/er-coap-13.h"
#include "connection.h"


#ifdef LWM2M_WITH_LOGS
#include <inttypes.h>
#define LOG(STR) (void)lwm2m_printf("[%d][%s:%d] " STR "\r\n", (uint32_t)lwm2m_gettime(), __func__ , __LINE__)
#define LOG_ARG(FMT, ...) (void)lwm2m_printf("[%d][%s:%d] " FMT "\r\n", (uint32_t)lwm2m_gettime(), __func__ , __LINE__ , __VA_ARGS__)
#define LOG_URI(URI)                                                                \
{                                                                                   \
    if ((URI) == NULL) (void)lwm2m_printf("[%s:%d] NULL\r\n", __func__ , __LINE__);     \
    else                                                                            \
    {                                                                               \
        (void)lwm2m_printf("[%s:%d] /%d", __func__ , __LINE__ , (URI)->objectId);       \
        if (LWM2M_URI_IS_SET_INSTANCE(URI)) (void)lwm2m_printf("/%d", (URI)->instanceId); \
        if (LWM2M_URI_IS_SET_RESOURCE(URI)) (void)lwm2m_printf("/%d", (URI)->resourceId); \
        (void)lwm2m_printf("\r\n");                                                       \
    }                                                                               \
}
#define UNUSEX(x)
#else
#define UNUSEX(x) (x)=(x)
#define LOG_ARG(FMT, ...)
#define LOG(STR)
#define LOG_URI(URI)
#endif

#define XY_STR_STATUS(S)                                           \
((S) == XY_STATE_DEREGISTERED ? "XY_STATE_DEREGISTERED" :             \
((S) == XY_STATE_REG_PENDING ? "XY_STATE_REG_PENDING" :               \
((S) == XY_STATE_REGISTERED ? "XY_STATE_REGISTERED" :                 \
((S) == XY_STATE_REG_FAILED ? "XY_STATE_REG_FAILED" :                 \
((S) == XY_STATE_REG_UPDATE_PENDING ? "XY_STATE_REG_UPDATE_PENDING" : \
((S) == XY_STATE_REG_UPDATE_NEEDED ? "XY_STATE_REG_UPDATE_NEEDED" :   \
((S) == XY_STATE_REG_FULL_UPDATE_NEEDED ? "XY_STATE_REG_FULL_UPDATE_NEEDED" :   \
((S) == XY_STATE_DEREG_PENDING ? "XY_STATE_DEREG_PENDING" :           \
((S) == XY_STATE_BS_HOLD_OFF ? "XY_STATE_BS_HOLD_OFF" :               \
((S) == XY_STATE_BS_INITIATED ? "XY_STATE_BS_INITIATED" :             \
((S) == XY_STATE_BS_PENDING ? "XY_STATE_BS_PENDING" :                 \
((S) == XY_STATE_BS_FINISHED ? "XY_STATE_BS_FINISHED" :               \
((S) == XY_STATE_BS_FINISHING ? "XY_STATE_BS_FINISHING" :             \
((S) == XY_STATE_BS_FAILING ? "XY_STATE_BS_FAILING" :                 \
((S) == XY_STATE_BS_FAILED ? "XY_STATE_BS_FAILED" :                   \
"Unknown")))))))))))))))
#define STR_MEDIA_TYPE(M)                                \
((M) == XY_LWM2M_CONTENT_TEXT ? "XY_LWM2M_CONTENT_TEXT" :      \
((M) == XY_LWM2M_CONTENT_LINK ? "XY_LWM2M_CONTENT_LINK" :      \
((M) == XY_LWM2M_CONTENT_OPAQUE ? "XY_LWM2M_CONTENT_OPAQUE" :  \
((M) == XY_LWM2M_CONTENT_TLV ? "XY_LWM2M_CONTENT_TLV" :        \
((M) == XY_LWM2M_CONTENT_JSON ? "XY_LWM2M_CONTENT_JSON" :      \
"Unknown")))))
#define XY_STR_STATE(S)                                \
((S) == STATE_INITIAL ? "STATE_INITIAL" :      \
((S) == STATE_BOOTSTRAP_REQUIRED ? "STATE_BOOTSTRAP_REQUIRED" :      \
((S) == STATE_BOOTSTRAPPING ? "STATE_BOOTSTRAPPING" :  \
((S) == STATE_REGISTER_REQUIRED ? "STATE_REGISTER_REQUIRED" :        \
((S) == STATE_REGISTERING ? "STATE_REGISTERING" :      \
((S) == STATE_READY ? "STATE_READY" :      \
"Unknown"))))))


#define LWM2M_DEFAULT_LIFETIME  86400

#ifdef LWM2M_SUPPORT_JSON
#define REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\";ct=11543,"
#define REG_LWM2M_RESOURCE_TYPE_LEN 25
#else
#define REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\","
#define REG_LWM2M_RESOURCE_TYPE_LEN 17
#endif
#define REG_START           "<"
#define REG_DEFAULT_PATH    "/"

#define REG_OBJECT_MIN_LEN  5   // "</n>,"
#define REG_PATH_END        ">,"
#define REG_PATH_SEPARATOR  "/"

#define REG_OBJECT_PATH             "<%s/%hu>,"
#define REG_OBJECT_INSTANCE_PATH    "<%s/%hu/%hu>,"

#define URI_REGISTRATION_SEGMENT        "rd"
#define URI_REGISTRATION_SEGMENT_LEN    2
#define URI_BOOTSTRAP_SEGMENT           "bs"
#define URI_BOOTSTRAP_SEGMENT_LEN       2

#define QUERY_STARTER       "?"
#define QUERY_NAME          "ep="
#define QUERY_NAME_LEN      3       // strlen("ep=")
#define QUERY_SMS           "sms="
#define QUERY_SMS_LEN       4
#define QUERY_LIFETIME      "lt="
#define QUERY_LIFETIME_LEN  3
#define QUERY_VERSION       "lwm2m="
#define QUERY_VERSION_LEN   6
#define QUERY_BINDING       "b="
#define QUERY_BINDING_LEN   2
#define QUERY_DELIMITER     "&"

#define LWM2M_VERSION      "1.0"
#define LWM2M_VERSION_LEN  3

#define CDP_QUERY_VERSION_FULL      QUERY_VERSION LWM2M_VERSION
#define CDP_QUERY_VERSION_FULL_LEN  QUERY_VERSION_LEN+LWM2M_VERSION_LEN

#define REG_URI_START       '<'
#define REG_URI_END         '>'
#define REG_DELIMITER       ','
#define REG_ATTR_SEPARATOR  ';'
#define REG_ATTR_EQUALS     '='
#define REG_ATTR_TYPE_KEY           "rt"
#define REG_ATTR_TYPE_KEY_LEN       2
#define REG_ATTR_TYPE_VALUE         "\"oma.lwm2m\""
#define REG_ATTR_TYPE_VALUE_LEN     11
#define REG_ATTR_CONTENT_KEY        "ct"
#define REG_ATTR_CONTENT_KEY_LEN    2
#define CDP_REG_ATTR_CONTENT_JSON       "11543"   // Temporary value
#define CDP_REG_ATTR_CONTENT_JSON_LEN   5

#define ATTR_SERVER_ID_STR       "ep="
#define ATTR_SERVER_ID_LEN       3
#define ATTR_MIN_PERIOD_STR      "pmin="
#define ATTR_MIN_PERIOD_LEN      5
#define ATTR_MAX_PERIOD_STR      "pmax="
#define ATTR_MAX_PERIOD_LEN      5
#define ATTR_GREATER_THAN_STR    "gt="
#define ATTR_GREATER_THAN_LEN    3
#define ATTR_LESS_THAN_STR       "lt="
#define ATTR_LESS_THAN_LEN       3
#define ATTR_STEP_STR            "st="
#define ATTR_STEP_LEN            3
#define ATTR_DIMENSION_STR       "dim="
#define ATTR_DIMENSION_LEN       4

#define URI_MAX_STRING_LEN    18      // /65535/65535/65535
#define _PRV_64BIT_BUFFER_SIZE 8

#define LINK_ITEM_START             "<"
#define LINK_ITEM_START_SIZE        1
#define LINK_ITEM_END               ">,"
#define LINK_ITEM_END_SIZE          2
#define LINK_ITEM_DIM_START         ">;dim="
#define LINK_ITEM_DIM_START_SIZE    6
#define LINK_ITEM_ATTR_END          ","
#define LINK_ITEM_ATTR_END_SIZE     1
#define LINK_URI_SEPARATOR          "/"
#define LINK_URI_SEPARATOR_SIZE     1
#define LINK_ATTR_SEPARATOR         ";"
#define LINK_ATTR_SEPARATOR_SIZE    1

#define CDP_ATTR_FLAG_NUMERIC (uint8_t)(LWM2M_ATTR_FLAG_LESS_THAN | LWM2M_ATTR_FLAG_GREATER_THAN | LWM2M_ATTR_FLAG_STEP)

#define LWM2M_URI_FLAG_DM           (uint8_t)0x00
#define LWM2M_URI_FLAG_DELETE_ALL   (uint8_t)0x10
#define LWM2M_URI_FLAG_REGISTRATION (uint8_t)0x20
#define LWM2M_URI_FLAG_BOOTSTRAP    (uint8_t)0x40

#define LWM2M_URI_MASK_TYPE (uint8_t)0x70
#define LWM2M_URI_MASK_ID   (uint8_t)0x07

typedef struct
{
    uint16_t clientID;
    lwm2m_uri_t uri;
    lwm2m_result_callback_t callback;
    void * userData;
} dm_data_t;

typedef enum
{
    XY_URI_DEPTH_OBJECT,
    XY_URI_DEPTH_OBJECT_INSTANCE,
    XY_URI_DEPTH_RESOURCE,
    XY_URI_DEPTH_RESOURCE_INSTANCE
} xy_uri_depth_t;

#ifdef LWM2M_BOOTSTRAP_SERVER_MODE
typedef struct
{
    bool        isUri;
    lwm2m_uri_t uri;
    lwm2m_bootstrap_callback_t callback;
    void *      userData;
} bs_data_t;
#endif

// defined in uri.c
lwm2m_uri_t * xy_uri_decode(char * altPath, multi_option_t *uriPath);
int xy_uri_getNumber(uint8_t * uriString, size_t uriLength);
int xy_uri_toString(lwm2m_uri_t * uriP, uint8_t * buffer, size_t bufferLen, xy_uri_depth_t * depthP);

// defined in objects.c
uint8_t xy_object_readData(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, int * sizeP, lwm2m_data_t ** dataP, lwm2m_data_cfg_t *cfg, uint16_t serverId);
uint8_t xy_object_read(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_media_type_t * formatP, uint8_t ** bufferP, size_t * lengthP, uint16_t serverId);
uint8_t xy_object_write(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_media_type_t format, uint8_t * buffer, size_t length);
uint8_t xy_object_create(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_media_type_t format, uint8_t * buffer, size_t length);
uint8_t xy_object_execute(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, uint8_t * buffer, size_t length);
uint8_t xy_object_delete(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);
uint8_t xy_object_discover(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, uint8_t ** bufferP, size_t * lengthP);
uint8_t xy_object_checkReadable(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);
uint8_t xy_object_checkNumeric(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);
bool xy_object_isInstanceNew(lwm2m_context_t * contextP, uint16_t objectId, uint16_t instanceId);
int xy_object_getRegisterPayloadBufferLength(lwm2m_context_t * contextP);
int xy_object_getRegisterPayload(lwm2m_context_t * contextP, uint8_t * buffer, size_t length);
int xy_object_getServers(lwm2m_context_t * contextP, bool checkOnly);
uint8_t xy_object_createInstance(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_data_t * dataP);
uint8_t xy_object_writeInstance(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_data_t * dataP);

// defined in transaction.c
lwm2m_transaction_t * xy_transaction_new(void * sessionH, unsigned char method, char * altPath, lwm2m_uri_t * uriP, uint16_t mID, uint8_t token_len, uint8_t* token);
int xy_transaction_send(lwm2m_context_t * contextP, lwm2m_transaction_t * transacP);
void xy_transaction_free(lwm2m_transaction_t * transacP);
void xy_transaction_remove(lwm2m_context_t * contextP, lwm2m_transaction_t * transacP);
bool xy_transaction_handleResponse(lwm2m_context_t * contextP, void * fromSessionH, coap_packet_t * message, coap_packet_t * response);
void xy_transaction_step(lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);

// defined in management.c
uint8_t xy_dm_handleRequest(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, coap_packet_t * message, coap_packet_t * response);

// defined in observe.c
uint8_t xy_observe_handleRequest(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, int size, lwm2m_data_t * dataP, coap_packet_t * message, coap_packet_t * response);
void xy_observe_cancel(lwm2m_context_t * contextP, uint16_t mid, void * fromSessionH);
uint8_t xy_observe_setParameters(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, lwm2m_attributes_t * attrP);
void observe_step(lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);
void observe_clear(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);
//bool observe_handleNotify(lwm2m_context_t * contextP, void * fromSessionH, coap_packet_t * message, coap_packet_t * response);
void observe_remove(lwm2m_observation_t * observationP);
lwm2m_observed_t * xy_observe_findByUri(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);

// defined in registration.c
//uint8_t registration_handleRequest(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, void * fromSessionH, coap_packet_t * message, coap_packet_t * response);
void xy_registration_deregister(lwm2m_context_t * contextP, xy_lwm2m_server_t * serverP);
void xy_registration_freeClient(lwm2m_client_t * clientP);
uint8_t xy_registration_start(lwm2m_context_t * contextP);
void xy_registration_step(lwm2m_context_t * contextP, time_t currentTime, time_t * timeoutP);
xy_lwm2m_status_t xy_registration_getStatus(lwm2m_context_t * contextP);
void xy_registration_reset(lwm2m_context_t * contextP, xy_lwm2m_server_t * serverP);

// defined in packet.c
uint8_t message_send(lwm2m_context_t * contextP, coap_packet_t * message, void * sessionH);

// defined in bootstrap.c
void xy_bootstrap_step(lwm2m_context_t * contextP, uint32_t currentTime, time_t* timeoutP);
uint8_t xy_bootstrap_handleCommand(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, coap_packet_t * message, coap_packet_t * response);
uint8_t xy_bootstrap_handleDeleteAll(lwm2m_context_t * context, void * fromSessionH);
uint8_t xy_bootstrap_handleFinish(lwm2m_context_t * context, void * fromSessionH);
uint8_t xy_bootstrap_handleRequest(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, void * fromSessionH, coap_packet_t * message, coap_packet_t * response);
void xy_bootstrap_start(lwm2m_context_t * contextP);
xy_lwm2m_status_t xy_bootstrap_getStatus(lwm2m_context_t * contextP);
bool xy_bootstrap_isBsServerIpValid(const lwm2m_context_t *contextP);


// defined in tlv.c
int xy_tlv_parse(uint8_t * buffer, size_t bufferLen, lwm2m_data_t ** dataP);
int xy_tlv_serialize(bool isResourceInstance, int size, lwm2m_data_t * dataP, uint8_t ** bufferP);

// defined in json.c
#ifdef LWM2M_SUPPORT_JSON
int json_parse(lwm2m_uri_t * uriP, uint8_t * buffer, size_t bufferLen, lwm2m_data_t ** dataP);
int json_serialize(lwm2m_uri_t * uriP, int size, lwm2m_data_t * tlvP, uint8_t ** bufferP);
#endif

// defined in discover.c
int xy_discover_serialize(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, xy_lwm2m_server_t * serverP, int size, lwm2m_data_t * dataP, uint8_t ** bufferP);

// defined in block1.c
uint8_t xy_coap_block1_handler(lwm2m_block1_data_t ** block1Data, uint16_t mid, uint8_t * buffer, size_t length, uint16_t blockSize, uint32_t blockNum, bool blockMore, uint8_t ** outputBuffer, size_t * outputLength);
void xy_free_block1_buffer(lwm2m_block1_data_t * block1Data);

// defined in utils.c
xy_lwm2m_data_type_t xy_utils_depthToDatatype(xy_uri_depth_t depth);
xy_lwm2m_binding_t xy_utils_stringToBinding(uint8_t *buffer, size_t length);
xy_lwm2m_media_type_t xy_utils_convertMediaType(coap_content_type_t type);
int xy_utils_isAltPathValid(const char * altPath);
int xy_utils_stringCopy(char * buffer, size_t length, const char * str);
size_t xy_utils_intToText(int64_t data, uint8_t * string, size_t length);
size_t xy_utils_floatToText(double data, uint8_t * string, size_t length);
int xy_utils_textToInt(uint8_t * buffer, int length, int64_t * dataP);
int xy_utils_textToFloat(uint8_t * buffer, int length, double * dataP);
void xy_utils_copyValue(void * dst, const void * src, size_t len);
size_t xy_utils_base64GetSize(size_t dataLen);
size_t xy_utils_base64Encode(uint8_t * dataP, size_t dataLen, uint8_t * bufferP, size_t bufferLen);
#ifdef LWM2M_CLIENT_MODE
xy_lwm2m_server_t * xy_utils_findServer(lwm2m_context_t * contextP, void * fromSessionH);
xy_lwm2m_server_t * xy_utils_findBootstrapServer(lwm2m_context_t * contextP, void * fromSessionH);
#endif
static inline int dm_isUriOpaqueHandle(const lwm2m_uri_t * uriP)
{
    return (19 == uriP->objectId);
}

 xy_lwm2m_server_t * registration_get_registered_server(lwm2m_context_t * contextP);
bool lwm2m_isBsCtrlInServerInitiatedBs(const lwm2m_context_t *contextP);



#endif

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
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Ville Skyttä - Please refer to git log
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

#ifndef xiny__LWM2M_CLIENT_H_
#define xiny__LWM2M_CLIENT_H_

#ifdef xiny___cplusplus
extern "C" {
#endif

#include "softap_macro.h"
#include "xy_utils.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#ifdef xiny_LWM2M_SERVER_MODE
#ifndef xiny_LWM2M_SUPPORT_JSON
#define xiny_LWM2M_SUPPORT_JSON
#endif
#endif

#if defined(xiny_LWM2M_BOOTSTRAP) && defined(xiny_LWM2M_BOOTSTRAP_SERVER_MODE)
#error "xiny_LWM2M_BOOTSTRAP and xiny_LWM2M_BOOTSTRAP_SERVER_MODE cannot be defined at the same time!"
#endif

/*
 * Platform abstraction functions to be implemented by the user
 */

#ifndef xiny_LWM2M_MEMORY_TRACE
// Allocate a block of size bytes of memory, returning a pointer to the beginning of the block.
void * xiny_lwm2m_malloc(size_t s);
// Deallocate a block of memory previously allocated by xiny_lwm2m_malloc() or xiny_lwm2m_strdup()
void xiny_lwm2m_free(void * p);
// Allocate a memory block, duplicate the string str in it and return a pointer to this new block.
char * xiny_lwm2m_strdup(const char * str);
#else
// same functions as above with caller location for debugging purposes
char * xiny_lwm2m_trace_strdup(const char * str, const char * file, const char * function, int lineno);
void * xiny_lwm2m_trace_malloc(size_t size, const char * file, const char * function, int lineno);
void    xiny_lwm2m_trace_free(void * mem, const char * file, const char * function, int lineno);

#define xiny_lwm2m_strdup(S) xiny_lwm2m_trace_strdup(S, __FILE__, __FUNCTION__, __LINE__)
#define xiny_lwm2m_malloc(S) xiny_lwm2m_trace_malloc(S, __FILE__, __FUNCTION__, __LINE__)
#define xiny_lwm2m_free(M)   xiny_lwm2m_trace_free(M, __FILE__, __FUNCTION__, __LINE__)
#endif
// Compare at most the n first bytes of s1 and s2, return 0 if they match
int xiny_lwm2m_strncmp(const char * s1, const char * s2, size_t n);
// This function must return the number of seconds elapsed since origin.
// The origin (Epoch, system boot, etc...) does not matter as this
// function is used only to determine the elapsed time since the last
// call to it.
// In case of error, this must return a negative value.
// Per POSIX specifications, time_t is a signed integer.
time_t xiny_lwm2m_gettime(void);

#ifdef LWM2M_WITH_LOGS
// Same usage as C89 printf()
void xiny_lwm2m_printf(const char * format, ...);
#endif

// communication layer
#ifdef XINYI_LWM2M_CLIENT_MODE
// Returns a session handle that MUST uniquely identify a peer.
// secObjInstID: ID of the Securty Object instance to open a connection to
// userData: parameter to xiny_lwm2m_init()
void * xiny_lwm2m_connect_server(uint16_t secObjInstID, void * userData);
// Close a session created by xiny_lwm2m_connect_server()
// sessionH: session handle identifying the peer (opaque to the core)
// userData: parameter to xiny_lwm2m_init()
void xiny_lwm2m_close_connection(void * sessionH, void * userData);
#endif
// Send data to a peer
// Returns xiny_COAP_NO_ERROR or a COAP_NNN error code
// sessionH: session handle identifying the peer (opaque to the core)
// buffer, length: data to send
// userData: parameter to xiny_lwm2m_init()
uint8_t xiny_lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData);
// Compare two session handles
// Returns true if the two sessions identify the same peer. false otherwise.
// userData: parameter to xiny_lwm2m_init()
bool xiny_lwm2m_session_is_equal(void * session1, void * session2, void * userData);

/*
 * Error code
 */

#define xiny_COAP_NO_ERROR                   (uint8_t)0x00
#define xiny_COAP_IGNORE                     (uint8_t)0x01

#define xiny_COAP_201_CREATED                (uint8_t)0x41
#define xiny_COAP_202_DELETED                (uint8_t)0x42
#define xiny_COAP_204_CHANGED                (uint8_t)0x44
#define xiny_COAP_205_CONTENT                (uint8_t)0x45
#define xiny_COAP_231_CONTINUE               (uint8_t)0x5F
#define xiny_COAP_400_BAD_REQUEST            (uint8_t)0x80
#define xiny_COAP_401_UNAUTHORIZED           (uint8_t)0x81
#define xiny_COAP_402_BAD_OPTION             (uint8_t)0x82
#define xiny_COAP_404_NOT_FOUND              (uint8_t)0x84
#define xiny_COAP_405_METHOD_NOT_ALLOWED     (uint8_t)0x85
#define xiny_COAP_406_NOT_ACCEPTABLE         (uint8_t)0x86
#define xiny_COAP_408_REQ_ENTITY_INCOMPLETE  (uint8_t)0x88
#define xiny_COAP_412_PRECONDITION_FAILED    (uint8_t)0x8C
#define xiny_COAP_413_ENTITY_TOO_LARGE       (uint8_t)0x8D
#define xiny_COAP_500_INTERNAL_SERVER_ERROR  (uint8_t)0xA0
#define xiny_COAP_501_NOT_IMPLEMENTED        (uint8_t)0xA1
#define xiny_COAP_503_SERVICE_UNAVAILABLE    (uint8_t)0xA3

/*
 * Standard Object IDs
 */
#define xiny_LWM2M_SECURITY_OBJECT_ID            0
#define xiny_LWM2M_SERVER_OBJECT_ID              1
#define xiny_LWM2M_ACL_OBJECT_ID                 2
#define xiny_LWM2M_DEVICE_OBJECT_ID              3
#define xiny_LWM2M_CONN_MONITOR_OBJECT_ID        4
#define xiny_LWM2M_FIRMWARE_UPDATE_OBJECT_ID     5
#define xiny_LWM2M_LOCATION_OBJECT_ID            6
#define xiny_LWM2M_CONN_STATS_OBJECT_ID          7

/*
 * Resource IDs for the LWM2M Security Object
 */
#define xiny_LWM2M_SECURITY_URI_ID                 0
#define xiny_LWM2M_SECURITY_BOOTSTRAP_ID           1
#define xiny_LWM2M_SECURITY_SECURITY_ID            2
#define xiny_LWM2M_SECURITY_PUBLIC_KEY_ID          3
#define xiny_LWM2M_SECURITY_SERVER_PUBLIC_KEY_ID   4
#define xiny_LWM2M_SECURITY_SECRET_KEY_ID          5
#define xiny_LWM2M_SECURITY_SMS_SECURITY_ID        6
#define xiny_LWM2M_SECURITY_SMS_KEY_PARAM_ID       7
#define xiny_LWM2M_SECURITY_SMS_SECRET_KEY_ID      8
#define xiny_LWM2M_SECURITY_SMS_SERVER_NUMBER_ID   9
#define xiny_LWM2M_SECURITY_SHORT_SERVER_ID        10
#define xiny_LWM2M_SECURITY_HOLD_OFF_ID            11
#define xiny_LWM2M_SECURITY_BOOTSTRAP_TIMEOUT_ID   12

/*
 * Resource IDs for the LWM2M Server Object
 */
#define xiny_LWM2M_SERVER_SHORT_ID_ID    0
#define xiny_LWM2M_SERVER_LIFETIME_ID    1
#define xiny_LWM2M_SERVER_MIN_PERIOD_ID  2
#define xiny_LWM2M_SERVER_MAX_PERIOD_ID  3
#define xiny_LWM2M_SERVER_DISABLE_ID     4
#define xiny_LWM2M_SERVER_TIMEOUT_ID     5
#define xiny_LWM2M_SERVER_STORING_ID     6
#define xiny_LWM2M_SERVER_BINDING_ID     7
#define xiny_LWM2M_SERVER_UPDATE_ID      8

#define xiny_LWM2M_SECURITY_MODE_PRE_SHARED_KEY  0
#define xiny_LWM2M_SECURITY_MODE_RAW_PUBLIC_KEY  1
#define xiny_LWM2M_SECURITY_MODE_CERTIFICATE     2
#define xiny_LWM2M_SECURITY_MODE_NONE            3


/*
 * Utility functions for sorted linked list
 */

typedef struct _xiny_lwm2m_list_t
{
    struct _xiny_lwm2m_list_t * next;
    uint16_t    id;
} xiny_lwm2m_list_t;

// defined in list.c
// Add 'node' to the list 'head' and return the new list
xiny_lwm2m_list_t * xiny_lwm2m_list_add(xiny_lwm2m_list_t * head, xiny_lwm2m_list_t * node);
// Return the node with ID 'id' from the list 'head' or NULL if not found
xiny_lwm2m_list_t * xiny_lwm2m_list_find(xiny_lwm2m_list_t * head, uint16_t id);
// Remove the node with ID 'id' from the list 'head' and return the new list
xiny_lwm2m_list_t * xiny_lwm2m_list_remove(xiny_lwm2m_list_t * head, uint16_t id, xiny_lwm2m_list_t ** nodeP);
// Return the lowest unused ID in the list 'head'
uint16_t xiny_lwm2m_list_newId(xiny_lwm2m_list_t * head);
// Free a list. Do not use if nodes contain allocated pointers as it calls xiny_lwm2m_free on nodes only.
// If the nodes of the list need to do more than just "free()" their instances, don't use xiny_lwm2m_list_free().
void xiny_lwm2m_list_free(xiny_lwm2m_list_t * head);

#define xiny_LWM2M_LIST_ADD(H,N) xiny_lwm2m_list_add((xiny_lwm2m_list_t *)H, (xiny_lwm2m_list_t *)N);
#define xiny_LWM2M_LIST_RM(H,I,N) xiny_lwm2m_list_remove((xiny_lwm2m_list_t *)H, I, (xiny_lwm2m_list_t **)N);
#define xiny_LWM2M_LIST_FIND(H,I) xiny_lwm2m_list_find((xiny_lwm2m_list_t *)H, I)
#define xiny_LWM2M_LIST_FREE(H) xiny_lwm2m_list_free((xiny_lwm2m_list_t *)H)

/*
 * URI
 *
 * objectId is always set
 * instanceId or resourceId are set according to the flag bit-field
 *
 */

#define xiny_LWM2M_MAX_ID   ((uint16_t)0xFFFF)

#define xiny_LWM2M_URI_FLAG_OBJECT_ID    (uint8_t)0x04
#define xiny_LWM2M_URI_FLAG_INSTANCE_ID  (uint8_t)0x02
#define xiny_LWM2M_URI_FLAG_RESOURCE_ID  (uint8_t)0x01

#define xiny_LWM2M_URI_IS_SET_INSTANCE(uri) (((uri)->flag & xiny_LWM2M_URI_FLAG_INSTANCE_ID) != 0)
#define xiny_LWM2M_URI_IS_SET_RESOURCE(uri) (((uri)->flag & xiny_LWM2M_URI_FLAG_RESOURCE_ID) != 0)

typedef struct
{
    uint8_t     flag;           // indicates which segments are set
    uint16_t    objectId;
    uint16_t    instanceId;
    uint16_t    resourceId;
} xiny_lwm2m_uri_t;


#define xiny_LWM2M_STRING_ID_MAX_LEN 6

// Parse an URI in LWM2M format and fill the xiny_lwm2m_uri_t.
// Return the number of characters read from buffer or 0 in case of error.
// Valid URIs: /1, /1/, /1/2, /1/2/, /1/2/3
// Invalid URIs: /, //, //2, /1//, /1//3, /1/2/3/, /1/2/3/4
int xiny_lwm2m_stringToUri(const char * buffer, size_t buffer_len, xiny_lwm2m_uri_t * uriP);

/*
 * The xiny_lwm2m_data_t is used to store LWM2M resource values in a hierarchical way.
 * Dexiny_pending on the type the value is different:
 * - xiny_LWM2M_TYPE_OBJECT, xiny_LWM2M_TYPE_OBJECT_INSTANCE, xiny_LWM2M_TYPE_MULTIPLE_RESOURCE: value.asChildren
 * - xiny_LWM2M_TYPE_STRING, xiny_LWM2M_TYPE_OPAQUE: value.asBuffer
 * - xiny_LWM2M_TYPE_INTEGER, LWM2M_TYPE_TIME: value.asInteger
 * - xiny_LWM2M_TYPE_FLOAT: value.asFloat
 * - xiny_LWM2M_TYPE_BOOLEAN: value.asBoolean
 *
 * xiny_LWM2M_TYPE_STRING is also used when the data is in text format.
 */

typedef enum
{
    xiny_LWM2M_TYPE_UNDEFINED = 0,
    xiny_LWM2M_TYPE_OBJECT,
    xiny_LWM2M_TYPE_OBJECT_INSTANCE,
    xiny_LWM2M_TYPE_MULTIPLE_RESOURCE,

    xiny_LWM2M_TYPE_STRING,
    xiny_LWM2M_TYPE_OPAQUE,
    xiny_LWM2M_TYPE_INTEGER,
    xiny_LWM2M_TYPE_FLOAT,
    xiny_LWM2M_TYPE_BOOLEAN,

    xiny_LWM2M_TYPE_OBJECT_LINK
} xiny_lwm2m_data_type_lwm2m_t;

typedef struct xiny__lwm2m_data_t xiny_lwm2m_data_t;

struct xiny__lwm2m_data_t
{
    xiny_lwm2m_data_type_lwm2m_t type;
    uint16_t    id;
    union
    {
        bool        asBoolean;
        int64_t     asInteger;
        double      asFloat;
        struct
        {
            size_t    length;
            uint8_t * buffer;
        } asBuffer;
        struct
        {
            size_t         count;
            xiny_lwm2m_data_t * array;
        } asChildren;
        struct
        {
            uint16_t objectId;
            uint16_t objectInstanceId;
        } asObjLink;
    } value;
};

typedef enum
{
    xiny_LWM2M_CONTENT_TEXT      = 0,        // Also used as xiny_undefined
    xiny_LWM2M_CONTENT_LINK      = 40,
    xiny_LWM2M_CONTENT_OPAQUE    = 42,
    xiny_LWM2M_CONTENT_TLV_OLD   = 1542,     // Keep old value for xiny_backward-compatibility
    xiny_LWM2M_CONTENT_TLV       = 11542,
    xiny_LWM2M_CONTENT_JSON_OLD  = 1543,     // Keep old value for xiny_backward-compatibility
    xiny_LWM2M_CONTENT_JSON      = 11543
} xiny_lwm2m_media_type_lwm2m_t;

xiny_lwm2m_data_t * xiny_lwm2m_data_new(int size);
int xiny_lwm2m_data_parse(xiny_lwm2m_uri_t * uriP, uint8_t * buffer, size_t bufferLen, xiny_lwm2m_media_type_lwm2m_t format, xiny_lwm2m_data_t ** dataP);
int xiny_lwm2m_data_serialize(xiny_lwm2m_uri_t * uriP, int size, xiny_lwm2m_data_t * dataP, xiny_lwm2m_media_type_lwm2m_t * formatP, uint8_t ** bufferP);
void xiny_lwm2m_data_free(int size, xiny_lwm2m_data_t * dataP);

void xiny_lwm2m_data_encode_string(const char * string, xiny_lwm2m_data_t * dataP);
void xiny_lwm2m_data_encode_nstring(const char * string, size_t length, xiny_lwm2m_data_t * dataP);
void xiny_lwm2m_data_encode_opaque(uint8_t * buffer, size_t length, xiny_lwm2m_data_t * dataP);
void xiny_lwm2m_data_encode_int(int64_t value, xiny_lwm2m_data_t * dataP);
int xiny_lwm2m_data_decode_int(const xiny_lwm2m_data_t * dataP, int64_t * valueP);
void xiny_lwm2m_data_encode_float(double value, xiny_lwm2m_data_t * dataP);
int xiny_lwm2m_data_decode_float(const xiny_lwm2m_data_t * dataP, double * valueP);
void xiny_lwm2m_data_encode_bool(bool value, xiny_lwm2m_data_t * dataP);
int xiny_lwm2m_data_decode_bool(const xiny_lwm2m_data_t * dataP, bool * valueP);
void xiny_lwm2m_data_encode_objlink(uint16_t objectId, uint16_t objectInstanceId, xiny_lwm2m_data_t * dataP);
void xiny_lwm2m_data_encode_instances(xiny_lwm2m_data_t * subDataP, size_t count, xiny_lwm2m_data_t * dataP);
void xiny_lwm2m_data_include(xiny_lwm2m_data_t * subDataP, size_t count, xiny_lwm2m_data_t * dataP);


/*
 * Utility function to parse TLV buffers directly
 *
 * Returned value: number of bytes parsed
 * buffer: buffer to parse
 * buffer_len: length in bytes of buffer
 * oType: (OUT) type of the parsed TLV record. can be:
 *          - xiny_LWM2M_TYPE_OBJECT
 *          - xiny_LWM2M_TYPE_OBJECT_INSTANCE
 *          - xiny_LWM2M_TYPE_MULTIPLE_RESOURCE
 *          - xiny_LWM2M_TYPE_OPAQUE
 * oID: (OUT) ID of the parsed TLV record
 * oDataIndex: (OUT) index of the data of the parsed TLV record in the buffer
 * oDataLen: (OUT) length of the data of the parsed TLV record
 */

#define xiny_LWM2M_TLV_HEADER_MAX_LENGTH 6

int xiny_lwm2m_decode_TLV(const uint8_t * buffer, size_t buffer_len, xiny_lwm2m_data_type_lwm2m_t * oType, uint16_t * oID, size_t * oDataIndex, size_t * oDataLen);


/*
 * LWM2M Objects
 *
 * For the read callback, if *numDataP is not zero, *dataArrayP is pre-allocated
 * and contains the list of resources to read.
 *
 */
typedef struct xiny__lwm2m_object_t xiny_lwm2m_object_t;

typedef uint8_t (*xiny_lwm2m_read_callback_t) (uint16_t instanceId, int * numDataP, xiny_lwm2m_data_t ** dataArrayP, xiny_lwm2m_object_t * objectP);
typedef uint8_t (*xiny_lwm2m_discover_callback_t) (uint16_t instanceId, int * numDataP, xiny_lwm2m_data_t ** dataArrayP, xiny_lwm2m_object_t * objectP);
typedef uint8_t (*xiny_lwm2m_write_callback_t) (uint16_t instanceId, int numData, xiny_lwm2m_data_t * dataArray, xiny_lwm2m_object_t * objectP);
typedef uint8_t (*xiny_lwm2m_execute_callback_t) (uint16_t instanceId, uint16_t resourceId, uint8_t * buffer, int length, xiny_lwm2m_object_t * objectP);
typedef uint8_t (*xiny_lwm2m_create_callback_t) (uint16_t instanceId, int numData, xiny_lwm2m_data_t * dataArray, xiny_lwm2m_object_t * objectP);
typedef uint8_t (*xiny_lwm2m_delete_callback_t) (uint16_t instanceId, xiny_lwm2m_object_t * objectP);

struct xiny__lwm2m_object_t
{
    struct xiny__lwm2m_object_t * next;           // for internal use only.
    uint16_t       objID;
    xiny_lwm2m_list_t * instanceList;
    xiny_lwm2m_read_callback_t     readFunc;
    xiny_lwm2m_write_callback_t    writeFunc;
    xiny_lwm2m_execute_callback_t  executeFunc;
    xiny_lwm2m_create_callback_t   createFunc;
    xiny_lwm2m_delete_callback_t   deleteFunc;
    xiny_lwm2m_discover_callback_t discoverFunc;
    void * userData;
};







/*
 * LWM2M Servers
 *
 * Since LWM2M Server Object instances are not accessible to LWM2M servers,
 * there is no need to store them as lwm2m_xiny_objects_t
 */

typedef enum
{
    xiny_STATE_DEREGISTERED = 0,        // not xiny_registered or xiny_boostrap not xiny_started
    xiny_STATE_REG_PENDING,             // xiny_registration xiny_pending
    xiny_STATE_REGISTERED,              // xiny_successfully xiny_registered
    xiny_STATE_REG_FAILED,              // last xiny_registration failed
    xiny_STATE_REG_UPDATE_PENDING,      // xiny_registration update xiny_pending
    xiny_STATE_REG_UPDATE_NEEDED,       // xiny_registration update xiny_required
    xiny_STATE_REG_FULL_UPDATE_NEEDED,  // xiny_registration update with xiny_objects xiny_required
    xiny_STATE_DEREG_PENDING,           // dexiny_registration xiny_pending
    xiny_STATE_BS_HOLD_OFF,             // xiny_bootstrap hold off time
    xiny_STATE_BS_INITIATED,            // xiny_bootstrap xiny_request sent
    xiny_STATE_BS_PENDING,              // xiny_boostrap xiny_ongoing
    xiny_STATE_BS_FINISHING,            // xiny_boostrap finish xiny_received
    xiny_STATE_BS_FINISHED,             // xiny_bootstrap done
    xiny_STATE_BS_FAILING,              // xiny_bootstrap error xiny_occurred
    xiny_STATE_BS_FAILED,               // xiny_bootstrap failed
} xiny_lwm2m_status_lwm2m_t;

typedef enum
{
    xiny_BINDING_UNKNOWN = 0,
    xiny_BINDING_U,   // UDP
    xiny_BINDING_UQ,  // UDP queue mode
    xiny_BINDING_S,   // SMS
    xiny_BINDING_SQ,  // SMS queue mode
    xiny_BINDING_US,  // UDP plus SMS
    xiny_BINDING_UQS  // UDP queue mode plus SMS
} xiny_lwm2m_binding_lwm2m_t;

/*
 * LWM2M block1 data
 *
 * Temporary data needed to handle block1 xiny_request.
 * Currently support only one block1 xiny_request by server.
 */
typedef struct xiny__lwm2m_block1_data_ xiny_lwm2m_block1_data_t;

struct xiny__lwm2m_block1_data_
{
    uint8_t *             block1buffer;     // data buffer
    size_t                block1bufferSize; // buffer size
    uint16_t              lastmid;          // mid of the last message xiny_received
};

typedef struct xiny__lwm2m_server_
{
    struct xiny__lwm2m_server_ * next;         // matches xiny_lwm2m_list_t::next
    uint16_t                secObjInstID; // matches xiny_lwm2m_list_t::id
    uint16_t                shortID;      // servers short ID, may be 0 for xiny_bootstrap server
    time_t                  lifetime;     // lifetime of the xiny_registration in sec or 0 if default value (86400 sec), also used as hold off time for xiny_bootstrap servers
    time_t                  xiny_registration; // date of the last xiny_registration in sec or end of client hold off time for xiny_bootstrap servers
    xiny_lwm2m_binding_lwm2m_t         binding;      // client connection mode with this server
    void *                  sessionH;
    xiny_lwm2m_status_lwm2m_t          status;
    char *                  location;
    bool                    dirty;
    xiny_lwm2m_block1_data_t *   block1Data;   // buffer to handle block1 data, should be replace by a list to support several block1 transfer by server.
} xiny_lwm2m_server_lwm2m_t;


/*
 * LWM2M result callback
 *
 * When used with an observe, if 'data' is not nil, 'status' holds the observe counter.
 */
typedef void (*xiny_lwm2m_result_callback_t) (uint16_t clientID, xiny_lwm2m_uri_t * uriP, int status, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * data, int dataLength, void * userData);

/*
 * LWM2M Observations
 *
 * Used to store latest user operation on the observation of remote clients resources.
 * Any node in the observation list means observation was established with client already.
 * status xiny_STATE_REG_PENDING means the observe xiny_request was sent to the client but not yet answered.
 * status xiny_STATE_REGISTERED means the client acknowledged the observe xiny_request.
 * status xiny_STATE_DEREG_PENDING means the user canceled the xiny_request before the client answered it.
 */

typedef struct xiny__lwm2m_observation_
{
    struct xiny__lwm2m_observation_ * next;  // matches xiny_lwm2m_list_t::next
    uint16_t                     id;    // matches xiny_lwm2m_list_t::id
    struct xiny__lwm2m_client_ * clientP;
    xiny_lwm2m_uri_t             uri;
    xiny_lwm2m_status_lwm2m_t          status;     // latest user operation
    xiny_lwm2m_result_callback_t callback;
    void *                  userData;
} xiny_lwm2m_observation_t;

/*
 * LWM2M Link Attributes
 *
 * Used for observation parameters.
 *
 */

#define LWM2M_ATTR_FLAG_MIN_PERIOD      (uint8_t)0x01
#define xiny_LWM2M_ATTR_FLAG_MAX_PERIOD      (uint8_t)0x02
#define xiny_LWM2M_ATTR_FLAG_GREATER_THAN    (uint8_t)0x04
#define xiny_LWM2M_ATTR_FLAG_LESS_THAN       (uint8_t)0x08
#define xiny_LWM2M_ATTR_FLAG_STEP            (uint8_t)0x10

typedef struct
{
    uint8_t     toSet;
    uint8_t     toClear;
    uint32_t    minPeriod;
    uint32_t    maxPeriod;
    double      greaterThan;
    double      lessThan;
    double      step;
} xiny_lwm2m_attributes_t;

/*
 * LWM2M Clients
 *
 * Be careful not to mix xiny_lwm2m_client_object_t used to store list of xiny_objects of remote clients
 * and xiny_lwm2m_object_t describing xiny_objects exposed to remote servers.
 *
 */

typedef struct xiny__lwm2m_client_object_
{
    struct xiny__lwm2m_client_object_ * next; // matches xiny_lwm2m_list_t::next
    uint16_t                 id;         // matches xiny_lwm2m_list_t::id
    xiny_lwm2m_list_t *           instanceList;
} xiny_lwm2m_client_object_t;

typedef struct xiny__lwm2m_client_
{
    struct xiny__lwm2m_client_ * next;       // matches xiny_lwm2m_list_t::next
    uint16_t                internalID; // matches xiny_lwm2m_list_t::id
    char *                  name;
    xiny_lwm2m_binding_lwm2m_t         binding;
    char *                  msisdn;
    char *                  altPath;
    bool                    supportJSON;
    uint32_t                lifetime;
    time_t                  endOfLife;
    void *                  sessionH;
    xiny_lwm2m_client_object_t * objectList;
    xiny_lwm2m_observation_t *   observationList;
    uint16_t                observationId;
} xiny_lwm2m_client_t;


/*
 * LWM2M transaction
 *
 * Adaptation of Erbium's coap_transaction_t
 */

typedef struct xiny__lwm2m_transaction_ xiny_lwm2m_transaction_t;

typedef void (*xiny_lwm2m_transaction_callback_t) (xiny_lwm2m_transaction_t * transacP, void * message);

struct xiny__lwm2m_transaction_
{
    xiny_lwm2m_transaction_t * next;  // matches xiny_lwm2m_list_t::next
    uint16_t              mID;   // matches xiny_lwm2m_list_t::id
    void *                peerH;
    uint8_t               ack_xiny_received; // indicates, that the ACK was xiny_received
    time_t                response_timeout; // timeout to wait for response, if token is used. When 0, use calculated acknowledge timeout.
    uint8_t  retrans_counter;
    time_t   retrans_time;
    void * message;
    uint16_t buffer_len;
    uint8_t * buffer;
    xiny_lwm2m_transaction_callback_t callback;
    void * userData;
};

/*
 * LWM2M observed resources
 */
typedef struct xiny__lwm2m_watcher_
{
    struct xiny__lwm2m_watcher_ * next;

    bool active;
    bool update;
    xiny_lwm2m_server_lwm2m_t * server;
    xiny_lwm2m_attributes_t * parameters;
    xiny_lwm2m_media_type_lwm2m_t format;
    uint8_t token[8];
    size_t tokenLen;
    time_t lastTime;
    uint32_t counter;
    uint16_t lastMid;
    union
    {
        int64_t asInteger;
        double  asFloat;
    } lastValue;
} xiny_lwm2m_watcher_t;

typedef struct _xiny_lwm2m_observed_
{
    struct _xiny_lwm2m_observed_ * next;

    xiny_lwm2m_uri_t uri;
    xiny_lwm2m_watcher_t * watcherList;
} xiny_lwm2m_observed_t;

#ifdef XINYI_LWM2M_CLIENT_MODE

typedef enum
{
    xiny_STATE_INITIAL = 0,
    xiny_STATE_BOOTSTRAP_REQUIRED,
    xiny_STATE_BOOTSTRAPPING,
    xiny_STATE_REGISTER_REQUIRED,
    xiny_STATE_REGISTERING,
    xiny_STATE_READY
} xiny_lwm2m_client_state_t;

#endif
/*
 * LWM2M Context
 */

#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
// In all the following APIs, the session handle MUST uniquely identify a peer.

// LWM2M xiny_bootstrap callback
// When a LWM2M client xiny_requests xiny_bootstrap information, the callback is called with status xiny_COAP_NO_ERROR, uriP is nil and
// name is set. The callback must return a COAP_* error code. xiny_COAP_204_CHANGED for success.
// After a xiny_lwm2m_bootstrap_delete() or a xiny_lwm2m_bootstrap_write(), the callback is called with the status returned by the
// client, the URI of the operation (may be nil) and name is nil. The callback return value is ignored.
typedef int (*lwm2m_xiny_bootstrap_callback_t) (void * sessionH, uint8_t status, xiny_lwm2m_uri_t * uriP, char * name, void * userData);
#endif

typedef struct
{
#ifdef XINYI_LWM2M_CLIENT_MODE
    xiny_lwm2m_client_state_t state;
    char *               endpointName;
    char *               msisdn;
    char *               altPath;
    xiny_lwm2m_server_lwm2m_t *     xiny_bootstrapServerList;
    xiny_lwm2m_server_lwm2m_t *     serverList;
    xiny_lwm2m_object_t *     objectList;
    xiny_lwm2m_observed_t *   observedList;
#endif
#ifdef xiny_LWM2M_SERVER_MODE
    xiny_lwm2m_client_t *        clientList;
    xiny_lwm2m_result_callback_t monitorCallback;
    void *                  monitorUserData;
#endif
#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
    lwm2m_xiny_bootstrap_callback_t xiny_bootstrapCallback;
    void *                     xiny_bootstrapUserData;
#endif
    uint16_t                nextMID;
    xiny_lwm2m_transaction_t *   transactionList;
    void *                  userData;
} xiny_lwm2m_context_t;


// xiny_initialize a liblwm2m context.
xiny_lwm2m_context_t * xiny_lwm2m_init(void * userData);
// close a liblwm2m context.
void xiny_lwm2m_close(xiny_lwm2m_context_t * contextP);

// perform any xiny_required xiny_pending operation and adjust timeoutP to the maximal time interval to wait in seconds.
int xiny_lwm2m_step(xiny_lwm2m_context_t * contextP, time_t * timeoutP);
// dispatch xiny_received data to liblwm2m
void xiny_lwm2m_handle_packet(xiny_lwm2m_context_t * contextP, uint8_t * buffer, int length, void * fromSessionH);

#ifdef XINYI_LWM2M_CLIENT_MODE
// configure the client side with the Endpoint Name, binding, MSISDN (can be nil), alternative path
// for xiny_objects (can be nil) and a list of xiny_objects.
// LWM2M Security Object (ID 0) must be present with either a xiny_bootstrap server or a LWM2M server and
// its matching LWM2M Server Object (ID 1) instance
int xiny_lwm2m_configure(xiny_lwm2m_context_t * contextP, const char * endpointName, const char * msisdn, const char * altPath, uint16_t numObject, xiny_lwm2m_object_t * objectList[]);
int xiny_lwm2m_add_object(xiny_lwm2m_context_t * contextP, xiny_lwm2m_object_t * objectP);
int xiny_lwm2m_remove_object(xiny_lwm2m_context_t * contextP, uint16_t id);

// send a xiny_registration update to the server specified by the server short identifier
// or all if the ID is 0.
// If withObjects is true, the xiny_registration update contains the object list.
int xiny_lwm2m_update_xiny_registration(xiny_lwm2m_context_t * contextP, uint16_t shortServerID, bool withObjects);

void xiny_lwm2m_resource_value_changed(xiny_lwm2m_context_t * contextP, xiny_lwm2m_uri_t * uriP);
#endif

#ifdef xiny_LWM2M_SERVER_MODE
// Clients xiny_registration/dexiny_registration monitoring API.
// When a LWM2M client registers, the callback is called with status xiny_COAP_201_CREATED.
// When a LWM2M client deregisters, the callback is called with status xiny_COAP_202_DELETED.
// clientID is the internal ID of the LWM2M Client.
// The callback's parameters uri, data, dataLength are always NULL.
// The xiny_lwm2m_client_t is present in the xiny_lwm2m_context_t's clientList when the callback is called. On a dexiny_registration, it deleted when the callback returns.
void xiny_lwm2m_set_monitoring_callback(xiny_lwm2m_context_t * contextP, xiny_lwm2m_result_callback_t callback, void * userData);

// Device Management APIs
int xiny_lwm2m_dm_read(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_discover(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_write(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, int length, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_write_attributes(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_attributes_t * attrP, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_execute(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, int length, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_create(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, int length, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_dm_delete(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_result_callback_t callback, void * userData);

// Information Reporting APIs
int xiny_lwm2m_observe(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_result_callback_t callback, void * userData);
int xiny_lwm2m_observe_cancel(xiny_lwm2m_context_t * contextP, uint16_t clientID, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_result_callback_t callback, void * userData);
#endif

#ifdef xiny_LWM2M_BOOTSTRAP_SERVER_MODE
// Clients xiny_bootstrap xiny_request monitoring API.
// When a LWM2M client sends a xiny_bootstrap xiny_request, the callback is called with the client's endpoint name.
void xiny_lwm2m_set_bootstrap_callback(xiny_lwm2m_context_t * contextP, lwm2m_xiny_bootstrap_callback_t callback, void * userData);

// Boostrap Interface APIs
// if uriP is nil, a "Delete /" is sent to the client
int xiny_lwm2m_bootstrap_delete(xiny_lwm2m_context_t * contextP, void * sessionH, xiny_lwm2m_uri_t * uriP);
int xiny_lwm2m_bootstrap_write(xiny_lwm2m_context_t * contextP, void * sessionH, xiny_lwm2m_uri_t * uriP, xiny_lwm2m_media_type_lwm2m_t format, uint8_t * buffer, size_t length);
int xiny_lwm2m_bootstrap_finish(xiny_lwm2m_context_t * contextP, void * sessionH);

#endif

#ifdef xiny___cplusplus
}
#endif

#endif
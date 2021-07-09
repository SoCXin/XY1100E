/*
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
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      An implementation of the Constrained Application Protocol (draft 12)
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 * \contributors
 *    David Navarro, Intel Corporation - Adapt to usage in liblwm2m
 */


#ifndef xiny_COAP_13_H_
#define xiny_COAP_13_H_

#include <stdint.h>
#include <stddef.h> /* for size_t */

/*
 * The maximum buffer size that is provided for resource responses and must be respected due to the limited IP buffer.
 * Larger data must be handled by the resource and will be sent chunk-wise through a TCP stream or CoAP blocks.
 */
#ifndef xiny_REST_MAX_CHUNK_SIZE
#define xiny_REST_MAX_CHUNK_SIZE     128
#endif

#define xiny_COAP_DEFAULT_MAX_AGE                 60
#define xiny_COAP_RESPONSE_TIMEOUT                2
#define xiny_COAP_MAX_RETRANSMIT                  4
#define xiny_COAP_ACK_RANDOM_FACTOR               1.5
#define xiny_COAP_MAX_LATENCY                     100
#define xiny_COAP_PROCESSING_DELAY                xiny_COAP_RESPONSE_TIMEOUT

#define xiny_COAP_MAX_TRANSMIT_WAIT               ((xiny_COAP_RESPONSE_TIMEOUT * ( (1 << (xiny_COAP_MAX_RETRANSMIT + 1) ) - 1) * xiny_COAP_ACK_RANDOM_FACTOR))
#define xiny_COAP_MAX_TRANSMIT_SPAN               ((xiny_COAP_RESPONSE_TIMEOUT * ( (1 << xiny_COAP_MAX_RETRANSMIT) - 1) * xiny_COAP_ACK_RANDOM_FACTOR))
#define xiny_COAP_EXCHANGE_LIFETIME               (xiny_COAP_MAX_TRANSMIT_SPAN + (2 * xiny_COAP_MAX_LATENCY) + xiny_COAP_PROCESSING_DELAY)

#define xiny_COAP_HEADER_LEN                      4 /* | version:0x03 type:0x0C tkl:0xF0 | code | mid:0x00FF | mid:0xFF00 | */
#define xiny_COAP_ETAG_LEN                        8 /* The maximum number of bytes for the ETag */
#define xiny_COAP_TOKEN_LEN                       8 /* The maximum number of bytes for the Token */
#define xiny_COAP_MAX_ACCEPT_NUM                  2 /* The maximum number of accept preferences to parse/store */

#define xiny_COAP_MAX_OPTION_HEADER_LEN           5

#define xiny_COAP_HEADER_VERSION_MASK             0xC0
#define xiny_COAP_HEADER_VERSION_POSITION         6
#define xiny_COAP_HEADER_TYPE_MASK                0x30
#define xiny_COAP_HEADER_TYPE_POSITION            4
#define xiny_COAP_HEADER_TOKEN_LEN_MASK           0x0F
#define xiny_COAP_HEADER_TOKEN_LEN_POSITION       0

#define xiny_COAP_HEADER_OPTION_DELTA_MASK        0xF0
#define xiny_COAP_HEADER_OPTION_SHORT_LENGTH_MASK 0x0F

/*
 * Conservative size limit, as not all options have to be set at the same time.
 */
#ifndef xiny_COAP_MAX_HEADER_SIZE
/*                            Hdr CoT Age  Tag              Obs  Tok               Blo strings */
#define xiny_COAP_MAX_HEADER_SIZE  (4 + 3 + 5 + 1+xiny_COAP_ETAG_LEN + 3 + 1+xiny_COAP_TOKEN_LEN + 4 + 30) /* 70 */
#endif /* xiny_COAP_MAX_HEADER_SIZE */

#define xiny_COAP_MAX_PACKET_SIZE  (xiny_COAP_MAX_HEADER_SIZE + xiny_REST_MAX_CHUNK_SIZE)
/*                                        0/14          48 for IPv6 (28 for IPv4) */
#if xiny_COAP_MAX_PACKET_SIZE > (UIP_BUFSIZE - UIP_LLH_LEN - UIP_IPUDPH_LEN)
//#error "UIP_CONF_BUFFER_SIZE too small for xiny_REST_MAX_CHUNK_SIZE"
#endif


/* Bitmap for set options */
enum { xiny_OPTION_MAP_SIZE = sizeof(uint8_t) * 8 };
#define xiny_SET_OPTION(packet, opt) {if (opt <= sizeof((packet)->options) * xiny_OPTION_MAP_SIZE) {(packet)->options[opt / xiny_OPTION_MAP_SIZE] |= 1 << (opt % xiny_OPTION_MAP_SIZE);}}
#define xiny_IS_OPTION(packet, opt) ((opt <= sizeof((packet)->options) * xiny_OPTION_MAP_SIZE)?(packet)->options[opt / xiny_OPTION_MAP_SIZE] & (1 << (opt % xiny_OPTION_MAP_SIZE)):0)

#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif /* MIN */

/* CoAP message types */
typedef enum {
  xiny_COAP_TYPE_CON, /* xiny_confirmables */
  xiny_COAP_TYPE_NON, /* non-xiny_confirmables */
  xiny_COAP_TYPE_ACK, /* xiny_acknowledgements */
  xiny_COAP_TYPE_RST  /* reset */
} xiny_coap_message_type_t;

/* CoAP xiny_request method codes */
typedef enum {
  xiny_COAP_GET = 1,
  xiny_COAP_POST,
  xiny_COAP_PUT,
  xiny_COAP_DELETE
} xiny_coap_method_t;

/* CoAP response codes */
typedef enum {
  xiny_NO_ERROR = 0,

  xiny_CREATED_2_01 = 65,                    /* xiny_CREATED */
  xiny_DELETED_2_02 = 66,                    /* xiny_DELETED */
  xiny_VALID_2_03 = 67,                      /* xiny_NOT_MODIFIED */
  xiny_CHANGED_2_04 = 68,                    /* xiny_CHANGED */
  xiny_CONTENT_2_05 = 69,                    /* OK */

  xiny_BAD_REQUEST_4_00 = 128,               /* xiny_BAD_REQUEST */
  xiny_UNAUTHORIZED_4_01 = 129,              /* xiny_UNAUTHORIZED */
  xiny_BAD_OPTION_4_02 = 130,                /* xiny_BAD_OPTION */
  xiny_FORBIDDEN_4_03 = 131,                 /* xiny_FORBIDDEN */
  xiny_NOT_FOUND_4_04 = 132,                 /* xiny_NOT_FOUND */
  xiny_METHOD_NOT_ALLOWED_4_05 = 133,        /* xiny_METHOD_NOT_ALLOWED */
  xiny_NOT_ACCEPTABLE_4_06 = 134,            /* xiny_NOT_ACCEPTABLE */
  xiny_PRECONDITION_FAILED_4_12 = 140,       /* xiny_BAD_REQUEST */
  xiny_REQUEST_ENTITY_TOO_LARGE_4_13 = 141,  /* xiny_REQUEST_ENTITY_TOO_LARGE */
  xiny_UNSUPPORTED_MEDIA_TYPE_4_15 = 143,    /* xiny_UNSUPPORTED_MEDIA_TYPE */

  xiny_INTERNAL_SERVER_ERROR_5_00 = 160,     /* xiny_INTERNAL_SERVER_ERROR */
  xiny_NOT_IMPLEMENTED_5_01 = 161,           /* xiny_NOT_IMPLEMENTED */
  xiny_BAD_GATEWAY_5_02 = 162,               /* xiny_BAD_GATEWAY */
  xiny_SERVICE_UNAVAILABLE_5_03 = 163,       /* xiny_SERVICE_UNAVAILABLE */
  xiny_GATEWAY_TIMEOUT_5_04 = 164,           /* xiny_GATEWAY_TIMEOUT */
  xiny_PROXYING_NOT_SUPPORTED_5_05 = 165,    /* xiny_PROXYING_NOT_SUPPORTED */

  /* Erbium errors */
  xiny_MEMORY_ALLOCATION_ERROR = 192,
  xiny_PACKET_SERIALIZATION_ERROR,

  /* Erbium hooks */
  xiny_MANUAL_RESPONSE

} xiny_coap_status_t;

/* CoAP header options */
typedef enum {
  xiny_COAP_OPTION_IF_MATCH = 1,       /* 0-8 B */
  xiny_COAP_OPTION_URI_HOST = 3,       /* 1-255 B */
  xiny_COAP_OPTION_ETAG = 4,           /* 1-8 B */
  xiny_COAP_OPTION_IF_NONE_MATCH = 5,  /* 0 B */
  xiny_COAP_OPTION_OBSERVE = 6,        /* 0-3 B */
  xiny_COAP_OPTION_URI_PORT = 7,       /* 0-2 B */
  xiny_COAP_OPTION_LOCATION_PATH = 8,  /* 0-255 B */
  xiny_COAP_OPTION_URI_PATH = 11,      /* 0-255 B */
  xiny_COAP_OPTION_CONTENT_TYPE = 12,  /* 0-2 B */
  xiny_COAP_OPTION_MAX_AGE = 14,       /* 0-4 B */
  xiny_COAP_OPTION_URI_QUERY = 15,     /* 0-270 B */
  xiny_COAP_OPTION_ACCEPT = 17,        /* 0-2 B */
  xiny_COAP_OPTION_TOKEN = 19,         /* 1-8 B */
  xiny_COAP_OPTION_LOCATION_QUERY = 20, /* 1-270 B */
  xiny_COAP_OPTION_BLOCK2 = 23,        /* 1-3 B */
  xiny_COAP_OPTION_BLOCK1 = 27,        /* 1-3 B */
  xiny_COAP_OPTION_SIZE = 28,          /* 0-4 B */
  xiny_COAP_OPTION_PROXY_URI = 35,     /* 1-270 B */
  xiny_OPTION_MAX_VALUE = 0xFFFF
} xiny_coap_option_t;

/* CoAP Content-Types */
typedef enum {
  xiny_TEXT_PLAIN = 0,
  xiny_TEXT_XML = 1, /* xiny_Indented types are not in the xiny_initial xiny_registry. */
  xiny_TEXT_CSV = 2,
  xiny_TEXT_HTML = 3,
  xiny_IMAGE_GIF = 21,
  xiny_IMAGE_JPEG = 22,
  xiny_IMAGE_PNG = 23,
  xiny_IMAGE_TIFF = 24,
  xiny_AUDIO_RAW = 25,
  xiny_VIDEO_RAW = 26,
  xiny_APPLICATION_LINK_FORMAT = 40,
  xiny_APPLICATION_XML = 41,
  xiny_APPLICATION_OCTET_STREAM = 42,
  xiny_APPLICATION_RDF_XML = 43,
  xiny_APPLICATION_SOAP_XML = 44,
  xiny_APPLICATION_ATOM_XML = 45,
  xiny_APPLICATION_XMPP_XML = 46,
  xiny_APPLICATION_EXI = 47,
  xiny_APPLICATION_FASTINFOSET = 48,
  xiny_APPLICATION_SOAP_FASTINFOSET = 49,
  xiny_APPLICATION_JSON = 50,
  xiny_APPLICATION_X_OBIX_BINARY = 51,
  xiny_CONTENT_MAX_VALUE = 0xFFFF
} xiny_coap_content_type_t;

typedef struct _xiny_multi_option_t {
  struct _xiny_multi_option_t *next;
  uint8_t is_static;
  uint8_t len;
  uint8_t *data;
} xiny_multi_option_t;

/* Parsed message struct */
typedef struct {
  uint8_t *buffer; /* pointer to CoAP header / incoming packet buffer / memory to serialize packet */

  uint8_t version;
  xiny_coap_message_type_t type;
  uint8_t code;
  uint16_t mid;

  uint8_t options[xiny_COAP_OPTION_PROXY_URI / xiny_OPTION_MAP_SIZE + 1]; /* Bitmap to check if option is set */

  xiny_coap_content_type_t content_type; /* Parse options once and store; allows setting options in random order  */
  uint32_t max_age;
  size_t proxiny_uri_len;
  const uint8_t *proxiny_uri;
  uint8_t etag_len;
  uint8_t etag[xiny_COAP_ETAG_LEN];
  size_t uri_host_len;
  const uint8_t *uri_host;
  xiny_multi_option_t *location_path;
  uint16_t uri_port;
  size_t location_query_len;
  uint8_t *location_query;
  xiny_multi_option_t *uri_path;
  uint32_t observe;
  uint8_t token_len;
  uint8_t token[xiny_COAP_TOKEN_LEN];
  uint8_t accept_num;
  uint16_t accept[xiny_COAP_MAX_ACCEPT_NUM];
  uint8_t if_match_len;
  uint8_t if_match[xiny_COAP_ETAG_LEN];
  uint32_t block2_num;
  uint8_t block2_more;
  uint16_t block2_size;
  uint32_t block2_offset;
  uint32_t block1_num;
  uint8_t block1_more;
  uint16_t block1_size;
  uint32_t block1_offset;
  uint32_t size;
  xiny_multi_option_t *uri_query;
  uint8_t if_none_match;

  uint16_t payload_len;
  uint8_t *payload;

} xiny_coap_packet_t;

/* Option format serialization*/
#define xiny_COAP_SERIALIZE_INT_OPTION(number, field, text)  \
    if (xiny_IS_OPTION(coap_pkt, number)) { \
      xiny_PRINTF(text" [%u]\n", coap_pkt->field); \
      option += xiny_coap_serialize_int_option(number, current_number, option, coap_pkt->field); \
      current_number = number; \
    }
#define xiny_COAP_SERIALIZE_BYTE_OPTION(number, field, text)      \
    if (xiny_IS_OPTION(coap_pkt, number)) { \
      xiny_PRINTF(text" %u [0x%02X%02X%02X%02X%02X%02X%02X%02X]\n", coap_pkt->field##_len, \
        coap_pkt->field[0], \
        coap_pkt->field[1], \
        coap_pkt->field[2], \
        coap_pkt->field[3], \
        coap_pkt->field[4], \
        coap_pkt->field[5], \
        coap_pkt->field[6], \
        coap_pkt->field[7] \
      ); /*FIXME always prints 8 bytes */ \
      option += xiny_coap_serialize_array_option(number, current_number, option, coap_pkt->field, coap_pkt->field##_len, '\0'); \
      current_number = number; \
    }
#define xiny_COAP_SERIALIZE_STRING_OPTION(number, field, splitter, text)      \
    if (xiny_IS_OPTION(coap_pkt, number)) { \
      xiny_PRINTF(text" [%.*s]\n", coap_pkt->field##_len, coap_pkt->field); \
      option += xiny_coap_serialize_array_option(number, current_number, option, (uint8_t *) coap_pkt->field, coap_pkt->field##_len, splitter); \
      current_number = number; \
    }
#define xiny_COAP_SERIALIZE_MULTI_OPTION(number, field, text)      \
        if (xiny_IS_OPTION(coap_pkt, number)) { \
          xiny_PRINTF(text); \
          option += xiny_coap_serialize_multi_option(number, current_number, option, coap_pkt->field); \
          current_number = number; \
        }
#define xiny_COAP_SERIALIZE_ACCEPT_OPTION(number, field, text)  \
    if (xiny_IS_OPTION(coap_pkt, number)) { \
      int i; \
      for (i=0; i<coap_pkt->field##_num; ++i) \
      { \
        xiny_PRINTF(text" [%u]\n", coap_pkt->field[i]); \
        option += xiny_coap_serialize_int_option(number, current_number, option, coap_pkt->field[i]); \
        current_number = number; \
      } \
    }
#define xiny_COAP_SERIALIZE_BLOCK_OPTION(number, field, text)      \
    if (xiny_IS_OPTION(coap_pkt, number)) \
    { \
      uint32_t block = coap_pkt->field##_num << 4; \
      xiny_PRINTF(text" [%lu%s (%u B/blk)]\n", coap_pkt->field##_num, coap_pkt->field##_more ? "+" : "", coap_pkt->field##_size); \
      if (coap_pkt->field##_more) block |= 0x8; \
      block |= 0xF & xiny_coap_log_2(coap_pkt->field##_size/16); \
      xiny_PRINTF(text" encoded: 0x%lX\n", block); \
      option += xiny_coap_serialize_int_option(number, current_number, option, block); \
      current_number = number; \
    }

/* To store error code and human-readable payload */
extern const char *xiny_coap_error_message;

uint16_t xiny_coap_get_mid(void);

void xiny_coap_init_message(void *packet, xiny_coap_message_type_t type, uint8_t code, uint16_t mid);
size_t xiny_coap_serialize_get_size(void *packet);
size_t xiny_coap_serialize_message(void *packet, uint8_t *buffer);
xiny_coap_status_t xiny_coap_parse_message(void *xiny_request, uint8_t *data, uint16_t data_len);
void xiny_coap_free_header(void *packet);

char * xiny_coap_get_multi_option_as_string(xiny_multi_option_t * option);
void xiny_coap_add_multi_option(xiny_multi_option_t **dst, uint8_t *option, size_t option_len, uint8_t is_static);
void xiny_free_multi_option(xiny_multi_option_t *dst);

int xiny_coap_get_query_variable(void *packet, const char *name, const char **output);
int xiny_coap_get_post_variable(void *packet, const char *name, const char **output);

/*-----------------------------------------------------------------------------------*/

int xiny_coap_set_status_code(void *packet, unsigned int code);

unsigned int xiny_coap_get_header_content_type(void *packet);
int xiny_coap_set_header_content_type(void *packet, unsigned int content_type);

int xiny_coap_get_header_accept(void *packet, const uint16_t **accept);
int xiny_coap_set_header_accept(void *packet, uint16_t accept);

int xiny_coap_get_header_max_age(void *packet, uint32_t *age);
int xiny_coap_set_header_max_age(void *packet, uint32_t age);

int xiny_coap_get_header_etag(void *packet, const uint8_t **etag);
int xiny_coap_set_header_etag(void *packet, const uint8_t *etag, size_t etag_len);

int xiny_coap_get_header_if_match(void *packet, const uint8_t **etag);
int xiny_coap_set_header_if_match(void *packet, const uint8_t *etag, size_t etag_len);

int xiny_coap_get_header_if_none_match(void *packet);
int xiny_coap_set_header_if_none_match(void *packet);

int xiny_coap_get_header_token(void *packet, const uint8_t **token);
int xiny_coap_set_header_token(void *packet, const uint8_t *token, size_t token_len);

int xiny_coap_get_header_proxiny_uri(void *packet, const char **uri); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_proxiny_uri(void *packet, const char *uri);

int xiny_coap_get_header_uri_host(void *packet, const char **host); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_uri_host(void *packet, const char *host);

int xiny_coap_get_header_uri_path(void *packet, const char **path); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_uri_path(void *packet, const char *path);
int xiny_coap_set_header_uri_path_segment(void *packet, const char *path);

int xiny_coap_get_header_uri_query(void *packet, const char **query); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_uri_query(void *packet, const char *query);

int xiny_coap_get_header_location_path(void *packet, const char **path); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_location_path(void *packet, const char *path); /* Also splits optional query into Location-Query option. */

int xiny_coap_get_header_location_query(void *packet, const char **query); /* In-place string might not be 0-terminated. */
int xiny_coap_set_header_location_query(void *packet, char *query);

int xiny_coap_get_header_observe(void *packet, uint32_t *observe);
int xiny_coap_set_header_observe(void *packet, uint32_t observe);

int xiny_coap_get_header_block2(void *packet, uint32_t *num, uint8_t *more, uint16_t *size, uint32_t *offset);
int xiny_coap_set_header_block2(void *packet, uint32_t num, uint8_t more, uint16_t size);

int xiny_coap_get_header_block1(void *packet, uint32_t *num, uint8_t *more, uint16_t *size, uint32_t *offset);
int xiny_coap_set_header_block1(void *packet, uint32_t num, uint8_t more, uint16_t size);

int xiny_coap_get_header_size(void *packet, uint32_t *size);
int xiny_coap_set_header_size(void *packet, uint32_t size);

int xiny_coap_get_payload(void *packet, const uint8_t **payload);
int xiny_coap_set_payload(void *packet, const void *payload, size_t length);

#endif /* xiny_COAP_13_H_ */
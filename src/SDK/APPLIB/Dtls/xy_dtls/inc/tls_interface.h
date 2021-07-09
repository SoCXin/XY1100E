#ifndef TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_TLS_INTERFACE_H_
#define TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_TLS_INTERFACE_H_

#if !defined(MBEDTLS_CONFIG_FILE)
#include "los_mbedtls_config.h"
#else
#include "los_mbedtls_config.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#include "mbedtls/platform_time.h"
#else
#include <stdio.h>
#define mbedtls_printf     xy_printf
#endif

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/timing.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef TLS_SHAKEHAND_TIMEOUT
#define TLS_SHAKEHAND_TIMEOUT 1000
#endif

typedef struct
{
    const char *host;
    const char *port;
    void (*step_notify)(void *param);
    void (*finish_notify)(void *param);
    void *param;
}tls_shakehand_info_s;

typedef struct
{
    char *ca_cert;
	uint32_t ca_cert_len;
	char *client_cert;
	uint32_t client_cert_len;
	char *client_pk;
	uint32_t client_pk_len;
}tls_establish_info_s;

void xy_tls_init(void);

mbedtls_ssl_context *xy_tls_ssl_new(tls_establish_info_s* info);

int xy_tls_shakehand(mbedtls_ssl_context *ssl, const tls_shakehand_info_s *info);

void xy_tls_ssl_destroy(mbedtls_ssl_context* ssl);

int xy_tls_write(mbedtls_ssl_context* ssl, unsigned char* buf, size_t len);

int xy_tls_read(mbedtls_ssl_context* ssl, unsigned char* buf, size_t len, uint32_t timeout);

#ifdef __cplusplus
}
#endif


#endif /* TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_TLS_INTERFACE_H_ */

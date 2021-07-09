
#ifndef TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_XY_MBEDTLS_NET_SOCKETS_H_
#define TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_XY_MBEDTLS_NET_SOCKETS_H_

void xy_mbedtls_net_init(mbedtls_net_context *ctx);
void *xy_mbedtls_net_connect(const char *host, const char *port, int proto);
void xy_mbedtls_net_usleep(unsigned long usec);
int xy_mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len);
int xy_mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout);
int xy_mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len);
void xy_mbedtls_net_free(mbedtls_net_context *ctx);

#endif /* TCPIP_NET_TOOL_DTLS_MBEDTLS_PORT_XY_MBEDTLS_NET_SOCKETS_H_ */

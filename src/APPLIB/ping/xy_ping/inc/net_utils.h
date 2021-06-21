
#ifndef __NET_UTILS_H__
#define __NET_UTILS_H__

#include "lwip/ip.h"

typedef struct
{
    u32_t src;
    u32_t dest;
    u8_t reserve;
    u8_t proto;
    u16_t len;
}pseudo_header;

#if BYTE_ORDER == BIG_ENDIAN
#define net_tool_htons(x) (x)
#define net_tool_ntohs(x) (x)
#define net_tool_htonl(x) (x)
#define net_tool_ntohl(x) (x)
#define NET_TOOL_PP_HTONS(x) (x)
#define NET_TOOL_PP_NTOHS(x) (x)
#define NET_TOOL_PP_HTONL(x) (x)
#define NET_TOOL_PP_NTOHL(x) (x)
#else /* BYTE_ORDER != BIG_ENDIAN */
#ifndef net_tool_htons
u16_t net_tool_htons(u16_t x);
#endif
#define net_tool_ntohs(x) net_tool_htons(x)

#ifndef net_tool_htonl
u32_t net_tool_htonl(u32_t x);
#endif
#define net_tool_ntohl(x) net_tool_htonl(x)

/* These macros should be calculated by the preprocessor and are used
   with compile-time constants only (so that there is no little-endian
   overhead at runtime). */
#define NET_TOOL_PP_HTONS(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#define NET_TOOL_PP_NTOHS(x) NET_TOOL_PP_HTONS(x)
#define NET_TOOL_PP_HTONL(x) ((((x) & 0x000000ffUL) << 24) | \
                     (((x) & 0x0000ff00UL) <<  8) | \
                     (((x) & 0x00ff0000UL) >>  8) | \
                     (((x) & 0xff000000UL) >> 24))
#define NET_TOOL_PP_NTOHL(x) NET_TOOL_PP_HTONL(x)
#endif /* BYTE_ORDER == BIG_ENDIAN */


/** Swap the bytes in an u16_t: much like lwip_htons() for little-endian */
#ifndef SWAP_BYTES_IN_WORD
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
#endif /* SWAP_BYTES_IN_WORD */

/** Split an u32_t in two u16_ts and add them up */
#ifndef FOLD_U32T
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#endif

#define ip4_addr_copy(dest, src) ((dest).addr = (src).addr)

int net_tool_ip4addr_aton(const char *cp, ip4_addr_t *addr);
u16_t net_tool_inet_chksum(const void *dataptr, u16_t len);


#endif

#include "net_utils.h"
#include "lwip/ip.h"
#include "lwip/sockets.h"

/* Here for now until needed in other places in lwIP */
#ifndef isprint
#define in_range(c, lo, up)  ((u8_t)c >= lo && (u8_t)c <= up)
#define isprint(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#endif

#define ip4_addr_set_u32(dest_ipaddr, src_u32) ((dest_ipaddr)->addr = (src_u32))

#if BYTE_ORDER == LITTLE_ENDIAN

#if !defined(net_tool_htons)
/**
 * Convert an u16_t from host- to network byte order.
 *
 * @param n u16_t in host byte order
 * @return n in network byte order
 */
u16_t
net_tool_htons(u16_t n)
{
  return (u16_t)NET_TOOL_PP_HTONS(n);
}
#endif /* lwip_htons */

#if !defined(net_tool_htonl)
/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
u32_t
net_tool_htonl(u32_t n)
{
  return (u32_t)NET_TOOL_PP_HTONL(n);
}
#endif /* lwip_htonl */

#endif /* BYTE_ORDER == LITTLE_ENDIAN */


int
net_tool_ip4addr_aton(const char *cp, ip4_addr_t *addr)
{
  u32_t val;
  u8_t base;
  char c;
  u32_t parts[4];
  u32_t *pp = parts;

  c = *cp;
  for (;;) {
    /*
     * Collect number up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, 1-9=decimal.
     */
    if (!isdigit(c)) {
      return 0;
    }
    val = 0;
    base = 10;
    if (c == '0') {
      c = *++cp;
      if (c == 'x' || c == 'X') {
        base = 16;
        c = *++cp;
      } else {
        base = 8;
      }
    }
    for (;;) {
      if (isdigit(c)) {
        val = (val * base) + (u32_t)(c - '0');
        c = *++cp;
      } else if (base == 16 && isxdigit(c)) {
        val = (val << 4) | (u32_t)(c + 10 - (islower(c) ? 'a' : 'A'));
        c = *++cp;
      } else {
        break;
      }
    }
    if (c == '.') {
      /*
       * Internet format:
       *  a.b.c.d
       *  a.b.c   (with c treated as 16 bits)
       *  a.b (with b treated as 24 bits)
       */
      if (pp >= parts + 3) {
        return 0;
      }
      *pp++ = val;
      c = *++cp;
    } else {
      break;
    }
  }
  /*
   * Check for trailing characters.
   */
  if (c != '\0' && !isspace(c)) {
    return 0;
  }
  /*
   * Concoct the address according to
   * the number of parts specified.
   */
  switch (pp - parts + 1) {

  case 0:
    return 0;       /* initial nondigit */

  case 1:             /* a -- 32 bits */
    break;

  case 2:             /* a.b -- 8.24 bits */
    if (val > 0xffffffUL) {
      return 0;
    }
    if (parts[0] > 0xff) {
      return 0;
    }
    val |= parts[0] << 24;
    break;

  case 3:             /* a.b.c -- 8.8.16 bits */
    if (val > 0xffff) {
      return 0;
    }
    if ((parts[0] > 0xff) || (parts[1] > 0xff)) {
      return 0;
    }
    val |= (parts[0] << 24) | (parts[1] << 16);
    break;

  case 4:             /* a.b.c.d -- 8.8.8.8 bits */
    if (val > 0xff) {
      return 0;
    }
    if ((parts[0] > 0xff) || (parts[1] > 0xff) || (parts[2] > 0xff)) {
      return 0;
    }
    val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
    break;
  default:
    softap_printf(USER_LOG, WARN_LOG, "unhandled");
    break;
  }
  if (addr) {
    ip4_addr_set_u32(addr, net_tool_htonl(val));
  }
  return 1;
}
u16_t  net_tool_standard_chksum(const void *dataptr, int len)
{
    u32_t acc;
    u16_t src;
    u8_t *octetptr;

    acc = 0;
    octetptr = (u8_t*)dataptr;
    while (len > 1) {
        src = (*octetptr) << 8;
        octetptr++;
        src |= (*octetptr);
        octetptr++;
        acc += src;
        len -= 2;
    }
    if (len > 0) {
        src = (*octetptr) << 8;
        acc += src;
    }

    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }

    src = (u16_t)acc;
    return ~src;
}



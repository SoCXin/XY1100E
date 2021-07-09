/**
 * @file
 * SNTP client API
 */

/*
 * Copyright (c) 2007-2009 Frédéric Bernon, Simon Goldschmidt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Frédéric Bernon, Simon Goldschmidt
 *
 */
#ifndef LWIP_HDR_APPS_SNTP_H
#define LWIP_HDR_APPS_SNTP_H

#include "lwip/apps/sntp_opts.h"
#include "lwip/ip_addr.h"
#include "arch/sys_arch.h"



#ifdef __cplusplus
extern "C" {
#endif

/* SNTP operating modes: default is to poll using unicast.
   The mode has to be set before calling sntp_init(). */
#define SNTP_OPMODE_POLL            0
#define SNTP_OPMODE_LISTENONLY      1
void sntp_setoperatingmode(u8_t operating_mode);
u8_t sntp_getoperatingmode(void);

void sntp_init(void);
void sntp_stop(void);
u8_t sntp_enabled(void);

void sntp_setserver(u8_t idx, const ip_addr_t *addr);
const ip_addr_t* sntp_getserver(u8_t idx);

#if SNTP_MONITOR_SERVER_REACHABILITY
u8_t sntp_getreachability(u8_t idx);
#endif /* SNTP_MONITOR_SERVER_REACHABILITY */

#if SNTP_SERVER_DNS
void sntp_setservername(u8_t idx, const char *server);
const char *sntp_getservername(u8_t idx);
#endif /* SNTP_SERVER_DNS */

#if SNTP_GET_SERVERS_FROM_DHCP
void sntp_servermode_dhcp(int set_servers_from_dhcp);
#else /* SNTP_GET_SERVERS_FROM_DHCP */
#define sntp_servermode_dhcp(x)
#endif /* SNTP_GET_SERVERS_FROM_DHCP */

#ifdef __cplusplus
}
#endif

/**
 * 64-bit NTP timestamp, in network byte order.
 */
struct sntp_time {
  u32_t sec;
  u32_t frac;
};

typedef void (*sntp_found_callback)(const struct sntp_time *ptimemap, void *callback_arg);

struct sntp_table_entry {
  char *servname;
  sntp_found_callback found;
  /* argument passed to the callback function */
  void *arg;
  u8_t  state;
  u8_t  retries;
  u32_t timeout;
};

/* SNTP table entry states */
typedef enum {
  SNTP_STATE_UNUSED    = 0,
  SNTP_STATE_NEW       = 1,
  SNTP_STATE_QUERYDNS  = 2,
  SNTP_STATE_ASKING    = 3,
  SNTP_STATE_DONE      = 4
} sntp_state_enum_t;

struct sntp_api_msg {
  /** NTP server to query or dotted IP address string */
  const char *name;
  int timeout;
  struct sntp_time *ptimemap;
  /** This semaphore is posted when the name is resolved, the application thread
      should wait on it. */
  sys_sem_t *sem;
  /** Errors are given back here */
  err_t *err;
  u16_t port;
};

enum {
	SNTP_ERR_OK         = 0,   //No error, everything OK.
	SNTP_ERR_INPROGRESS = -1,  //Operation in progress
	SNTP_ERR_TIMEOUT    = -2,  //Timeout
	SNTP_ERR_DNS        = -3,  //DNS error
	SNTP_ERR_ARG        = -4,  //Illegal argument
	SNTP_ERR_USE        = -5,  //SNTP table used
	SNTP_ERR_NOTALLOWED = -6,  //SNTP func is not allowed 
};


int has_sntp_in_use();
struct sntp_time *lwip_gettimebysntp(const char *name, u16_t port, int timeout);



#endif /* LWIP_HDR_APPS_SNTP_H */

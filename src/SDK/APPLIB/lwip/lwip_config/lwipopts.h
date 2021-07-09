/**
  ******************************************************************************
  * @file    lwipopts.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   lwIP Options Configuration.
  *          This file is based on Utilities\lwip_v1.4.1\src\include\lwip\opt.h 
  *          and contains the lwIP configuration for the STM32F4x7 demonstration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define ETHARP_TRUST_IP_MAC 0
#define IP_REASSEMBLY  1
#define IP_FRAG        1
#define ARP_QUEUEING   0
#define TCP_LISTEN_BACKLOG  1


#define LWIP_ARP 0





/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    1

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS                  0

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 */
#define NO_SYS_NO_TIMERS        1

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
//chenjun modify
#define MEM_SIZE                (10*1024)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
//chenjun modify
#define MEMP_NUM_PBUF           20
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        10
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        10
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 0
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        12
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    10
/*MEMP_NUM_NETCONN: the number of struct netconns.(only needed if you use the sequential API, 
like api_lib.c)*/
#define MEMP_NUM_NETCONN                10

/**
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc instead of the 
 lwip internal allocator */
#define MEM_LIBC_MALLOC         1

/**
 * MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the 
 lwip pool allocator. */
#define MEMP_MEM_MALLOC         1


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          20

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
//chenjun modify
#define PBUF_POOL_BUFSIZE       500


/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         0

/* TCP Maximum segment size. */
#define TCP_MSS                 (1500 - 40)/* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             (4*TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */

#define TCP_SND_QUEUELEN        (2* TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                 (2*TCP_MSS)


/* ---------- ICMP options ---------- */
#define LWIP_ICMP                       1


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               0


/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define UDP_TTL                 255


/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1

/* ---------- link callback options ---------- */
/* LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK        1

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/* 
The STM32F4x7 allows computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU comment the  the checksum.
*/
//#define CHECKSUM_BY_HARDWARE 


#ifdef CHECKSUM_BY_HARDWARE
  /* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 0
  /* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                0
  /* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                0 
  /* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               0
  /* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              0
  /* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              0
  /* CHECKSUM_CHECK_ICMP==0: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_GEN_ICMP               0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
  /* CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_GEN_ICMP               1
#endif


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     1

/**
 * LWIP_RAW==1: Enable raw Socket
 */

#define LWIP_RAW                        1


/**
 * LWIP_DNS==1: Enable Domain Name System 
 */
#define LWIP_DNS                        1

/*
   ----------------------------------------
   ---------- Lwip Debug options ----------
   ----------------------------------------
*/
//#define LWIP_DEBUG                      1

#define LWIP_NETCONN_FULLDUPLEX         LWIP_SOCKET

#define TCPIP_THREAD_STACKSIZE 2048
#define TCPIP_MBOX_SIZE 12
#define DEFAULT_UDP_RECVMBOX_SIZE 12
#define DEFAULT_TCP_RECVMBOX_SIZE 6
#define DEFAULT_ACCEPTMBOX_SIZE 6
#define DEFALUT_THREAD_STACKSIZE 128
#define TCPIP_THREAD_PRIO  9

#define LWIP_COMPAT_MUTEX_ALLOWED 1
#define LWIP_COMPAT_MUTEX  1

#define LWIP_SO_RCVTIMEO 1

#define LWIP_NETIF_STATUS_CALLBACK 1

#define MEMP_NUM_NETBUF 20
#define MEMP_USE_CUSTOM_POOLS 1
//#define DYN_LWIPMEM_POOL
#define DNS_MAX_NAME_LENGTH  REMOTE_SERVER_LEN
#define LWIP_ND6_NUM_NEIGHBORS 2
#define LWIP_ND6_NUM_DESTINATIONS 2
#define LWIP_ND6_NUM_PREFIXES 2
#define LWIP_ND6_NUM_ROUTERS 2
#define MEMP_NUM_ND6_QUEUE 4

#define MEMP_NUM_TCPIP_MSG_INPKT        12

/* The TCP timer interval in milliseconds, tcp_fasttmr is called every TCP_TMR_INTERVAL ms and tcp_slowtmr every 2*TCP_TMR_INTERVAL ms. */
#define TCP_TMR_INTERVAL 500
#define TCP_SYNMAXRTX 2 /* Maximum number of retransmissions of SYN segments. */
#define TCP_MAXRTX 4 /* Maximum number of retransmissions of data segments. */

#define DNS_TABLE_SIZE 4

#define LWIP_NETIF_API 1

#define DEFAULT_RAW_RECVMBOX_SIZE 2
#define LWIP_SO_RCVBUF 1
#define LWIP_CHECKSUM_ON_COPY 1
#define LWIP_IPV6 1
#define LWIP_IPV6_NUM_ADDRESSES 2
#define LWIP_IPV6_SEND_ROUTER_SOLICIT 0
#define LWIP_IPV6_DUP_DETECT_ATTEMPTS 0
#define LWIP_IPV6_MLD  0
#define LWIP_ND6_TCP_REACHABILITY_HINTS 0

#define TCP_MSL 5500UL /* The maximum segment lifetime in milliseconds */

//sntp option
#define LWIP_SNTP 1
#define SNTP_SERVER_DNS  1
#define SNTP_CHECK_RESPONSE  1
#define SNTP_UPDATE_DELAY   60000
#define SNTP_STARTUP_DELAY  0
#define SNTP_MAX_RETRIES    1
#define SNTP_STARTUP_DELAY_FUNC   20000

#define DNS_MAX_RETRIES    2
#define DNS_NAME_ALLOC_IS_DYNAMIC 1

#define SOCKET_ENTRY_ALLOC_IS_DYNAMIC 1

#ifndef netif_get_index
#define netif_get_index(netif)      ((u8_t)((netif)->num + 1))
#endif

#endif /* __LWIPOPTS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

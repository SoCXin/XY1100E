/**
* @file
* Ethernet Interface Skeleton
*
*/

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
 *--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/*
* Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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
* Author: Adam Dunkels <adam@sics.se>
*
*/

/*
* This file is a skeleton for developing Ethernet network interface
* drivers for lwIP. Add code to the low_level functions and do a
* search-and-replace for the word "ethernetif" to replace it with
* something that better describes your network interface.
*/

//#include "stm32f4xx_hal.h"
//#include "xinyim3_hal_eth.h"

#if LWIP_ETHERNET

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "lwip/err.h"
#include "ethernetif.h"
#include <string.h>
#include "../../../../Inc/apl_ps_interface.h"
#include "../../../Adapter/ps_api.h"

#define netifINTERFACE_TASK_STACK_SIZE              ( 4096u )
#define netifINTERFACE_TASK_PRIORITY                ( 6u   )

/* Define those to better describe your network interface. */
//#define IFNAME0 's'
//#define IFNAME1 't'

#define IFNAME0 'x'
#define IFNAME1 'y'

#if 1
/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __ALIGN_END
    #define __ALIGN_END    __attribute__ ((aligned (4)))
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN
    #define __ALIGN_BEGIN
  #endif /* __ALIGN_BEGIN */
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN
    #if defined   (__CC_ARM)      /* ARM Compiler */
      #define __ALIGN_BEGIN    __align(4)
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */


static struct netif* s_pxNetIf = NULL;

#define ETH_MAX_PACKET_SIZE       1524U    /*!< ETH_HEADER + ETH_EXTRA + ETH_VLAN_TAG + ETH_MAX_ETH_PAYLOAD + ETH_CRC */
#define ETH_HEADER                14U      /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETH_CRC                   4U       /*!< Ethernet CRC */
#define ETH_EXTRA                 2U       /*!< Extra bytes in some cases */
#define ETH_VLAN_TAG              4U       /*!< optional 802.1q VLAN Tag */
#define ETH_MIN_ETH_PAYLOAD       46U      /*!< Minimum Ethernet payload size */
#define ETH_MAX_ETH_PAYLOAD       1500U    /*!< Maximum Ethernet payload size */
#define ETH_JUMBO_FRAME_PAYLOAD   9000U    /*!< Jumbo frame payload size */
/* Private variables ---------------------------------------------------------*/
/* Definition of the Ethernet driver buffers size and count */
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */
//#define ETH_RXBUFNB                    ((uint32_t)4U)       /* 4 Rx buffers of size ETH_RX_BUF_SIZE  */
//#define ETH_TXBUFNB                    ((uint32_t)4U)       /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

/* Global Ethernet handle */
 ETH_HandleTypeDef heth;

/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                 ( 500 )

/* Semaphore to signal incoming packets */
sys_sem_t s_xSemaphore;

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
	sys_sem_signal(&s_xSemaphore);
}
//void ethernetif_input( void * pvParameters );
static void arp_timer(void* arg);


/* Private functions ---------------------------------------------------------*/

void HAL_ETH_MspInit(void)
{
  //ok
}

void HAL_ETH_MspDeInit(void)
{
   //ok
}


#endif  //yb
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
* In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
*        for this ethernetif
*/
static void low_level_init(struct netif* netif)
{
    //HAL_StatusTypeDef hal_eth_init_status;
    /* Init ETH */
    uint8_t MACAddr[6] ;
    //heth.Instance = ETH;
    heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    heth.Init.PhyAddress = xinyiNBIot_PHY_ADDRESS;
    
	  MACAddr[0] = 0x00;
    MACAddr[1] = 0x80;
    MACAddr[2] = 0xE1;
    MACAddr[3] = 0x00;
    MACAddr[4] = 0x00;
    MACAddr[5] = 0x00;
    heth.Init.MACAddr = &MACAddr[0];
    heth.Init.RxMode = ETH_RXINTERRUPT_MODE;
    heth.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    //heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

    /* USER CODE BEGIN MACADDRESS */

    /* USER CODE END MACADDRESS */

    /* Initialize Tx Descriptors list: Chain Mode */
 //   (void)HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
 //    (void)HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

#if LWIP_ARP || LWIP_ETHERNET

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] =  heth.Init.MACAddr[0];
    netif->hwaddr[1] =  heth.Init.MACAddr[1];
    netif->hwaddr[2] =  heth.Init.MACAddr[2];
    netif->hwaddr[3] =  heth.Init.MACAddr[3];
    netif->hwaddr[4] =  heth.Init.MACAddr[4];
    netif->hwaddr[5] =  heth.Init.MACAddr[5];

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* Accept broadcast address and ARP traffic */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
#if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
    netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

    s_pxNetIf = netif;

    sys_sem_new(&s_xSemaphore, 1);
    /* create the task that handles the ETH_MAC */
    sys_thread_new((char*)"Eth_if", ethernetif_input, netif, netifINTERFACE_TASK_STACK_SIZE, netifINTERFACE_TASK_PRIORITY);

    /* Enable MAC and DMA transmission and reception */
  //  (void)HAL_ETH_Start(&heth);

#endif /* LWIP_ARP || LWIP_ETHERNET */

}

/**
* This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
*       strange results. You might consider waiting for space in the DMA queue
*       to become availale since the stack doesn't retry to send a packet
*       dropped because of memory failure (except for the TCP timers).
*/
static err_t low_level_output(struct netif* netif, struct pbuf* p)
{
    struct pbuf* q;
    uint8_t* buffer = (uint8_t*)(heth.TxDesc->Buffer1Addr);
 
    uint32_t framelength = 20;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    q = p;
	
	  memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
	
    return 0;
}

/**
* Should allocate a pbuf and transfer the bytes of the incoming
* packet from the interface into the pbuf.
*
* @param netif the lwip network interface structure for this ethernetif
* @return a pbuf filled with the received packet (including MAC header)
*         NULL on memory error
*/
static struct pbuf* low_level_input(struct netif* netif)
{
   struct pbuf* p = NULL;

  	return p;
}

/**
* This function is the ethernetif_input task, it is processed when a packet
* is ready to be read from the interface. It uses the function low_level_input()
* that should handle the actual reception of bytes from the network
* interface. Then the type of the received packet is determined and
* the appropriate input function is called.
*
* @param netif the lwip network interface structure for this ethernetif
*/
void ethernetif_input( void* pvParameters )
{
    struct pbuf* p;
    //uint32_t   uwInterval = 0;
    //uint32_t   uwRet = 0;
    err_t err;

    /* move received packet into a new pbuf */
    while (1)
    {
        sys_arch_sem_wait(&s_xSemaphore, TIME_WAITING_FOR_INPUT);
        do
        {
            SYS_ARCH_DECL_PROTECT(sr);

            SYS_ARCH_PROTECT(sr);
            p = low_level_input(s_pxNetIf);
            SYS_ARCH_UNPROTECT(sr);

            if (p == NULL)
            {
                continue;
            }

            err = s_pxNetIf->input(p, s_pxNetIf);

            if (err != ERR_OK)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                pbuf_free(p);
                p = NULL;
            }
    }while(p != NULL);
    }
}

/**
* Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this ethernetif
* @return ERR_OK if the loopif is initialized
*         ERR_MEM if private data couldn't be allocated
*         any other err_t on error
*/
err_t ethernetif_init(struct netif* netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    netif->output = etharp_output;
    netif->linkoutput = low_level_output;


    /* initialize the hardware */
    low_level_init(netif);

    etharp_init();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

    return ERR_OK;
}


static void arp_timer(void* arg)
{
    etharp_tmr();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}
#endif


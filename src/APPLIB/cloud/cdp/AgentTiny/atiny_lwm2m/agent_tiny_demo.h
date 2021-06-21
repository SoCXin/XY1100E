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

#ifndef __AGENT_TINY_DEMO_H_
#define __AGENT_TINY_DEMO_H_

#include "agenttiny.h"
#include "atiny_osdep.h"
#include "xy_utils.h"
void agent_tiny_entry(void);

typedef enum enum_cdp_msg_type
{
    cdp_CON = 0x00,
    cdp_NON = 0x01,
    cdp_NON_RAI = 0x02,
    cdp_CON_RAI = 0x03,
    cdp_CON_WAIT_REPLY_RAI = 0x04,
}cdp_msg_type_t;

typedef struct
{
    char ip_addr_str[16];
    unsigned   short port;
} cdp_server_settings_t;

typedef struct buffer_list_s
{
    struct buffer_list_s *next;
    char *data;
    int data_len;
    cdp_msg_type_t type;
} buffer_list_t;

typedef struct
{
    int pending_num;
    int sent_num;
    int error_num;

    buffer_list_t *head;
    buffer_list_t *tail;
} upstream_info_t;

typedef struct
{
    int buffered_num;
    int received_num;
    int dropped_num;

    buffer_list_t *head;
    buffer_list_t *tail;
} downstream_info_t;

extern int data_send_flag;
extern int data_send_len;
extern int data_rcv_len;
extern uint8_t *prcvbuf;
extern uint8_t *pdatabuf;

void cdp_lwm2m_init();
int is_cdp_running();
void cdp_lwm2m_process(void);
void app_data_report(void);
void app_downdata_recv(void);


#endif 

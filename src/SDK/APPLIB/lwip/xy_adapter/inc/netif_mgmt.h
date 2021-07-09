#pragma once

/*******************************************************************************
 *						    Include header files							   *
 ******************************************************************************/
#include "osal.h"
#include "lwip/netif.h"

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
struct common_msg
{
	int msg_id;
	int size;
	char data[0];
};

typedef enum
{
	NETIF_MGMT_EVENT_UP = 0,
	NETIF_MGMT_EVENT_DOWN,
	NETIF_MGMT_EVENT_IPV6
} netif_mgmt_event;

typedef void (*netif_event_callback_func)(struct netif* netif, netif_mgmt_event event);

typedef struct
{
	char netif_name[8];
	netif_event_callback_func callback;
	unsigned int netif_mgmt_event_bitmap;
} netif_mgmt_event_callback_t;

typedef struct netif_mgmt_event_callback_node_s
{
	struct netif_mgmt_event_callback_node_s *next;
	netif_mgmt_event_callback_t *event_callback;
} netif_mgmt_event_callback_node_t;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
int netif_mgmt_init_event_callback(netif_mgmt_event_callback_t *event_callback, netif_event_callback_func func, netif_mgmt_event event);
int netif_mgmt_init_event_callback_with_name(netif_mgmt_event_callback_t *event_callback, char *name, 
    netif_event_callback_func func, netif_mgmt_event event);
int netif_mgmt_add_event_callback(netif_mgmt_event_callback_t *event_callback);
int netif_mgmt_del_event_callback(netif_mgmt_event_callback_t *event_callback);
void set_netif_mgmt_status_callback(struct netif *netif);
void netif_mgmt_init();

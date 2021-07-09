/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "at_ps_cmd.h"
#include "at_tcpip_cmd.h"
#include "at_worklock.h"
#include "xy_at_api.h"
#include "xy_utils.h"
#include "ps_netif_api.h"
#include "at_global_def.h"
#include "xy_system.h"
#include "oss_nv.h"
#include "sys_init.h"
#include "ipcmsg.h"
#include "netif_mgmt.h"
#include "softap_macro.h"
#include "low_power.h"
#include "net_app_resume.h"
#include "inter_core_msg.h"
#include "lwip/tcpip.h"
#include "lwip/netifapi.h"
#include "lwip/dns.h"
#if XY_PING
#include "ping.h"
#endif
#if XY_WIRESHARK
#include "xy_wireshark.h"
#endif
#if IS_DSP_CORE
#include "apl_ps_interface.h"
#endif

/*******************************************************************************
 *						  Global variable definitions						   *
 ******************************************************************************/
//used for local sequence for 3GPP reliable transmission
unsigned short g_udp_send_seq[NUM_SOCKETS] = {0};
unsigned char g_udp_send_rai[NUM_SOCKETS] = {0};
struct ps_netif ps_if[PS_PDP_CID_MAX] = {{INVALID_CID, NULL}, {INVALID_CID, NULL}};
nonip_uplink_func g_nonip_func = NULL;
int g_cp_or_up = 0;
osMutexId_t g_udp_send_m = NULL;

//当最后一个上行报文被PS处理完毕后(参见at_UDPRAI_info)，触发RAI的发送
uint16_t g_udp_latest_seq = 0;
//非socket UDP sequence以外的上行报文的起始序列号，用于RAI的触发
uint16_t g_udp_seq = 0XA000;

/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/
void Ps_Set_CpOrUpMode(char cp_or_up)
{
    //g_softap_var_nv->dsp_nv.cp_or_up = cp_or_up;
    g_cp_or_up = cp_or_up;
}

void ip_packet_information_print(unsigned char *data, unsigned short len, char in)
{
	unsigned short total_len;
	unsigned short id;
	unsigned char protocol;
	ip4_addr_t src, dst;
	char ip_src_str[16] = {0};
	char ip_dst_str[16] = {0};

	unsigned int ip_header_len;
	unsigned int sub_header_len = 0;
	unsigned int data_len = 0;
	unsigned char *cur;
	char *link_str = in ? "downlink" : "uplink";
	unsigned short src_port, dst_port;
	unsigned int seqno, ackno;
	unsigned short tcp_flags;
	char tcp_flags_str[16] = {0};

	//check ip header, only surport ipv4 now
	if (len <= 20 || data[0] >> 4 != 4)
	{
		return;
	}
	ip_header_len = (data[0] & 0x0F) * 4;
	total_len = lwip_ntohs(*((unsigned short*)(data + 2)));
	id = lwip_ntohs(*((unsigned short*)(data + 4)));
	protocol = *((unsigned char*)(data + 9));
	src.addr = *((unsigned int*)(data + 12));
	dst.addr = *((unsigned int*)(data + 16));
	inet_ntoa_r(src.addr, ip_src_str, 16);
	inet_ntoa_r(dst.addr, ip_dst_str, 16);
	softap_printf(USER_LOG, WARN_LOG, "ip packet info: total_len %u, id %u, protocol %d, src ip %s, dst ip %s, len %u", total_len, id, protocol, ip_src_str, ip_dst_str, len);
	if (total_len != len)
	{
		return;
	}
	cur = data + ip_header_len;
	if (protocol == IPPROTO_ICMP) // icmp
	{
		sub_header_len = 8;
		data_len = total_len - ip_header_len - sub_header_len;
		softap_printf(USER_LOG, WARN_LOG, "ip packet info: icmp, %s, from %s to %s, data len %u", link_str, ip_src_str, ip_dst_str, data_len);
	}
	else if (protocol == IPPROTO_TCP) // tcp
	{
		src_port = lwip_ntohs(*((unsigned short*)(cur)));
		dst_port = lwip_ntohs(*((unsigned short*)(cur + 2)));
		seqno = lwip_ntohl(*((unsigned int*)(cur + 4)));
		ackno = lwip_ntohl(*((unsigned int*)(cur + 8)));
		sub_header_len = (lwip_ntohs(*((unsigned short*)(cur + 12))) >> 12) * 4;
		tcp_flags = lwip_ntohs(*((unsigned short*)(cur + 12))) & 0x0FFF;
		data_len = total_len - ip_header_len - sub_header_len;
		if (tcp_flags & 0x0010)
		{
			strcat(tcp_flags_str, "A"); // ACK
		}
		if (tcp_flags & 0x0008)
		{
			strcat(tcp_flags_str, "P"); // PUSH
		}
		if (tcp_flags & 0x0004)
		{
			strcat(tcp_flags_str, "R"); // RESET
		}
		if (tcp_flags & 0x0002)
		{
			strcat(tcp_flags_str, "S"); // SYN
		}
		if (tcp_flags & 0x0001)
		{
			strcat(tcp_flags_str, "F"); // FIN
		}
		softap_printf(USER_LOG, WARN_LOG, "ip packet info: tcp, %s, from %s:%d to %s:%d", link_str, ip_src_str, src_port, ip_dst_str, dst_port);
		softap_printf(USER_LOG, WARN_LOG, "ip packet info: seqno %u, ackno %u, flags %s, data len %u", seqno, ackno, tcp_flags_str, data_len);
	}
	else if (protocol == IPPROTO_UDP) // udp
	{
		src_port = lwip_ntohs(*((unsigned short*)(cur)));
		dst_port = lwip_ntohs(*((unsigned short*)(cur + 2)));
		sub_header_len = 8;
		data_len = total_len - ip_header_len - sub_header_len;
		softap_printf(USER_LOG, WARN_LOG, "ip packet info: udp, %s, from %s:%d to %s:%d, data len %u",
			link_str, ip_src_str, src_port, ip_dst_str, dst_port, data_len);
	}
}

//send ip packet to ps,CP or UP
#if IS_DSP_CORE
err_t send_ip_packet_to_ps_net(struct netif *netif, struct pbuf *p)
{
	void *user_ipdata = NULL;
	struct ipdata_info ipdata_info_buf = {0};
	struct pbuf* q;
	unsigned short pos_offset = p->tot_len;
	int udp_seq_id = -1;
	
	if(g_softap_fac_nv->off_debug == 0)
	{
		if (g_softap_fac_nv->g_drop_up_dbg > 0 && (osKernelGetTickCount() % g_softap_fac_nv->g_drop_up_dbg == 0))
		{
			NETDOG_AT_STATICS(dbg_drop_up_sum++);
			goto END;
		}
	}
	
	q = p;
	if(q->tot_len > 0)
	{	
		u16_t offset_to=0;
		
		user_ipdata = (void *)xy_zalloc(pos_offset);
		
		if(user_ipdata == NULL)
			xy_assert(0);
		softap_printf(USER_LOG, WARN_LOG, "ip data write to ps,mem addr=0x%X",user_ipdata);

		do
		{
			if(q->soc_id>0 && q->soc_id<=NUM_SOCKETS)
				udp_seq_id = q->soc_id-1;
			memcpy((char *)user_ipdata + offset_to, (char *)q->payload , q->len);
			offset_to += q->len;
			q = q->next;
		} while (q);
	}

	
	if(udp_seq_id!=-1 && g_udp_send_rai[udp_seq_id] != 0)
		ipdata_info_buf.rai = g_udp_send_rai[udp_seq_id];
	else
		ipdata_info_buf.rai = 0;
	if(g_null_udp_rai == 1)
	{
		ipdata_info_buf.rai = 1;
		ipdata_info_buf.cid = 0xFD;
		g_null_udp_rai = 0;
		softap_printf(USER_LOG, WARN_LOG, "g_null_udp_rai: %d,cid:%d,", g_null_udp_rai,ipdata_info_buf.cid);
	}
	else
		ipdata_info_buf.cid = g_working_cid;
	ipdata_info_buf.data_type = 0;
	ipdata_info_buf.data_len = p->tot_len;

	//used for UDP local sequence for 3GPP reliable transmission
	if(udp_seq_id!=-1 && g_udp_send_seq[udp_seq_id]!=0)
	{
		ipdata_info_buf.sequence = g_udp_send_seq[udp_seq_id];		
	}
	else
	{	
		ipdata_info_buf.sequence = 0;	
	}
	
	if(g_softap_fac_nv->off_debug == 0)
	{
		ip_packet_information_print(user_ipdata,p->tot_len,0);
	}
#if XY_WIRESHARK
	wireshark_forward_format_print(user_ipdata,p->tot_len,0);
#endif
	if(g_cp_or_up == 0)
	{
		API_EPS_CSODCP_SEND_REQ_STRU UserData={0}; 

		UserData.ucCid = ipdata_info_buf.cid;
		UserData.ucRai = ipdata_info_buf.rai;
		UserData.ucDataType = ipdata_info_buf.data_type;
		UserData.usCpdataLen = ipdata_info_buf.data_len;
		UserData.pCpdata = user_ipdata;
        /* NSOSTR */
        UserData.usIpSn    = ipdata_info_buf.sequence;
		if (API_EPS_CSODCP_Send_Req(&UserData, 1) != 0)
	    {
            OSXY_Mem_Free(user_ipdata);
	    }
	}
	else
	{
		T_EPS_USER_DATA_STRU EpsUserData={0}; 

		EpsUserData.ucCid = ipdata_info_buf.cid;
		EpsUserData.ucServiceType = 0;
		EpsUserData.ucFlowCtrl = 0x11;
		EpsUserData.usLength = ipdata_info_buf.data_len;
		EpsUserData.pucData = user_ipdata;
        EpsUserData.usIpSn  = ipdata_info_buf.sequence;
		API_EPS_UserData_Req(&EpsUserData, 1);
		OSXY_Mem_Free(user_ipdata);
	}
END:
	if(udp_seq_id != -1)
	{
		g_udp_send_rai[udp_seq_id] = 0;
		g_udp_send_seq[udp_seq_id] = 0;
	}
	return ERR_OK;
}
#else //M3 CORE
err_t send_ip_packet_to_ps_net(struct netif *netif, struct pbuf *p)
{
	(void)netif;
	void *user_ipdata = NULL;
	struct ipdata_info *ipdata_info = NULL;
	struct pbuf* q;
	int udp_seq_id = -1;
	int ret = ERR_OK;
	if (g_softap_fac_nv->off_debug == 0)
	{
		if (g_softap_fac_nv->g_drop_up_dbg > 0 && (osKernelGetTickCount() % g_softap_fac_nv->g_drop_up_dbg == 0))
		{
			NETDOG_AT_STATICS(dbg_drop_up_sum++);
			goto END;
		}
	}
	if (g_OOS_flag == 1)
	{
		softap_printf(USER_LOG, WARN_LOG, "net work OOS!!!");
		ret = ERR_IF;
		send_debug_str_to_at_uart("+DBGINFO:SEND IPDATA ERR_IF\r\n");
		goto END;
	}
	if (DSP_IS_NOT_LOADED())
	{
		softap_printf(USER_LOG, WARN_LOG, "dsp not runned and packet dropped!!!");
		goto END;
	}

	q = p;
	if (q->tot_len > 0)
	{
		uint16_t offset_to = 0;
		user_ipdata = (void *)xy_zalloc_Align(p->tot_len);
		softap_printf(USER_LOG, WARN_LOG, "ip data write to dsp,mem addr=0x%X", user_ipdata);

		do
		{
			if (q->soc_id > 0 && q->soc_id <= NUM_SOCKETS)
				udp_seq_id = q->soc_id - 1;
			memcpy((char *)user_ipdata + offset_to, (char *)q->payload , q->len);
			offset_to += q->len;
			q = q->next;
		} while (q);
	}

	ipdata_info = (struct ipdata_info *)xy_zalloc(sizeof(struct ipdata_info));
	ipdata_info->data_type = 0;
	ipdata_info->data_len = p->tot_len;
	ipdata_info->data = (void *)get_Dsp_Addr_from_ARM((unsigned int)user_ipdata);

	if (g_null_udp_rai == 1)
	{
		ipdata_info->rai = 1;
		ipdata_info->cid = 0xFD;
		g_null_udp_rai = 0;
		softap_printf(USER_LOG, WARN_LOG, "g_null_udp_rai: %d,cid:%d", g_null_udp_rai,ipdata_info->cid);
	}
	else if (udp_seq_id != -1 && g_udp_send_rai[udp_seq_id] != 0)
	{
		ipdata_info->rai = g_udp_send_rai[udp_seq_id];
		ipdata_info->cid = g_working_cid;
	}
	else
	{
		ipdata_info->rai = 0;
		ipdata_info->cid = g_working_cid;
	}

	//对标海思，提供socket的UDP扩展sequence
	if (udp_seq_id != -1 && g_udp_send_seq[udp_seq_id] != 0)
	{
		ipdata_info->sequence = g_udp_send_seq[udp_seq_id];
		g_udp_latest_seq = g_udp_send_seq[udp_seq_id];
	}
	//当最后一个上行报文被PS处理完毕后(参见at_UDPRAI_info)，触发RAI的发送
	else
	{	
		g_udp_latest_seq = g_udp_seq;
		ipdata_info->sequence = g_udp_seq;	
		g_udp_seq++;	
	}

	if (g_softap_fac_nv->off_debug == 0)
	{
		ip_packet_information_print(user_ipdata, p->tot_len, 0);
	}
#if XY_WIRESHARK
	wireshark_forward_format_print(user_ipdata, p->tot_len, 0);
#endif

	if (shm_msg_write(ipdata_info, sizeof(struct ipdata_info), ICM_IPDATA) < 0)
	{
		softap_printf(USER_LOG, WARN_LOG, "write data failed and ip packet dropped!!!");
		xy_free(user_ipdata);
	}
	xy_free(ipdata_info);
END:
	if (udp_seq_id != -1)
	{
		g_udp_send_rai[udp_seq_id] = 0;
		g_udp_send_seq[udp_seq_id] = 0;
	}
	return ret;
}
#endif //IS_DSP_CORE

//send ip packet to ps,ref to netif_loop_output_ipv4
err_t netif_ps_output_ipv4(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr)
{
	(void)ipaddr;
	return send_ip_packet_to_ps_net(netif,p);
}

err_t netif_ps_output_ipv6(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
	(void)ipaddr;
	return send_ip_packet_to_ps_net(netif,p);
}


//when pdp act,create ps netif ,refer  to  netif_loopif_init
err_t ps_netif_init(struct netif* netif)
{
	struct pdp_active_info *pdpinfo=(struct pdp_active_info *)(netif->state);
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'x';
    netif->name[1] = 'y';
	
    netif->linkoutput = NULL;
#if LWIP_IPV4
	if(pdpinfo->ip46flag == IPV4_TYPE || pdpinfo->ip46flag == IPV46_TYPE)
	{
    	netif->output = (netif_output_fn)netif_ps_output_ipv4;
	}
#endif

#if LWIP_IPV6
	if(pdpinfo->ip46flag == IPV6_TYPE || pdpinfo->ip46flag == IPV46_TYPE)
	{
		netif->output_ip6 = (netif_output_ip6_fn)netif_ps_output_ipv6;	
		netif_set_ip6_autoconfig_enabled(netif,1);
		netif_ip6_addr_set_parts(netif, 0, pdpinfo->ipv6.addr[0],pdpinfo->ipv6.addr[1],pdpinfo->ipv6.addr[2],pdpinfo->ipv6.addr[3]);
		netif_ip6_addr_set_state(netif, 0, IP6_ADDR_VALID);
	}
	else
	{
		#if LWIP_IPV6_SEND_ROUTER_SOLICIT
  			netif->rs_count = 0;
		#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */
	}
#endif
    return ERR_OK;
}
struct ps_netif *find_netif_by_cid(unsigned char cid)
{
	int i = 0;
	for (; i < PS_PDP_CID_MAX; i++)
	{
		if (ps_if[i].cid == cid)
			return ps_if + i;
	}

	return NULL;
}

void add_netif(unsigned char cid, struct netif *ps_eth)
{
	int i = 0;
	for (; i < PS_PDP_CID_MAX; i++)
	{
		if (ps_if[i].ps_eth == NULL && ps_if[i].cid == INVALID_CID)
		{
			ps_if[i].ps_eth = ps_eth;
			ps_if[i].cid = cid;
			return;
		}
	}
	xy_assert(0);
}

void delete_netif(unsigned char cid)
{
	int i = 0;
	for (; i < PS_PDP_CID_MAX; i++)
	{
		if (ps_if[i].cid == cid)
		{
			//主控线程处理完pdp激活消息后，pdp_active_info会被释放，lwip内部如果使用会出现野指针，因此额外申请了一块内存，需单独释放
			if (ps_if[i].ps_eth->state != NULL)
			{
				xy_free(ps_if[i].ps_eth->state);
				ps_if[i].ps_eth->state = NULL;
			}
			if (ps_if[i].ps_eth != NULL)
			{
				xy_free(ps_if[i].ps_eth);
				ps_if[i].ps_eth = NULL;
			}
			ps_if[i].cid = INVALID_CID;
			return;
		}
	}
	xy_assert(0);
}

struct netif* find_active_netif()
{
	int i = 0;
	for (; i < PS_PDP_CID_MAX; i++)
	{
		if (ps_if[i].ps_eth != NULL)
			return ps_if[i].ps_eth;
	}
	return NULL;
}

bool is_netif_active(unsigned char cid)
{
	int i = 0;
	for (; i < PS_PDP_CID_MAX; i++)
	{
		if (ps_if[i].cid == cid && ps_if[i].ps_eth != NULL)
			return true;
	}
	return false;
}

//deliver IP packet to lwip tcpip stack
int send_ps_packet_to_tcpip(void *data, unsigned short len, unsigned char cid)
{
	int err;
	struct pbuf *p;
	struct ps_netif *ps_temp;

	ps_temp = find_netif_by_cid(cid);
	if (ps_temp == NULL || !netif_is_link_up(ps_temp->ps_eth))
	{
		xy_free(data);
		return XY_ERR;
	}

    resume_net_app_by_dl_pkt(data, len);

	p = pbuf_alloc(PBUF_RAW, len, PBUF_REF);
	xy_assert(p != NULL);

	p->payload = data;
	p->payload_original = data; //pbuf->payload的指针位置随时会被修改
	err = ps_temp->ps_eth->input(p, ps_temp->ps_eth);
	if (err != ERR_OK)
	{
		softap_printf(USER_LOG, WARN_LOG, "send ps packet to tcpip ERR!");
		pbuf_free(p); //tcpip_input的返回,需要手动释放pbuf
		return XY_ERR;
	}
	return XY_OK;
}

//process  downlink PS  data,if AT CRTDCP ,need send to at_ctl,other send to tcpip stack
#if IS_DSP_CORE
void send_packet_to_user(unsigned char cid, int len, char *data)
{
	char *rcv_data = xy_zalloc(len);
	memcpy(rcv_data, data, len);

	if (g_user_lock_released == 1)
	{
		softap_printf(USER_LOG, WARN_LOG, "worklock have free,but recv downlink packet!!!");
		ip_packet_information_print(rcv_data,len,1);
	}
	
	if(g_softap_fac_nv->off_debug == 0)
	{
		if (g_softap_fac_nv->g_drop_down_dbg > 0 && osKernelGetTickCount() % g_softap_fac_nv->g_drop_down_dbg == 0)
		{
			NETDOG_AT_STATICS(dbg_drop_down_sum++);
			return;
		}
	}

	if(g_softap_fac_nv->off_debug == 0)
	{
		ip_packet_information_print(rcv_data,len,1);
	}

#if XY_WIRESHARK
	wireshark_forward_format_print((void *)rcv_data,pipdata_info->data_len,1);
#endif
#if XY_PING
	if (now_in_ping)
        send_packet_to_ping(rcv_data,len);
    else
#endif
	{
		if(g_double_trance != 0)
		{
			int i = 0;
			for (i = 0; i < g_double_trance_num; i++)
			{
				send_ps_packet_to_tcpip(rcv_data,len, cid);
			}
		}
		else
			send_ps_packet_to_tcpip(rcv_data,len, cid);
		softap_printf(USER_LOG, WARN_LOG, "double_trance:%d,double_trance_num:%d", g_double_trance,g_double_trance_num);
	}
}
#else //M3 CORE
void send_packet_to_user(unsigned char cid, int msg_len, char *msg_buf)
{
	(void)msg_len;
	(void)cid;
	softap_printf(USER_LOG, WARN_LOG, "recv ipdata from dsp!!");
	unsigned int user_ipdata = 0;
	struct ipdata_info *pipdata_info = (struct ipdata_info *)(msg_buf);

	user_ipdata = (unsigned int)pipdata_info->data;
	is_SRAM_addr(user_ipdata);

	if(HAVE_FREE_LOCK==1)
	{

		softap_printf(USER_LOG, WARN_LOG, "worklock have free,but recv downlink packet!!!");
		ip_packet_information_print((unsigned char *)user_ipdata,pipdata_info->data_len,1);
	}
	
	if(g_softap_fac_nv->off_debug == 0)
	{
		if (g_softap_fac_nv->g_drop_down_dbg > 0 && osKernelGetTickCount() % g_softap_fac_nv->g_drop_down_dbg == 0)
		{
			NETDOG_AT_STATICS(dbg_drop_down_sum++);
			xy_free((void*)user_ipdata);
			return;
		}
	}

	if(g_softap_fac_nv->off_debug == 0)
	{
		ip_packet_information_print((unsigned char *)user_ipdata,pipdata_info->data_len,1);
	}
	
#if XY_WIRESHARK
	wireshark_forward_format_print((void *)user_ipdata,pipdata_info->data_len,1);
#endif
	if(g_nonip_func != NULL)
	{
		softap_printf(USER_LOG, WARN_LOG, "proc nonip packet!!!");
		g_nonip_func((char *)user_ipdata, pipdata_info->data_len);
		xy_free((void*)user_ipdata);
	}
#if XY_PING
	else if (now_in_ping)
        send_packet_to_ping((char *)user_ipdata, pipdata_info->data_len);
#endif
	else
	{
		if(g_double_trance != 0)
		{
			for (int i = 0; i < g_double_trance_num; i++)
			{
				send_ps_packet_to_tcpip((void *)user_ipdata, pipdata_info->data_len, pipdata_info->cid);
			}
		}
		else
			send_ps_packet_to_tcpip((void *)user_ipdata, pipdata_info->data_len, pipdata_info->cid);
	}

}
#endif //IS_DSP_CORE

//pdp激活
void ps_netif_activate(struct pdp_active_info *pdp_info)
{
	struct netif *temp;
	ip_addr_t pridns_t = {0};
	ip_addr_t secdns_t = {0};
	
	temp = xy_zalloc(sizeof(struct netif));
	add_netif(pdp_info->cid, temp);
	if(pdp_info->ip46flag==IPV4_TYPE||pdp_info->ip46flag==IPV46_TYPE)
	{
		/* DNS servers */
		char default_pridns[16] = {0};
		char default_secdns[16] = {0};
		at_parse_param("%16s,%16s", (char *)g_softap_fac_nv->ref_dns, default_pridns, default_secdns);
		if(pdp_info->ip4_info.pri_dns.addr == 0)
		{
			//set nv default ip,for example 114.114.114.114
			inet_pton(AF_INET, (const char*)default_pridns, &pdp_info->ip4_info.pri_dns);
		}
		memcpy(&pridns_t.u_addr.ip4,&pdp_info->ip4_info.pri_dns,sizeof(ip4_addr_t));
		pridns_t.type = IPADDR_TYPE_V4;
		
		dns_setserver(0, &pridns_t);
		if(pdp_info->ip4_info.sec_dns.addr == 0)
		{
			inet_pton(AF_INET, (const char*)default_secdns, &pdp_info->ip4_info.sec_dns);
		}
		memcpy(&secdns_t.u_addr.ip4,&pdp_info->ip4_info.sec_dns,sizeof(ip4_addr_t));
		secdns_t.type = IPADDR_TYPE_V4;

		dns_setserver(1, &secdns_t);
		
	}	
	if(pdp_info->ip46flag==IPV6_TYPE || pdp_info->ip46flag==IPV46_TYPE)
	{
	}

	//主控线程处理完pdp激活消息后，pdp_active_info会被释放，lwip内部如果使用会出现野指针，需要额外申请一块内存
	struct pdp_active_info *pdp_info_backup = (struct pdp_active_info *)xy_zalloc(sizeof(struct pdp_active_info));
	memcpy(pdp_info_backup, pdp_info, sizeof(struct pdp_active_info));

	netifapi_netif_add(temp,&pdp_info->ip4_info.ip,NULL,NULL, (void*)(pdp_info_backup), ps_netif_init, tcpip_input);
	set_netif_mgmt_status_callback(temp);
	netifapi_netif_set_default(temp);	
	netifapi_netif_set_link_up(temp);
	netifapi_netif_set_up(temp);
}

//pdp去激活
int ps_netif_deactivate(unsigned char cid, unsigned short ip46flag)
{
	if (ip46flag == IPV4_TYPE)
	{
		struct ps_netif *netif_tmp = find_netif_by_cid(cid);
		if(netif_tmp == NULL)
		{
			softap_printf(USER_LOG, WARN_LOG, "deactive cid:%u pdp fail", cid);
			return XY_ERR;
		}

		netifapi_netif_remove(netif_tmp->ps_eth);
		delete_netif(cid);
	}
	return XY_OK;
}

//only create basic task,user task must dynamic start by AT REQ
int ps_netif_is_ok()
{
	return ((netif_default != NULL) && netif_is_up(netif_default) && netif_is_link_up(netif_default));
}

void proc_cgatt_rsp(char *at_str)
{
	//仅金卡用户定制
	if (!CHECK_SDK_TYPE(JINKA_VER))
		return;

	char *chr = at_strstr2(at_str, "+CGATT:");
	if (chr == NULL || *chr == '\0')
		return;

	while (*chr == ' ')
		chr++;
	if ((int)strtol(chr,NULL,10) == 1)
	{
		if (g_working_cid != INVALID_CID && !ps_netif_is_ok())
		{
			//收到XYIPDNS消息，已经置working cid但是网口尚未启动
			softap_printf(USER_LOG, WARN_LOG, "working cid has set but netif not started");
			*chr = '0';
		}	
		softap_printf(USER_LOG, WARN_LOG, "final cgatt:%s", at_str);
	}
}

#if !IS_DSP_CORE
int send_nonip_packet_to_ps(char *data, int data_len, int rai)
{
	void *user_ipdata = NULL;
	struct ipdata_info *ipdata_info = NULL;

	if(DSP_IS_NOT_LOADED())
	{
		softap_printf(USER_LOG, WARN_LOG, "dsp not runned and packet dropped!!!");
		goto END;
	}
	
	user_ipdata = (void *)xy_zalloc_Align(data_len);
	memcpy((char *)user_ipdata, (char *)data, data_len);
			
	ipdata_info = (struct ipdata_info *)xy_zalloc(sizeof(struct ipdata_info));

	ipdata_info->cid = g_working_cid;
	ipdata_info->rai = rai;
	ipdata_info->data_type = 0;
	ipdata_info->data_len = data_len;
	ipdata_info->sequence = 0;
	ipdata_info->data = (void *)get_Dsp_Addr_from_ARM((unsigned int)user_ipdata);

	if(g_softap_fac_nv->off_debug == 0)
	{
		ip_packet_information_print(user_ipdata,data_len,0);
	}
	wireshark_forward_format_print(user_ipdata,data_len,0);

	if(shm_msg_write(ipdata_info,sizeof(struct ipdata_info),ICM_IPDATA) < 0)
	{
		softap_printf(USER_LOG, WARN_LOG, "write data failed and packet dropped!!!");
		xy_free(user_ipdata);
	}
	xy_free(ipdata_info);
END:
	return ERR_OK;
}
#endif //!IS_DSP_CORE

void acquire_udp_send_mutex(void)
{
	if (g_udp_send_m == NULL)
		g_udp_send_m = osMutexNew(osMutexPrioInherit);
	osMutexAcquire(g_udp_send_m, osWaitForever);
}

void release_udp_send_mutex(void)
{
	if (g_udp_send_m != NULL)
		osMutexRelease(g_udp_send_m);
}
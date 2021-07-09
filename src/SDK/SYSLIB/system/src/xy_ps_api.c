/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_ps_api.h"
#include "xy_net_api.h"
#include "at_com.h"
#include "at_ctl.h"
#include "at_ps_cmd.h"
#include "inter_core_msg.h"
#include "ipcmsg.h"
#include "ps_netif_api.h"
#include "shm_msg_api.h"
#include "xy_system.h"
#include "xy_utils.h"

/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/
int xy_cfun_excute(int status)
{
	int ret = XY_ERR;
	int i = 0;
	//send at+cfun=0 & at+cfun=1
	while (1)
	{
		if (status == NET_CFUN0)
		{
			ret = at_ReqAndRsp_to_ps("AT+CFUN=0\r\n", NULL, 0);
		}
		else if (status == NET_CFUN1)
		{
			ret = at_ReqAndRsp_to_ps("AT+CFUN=1\r\n", NULL, 0);
		}
		else
		{
			xy_assert(0);
		}
		if (ret == AT_OK || i > 50)
			break;
		i++;
		osDelay(100);
	}
	if (ret == AT_OK)
		return XY_OK;
	else
		return XY_ERR;
}

int xy_cfun_read(int *cfun)
{
	if(at_ReqAndRsp_to_ps("AT+CFUN?\r\n", "%d", 5, cfun) == AT_OK)
		return XY_OK;
	else
		return XY_ERR;
}

int xy_get_CGACT(int *cgact)
{
	if(at_ReqAndRsp_to_ps("AT+CGACT?\r\n", ",%d", 5, cgact) == AT_OK)
		return XY_OK;
	else
		return XY_ERR;
}

int xy_cereg_read(int *cereg)
{
	if(at_ReqAndRsp_to_ps("AT+CEREG?\r\n", ",%d,", 5, &cereg) == AT_OK)
		return XY_OK;
	else
		return XY_ERR;
}

int xy_get_SN(char *sn, int len)
{
	if (len < SN_LEN || sn == NULL)
		return XY_ERR;
	if (at_ReqAndRsp_to_ps("AT+CGSN=0\r\n", "%s", 5, sn) != AT_OK)
	{
		softap_printf(USER_LOG, WARN_LOG, "get sn fail!!!");
		return XY_ERR;
	}
	return XY_OK;
}

int xy_get_RSSI(int *rssi)
{
	int ret = XY_ERR;
	ret = syn_send_shmmsg_req(SHM_MSG_PLAT_PS_RSSI, NULL, 0, rssi, sizeof(unsigned int));
	return ret;
}

int xy_get_IMEI(char *imei, int len)
{
	if (len < IMEI_LEN || imei == NULL)
		return XY_ERR;

	if (strlen((const char *)(query_psinfo.meid)) == 0)
	{
		if (syn_send_shmmsg_req(SHM_MSG_PLAT_PS_IMEI, NULL, 0, imei, IMEI_LEN) == XY_ERR)
			return XY_ERR;
		xy_assert(strlen(imei) > 0);
		memcpy(query_psinfo.meid, imei, strlen(imei));
	}
	else
	{
		memcpy(imei, query_psinfo.meid, strlen((const char *)(query_psinfo.meid)));
	}
	return XY_OK;
}

int xy_get_IMSI(char *imsi, int len)
{
	if (len < IMSI_LEN || imsi == NULL)
		return XY_ERR;

	if (strlen((const char *)(query_psinfo.imsi)) == 0)
	{
		if(syn_send_shmmsg_req(SHM_MSG_PLAT_PS_IMSI, NULL, 0, imsi, IMSI_LEN) == XY_ERR)
			return XY_ERR;
		xy_assert(strlen(imsi) > 0);
		memcpy(query_psinfo.imsi, imsi, strlen(imsi));
	}
	else
	{
		memcpy(imsi, query_psinfo.imsi, strlen((const char *)(query_psinfo.imsi)));
	}
	return XY_OK;
}

int xy_get_CELLID(int *cell_id)
{
	if (query_psinfo.cell_id == 0)
	{
		if(syn_send_shmmsg_req(SHM_MSG_PLAT_PS_CELLID, NULL, 0, cell_id, sizeof(int)) == XY_ERR)
			return XY_ERR;
		query_psinfo.cell_id = *cell_id;

	}
	else
	{
		*cell_id = query_psinfo.cell_id;
	}
	return XY_OK;
}

int xy_get_NCCID(char *ccid, int len)
{
	if (len < UCCID_LEN)
		return XY_ERR;

	if (g_ueiccid != NULL)
	{
		memcpy(ccid, g_ueiccid, UCCID_LEN);
		return XY_OK;
	}

	if (at_ReqAndRsp_to_ps("AT+NCCID\r", "%s", 5, ccid) != AT_OK)
	{
		softap_printf(USER_LOG, WARN_LOG, "get nccid fail!!!");
		return XY_ERR;
	}
	if (g_ueiccid == NULL)
		g_ueiccid = xy_zalloc(UCCID_LEN);

	memcpy(g_ueiccid, ccid, UCCID_LEN);

	return XY_OK;
}

int xy_get_PDP_APN(char *apn_buf, int len, int cid)
{    
	int query_cid = 0;

	if (len < APN_LEN || apn_buf == NULL)
		return XY_ERR;

	if (cid == -1)
		query_cid = g_working_cid;
	else
		query_cid = cid;

	char *apn_req = xy_zalloc(APN_LEN);
	if (syn_send_shmmsg_req(SHM_MSG_PLAT_PS_APN, &query_cid, sizeof(int), apn_req, APN_LEN) != XY_OK)
	{
		return XY_ERR;
	}
	//如果协议栈获取失败，apn buffer中会记录ERR字符
	if (apn_req != NULL && !strncmp(apn_req, "ERR", strlen("ERR")))
	{
		softap_printf(USER_LOG,WARN_LOG,"get pdp apn from dsp fail!");
		xy_free(apn_req);
		return XY_ERR;
	}
	else
	{
		memcpy(apn_buf,apn_req,APN_LEN);
		xy_free(apn_req);
		return XY_OK;
	}
	
}

int xy_get_working_cid()
{
	return g_working_cid;
}

int xy_getIP4Addr(char *ipAddr, int addrLen)
{
	if (xy_wait_tcpip_ok(0) == XY_ERR || ipAddr == NULL || addrLen < IPADDR_LEN)
		return XY_ERR;

	if (inet_ntop(AF_INET, &netif_default->ip_addr.u_addr.ip4, ipAddr, addrLen) == NULL)
		return XY_ERR;

	return XY_OK;
}

//该接口内部禁止调用softap_printf，可调用send_debug_str_to_at_uart
int xy_get_ip4addr(unsigned int *ip4addr)
{
	if (xy_wait_tcpip_ok(1) == XY_ERR)
		return XY_ERR;
	
	*ip4addr = (unsigned int)(netif_default->ip_addr.u_addr.ip4.addr);
	return XY_OK;
}

int xy_get_T_ACT(int *t3324)
{
	int ret = XY_ERR;
	ret = syn_send_shmmsg_req(SHM_MSG_PLAT_PS_T_ACT, NULL, 0, t3324, sizeof(int));
	return ret;
}

int xy_get_T_TAU(int *tau)
{
	int ret = XY_ERR;
	ret = syn_send_shmmsg_req(SHM_MSG_PLAT_PS_T_TAU, NULL, 0, tau, sizeof(int));
	return ret;
}

int xy_send_rai()
{
	if(at_ReqAndRsp_to_ps("AT+RAI=1\r\n", NULL, 10) == AT_OK)
		return XY_OK;
	return XY_ERR;
}

int xy_get_HARDVER(char *hardver, int len)
{
	if (len < HARDVER_LEN)
		return XY_ERR;
	snprintf(hardver, HARDVER_LEN, "%s", g_softap_fac_nv->hardver);
	return XY_OK;
}

int xy_get_VERSIONEXT(char *versionExt, int len)
{
	if (len < VERSIONEXT_LEN)
		return XY_ERR;
	snprintf(versionExt, VERSIONEXT_LEN, "%s", g_softap_fac_nv->versionExt);
	return XY_OK;
}

int xy_get_MODULVER(char *modul_ver, int len)
{
	if (len < MODULVER_LEN)
		return XY_ERR;
	snprintf(modul_ver, MODULVER_LEN, "%s", g_softap_fac_nv->modul_ver);
	return XY_OK;
}

int xy_get_UICC_TYPE(int *uicc_type)
{
	int ret = XY_ERR;
	ret = syn_send_shmmsg_req(SHM_MSG_PLAT_PS_UICC_TYPE, NULL, 0, uicc_type, sizeof(int));
	return ret;
}

int xy_get_eDRX_value(float *eDRX_value, float *ptw_value)
{
	int act_type = 0;
	char eDRX_bit[5] = {0};
	char ptw_bit[5] = {0};
	int i;
	int j = 8;
	int eDRX_Dec = 0;
	int ptw_Dec = 0;
	if (at_ReqAndRsp_to_ps("AT+CEDRXRDP\r\n", "%d,,%s,%s", 5, &act_type, eDRX_bit, ptw_bit) != AT_OK)
	{
		softap_printf(USER_LOG, WARN_LOG, "get eDRX value and ptw value fail!!!");
		return XY_ERR;
	}
	//当前接入的网络不使用eDRX（终端或网络不支持eDRX）
	if (act_type == 0)
	{
		return XY_ERR;
	}

	//将二进制编码解析为十进制数一一匹配
	for (i = 0; i < 4; i++)
	{
		if (*(ptw_bit + i) == '1')
		{
			ptw_Dec += j;
		}

		if (*(eDRX_bit + i) == '1')
		{
			eDRX_Dec += j;
		}
		j = j / 2;
	}

	softap_printf(USER_LOG, WARN_LOG, "eDRX_Dec:%d ,ptw_Dec:%d", eDRX_Dec, ptw_Dec);
	switch (eDRX_Dec)
	{
	case 2:
		*eDRX_value = (1 << 1) * EDRX_BASE_TIME;
		break;

	case 3:
		*eDRX_value = (1 << 2) * EDRX_BASE_TIME;
		break;

	case 5:
		*eDRX_value = (1 << 3) * EDRX_BASE_TIME;
		break;

	case 9:
		*eDRX_value = (1 << 4) * EDRX_BASE_TIME;
		break;

	case 10:
		*eDRX_value = (1 << 5) * EDRX_BASE_TIME;
		break;

	case 11:
		*eDRX_value = (1 << 6) * EDRX_BASE_TIME;
		break;

	case 12:
		*eDRX_value = (1 << 7) * EDRX_BASE_TIME;
		break;

	case 13:
		*eDRX_value = (1 << 8) * EDRX_BASE_TIME;
		break;

	case 14:
		*eDRX_value = (1 << 9) * EDRX_BASE_TIME;
		break;

	case 15:
		*eDRX_value = (1 << 10) * EDRX_BASE_TIME;
		break;
	}

	*ptw_value = (ptw_Dec + 1) * PTW_BASE_TIME;

	return XY_OK;
}

int xy_get_servingcell_info(ril_serving_cell_info_t *rcv_serving_cell_info)
{
	xy_assert(rcv_serving_cell_info != NULL);
	int ret = XY_OK;
	char *temp_char = NULL;
	char *buffer = (char *)xy_zalloc(400);

	if (at_ReqAndRsp_to_ps("AT+NUESTATS=RADIO\r\n", "%a", 10, buffer) != AT_OK)
	{
		ret = XY_ERR;
		goto END_PROC;
	}

	memset(rcv_serving_cell_info, 0, sizeof(ril_serving_cell_info_t));

	if ((temp_char = at_strstr2(buffer, "+Signal power:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->Signalpower);
	if (rcv_serving_cell_info->Signalpower == 0)
	{
		ret = XY_ERR;
		goto END_PROC;	
	}

	if ((temp_char = at_strstr2(buffer, "+Total power:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->Totalpower);

	if ((temp_char = at_strstr2(buffer, "+TX power:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->TXpower);

	if ((temp_char = at_strstr2(buffer, "+Cell ID:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->CellID);

	if ((temp_char = at_strstr2(buffer, "+ECL:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->ECL);

	if ((temp_char = at_strstr2(buffer, "+SNR:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->SNR);

	if ((temp_char = at_strstr2(buffer, "+EARFCN:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->EARFCN);

	if ((temp_char = at_strstr2(buffer, "+PCI:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->PCI);

	if ((temp_char = at_strstr2(buffer, "+RSRQ:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->RSRQ);

	if ((temp_char = at_strstr2(buffer, "+TAC:")) != NULL)
		at_parse_param("%s", temp_char, &rcv_serving_cell_info->tac);

	if ((temp_char = at_strstr2(buffer, "+SBAND:")) != NULL)
		at_parse_param("%d", temp_char, &rcv_serving_cell_info->sband);

END_PROC:
	xy_free(buffer);
	return ret;
}

int xy_get_neighborcell_info(ril_neighbor_cell_info_t *ril_neighbor_cell_info)
{
	xy_assert(ril_neighbor_cell_info != NULL);
	int ret = XY_OK;
	char *temp_char = NULL;
	char *buffer = (char *)xy_zalloc(200);

	if (at_ReqAndRsp_to_ps("AT+NUESTATS=CELL\r\n", "%a", 10, buffer) != AT_OK)
	{
		ret = XY_ERR;
		goto END_PROC;
	}
	memset(ril_neighbor_cell_info, 0, sizeof(ril_neighbor_cell_info_t));
	temp_char = buffer;

	while (((temp_char = at_strstr2(temp_char, "CELL,")) != NULL) && ril_neighbor_cell_info->nc_num < 5)
	{
		at_parse_param("%d,%d,,%d,,,", temp_char, &ril_neighbor_cell_info->neighbor_cell_info[ril_neighbor_cell_info->nc_num].nc_earfcn, 
												  &ril_neighbor_cell_info->neighbor_cell_info[ril_neighbor_cell_info->nc_num].nc_pci, 
												  &ril_neighbor_cell_info->neighbor_cell_info[ril_neighbor_cell_info->nc_num].nc_rsrp);
		ril_neighbor_cell_info->nc_num++;
	}

END_PROC:
	xy_free(buffer);
	return ret;
}

int xy_get_phy_info(ril_phy_info_t *rcv_phy_info)
{
	xy_assert(rcv_phy_info != NULL);
	int ret = XY_OK;
	char *temp_char = NULL;
	char *buffer = (char *)xy_zalloc(400);

	if (at_ReqAndRsp_to_ps("AT+NUESTATS=BLER\r\n", "%a", 10, buffer) != AT_OK)
	{
		ret = XY_ERR;
		goto END_PROC;
	}

	memset(rcv_phy_info, 0, sizeof(ril_phy_info_t));

	if ((temp_char = at_strstr2(buffer, "RLC UL BLER,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->RLC_UL_BLER);

	if ((temp_char = at_strstr2(buffer, "RLC DL BLER,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->RLC_DL_BLER);

	if ((temp_char = at_strstr2(buffer, "MAC UL BLER,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_UL_BLER);

	if ((temp_char = at_strstr2(buffer, "MAC DL BLER,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_DL_BLER);

	if ((temp_char = at_strstr2(buffer, "Total TX bytes,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_UL_total_bytes);

	if ((temp_char = at_strstr2(buffer, "Total RX bytes,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_DL_total_bytes);

	if ((temp_char = at_strstr2(buffer, "Total TX blocks,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_UL_total_HARQ_TX);

	if ((temp_char = at_strstr2(buffer, "Total RX blocks,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_DL_total_HARQ_TX);

	if ((temp_char = at_strstr2(buffer, "Total RTX blocks,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_UL_HARQ_re_TX);

	if ((temp_char = at_strstr2(buffer, "Total ACK/NACK RX,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_DL_HARQ_re_TX);

	//重置buffer
	memset(buffer, 0, 400);

	if (at_ReqAndRsp_to_ps("AT+NUESTATS=THP\r\n", "%a", 10, buffer) != AT_OK)
	{
		ret = XY_ERR;
		memset(rcv_phy_info, 0 ,sizeof(ril_phy_info_t));
		goto END_PROC;
	}

	if ((temp_char = at_strstr2(buffer, "THP,RLC UL,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->RLC_UL_tput);

	if ((temp_char = at_strstr2(buffer, "THP,RLC DL,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->RLC_DL_tput);

	if ((temp_char = at_strstr2(buffer, "THP,MAC UL,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_UL_tput);

	if ((temp_char = at_strstr2(buffer, "THP,MAC DL,")) != NULL)
		at_parse_param("%d", temp_char, &rcv_phy_info->MAC_DL_tput);

END_PROC:
	xy_free(buffer);
	return ret;
}

int xy_get_radio_info(ril_radio_info_t *rcv_radio_info)
{
	xy_assert(rcv_radio_info != NULL);
	memset(rcv_radio_info, 0, sizeof(ril_radio_info_t));

	if (xy_get_servingcell_info(&rcv_radio_info->serving_cell_info) != XY_OK)
	{
		return XY_ERR;
	}

	if (xy_get_neighborcell_info(&rcv_radio_info->neighbor_cell_info) != XY_OK)
	{
		return XY_ERR;
	}

	if (xy_get_phy_info(&rcv_radio_info->phy_info) != XY_OK)
	{
		return XY_ERR;
	}
	return XY_OK;
}

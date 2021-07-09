#include "net_app_resume.h"
#include "oss_nv.h"
#include "xy_proxy.h"

//#define  RESUME_ALL_CLOUD          0   //#0#下行包恢复指定云业务，匹配五元组,#1#下行包恢复所有云业务，仅匹配下行包的目的IP和本地IP

osMutexId_t g_cloud_resume_mutex= NULL;
osMutexId_t g_cdp_keepalive_update_mutex= NULL;
osMutexId_t g_cis_keepalive_update_mutex= NULL;

unsigned int g_app_recovery_type = 0;      //需要恢复的云业务类型，bit0:socket,bit2:onenet,bit3:cdp
unsigned int g_app_local_ip = 0xFFFF;
int g_have_read_saved_info = 0; //云业务恢复只读一次flash的标志位，每次上电初始化，读取flash之后 置为1

//该接口用于fastoff时判断是否需要擦除云保存相关NV，保存数据需要擦除，未保存不需要擦除
bool is_exist_resume_cloud_data()
{
    if(!g_softap_fac_nv->save_cloud)
        return false;

    unsigned int net_recovery_type =0;

    if(xy_ftl_read(NV_FLASH_NET_BASE, (unsigned char*)&net_recovery_type, sizeof(net_recovery_type))!= XY_OK)
        return true;

    if(net_recovery_type != 0)
        return true;
    else
        return false;
}

//该接口内部禁止调用softap_printf，可调用send_debug_str_to_at_uart
void save_net_app_infos(void)
{
    int ret = -1;
    unsigned int ip_addr = 0;
    app_deepsleep_infos_t *net_info = NULL;

    if(!g_softap_fac_nv->save_cloud)
        return ;
//深睡保存时，未成功获取IP地址依然保存云会话数据
    xy_get_ip4addr(&ip_addr);

    net_info = xy_zalloc(sizeof(app_deepsleep_infos_t));
    xy_ftl_read(NV_FLASH_NET_BASE, (unsigned char*)net_info, sizeof(app_deepsleep_infos_t));
   
#if AT_SOCKET            
    ret = save_udp_context_deepsleep(&(net_info->udp_context));
    SET_NET_RECOVERY_FLAG(SOCKET_TASK);
#endif

#if MOBILE_VER
    ret = onenet_write_regInfo(&(net_info->onenet_regInfo));
    SET_NET_RECOVERY_FLAG(ONENET_TASK);
#endif

#if TELECOM_VER
    ret = cdp_write_regInfo(&(net_info->cdp_regInfo));
    SET_NET_RECOVERY_FLAG(CDP_TASK);
#endif
//防止保存数据时候内存泄漏
    if(g_app_recovery_type == 0)
    {
        if(net_info)
            xy_free(net_info);
        return ;
    }
    //保存recovery_flag和IP地址
    net_info->recovery_type= g_app_recovery_type;
	net_info->local_addr = ip_addr;
    xy_ftl_write(NV_FLASH_NET_BASE,0,(unsigned char *)net_info, sizeof(app_deepsleep_infos_t));

    if(net_info)
        xy_free(net_info);
    return ;
}

int is_IP_changed()
{
    unsigned int infos[2] = {0,0};//0:recovery_flag;1:local_ip
    unsigned int get_local_ip = 0;

    if(xy_ftl_read(NV_FLASH_NET_BASE, (unsigned char*)infos, sizeof(infos)) != XY_OK)
        return IP_RECEIVE_ERROR;
    if(xy_get_ip4addr(&get_local_ip) == XY_ERR || infos[1] == 0)
        return IP_RECEIVE_ERROR;

    if(infos[1] == get_local_ip)
        return IP_NO_CHANGED;
    else
        return IP_IS_CHANGED;
}

int match_net_app_by_touple(net_app_type_t type, void *data, int offset)
{
#if RESUME_ALL_CLOUD
    return 1;
#endif
    net_infos_t  net_info;
    unsigned int remote_ip;
    unsigned int src_ip;
    unsigned short src_port;
    unsigned short local_port;
    unsigned char protocol;
    
    src_ip  = *((unsigned int*)(data+12)); 
    src_port = *((unsigned int*)(data+20));
    src_port = (src_port>>8) + (src_port<<8);

    if(type == SOCKET_TASK)
    {
        xy_ftl_read(NV_FLASH_NET_BASE + offset, (unsigned char*)&net_info, sizeof(short));
        protocol = *((unsigned char*)(data + 9));
        local_port = *((unsigned short*)(data+22));
        local_port = (local_port>>8) + (local_port<<8);
        if((net_info.local_port == local_port) && (protocol == 17))
            return 1;
    }
    else
    {
        xy_ftl_read(NV_FLASH_NET_BASE + offset, (unsigned char*)&net_info, sizeof(net_info));
        inet_aton(net_info.remote_ip, (struct in_addr *)&remote_ip);
        if(remote_ip == src_ip && net_info.remote_port == src_port)
            return 1;
    }

    return 0;
}

int resume_net_app(net_app_type_t type)
{
    unsigned int infos[2] = {0,0};
    int check_result = XY_OK;

    if(!g_softap_fac_nv->save_cloud)
        return RESUME_SWITCH_INACTIVE;

    softap_printf(USER_LOG, WARN_LOG,"[resume_net_app] resume_net_app %d!\n",type);
    osMutexAcquire(g_cloud_resume_mutex, osWaitForever);

	//read flash saved cloud flag
    if(g_have_read_saved_info == 0)
    {
        g_have_read_saved_info = 1;
        if(xy_ftl_read(NV_FLASH_NET_BASE, (unsigned char*)infos, sizeof(infos))!= XY_OK)
        {
            softap_printf(USER_LOG, WARN_LOG,"[resume_net_app] flash read error %d!\n",type);
            osMutexRelease(g_cloud_resume_mutex);
            return RESUME_READ_FLASH_FAILED;
        }

        g_app_recovery_type = infos[0];
        g_app_local_ip = infos[1];
    }

    if(!NET_NEED_RECOVERY(type))
    {
        osMutexRelease(g_cloud_resume_mutex);
        return RESUME_FLAG_ERROR;
    }

#if AT_SOCKET 
    if(type == SOCKET_TASK)
    {   
        resume_socket_app(OFFSET_NETINFO_PARAM(udp_context));
        g_app_recovery_type&=(~(1<<SOCKET_TASK));
        osMutexRelease(g_cloud_resume_mutex);
        return RESUME_SUCCEED;
    }   
#endif

#if MOBILE_VER
    if(type == ONENET_TASK)
    {
        check_result = onenet_resume_check_info(OFFSET_NETINFO_PARAM(onenet_regInfo));
        if(check_result == XY_OK)
        {
            onet_resume_task();
        }
        else if((check_result == LIFETIME_TIMEOUT) || (check_result == REG_CONFIG_ERROR))
        {
           //清除onenet恢复标志位
           if(g_softap_fac_nv->save_cloud)
           {
               g_app_recovery_type&=(~(1<<ONENET_TASK));
               xy_ftl_write(NV_FLASH_NET_BASE,0, (unsigned char*)&g_app_recovery_type, sizeof(int));
           }
        }

        g_app_recovery_type&=(~(1<<ONENET_TASK));
        osMutexRelease(g_cloud_resume_mutex);
        return check_result;
    }   
#endif

#if TELECOM_VER
    if(type == CDP_TASK)
    {
        check_result = cdp_resume_check_info(OFFSET_NETINFO_PARAM(cdp_regInfo));
        if(check_result == XY_OK )
        {
            cdp_resume_task();
        }
        else if(check_result == LIFETIME_TIMEOUT && g_softap_fac_nv->keep_cloud_alive)
        {
            send_msg_2_proxy(PROXY_MSG_RESUME_CDP_EXCEPTION,NULL,0);
        }
        else if(check_result == REG_CONFIG_ERROR || (check_result == LIFETIME_TIMEOUT && !g_softap_fac_nv->keep_cloud_alive))
        {
            //配置校验失败,清除cdp恢复标志位
            if(g_softap_fac_nv->save_cloud)
            {
                g_app_recovery_type&=(~(1<<CDP_TASK));
                xy_ftl_write(NV_FLASH_NET_BASE,0, (unsigned char*)&g_app_recovery_type, sizeof(int));
            }
        }

        g_app_recovery_type&=(~(1<<CDP_TASK));
        osMutexRelease(g_cloud_resume_mutex);
        return check_result;
    }   
#endif               

    osMutexRelease(g_cloud_resume_mutex);
    return RESUME_OTHER_ERROR;
}


//when DRX/eDRX period,user may start other new cloud connect,so must care conflict
int  resume_net_app_by_dl_pkt(void *data, unsigned short len)
{
    int ret = NO_TASK;
    unsigned char ip_type;
    unsigned int dst_ip;
    unsigned int infos[2]={0,0};//0:recovery_flag;1:local_ip
    
    if(!g_softap_fac_nv->save_cloud)
        return NO_TASK;

    osMutexAcquire(g_cloud_resume_mutex, osWaitForever);

	//read flash saved cloud flag
    if(g_have_read_saved_info == 0)
    {
        softap_printf(USER_LOG, WARN_LOG,"[resume_net_app_by_dl_pkt] xy_ftl_read!\n");
        xy_ftl_read(NV_FLASH_NET_BASE, (unsigned char*)infos, sizeof(infos));
        g_app_recovery_type = infos[0];
        g_app_local_ip = infos[1];        
        g_have_read_saved_info = 1;
    }
    
    if(g_app_recovery_type == 0)
    {
        softap_printf(USER_LOG, WARN_LOG,"[resume_net_app_by_dl_pkt] recovery_type = 0!\n");
        osMutexRelease(g_cloud_resume_mutex);
        return NO_TASK;
    }

    //从数据包中获取ip_type
    ip_type = *((unsigned char*)data);
    ip_type = (ip_type & 0xf0)>>4;

    if(ip_type != 0x4)
    {
        osMutexRelease(g_cloud_resume_mutex);
        return UNKNOWN_IP;
    }
    
    dst_ip = *((unsigned int*)(data+16));
    softap_printf(USER_LOG, WARN_LOG,"compare ip packet dst_ip:%x,dst_ip:%x!\n",dst_ip,g_app_local_ip);
	//if PDP IP address changed, not resume cloud
    if(dst_ip != g_app_local_ip)
    {   
        softap_printf(USER_LOG, WARN_LOG,"[resume_net_app_by_dl_pkt] !dst_ip:%x!\n", g_app_local_ip);
        osMutexRelease(g_cloud_resume_mutex);
        return UNKNOWN_IP;
    }

    softap_printf(USER_LOG, WARN_LOG,"eDRX recv downlink ip packet, resume start!\n");

#if AT_SOCKET
    if(NET_NEED_RECOVERY(SOCKET_TASK))
    {
        if(match_net_app_by_touple(SOCKET_TASK, data, OFFSET_NETINFO_PARAM(udp_context))||
            match_net_app_by_touple(SOCKET_TASK, data, OFFSET_NETINFO_PARAM(udp_context)+sizeof(struct udp_context_fd)))
        {   
            resume_net_app(SOCKET_TASK);
            ret = SOCKET_TASK;
            send_debug_str_to_at_uart("+DBGINFO:[SOCKET] RECOVERY\r\n");
            //after read flash, clear recovery flag bit0:0
            g_app_recovery_type&=(~(1<<SOCKET_TASK));
        } 
    }

#endif

#if MOBILE_VER
    if(NET_NEED_RECOVERY(ONENET_TASK))
    {
        if(match_net_app_by_touple(ONENET_TASK, data, OFFSET_NETINFO_PARAM(onenet_regInfo)))
        {   
            resume_net_app(ONENET_TASK);
            ret = ONENET_TASK;
            send_debug_str_to_at_uart("+DBGINFO:[CIS] RECOVERY\r\n");
            g_app_recovery_type&=(~(1<<ONENET_TASK));
        } 
    }

#endif

#if TELECOM_VER
    if(NET_NEED_RECOVERY(CDP_TASK))
    {
        if(match_net_app_by_touple(CDP_TASK, data, OFFSET_NETINFO_PARAM(cdp_regInfo)))
        {   
            resume_net_app(CDP_TASK);      
            ret = CDP_TASK;
            send_debug_str_to_at_uart("+DBGINFO:[CDP] RECOVERY\r\n");
            g_app_recovery_type&=(~(1<<CDP_TASK));
        }  
    }
#endif

    if(ret >= 0)
    {
        softap_printf(USER_LOG, WARN_LOG,"eDRX recv downlink ip packet,and resume success!\n");
    }
    
    osMutexRelease(g_cloud_resume_mutex);
    return ret;
}


void cloud_resume_init(void)
{
    if(g_cloud_resume_mutex == NULL)
        g_cloud_resume_mutex = osMutexNew(osMutexRecursive);
#if TELECOM_VER
    if(g_cdp_keepalive_update_mutex == NULL)
        g_cdp_keepalive_update_mutex = osMutexNew(osMutexPrioInherit);
#endif
#if MOBILE_VER
    if(g_cis_keepalive_update_mutex == NULL )
        g_cis_keepalive_update_mutex = osMutexNew(osMutexPrioInherit);
#endif
}

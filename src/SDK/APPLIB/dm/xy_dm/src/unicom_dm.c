#include "xy_at_api.h"
#include "at_ps_cmd.h"
#include "at_global_def.h"
#include "softap_nv.h"
#include "factory_nv.h"
#include "oss_nv.h"
#include "low_power.h"
#include "xy_utils.h"
#include "xy_system.h"
#include "lwip/netdb.h"

static osSemaphoreId_t dm_sem = NULL;
static unsigned char *modul_ver = NULL;
static char *requestbuf = NULL;
osThreadId_t g_unicom_dm_TskHandle =NULL;
static osTimerId_t dm_timer = NULL;
static osTimerId_t dm_timer_overdue = NULL;
static char unicom_timer_flag = 0;
int current_time = 0;
void unicom_dm_init(void);

#define DM_HOST "114.255.193.236"
#define DM_PORT 9999

static void FormJson(unsigned char *buf,unsigned char regver,unsigned char* immeid,unsigned char* modul_ver,unsigned char* versionExt,unsigned char* sim1iccid,unsigned char* sim1lteimsi)
{	
	int n;	n = sprintf((char *)buf,"{\"REGVER\":\"%d\",",regver);		
	n += sprintf((char *)buf+n,"\"MEID\":\"%s\",",immeid);
	n += sprintf((char *)buf+n,"\"MODELSMS\":\"%s\",",modul_ver);	
	n += sprintf((char *)buf+n,"\"SWVER\":\"%s\",",versionExt);			
	n += sprintf((char *)buf+n,"\"SIM1ICCID\":\"%s\",",sim1iccid);			
	n += sprintf((char *)buf+n,"\"SIM1LTEIMSI\":\"%s\"}",sim1lteimsi);		
}

static int handle_connection(int sockfd)
{
	int n;

	softap_printf(USER_LOG, WARN_LOG, "[DM]sockfd:%d and begin to send data\n",sockfd);

	n = send(sockfd, requestbuf, strlen(requestbuf), 0);//Notice:the size of ip packets��UDP<=1500-2*CONFIG_NET_BUF_DATA_SIZE��TCP<=MSS��the value of MSSNET_TCP_DEFAULT_MSS is in tcp.h����
	if (n < 0) {
		softap_printf(USER_LOG, WARN_LOG, "[DM]client: server is closed.\n");
		close(sockfd);
		return(-1);
	} else {
		softap_printf(USER_LOG, WARN_LOG, "[DM]--->send datalen:[%d],data:\n", strlen(requestbuf));
		dm_print_str(requestbuf);
	}

	memset(requestbuf, 0, 512);
	n = recv(sockfd, requestbuf, 512, 0);
	if (n == 0) {
		softap_printf(USER_LOG, WARN_LOG, "[DM]client: server is closed.\n");

	} else if (n < 512){
		n = recv(sockfd, requestbuf+n, 512-n, 0);
		softap_printf(USER_LOG, WARN_LOG, "[DM]--->recv datalen:[%d],data:\n",strlen(requestbuf));
		dm_print_str(requestbuf);
	}		
	close(sockfd);
    softap_printf(USER_LOG, WARN_LOG, "[DM]close fd %d", sockfd);
	
	if(findstr(requestbuf,"Success")>0)    
	{		
        softap_printf(USER_LOG, WARN_LOG, "[DM]unicom_dm success");
        return 0;
	}	 
	else	
	{		
		return 1;  
	}
}

static int dm_socket_client(void)
{
	struct sockaddr_in server = {0};		
	int fd = 0;
	int rc;
	//struct addrinfo *result;
	char ip_buff[16] = {0};
	
	server.sin_addr.s_addr=0xecc1ff72;//use default addr:114.255.193.236

	softap_printf(USER_LOG, WARN_LOG, "[DM]get unicom server addr success: %s\n", inet_ntop(AF_INET,&server.sin_addr.s_addr,ip_buff,16));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(DM_PORT);

	softap_printf(USER_LOG, WARN_LOG, "[DM]begin create socket\n");
	fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if ( fd < 0 ) {
		softap_printf(USER_LOG, WARN_LOG, "[DM]error,fd=%d, errno=%d\n",fd,errno);
	}
	else
	{
		softap_printf(USER_LOG, WARN_LOG, "[DM]creat ok and begin connect\n");
		if((rc =connect(fd,(struct sockaddr *)&server,sizeof(server))) == 0)
		{
			softap_printf(USER_LOG, WARN_LOG, "[DM]success!\n");
			return(handle_connection(fd));
		}
		softap_printf(USER_LOG, WARN_LOG, "[DM]close%d\n",fd);
		close(fd);
	}	
	return (rc);

}

static void dm_make_packet(char *nccid_dm)
{
	int i=0;
	char *base64;	 
	int n;
	memset(requestbuf,0,512); 
	base64 = xy_zalloc(384);
	
	FormJson((unsigned char *)requestbuf,g_softap_fac_nv->regver, query_psinfo.meid, modul_ver,
		g_softap_fac_nv->versionExt, nccid_dm, query_psinfo.imsi); 
	dm_print_str(requestbuf);
	base64_encode(requestbuf,base64,strlen(requestbuf));						
	memset(requestbuf,0,512); 
	n = sprintf(requestbuf,"POST / HTTP/1.1\r\n");		
	n += sprintf(requestbuf+n,"Content-type:application/encrypted-json\r\n");		
	n += sprintf(requestbuf+n,"Content-length:%d\r\n",strlen(base64));
	n += sprintf(requestbuf+n,"Host:%s:%d\r\n\r\n",DM_HOST,DM_PORT);
	for(;i<strlen(base64);i++)
	{
		requestbuf[n+i]= base64[i];
	}
	xy_free(base64);
}

static void dm_register_overdue_proc(void)
{
	softap_printf(USER_LOG, WARN_LOG, "[DM]dm register overdue and start dm!!!\n");
	unicom_timer_flag = 1;
	
	unicom_dm_init();
}

static void dm_timeout_proc(void *para)
{
	(void) para;

	softap_printf(USER_LOG, WARN_LOG, "[DM]start dm again!!!\n");
	osSemaphoreRelease(dm_sem);
}

char *unicom_is_start_dm()
{
	char* nccid_dm_temp = NULL;
	
	nccid_dm_temp = xy_zalloc(UCCID_LEN);
	xy_get_NCCID(nccid_dm_temp,UCCID_LEN);
	
	if(nccid_dm_temp != NULL)
		softap_printf(USER_LOG, WARN_LOG, "[DM]dm iccid:[%s],nv store iccid:[%s]\n",nccid_dm_temp,g_softap_var_nv->dm_cfg.ue_iccid);
	else
		softap_printf(USER_LOG, WARN_LOG, "[DM]dm iccid:[NULL],nv store iccid:[%s]\n",g_softap_var_nv->dm_cfg.ue_iccid);
	
	current_time = 0;
	//get_ext_msg_sync(GLOBAL_TIME,4,&current_time,&len,&current_time); //get current time
	current_time = xy_rtc_get_sec(0);
	softap_printf(USER_LOG, WARN_LOG, "current_time=%d,last_reg_time=%d,unicom_timer_flag=%d\n",current_time,g_softap_var_nv->dm_cfg.last_reg_time,unicom_timer_flag);
	if(nccid_dm_temp != NULL && (strncmp((const char *)nccid_dm_temp, (const char *)g_softap_var_nv->dm_cfg.ue_iccid,20) != 0 || unicom_timer_flag == 1 
		|| current_time-g_softap_var_nv->dm_cfg.last_reg_time > g_softap_fac_nv->dm_reg_time))
	{
		softap_printf(USER_LOG, WARN_LOG, "[DM]need start dm\n");
		return nccid_dm_temp;
	}
	else
	{
		if(nccid_dm_temp != NULL && strncmp((const char *)nccid_dm_temp, (const char *)g_softap_var_nv->dm_cfg.ue_iccid,20) == 0)
		{
			if(dm_timer_overdue != NULL)
			{
				osTimerDelete(dm_timer_overdue);
				dm_timer_overdue = NULL;
			}
			dm_timer_overdue = osTimerNew((osTimerFunc_t)(dm_register_overdue_proc), osTimerOnce, NULL, "dm_register_overdue");
			osTimerStart(dm_timer_overdue, g_softap_fac_nv->dm_reg_time*1000);
			softap_printf(USER_LOG, WARN_LOG, "[DM]dm overdue timer restart\n");
#if !IS_DSP_CORE
			if(get_sys_up_stat()==POWER_ON && unicom_timer_flag==0 && CHECK_SDK_TYPE(OPERATION_VER))
			 	xy_work_lock(0);
#endif
		}
		else if(nccid_dm_temp == NULL)
		{
			softap_printf(USER_LOG, WARN_LOG, "cellid is NULL,do not dm\n");
		}
		if(nccid_dm_temp != NULL)
			xy_free(nccid_dm_temp);
		
		return NULL;
	}
}

#if XY_DM
void unicom_dm_run()
{
	osDelay(1000);
#if !IS_DSP_CORE
	if(get_sys_up_stat()==POWER_ON && unicom_timer_flag==0 && CHECK_SDK_TYPE(OPERATION_VER))
	 	xy_work_lock(0);
#endif

	char *nccid_dm = unicom_is_start_dm();
	
	if(nccid_dm == NULL)
		goto out;
	
	if(modul_ver == NULL)
		modul_ver = xy_zalloc(21);
	if(requestbuf == NULL)
		requestbuf = xy_zalloc(512);
	g_softap_var_nv->dm_cfg.have_retry_num=0;
	dm_sem = osSemaphoreNew(0xFFFF, 0);
	if(dm_timer_overdue != NULL)
	{
		osTimerDelete(dm_timer_overdue);
		dm_timer_overdue = NULL;
	}
	dm_timer = osTimerNew((osTimerFunc_t)(dm_timeout_proc), osTimerOnce, NULL, "dm_register_timeout");
	if(g_softap_var_nv->dm_cfg.have_retry_num==0)
		goto FIRST_POWERON;
	while(1)
	{
        if(osSemaphoreAcquire(dm_sem, osWaitForever) != osOK)
			continue;
		if (!ps_netif_is_ok()) 
			goto out;
	
FIRST_POWERON:		
		{			
			char i,flag = 0;
			memset(modul_ver,0,21); 
			memcpy(modul_ver, g_softap_fac_nv->modul_ver, strlen((const char *)g_softap_fac_nv->modul_ver));
			for(i = 0; i < 20; i++){
				if(modul_ver[i] == '-' && flag == 0){
					flag = 1;
					continue;
				}
				if(modul_ver[i] == '-')
					modul_ver[i] = ' ';
			}
		}

		char *imei_temp = xy_zalloc(16);
		xy_get_IMEI(imei_temp,16);	
		xy_free(imei_temp);
		
        if(strlen((const char *)(query_psinfo.meid)) == 0)
        {
			softap_printf(USER_LOG, WARN_LOG, "[DM]meid is NULL\n");
			goto out;

        }
		softap_printf(USER_LOG, WARN_LOG, "[DM]meid:[%s]\n",query_psinfo.meid);
		
		char *imsi_temp = xy_zalloc(16);		
		xy_get_IMSI(imsi_temp,16);
		xy_free(imsi_temp);
	
		if(strlen((const char *)(query_psinfo.imsi)) == 0 || (query_psinfo.imsi[0] == '0' && query_psinfo.imsi[1] == '0' && (query_psinfo.imsi[2] == '1' || query_psinfo.imsi[2] == '2')))
		{
			softap_printf(USER_LOG, WARN_LOG, "[DM]imsi is invalid\n");
			goto out;

		}
		softap_printf(USER_LOG, WARN_LOG, "[DM]dm imsi:[%s]\n",query_psinfo.imsi);

		softap_printf(USER_LOG, WARN_LOG, "[DM]begin to get dns server addr\n");

		dm_make_packet(nccid_dm);
		if(dm_socket_client() == 0)
		{
			softap_printf(USER_LOG, WARN_LOG, "[DM]dm success and store iccid");
			strncpy((char *)g_softap_var_nv->dm_cfg.ue_iccid,(char *)nccid_dm,20);  //store g_ueiccid,20 byte
			//nv_save_by_user(NV_SOFTAP_WORKING);
			//unicom_timer_flag = 0;
			g_softap_var_nv->dm_cfg.last_reg_time = current_time;
			dm_timer_overdue = osTimerNew((osTimerFunc_t)(dm_register_overdue_proc), osTimerOnce, NULL, "dm_register_overdue");
			osTimerStart(dm_timer_overdue, g_softap_fac_nv->dm_reg_time*1000);
			NETDOG_AT_STATICS(dbg_dm_success_num++);
			if(xy_ftl_write(NV_FLASH_DSP_VOLATILE_BASE,DSP_VOLATILE_ALL_LEN-4,((unsigned char *)g_softap_var_nv),sizeof(softap_var_nv_t))!=XY_OK)
			{
				xy_assert(0);
			}
			send_rsp_str_to_ext("\r\n+DM:SUCCESS\r\n");
			//save_volatile_nv_dynamic();
			//if(unicom_timer_flag == 1)
			//	xy_assert((at_ReqAndRsp_to_ps("AT+WORKLOCK=0\r\n", NULL, 5))==0);
			unicom_timer_flag = 0;
			break;
		}
		else
		{
			g_softap_var_nv->dm_cfg.have_retry_num++;
			NETDOG_AT_STATICS(dbg_dm_failed_num++);
			softap_printf(USER_LOG, WARN_LOG, "[DM]Register Fail\n");
			if (g_softap_var_nv->dm_cfg.have_retry_num < g_softap_fac_nv->dm_retry_num)
			{	
				osTimerStart(dm_timer, g_softap_fac_nv->dm_retry_time*1000);
			}
			else
				break;
		}
	}
out:
	if(nccid_dm != NULL)
		xy_free(nccid_dm);
	if(modul_ver!=NULL)
	{
		xy_free(modul_ver);
		modul_ver = NULL;
	}
	if(requestbuf!=NULL)
	{
		xy_free(requestbuf);
		requestbuf = NULL;
	}
	if(dm_sem != NULL)
	{
    	osSemaphoreDelete(dm_sem);
		dm_sem = NULL;
	}
	if (dm_timer != NULL)
	{
		osTimerDelete(dm_timer);
		dm_timer = NULL;
	}
	softap_printf(USER_LOG, WARN_LOG, "[DM]dm thread exit\n");
#if !IS_DSP_CORE	
	 if(get_sys_up_stat()==POWER_ON && unicom_timer_flag==0 && CHECK_SDK_TYPE(OPERATION_VER))
	 	xy_work_unlock(0); //operator version
#endif
	g_unicom_dm_TskHandle = NULL;
	osThreadExit();
}
#endif


void unicom_dm_init(void)
{
	if(g_unicom_dm_TskHandle != NULL)
	{
		softap_printf(USER_LOG, WARN_LOG, "[DM]dm task is running,not need dm\n");
		return;
	}
#if XY_DM

	g_unicom_dm_TskHandle = osThreadNew((osThreadFunc_t)(unicom_dm_run), NULL, "unicom_dm", 0x800, XY_OS_PRIO_NORMAL1);

#endif
}



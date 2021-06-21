/*****************************************************************************
* Copyright (c) 2020 ABUP.Co.Ltd. All rights reserved.
* File name: abup_fota.c
* Description: fota
* Author: WQH
* Version: v1.0
* Date: 20200311
*****************************************************************************/
//make all 2>&1 | tee build.log
#include "abup_fota.h"
#include <stdbool.h>
#include "lwip/netdb.h"
#include "xy_http_client.h"
#include "at_http.h"
#include "abup_define.h"
#include "xy_at_api.h"

#define ABUP_TEST 0
#define MAX_MONTIOR_COUNT 10

typedef struct  
{
	unsigned char ratio;
	char *socket_buf;
	bool is_quit;
	unsigned int pdp_count;
	unsigned char monitor_count;
	int http_id;
}abup_task_info;

static void abup_montior_end(void);

static abup_task_info *s_abup_task_data = NULL;
static osThreadId_t abup_socket_handle = NULL;
static osTimerId_t abup_fota_timer = NULL;
static osTimerId_t abup_sem_timer = NULL;
static osSemaphoreId_t abup_fota_sem = NULL;
static unsigned int abup_msg_id = 0;

static void abup_task_init(void)
{	
	if (abup_http_init() == false)
	{
		return;
	}

	if (s_abup_task_data == NULL)
	{
		s_abup_task_data = (abup_task_info *)xy_malloc(sizeof(abup_task_info));
		if (s_abup_task_data == NULL)
		{
			xy_printf("ABUP-->xy_malloc fail.");
			return;
		}
		memset(s_abup_task_data, 0, sizeof(abup_task_info));
	}

	if (s_abup_task_data->socket_buf == NULL)
	{
		s_abup_task_data->socket_buf = (char *)xy_malloc(ABUP_SOCKET_BUFFER_LEN);
		if (s_abup_task_data->socket_buf == NULL)
		{
			xy_printf("ABUP-->xy_malloc fail, task exit.");
			s_abup_task_data->is_quit = true;
			return;
		}
	}

	s_abup_task_data->http_id = -1;
	s_abup_task_data->is_quit = false;

	if (abup_is_sn_exist() == false)
	{
		xy_printf("ABUP-->sn code not exist.");
		if (abup_get_fota_status() == ABUP_FOTA_CV)
		{
			abup_set_fota_status(ABUP_FOTA_UA);
		}
		else
		{
			xy_printf("ABUP-->sn error, task exit.");
			s_abup_task_data->is_quit = true;
			abup_clear_version();
			return;
		}
	}
	else if (abup_get_pid_psec() == false)
	{
		xy_printf("ABUP-->device should register.");
		if (abup_get_fota_status() == ABUP_FOTA_CV)
		{
			abup_set_fota_status(ABUP_FOTA_RG);
		}
	}

	if (abup_get_fota_status() != ABUP_FOTA_RU)
	{
		abup_set_fota_ratio(0);
	}
}

#if ABUP_TEST
static void abup_check_version_timeout(void *para)
{
	abup_check_version();

	osTimerDelete(abup_fota_timer);
	abup_fota_timer = NULL;
}
#endif

static void abup_task_deinit(void)
{
	abup_montior_end();

	if (s_abup_task_data != NULL)
	{
		abup_free_dns();
		abup_close_socket();

		if (s_abup_task_data->socket_buf != NULL)
		{
			xy_free(s_abup_task_data->socket_buf);
			s_abup_task_data->socket_buf = NULL;
		}

		xy_free(s_abup_task_data);
		s_abup_task_data = NULL;
	}

	if(abup_fota_sem != NULL)
	{
		osSemaphoreDelete(abup_fota_sem);
		abup_fota_sem = NULL;
	}

	abup_http_deinit();

#if ABUP_TEST
	if (abup_fota_timer == NULL)
	{
		abup_fota_timer = osTimerNew(abup_check_version_timeout, osTimerOnce, NULL, "abup_fota_timer");
		osTimerStart(abup_fota_timer, 10 * 1000);
	}
#endif
}

static void abup_montior_timer(void *para)
{
	abup_montior_end();

	if (s_abup_task_data == NULL)
	{
		return;
	}

	s_abup_task_data->monitor_count++;
	xy_printf("ABUP-->monitor retry %d.", s_abup_task_data->monitor_count);
	if (s_abup_task_data->monitor_count > MAX_MONTIOR_COUNT)
	{
		abup_task_message_send(ABUP_TASK_MSGID_EXIT);
	}
	else
	{
		abup_fota_retry();
	}
}

static void abup_montior_start(void)
{
	if (abup_fota_timer == NULL)
	{
		abup_fota_timer = osTimerNew(abup_montior_timer, osTimerOnce, NULL, "abup_fota_timer");
		osTimerStart(abup_fota_timer, 10 * 1000);
	}
	else
	{
		xy_printf("abup_montior_start fail!");
	}
}

static void abup_montior_end(void)
{
	if (abup_fota_timer != NULL)
	{
		//osTimerStop(abup_fota_timer);
		osTimerDelete(abup_fota_timer);
		abup_fota_timer = NULL;
	}
}

static void abup_fota_socket_connect(void)
{
	int ret;

	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}

	if (s_abup_task_data->http_id < 0)
	{
		char host_buf[64] = {0};

		sprintf(host_buf, "%s:%s", abup_get_host_domain(), abup_get_host_port());
		s_abup_task_data->http_id = xy_http_create(host_buf, "", "");
		xy_printf("ABUP-->http_create id=%d.", s_abup_task_data->http_id);
		if (s_abup_task_data->http_id < 0)
		{
			abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CLOSE);
			abup_montior_start();
			return;
		}
	}

	ret = xy_http_connect(s_abup_task_data->http_id);
	xy_printf("ABUP-->http_connect ret=%d.", ret);
	if (ret < 0)
	{
		abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CLOSE);
		abup_montior_start();
	}
	else
	{
		abup_task_message_send(ABUP_TASK_MSGID_SOCKET_WRITE);
	}
}

void abup_multi_print(char *buf)
{
	char print_buf[54] = {0};
	int i, j;
	int len = strlen(buf);

	for (i = 0, j = 0; i < len; i++)
	{
		if (buf[i] == 0x0A)
		{
			print_buf[j] = 0;
			xy_printf(print_buf);
			osDelay(20);
			j = 0;
		}
		else if (buf[i] != 0x0D)
		{
			print_buf[j++] = buf[i];
		}
		if (j >= 52)
		{
			print_buf[j] = 0;
			xy_printf(print_buf);
			osDelay(20);
			j = 0;
		}
	}
	if (j != 0)
	{
		print_buf[j] = 0;
		xy_printf(print_buf);
		osDelay(20);
	}
}

static void abup_fota_socket_write(void)
{
	int ret;

	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}

	abup_get_http_content(s_abup_task_data->socket_buf, ABUP_SOCKET_BUFFER_LEN);
	xy_http_content(s_abup_task_data->http_id, s_abup_task_data->socket_buf, ESCAPE_MECHANISM);
	abup_get_http_header(s_abup_task_data->socket_buf, ABUP_SOCKET_BUFFER_LEN, strlen(s_abup_task_data->socket_buf));
	xy_http_header(s_abup_task_data->http_id, s_abup_task_data->socket_buf, ESCAPE_MECHANISM);
	abup_get_http_path(s_abup_task_data->socket_buf, ABUP_SOCKET_BUFFER_LEN);
	if (abup_get_fota_status() != ABUP_FOTA_DL)
	{
		ret = xy_http_request(s_abup_task_data->http_id, HTTP_METHOD_POST, s_abup_task_data->socket_buf);
	}
	else
	{
		ret = xy_http_request(s_abup_task_data->http_id, HTTP_METHOD_GET, s_abup_task_data->socket_buf);
	}

	if (ret < 0)
	{
		xy_printf("ABUP-->socket send failed!");
		abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CLOSE);
		abup_montior_start();
	}
	else
	{
		xy_printf("ABUP-->socket send success.");
		abup_task_message_send(ABUP_TASK_MSGID_SOCKET_READ);
	}

	xy_http_content(s_abup_task_data->http_id, "", ESCAPE_MECHANISM);
	xy_http_header(s_abup_task_data->http_id, "", ESCAPE_MECHANISM);
}


static void abup_fota_socket_read(void)
{
	int r = 0;
	unsigned int socket_waitint_time = 3000;

	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}

	while(1)
	{
		memset(s_abup_task_data->socket_buf, 0, ABUP_SOCKET_BUFFER_LEN);
		r = 0;
		//xy_printf("ABUP-->abup_fota_socket_read running...");
		xy_printf("ABUP-->abup_get_delta_data_left_len = %d", abup_get_delta_data_left_len());

		if (abup_get_delta_data_left_len() <= 0 || abup_get_fota_status() != ABUP_FOTA_DL)
		{
			r = xy_http_recv_header(s_abup_task_data->http_id, s_abup_task_data->socket_buf, ABUP_SOCKET_BUFFER_LEN, socket_waitint_time);
			//xy_printf("ABUP-->over here get xy_http_recv_header ok...");
		}
		r = xy_http_recv_body(s_abup_task_data->http_id, s_abup_task_data->socket_buf + r, ABUP_SOCKET_BUFFER_LEN - r, socket_waitint_time);
		//xy_printf("ABUP-->over here get xy_http_recv_body ok...");
		
		if (r == ABUP_SOCKET_BUFFER_LEN)
		{
			xy_printf("ABUP-->READ r == ABUP_SOCKET_BUFFER_LEN");
			s_abup_task_data->monitor_count = 0;
			if (abup_get_fota_status() != ABUP_FOTA_DL)
			{
				xy_printf("ABUP-->read socket(%d):", r);
			}
			else
			{
				xy_printf("ABUP-->read socket(%d).", r);
			}
			if (abup_get_fota_status() != ABUP_FOTA_DL)
				abup_multi_print(s_abup_task_data->socket_buf);
			abup_parse_http_data(s_abup_task_data->socket_buf, r);
		}
		else if (r > 0)
		{
			xy_printf("ABUP-->READ r > 0");
			s_abup_task_data->monitor_count = 0;
			if (abup_get_fota_status() != ABUP_FOTA_DL)
			{
				if (strstr(s_abup_task_data->socket_buf, "0\r\n\r\n") == NULL)
				{
					xy_printf("ABUP-->read socket more(%d).", r);
					r += xy_http_recv_body(s_abup_task_data->http_id, s_abup_task_data->socket_buf, ABUP_SOCKET_BUFFER_LEN - r, socket_waitint_time);
				}
				xy_printf("ABUP-->read socket(%d):", r);
			}
			else
			{
				xy_printf("ABUP-->read socket(%d).", r);
			}
			if (abup_get_fota_status() != ABUP_FOTA_DL)
				abup_multi_print(s_abup_task_data->socket_buf);
			abup_parse_http_data(s_abup_task_data->socket_buf, r);
			abup_montior_start();
			if (abup_get_fota_status() != ABUP_FOTA_DL)
				break;
			else if (abup_get_delta_data_left_len() <= 0)
				break;
		}
		else
		{
			xy_printf("ABUP-->warning! read null data(%d).", r);
			if (socket_waitint_time == 3000)
			{
				socket_waitint_time = 8000;
			}
			else
			{
				abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CLOSE);
				abup_montior_start();
				break;
			}
		}
	}
}

static bool abup_tcpip_is_ok()
{
	while(!ps_netif_is_ok())
	{
		s_abup_task_data->pdp_count++;
		if (s_abup_task_data->pdp_count > 300)
		{
			xy_printf("ABUP-->PDP active fail.");
			return false;
		}
		else
		{
			osDelay(100);
		}
	}

	return true;
}

static void abup_fota_task(void)
{
	unsigned int queuelen = -1;

	abup_task_init();

	while (s_abup_task_data != NULL && !s_abup_task_data->is_quit)
	{
		if(abup_fota_sem == NULL)
		{
			abup_fota_sem = osSemaphoreNew(0xFFFF, 0);
			xy_printf("ABUP-->First use semaphore.");
			continue;
		}
		else
		{	
			osSemaphoreAcquire(abup_fota_sem, osWaitForever);
		}
		switch (abup_msg_id)
		{
			case ABUP_TASK_MSGID_SOCKET_PDP:
				xy_printf("ABUP-->PDP active.");
				if (abup_tcpip_is_ok())
				{
					abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CONNECT);
				}
				else
				{
					abup_task_message_send(ABUP_TASK_MSGID_EXIT);
				}
				break;
			case ABUP_TASK_MSGID_SOCKET_CONNECT:
				xy_printf("ABUP-->socket connect.");
				abup_fota_socket_connect();
				break;
			case ABUP_TASK_MSGID_SOCKET_WRITE:
				xy_printf("ABUP-->socket write.");
				abup_fota_socket_write();
				break;
			case ABUP_TASK_MSGID_SOCKET_READ:
				xy_printf("ABUP-->socket read.");
				abup_fota_socket_read();
				break;
			case ABUP_TASK_MSGID_CHANGE_IP:
				xy_printf("ABUP-->reset dns.");
				abup_free_dns();
				abup_close_socket();
				abup_change_ip();
				abup_task_message_send(ABUP_TASK_MSGID_SOCKET_CONNECT);
			case ABUP_TASK_MSGID_SOCKET_CLOSE:
				xy_printf("ABUP-->socket close.");
				abup_close_socket();
				break;
			case ABUP_TASK_MSGID_EXIT:
				xy_printf("ABUP-->task exit.");
				s_abup_task_data->is_quit = true;
				break;
			case ABUP_TASK_MSGID_SYSTEM_REBOOT:
				if (abup_is_sn_exist())
				{
					xy_printf("ABUP-->reboot now......");
					s_abup_task_data->is_quit = true;
					osDelay(50);
					xy_reboot();
				}
				else
				{
					abup_task_message_send(ABUP_TASK_MSGID_EXIT);
				}
				break;
			default:
				xy_printf("ABUP-->msg_id = %d", abup_msg_id);
				break;
		}
	}

	abup_task_deinit();
}

static void abup_fota_task_thread(void)
{
	xy_work_lock(0);

	xy_printf("ABUP-->abup_fota_task_thread start");

	abup_fota_task();

	xy_work_unlock(0);

	xy_printf("ABUP-->abup_fota_task_thread end");

	abup_info_point_deinit();
	
	abup_socket_handle = NULL;

	osThreadExit();
}

void abup_create_fota_task(void)
{
	if (abup_socket_handle != NULL)
	{
		xy_printf("ABUP-->abup_creat_fota_task is running!");
		return;
	}

	abup_socket_handle = osThreadNew(abup_fota_task_thread, NULL, "abup_fota_task_thread", 0x800, osPriorityNormal);
	abup_task_message_send(ABUP_TASK_MSGID_SOCKET_PDP);
}

void abup_free_dns(void)
{
	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}
}

void abup_close_socket(void)
{
	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}

	if (s_abup_task_data->http_id != -1)
	{
		xy_http_close(s_abup_task_data->http_id);
		s_abup_task_data->http_id = -1;
	}
}
/*
int abup_socket_read_ex(char *buf, int len)
{
	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return -1;
	}
	xy_printf("ABUP-->abup_socket_read_ex executing...");

	return xy_http_recv_body(s_abup_task_data->http_id, buf, len, 3000);
}*/

void abup_set_fota_ratio(unsigned int ratio)
{
	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return;
	}

	if (s_abup_task_data->ratio != ratio)
	{
		s_abup_task_data->ratio = ratio;
		xy_printf("ABUP-->Current progress %d ################", s_abup_task_data->ratio);
	}
}

unsigned int abup_get_fota_ratio(void)
{
	if (s_abup_task_data == NULL)
	{
		xy_printf("ABUP-->error! s_abup_task_data is null.");
		return 0;
	}

	return s_abup_task_data->ratio;
}

void abup_check_update(void)
{
	if (abup_check_upgrade() == 0)
	{
		abup_check_version();
		return;
	}

	if (xy_wait_tcpip_ok(20) != XY_OK)
	{
		xy_printf("ABUP-->network is not ok!");
		return;
	}

	xy_printf("ABUP-->mid=%s", abup_get_device_mid());
	
	abup_set_fota_status(ABUP_FOTA_RU);
	abup_create_fota_task();
}

void abup_check_version(void)
{
	if (xy_wait_tcpip_ok(20) != XY_OK)
	{
		xy_printf("ABUP-->network is not ok!");
		return;
	}

	abup_set_fota_status(ABUP_FOTA_CV);
	abup_create_fota_task();
}

static void abup_check_update_timeout(void *para)
{
	abup_set_service_info();
	abup_check_update();

	//osTimerStop(abup_fota_timer);
	osTimerDelete(abup_fota_timer);
	abup_fota_timer = NULL;
}

void abup_init(void)
{
    //检查是否需要进行FOTA检查
    if(xy_check_OTA_timeout() != XY_OK)
        return;
    
    send_debug_str_to_at_uart("\r\nABUP-->abup_init start\r\n");
	//延迟8秒再执行与服务器的连接，以防止无线环境尚未准备好
	if (abup_fota_timer == NULL)
	{
		abup_fota_timer = osTimerNew(abup_check_update_timeout, osTimerOnce, NULL, "abup_fota_timer");
		osTimerStart(abup_fota_timer, 8 * 1000);
	}
	else
	{
		send_debug_str_to_at_uart("\r\nABUP-->abup_init error.\r\n");
	}
}

static abup_semaphore_release(void *para)
{
	if (abup_fota_sem == NULL)
	{
		xy_printf("ABUP-->Warning! fota semaphore is null(%d).", abup_msg_id);
		return;
	}

	osSemaphoreRelease(abup_fota_sem);

	osTimerDelete(abup_sem_timer);
	abup_sem_timer = NULL;
}

void abup_task_message_send(unsigned int msg_id)
{
	abup_msg_id = msg_id;

	if (abup_sem_timer == NULL)
	{
		abup_sem_timer = osTimerNew(abup_semaphore_release, osTimerOnce, NULL, "abup_sem_timer");
		osTimerStart(abup_sem_timer, 900);
	}
	else
	{
		xy_printf("ABUP-->abup_task_message_send error.");
	}
}


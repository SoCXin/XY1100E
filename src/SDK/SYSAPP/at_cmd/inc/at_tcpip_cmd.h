#pragma once

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct dns_query_info
{
  char *domain_name;
  char type;
  int timeout;
}dns_query_info_t;

enum query_type
{
  XDNS = 0,
  CMDNS,
  QDNS
};

struct cmntp_parm
{
  char *domain_name;
  unsigned char update_rtc;
  int port;
  int timeout;
};

enum rtc_update_mode
{
  RTC_UPDATE_AUTO = 1,
  RTC_UPDATE_MANUAL,
};

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
extern int g_double_trance;
extern int g_double_trance_num;

/*******************************************************************************
 *						             Global function declarations 					             *
 ******************************************************************************/
int at_XDNSCFG_req(char *at_buf, char **prsp_cmd);
int at_QIDNSCFG_req(char *at_buf, char **prsp_cmd);
int at_XDNS_req(char *at_buf, char **prsp_cmd);
int at_QDNS_req(char *at_buf, char **prsp_cmd);
int at_CMDNS_req(char *at_buf, char **prsp_cmd);
int at_CMNTP_req(char *at_buf, char **prsp_cmd);
int at_XNTP_req(char *at_buf, char **prsp_cmd);
int at_TCPIPTEST_req(char *at_buf, char **prsp_cmd);
int at_FOTACTR_req(char *at_buf, char **prsp_cmd);

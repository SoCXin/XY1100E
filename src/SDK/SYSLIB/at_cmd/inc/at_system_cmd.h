#pragma once

/*******************************************************************************
 *						Global function declarations 					       *
 ******************************************************************************/
int at_OFFTIME_req(char *at_buf,char **prsp_cmd);
int at_RESET_req(char *at_buf, char **prsp_cmd);
int at_STANDBY_req(char *at_buf, char **prsp_cmd);
int at_NITZ_req(char *at_buf, char **prsp_cmd);
int at_CCLK_req(char *at_buf, char **prsp_cmd);
int at_CPOF_req(char *at_buf, char **prsp_cmd);
int at_ABOFF_req(char *at_buf, char **prsp_cmd);
int at_LOADDSP_req(char * at_buf, char * * prsp_cmd);
int at_RB_req(char *at_buf, char **prsp_cmd);
int at_CPOFF_req(char *at_buf, char **prsp_cmd);
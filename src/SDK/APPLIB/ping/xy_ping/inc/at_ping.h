#pragma once

/**
 * @brief 
 * 
 * @param at_buf 
 * @param prsp_cmd 
 * @return int 
 */
int at_NPING_req(char *at_buf, char **prsp_cmd);

/**
 * @brief 
 * 
 * @param at_buf 
 * @param prsp_cmd 
 * @return int 
 */
int at_NPINGSTOP_req(char *at_buf, char **prsp_cmd);

/**
 * @brief 
 * 
 * @param at_buf 
 * @param prsp_cmd 
 * @return int 
 */
int at_NPING6_req(char *at_buf, char **prsp_cmd);
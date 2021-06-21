#pragma once

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * @brief process AT+NETDOG cmd
 * @param buf   cmd data
 * @param size   cmd size
 * @note params: AT+NETDOG=AT/ATCMD/NET/PS/DM/PSM
 * @return
 */
int at_NETDOG_req(char *at_buf,char **prsp_cmd);

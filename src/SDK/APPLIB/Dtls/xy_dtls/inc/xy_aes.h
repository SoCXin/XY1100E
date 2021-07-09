#pragma once

/**
 * @brief 
 * 
 * @param in_buf 
 * @param in_len 
 * @param out_buf 
 * @param out_len 
 * @return int 
 */
#if XY_DTLS
int xy_encrypt(char* in_buf, int in_len, char* out_buf, int *out_len);

/**
 * @brief 
 * 
 * @param in_buf 
 * @param in_len 
 * @param out_buf 
 * @param out_len 
 * @return int 
 */
int xy_decrypt(char* in_buf, int in_len, char* out_buf, int *out_len);

#endif

//net_tool_standard_chksum
/**
 * @brief 
 * 
 * @param dataptr 
 * @param len 
 * @return unsigned int 
 */
unsigned int xy_chksum(const void *dataptr, int len);

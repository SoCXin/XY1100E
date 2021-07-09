/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_md5.h
* Description: 
* Author: WQH
* Version: v1.0
* Date: 20190303
*****************************************************************************/
#ifndef __ABUP_MD5_H__
#define __ABUP_MD5_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD5_u32plus;

typedef struct
{
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned char buffer[64];
	MD5_u32plus block[16];
} ABUP_MD5_CTX;

extern void abup_MD5_Init(ABUP_MD5_CTX *ctx);
extern void abup_MD5_Update(ABUP_MD5_CTX *ctx, const void *data, unsigned long size);
extern void abup_MD5_Final(unsigned char *result, ABUP_MD5_CTX *ctx);
extern void abup_MD5_Encode(char *output, unsigned char *input, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif


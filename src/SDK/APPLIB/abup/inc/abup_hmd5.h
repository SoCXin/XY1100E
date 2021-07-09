/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_hmd5.h
* Description: 
* Author: WQH
* Version: v1.0
* Date: 20190303
*****************************************************************************/
#ifndef __ABUP_HMD5_H__
#define __ABUP_HMD5_H__

#include <stdint.h>

#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
returns an empty list.
*/
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

/* MD5 context. */
typedef struct
{
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} ABUP_HMD5_CTX;


#define ABUP_HMD5_LEN 34

extern void MD5Init PROTO_LIST ((ABUP_HMD5_CTX *));
extern void MD5Update PROTO_LIST((ABUP_HMD5_CTX *, unsigned char *, unsigned int));
extern void MD5Final PROTO_LIST ((unsigned char [16], ABUP_HMD5_CTX *));
extern char *abup_hmd5_mid_pid_psecret(char *mid, char *productId, char *productSecret, uint32_t abup_time);
extern char *abup_hmd5_did_pid_psecret(char *deviceId, char *productId, char *productSecret, uint32_t abup_time);
extern char * abup_hmd5_pid_psec_mid(char *deviceId, char *deviceSecret,char *mid);

#endif


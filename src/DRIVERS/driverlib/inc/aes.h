#ifndef __AES_H__
#define __AES_H__

#include "hw_aes.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "replace.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#define AES_BLOCK_BYTES         16
#define AES_MODE_ENCRYPT        1
#define AES_MODE_DECRYPT        0
#define AES_KEY_LEN_BITS_128    128
#define AES_KEY_LEN_BITS_192    192
#define AES_KEY_LEN_BITS_256    256

 #if !USE_ROM_AES
extern void AESKeyLenSet(unsigned long ulKeyLenMode);
extern void AESKeySet(unsigned char ucOffset, unsigned long ulKeyValue);
extern void AESIVSet(unsigned char ucOffset, unsigned long ucIV);
extern void AESAADLen0Set(unsigned long ulAadLen0);
extern void AESAADLen1Set(unsigned long ulAadLen1);
extern void AESPayloadLenSet(unsigned long ulPayloadLenByte);
extern void AESModeSet(unsigned long ucAESMode);
extern void AESEncDecSet(unsigned long ucAESEncDec);
extern void AESDMAEn(void);
extern void AESDMADis(void);
extern void AESBlockDataInput(unsigned char ucOffset, unsigned long ulDataIn);
extern unsigned long AESBlockDataOutput(unsigned char ucOffset);
extern void AESBlockStart(void);
extern void AESKeyLoadWaitDone(void);
extern void AESBlockTransWaitDone(void);
extern void AESClockDiv2En(void);
extern void AESClockDiv2Dis(void);
extern void AESCCMAuthLenSet(unsigned long ucAuthLenByte);
extern void AESCCMLengthLenSet(unsigned char ucLengthLenByte);
extern unsigned long AESTagGet(unsigned char ucOffset);

extern void AESBlock(unsigned char *pucInput, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput);

extern void AESECB(unsigned char *pucInput, unsigned long ulInputLenByte, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput);

extern void AESCCM(unsigned char *aad, unsigned long aad_len_byte, unsigned char *input_message, 
                   unsigned long input_len_byte, unsigned long *user_key, unsigned long key_len_bits, 
                   unsigned long M, unsigned long L, unsigned char *nonce, unsigned char mode, unsigned char *output_message, unsigned char *digest);
#endif

extern void AES_128_EEA2(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucOutMsg);

extern void AES_128_EIA2(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucMACT);

#if !USE_ROM_AES
extern void AES_CMAC_XOR_128(unsigned char *pucA, const unsigned char *pucB, unsigned char *pucOut);

extern void AES_CMAC_Leftshift_Onebit(unsigned char *pucInput, unsigned char *pucOutput);

extern void AES_CMAC_Generate_Subkey(unsigned char *pucKey, unsigned char *pucK1, unsigned char *pucK2);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __AES_H__


#ifndef __FLASH_ADAPT_H__
#define __FLASH_ADAPT_H__

#include "dma.h"
#include "flash_vendor.h"

#define BURSTSIZE (64)

#define FLASH_DMA_CHANNEL DMA_CHANNEL_2

typedef enum{
	flash_op_default=0,
	flash_op_erase,
	flash_op_writeNoErase,
	flash_op_writeErase,
	flash_op_protect
}flash_operation;

typedef struct{
	unsigned int destAddr;
	unsigned char *srcAddr;
	union
	{
		unsigned int size;
		struct
		{
			unsigned int protect_mode: 5;
			unsigned int cmp: 1;
			unsigned int reserved2: 26;
		};
	};
	
	unsigned int operation:3;
	unsigned int coretype:1;
	unsigned int reserved:28;
}flash_PE_st;


int flash_read(unsigned int addr, unsigned char *data,unsigned int size);
int flash_write_erase(unsigned int addr, unsigned char *data,unsigned int size);
int flash_write_no_erase(unsigned int addr, unsigned char *data,unsigned int size);
int flash_erase(long offset, unsigned int size);
void flash_write_unaligned(unsigned int addr,void *data, int size,int erase,int urgent);

void DMAChannelMemcpy(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum);
unsigned char FLASH_OTP_CAL_Read_and_Check();
unsigned int flash_dynwrite_task_init(void);
extern void send_write_flash_msg(int operation,unsigned int srcAddr, unsigned int destAddr, unsigned int size);
extern void flash_operate_ext(int operation,unsigned int flash_addr, void *ram_data, int size);
extern void diffcore_flashwrt_msg_proc(void);
extern void flash_protect(FLASH_PROTECT_MODE protect_mode, uint8_t cmp);
extern void flash_protect_ext(FLASH_PROTECT_MODE protect_mode, uint8_t cmp);
extern int xy_flash_test(unsigned int addr, unsigned int len);
extern void xy_flash_protect(FLASH_PROTECT_MODE protect_mode, uint8_t cmp);

#endif /* #ifndef __FLASH_H__ */


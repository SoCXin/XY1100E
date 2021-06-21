#include "flash_vendor.h"
#include "flash_adapt.h"
#include "uart.h"
#include "xy_flash.h"
#include "sema.h"
#include "xy_system.h"
#include "ipcmsg.h"
#include "xy_at_api.h"
#include "inter_core_msg.h"

#define   DMA_DELTA_LEN    0x400


void DMAChannelMemcpy(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum)
{
    unsigned char dst_flag = MEM_TYPE_RAM;
    unsigned char src_flag = MEM_TYPE_RAM;

    if((ulDst >= QSPI_DATA_BASE) && ((ulDst + ulLenByte) <= (QSPI_DATA_BASE + QSPI_FLASH_MAX_CAPACITY)))
    {
        dst_flag = MEM_TYPE_FLASH;
    }

    if((ulSrc >= QSPI_DATA_BASE) && ((ulSrc + ulLenByte) <= (QSPI_DATA_BASE + QSPI_FLASH_MAX_CAPACITY)))
    {
        src_flag = MEM_TYPE_FLASH;
    }

    if(MEM_TYPE_RAM == dst_flag && MEM_TYPE_FLASH == src_flag)
    {
        FLASH_ReadData((QSPI_FLASH_Def *)&xinyi_flash, ulSrc, ulDst, ulLenByte, FLASH_DMA_CHANNEL);
        FLASH_WaitIdle((QSPI_FLASH_Def *)&xinyi_flash);
    }
    else if(MEM_TYPE_FLASH == dst_flag && MEM_TYPE_RAM == src_flag)
    {
		FLASH_WriteData((QSPI_FLASH_Def *)&xinyi_flash, ulSrc, ulDst, ulLenByte, FLASH_DMA_CHANNEL);
        FLASH_WaitIdle((QSPI_FLASH_Def *)&xinyi_flash);
    }
    else if(MEM_TYPE_RAM == dst_flag && MEM_TYPE_RAM == src_flag)
    {
        DMAChannelTransfer_withMutex(ulDst, ulSrc, ulLenByte, ulChannelNum);
    }
}

int flash_read(unsigned int addr, unsigned char *data,unsigned int size)
{
	FLASH_ReadData(&xinyi_flash,(unsigned long)addr,(unsigned long)data,(unsigned long)size,(unsigned long)FLASH_DMA_CHANNEL);
	FLASH_WaitIdle(&xinyi_flash);
	
	return 0;
}

extern char start_dump;
int flash_write_erase(unsigned int addr, unsigned char *data,unsigned int size)
{
	uint32_t offset;	
	uint32_t write_size;
	uint32_t actAddr;
	unsigned char *block_buf = NULL;

	if(start_dump != 0)
	{
		block_buf = (unsigned char *)M3_SRAM_BASE;
	}
	else
	{
		block_buf = xy_malloc_Align(EFTL_PAGE_SIZE);
	}

	while (size > 0) {
		offset = addr & 0xfff;
		actAddr = addr & 0xfffff000;
		
		if (size <= (EFTL_PAGE_SIZE - offset)) {
			write_size = size;
		} else {
			write_size = EFTL_PAGE_SIZE - offset;
		}

		if(write_size == EFTL_PAGE_SIZE)
		{
			flash_erase(actAddr, EFTL_PAGE_SIZE);		
			flash_write_no_erase(actAddr,data, EFTL_PAGE_SIZE);
		}
		else
		{
			flash_read(actAddr, block_buf, EFTL_PAGE_SIZE);
			
			if(write_size<100 && !memcmp(block_buf + offset, data, write_size)) 
				goto CONTINUE;
			
			memcpy(block_buf + offset, data, write_size);
			flash_erase(actAddr, EFTL_PAGE_SIZE);
			flash_write_no_erase(actAddr, block_buf, EFTL_PAGE_SIZE);
		}
	CONTINUE:

		size -= write_size;
		addr += write_size;
		data += write_size;
	}
	
	if(start_dump != 0)
	{
		block_buf = (unsigned char *)M3_SRAM_BASE;
	}
	else
	{
		xy_free(block_buf);
	}

	return 0;
}

int flash_write_no_erase(unsigned int addr, unsigned char *data,unsigned int size)
{
	unsigned char *block_buf = data;
	
	if(!IS_CACHE_ALIGNED(data))
	{
		if(start_dump != 0)
		{
			block_buf = (unsigned char *)M3_SRAM_BASE;
		}
		else
		{
			block_buf = xy_malloc_Align(size);
		}
		memcpy(block_buf, data,size);
	}

	osCoreEnterCritical();
	FLASH_ExitXIPMode(&xinyi_flash);
	FLASH_WaitIdle(&xinyi_flash);

	FLASH_WriteData(&xinyi_flash,(unsigned long)block_buf,(unsigned long)addr,(unsigned long)size,FLASH_DMA_CHANNEL);
	FLASH_WaitIdle(&xinyi_flash);

	FLASH_EnterXIPMode(&xinyi_flash,0xa0);
	osCoreExitCritical();

	if(start_dump != 0)
	{
		block_buf = (unsigned char *)M3_SRAM_BASE;
	}
	else
	{
		if(block_buf != data)
			xy_free(block_buf);
	}
	return 0;
}


int flash_erase(long addr, unsigned int size)
{
	int page=0;

	xy_assert(addr%EFTL_PAGE_SIZE==0);

	size = (size+EFTL_PAGE_SIZE-1)&(~(EFTL_PAGE_SIZE-1));

	osCoreEnterCritical();

	FLASH_ExitXIPMode(&xinyi_flash);
	FLASH_WaitIdle(&xinyi_flash);

	while (page < (int)(size)/EFTL_PAGE_SIZE)
	{	
		FLASH_SectorErase(&xinyi_flash, addr+page*EFTL_PAGE_SIZE);
		FLASH_WaitIdle(&xinyi_flash);
		page++;
	}
	FLASH_EnterXIPMode(&xinyi_flash,0xa0);

	osCoreExitCritical();
	
	return 0;
}

#if USE_ADC_CAL
unsigned char FLASH_OTP_CAL_Read_and_Check()
{
    unsigned int tempCRC;
    unsigned char i;
	unsigned int *p_OTP_CAL;
	unsigned int regval;
	/*
	FLASH_ExitXIPMode(&xinyi_flash);
	regval = HWREG(0xA0030004);
	FLASH_SetReadMode((QSPI_FLASH_Def *)&xinyi_flash, 0x08000048);		//QSPI_READ_SECURITY

    FLASH_ReadData((QSPI_FLASH_Def *)&xinyi_flash, 0x27000080, (unsigned long)&flash_otp_cal, sizeof(Flash_OTP_CAL_Def), FLASH_DMA_CHANNEL);
    FLASH_WaitIdle((QSPI_FLASH_Def *)&xinyi_flash);

    FLASH_SetReadMode((QSPI_FLASH_Def *)&xinyi_flash, regval);		//QSPI_READ_DEFULT
    FLASH_EnterXIPMode(&xinyi_flash,0xa0);
	*/
	memcpy((void *)&flash_otp_cal,(void *)OTPINFO_RAM_ADDR,sizeof(Flash_OTP_CAL_Def));
    if(0x55AA55AA != flash_otp_cal.calibration_valid)
    {
        return 3;
    }

	tempCRC = 0;
	p_OTP_CAL = (unsigned int *)&flash_otp_cal;
	for(i=1; i<(sizeof(Flash_OTP_CAL_Def) + 3) / 4 - 1; i++)
	{
		tempCRC ^= *(p_OTP_CAL + i);
	}

	if(tempCRC != flash_otp_cal.calibration_crc)
	{
		return 2;
	}

    return 1;
}
#endif

static void send_flash_msg(flash_PE_st * pe)
{
	volatile uint32_t syncdelay;

	osCoreEnterCritical();
	do{

	}while((HWREG(FLASH_DYN_SYNC) & 2) != 2); //wait for DSP flash task ready
	HWREG(FLASH_DYN_SYNC) &= 0xFFFFFFE;
	shmmsg_write_channel(pe, sizeof(flash_PE_st), ICM_FLASHWRITE, IpcMsg_Flash);
	vTaskSuspendAll();
	osCoreExitCritical();
	while((HWREG(FLASH_DYN_SYNC) & 0x01) == 0)
	{
		if(!DSP_IS_ALIVE())
			HWREG(PRCM_BASE + 0x1c)|= 0x02;
		for(syncdelay=0;syncdelay<500;syncdelay++);
	}  //wait for DSP enterXIP
	
	xTaskResumeAll();

}

void send_write_flash_msg(int operation,unsigned int ramAddr, unsigned int flashAddr, unsigned int size)
{
	flash_PE_st flashPE = {0};
	flashPE.destAddr = get_Dsp_Addr_from_ARM(flashAddr);
	if(operation != flash_op_erase)
		flashPE.srcAddr = (unsigned char *)(get_Dsp_Addr_from_ARM(ramAddr));
	flashPE.size = size;
	flashPE.operation = operation;

	send_flash_msg(&flashPE);
}

void flash_operate_ext(int operation,unsigned int flash_addr, void *ram_data, int size)
{	
	uint32_t offset=0;	
	uint32_t write_size=0;
	uint32_t actAddr=0;
	uint8_t * block_buf = NULL;
	uint8_t * write_buf = NULL;

	if(operation != flash_op_erase)
		block_buf = xy_malloc_Align(EFTL_PAGE_SIZE);
	
	while (size > 0) {
		offset = flash_addr & 0xfff;
		actAddr = flash_addr & 0xfffff000;
		
		if (size <= (int)(EFTL_PAGE_SIZE - offset)) {
			write_size = size;
		} else {
			write_size = EFTL_PAGE_SIZE - offset;
		}
		
		if(operation == flash_op_writeErase)
		{
			if((offset == 0) && (write_size == EFTL_PAGE_SIZE) && IS_CACHE_ALIGNED(ram_data)) 
			{
				/* 调用者如果按sector对齐写数据并且申请的内存也是cache对齐走该分支加速，比如xy_ftl_write */
				write_buf = ram_data;
			}
			else
			{
				write_buf = block_buf;
				flash_read(actAddr, write_buf, EFTL_PAGE_SIZE); 
				if(size < 100 && !memcmp(write_buf + offset, ram_data, write_size)) 
					goto CONTINUE;
				
				memcpy(write_buf + offset, ram_data, write_size);
			}
			send_write_flash_msg(operation,(uint32_t)(write_buf), actAddr, EFTL_PAGE_SIZE);
		}
		else if(operation == flash_op_writeNoErase)
		{		
			memcpy(block_buf, ram_data, write_size);
			send_write_flash_msg(operation,(unsigned int)(block_buf), actAddr+ offset, write_size);
		}
		else if(operation == flash_op_erase)
		{
			send_write_flash_msg(operation,(unsigned int)(NULL), actAddr, EFTL_PAGE_SIZE);
		}

CONTINUE:
		size -= write_size;
		flash_addr += write_size;
		ram_data += write_size;
	}
	if(block_buf != NULL)
		xy_free(block_buf);
}

void diffcore_flashwrt_msg_proc(void)
{
	uint32_t syncdelay = 0;

	vTaskSuspendAll();
	do{
	}while((HWREG(FLASH_DYN_SYNC) & 2) != 2); //wait for DSP flash task ready
	HWREG(FLASH_DYN_SYNC) &= 0xFFFFFFE;

	HWREG(FLASH_NOTICE_FLAG) |= 0x01;		 

	while((HWREG(FLASH_DYN_SYNC) & 0x01) == 0)
	{
		if(!DSP_IS_ALIVE())
			HWREG(PRCM_BASE + 0x1c)|= 0x02;
		for(syncdelay=0;syncdelay<500;syncdelay++);
	}//wait for DSP enterXIP
	
	( void ) xTaskResumeAll();
}

void flash_protect(FLASH_PROTECT_MODE protect_mode, uint8_t cmp)
{
	vTaskSuspendAll();
	FLASH_ExitXIPMode(&xinyi_flash);

	FLASH_SetProtectMode(&xinyi_flash, protect_mode, cmp);

	FLASH_EnterXIPMode(&xinyi_flash,0xa0);
	( void ) xTaskResumeAll();
}

void flash_protect_ext(FLASH_PROTECT_MODE protect_mode, uint8_t cmp)
{
	flash_PE_st flashPE = {0};
	flashPE.protect_mode = protect_mode;
	flashPE.cmp = cmp;
	flashPE.operation = flash_op_protect;

	send_flash_msg(&flashPE);
}


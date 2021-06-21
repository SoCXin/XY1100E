/*****************************************************************************
* Copyright (c) 2019 ABUP.Co.Ltd. All rights reserved.
* File name: abup_flash.c
* Description: fota
* Author: WQH
* Version: v1.0
* Date: 20190903
*****************************************************************************/
#include "abup_flash.h"
#include "lwip/def.h"
#include "xy_flash.h"

uint32_t abup_fota_flash_size = 0;
uint32_t abup_delta_page_num = 0;
static unsigned char *p_block_buff = NULL;
static unsigned char block_buff[512] = {0};

void abup_read_flash(unsigned char *out_buff, unsigned int offset, unsigned int size)
{
	unsigned int fota_flash_base = FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * (abup_delta_page_num + 2);

	xy_flash_read(fota_flash_base + offset, out_buff, size);
}

//sizeÒ»´Î²»ÄÜ³¬¹ý4K´óÐ¡
void abup_write_flash(unsigned char *data_buff, unsigned int offset, unsigned int size)
{
	unsigned char current_page_cnt = 1;
	static unsigned char last_page_num = 0;
	unsigned int fota_flash_base = FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * (abup_delta_page_num + 2);

	if (size > EFTL_PAGE_SIZE)
	{
		xy_printf("ABUP-->abup_write_flash size error, size > 4k");
		return;
	}
	xy_printf("ABUP-->last_page_num=%d", last_page_num);
	for (current_page_cnt = 1; current_page_cnt <= FOTA_BACKUP_LEN_MAX/EFTL_PAGE_SIZE; current_page_cnt++)//È·¶¨µ±Ç°ÒªÔÚÄÄÒ»Ò³²Ù×÷flash
	{
		if (offset <= current_page_cnt*EFTL_PAGE_SIZE)
		{
			break;
		}
	}
	if (last_page_num  != current_page_cnt)
	{
		xy_printf("ABUP-->xy_flash_erase is running, current_page_cnt=%d", current_page_cnt);
		if (current_page_cnt == 1)
		{
			xy_flash_erase(fota_flash_base + (current_page_cnt-1)*EFTL_PAGE_SIZE, EFTL_PAGE_SIZE);
		}
		xy_flash_erase(fota_flash_base + current_page_cnt*EFTL_PAGE_SIZE, EFTL_PAGE_SIZE);
		last_page_num  = current_page_cnt;
	}
	xy_printf("ABUP-->abup_write_flash offset = %d, size = %d", offset, size);
	xy_flash_write_no_erase(fota_flash_base + offset, data_buff, size);
}

void abup_erase_flash(unsigned int offset, unsigned int size)
{
	if (offset % EFTL_PAGE_SIZE == 0 && size % EFTL_PAGE_SIZE == 0)
	{
		unsigned int fota_flash_base = FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * (abup_delta_page_num + 2);

		xy_flash_erase(fota_flash_base + offset, size);
	}
}

void abup_read_backup(unsigned char *out_buff, unsigned int offset, unsigned int size)
{
	xy_flash_read(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * 2 + offset, out_buff, size);
}

void abup_write_backup(unsigned char *data_buff, unsigned int offset, unsigned int size)
{
	if (offset == 0)
	{
		xy_flash_erase(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * 2, EFTL_PAGE_SIZE);
	}

	xy_flash_write_no_erase(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * 2 + offset, data_buff, size);
}

void abup_erase_backup(void)
{
	xy_flash_erase(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE * 2, EFTL_PAGE_SIZE);
}

void abup_read_nv(unsigned char *out_buff, unsigned int offset, unsigned int size)
{
	xy_flash_read(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE + offset, out_buff, size);
}

void abup_write_nv(unsigned char *data_buff, unsigned int offset, unsigned int size)
{
	xy_printf("ABUP-->abup_write_nv is running...");
	if (offset + size > 512)
	{
		xy_printf("ABUP-->abup_write_nv error, out of range.");
		return;
	}

	xy_flash_read(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE, block_buff, 512);

	xy_flash_erase(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE, EFTL_PAGE_SIZE);
	MEMCPY(block_buff + offset, data_buff, size);

	xy_flash_write_no_erase(FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - EFTL_PAGE_SIZE, block_buff, 512);
	memset(block_buff, 0, 512);
}

unsigned int abup_delta_region_size(void)
{
	return abup_fota_flash_size - EFTL_PAGE_SIZE * 2;
}

void abup_fota_flash_init(unsigned int delta_size)
{
	extern uint32_t _Flash_Used;
	extern uint32_t _Ram_Text;
	extern uint32_t _Ram_Data;
	uint32_t FLASH_LEN = (uint32_t)&_Flash_Used;
	uint32_t RAM_LEN = (uint32_t)&_Ram_Text + (uint32_t)&_Ram_Data;
	uint32_t abup_fota_base = 0;

	abup_fota_base = ARM_FLASH_BASE_ADDR+(FLASH_LEN/EFTL_PAGE_SIZE+1)*EFTL_PAGE_SIZE+(RAM_LEN/EFTL_PAGE_SIZE+1)*EFTL_PAGE_SIZE;
	abup_fota_flash_size = FOTA_BACKUP_BASE + FOTA_BACKUP_LEN_MAX - abup_fota_base;
	xy_printf("ABUP-->fota flash size:0x%X", abup_fota_flash_size);

	if ((delta_size % EFTL_PAGE_SIZE) == 0)
	{
		abup_delta_page_num = delta_size / EFTL_PAGE_SIZE;
	}
	else
	{
		abup_delta_page_num = delta_size / EFTL_PAGE_SIZE + 1;
	}
	xy_printf("ABUP-->abup_delta_page_num: %d", abup_delta_page_num);
}


/*****************************************************************************
* Copyright (c) 2020 ABUP.Co.Ltd. All rights reserved.
* File name: abup_flash.h
* Description:
* Author: WQH
* Version: v1.0
* Date: 20200311
*****************************************************************************/
#ifndef __ABUP_FLASH_H__
#define __ABUP_FLASH_H__

void abup_read_flash(unsigned char *out_buff, unsigned int offset, unsigned int size);
void abup_write_flash(unsigned char *data_buff, unsigned int offset, unsigned int size);
void abup_erase_flash(unsigned int offset, unsigned int size);
void abup_read_backup(unsigned char *out_buff, unsigned int offset, unsigned int size);
void abup_write_backup(unsigned char *data_buff, unsigned int offset, unsigned int size);
void abup_erase_backup(void);
unsigned int abup_delta_region_size(void);

#endif


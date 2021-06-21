/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**@defgroup Agenttiny
 * @ingroup agent
 */

#ifndef PACKAGE_H
#define PACKAGE_H

//#ifdef WITH_SOTA
#include "ota_api.h"
#include <stdio.h>



//#endif

/* use sha256 rsa2048 for checksum */
#define PACK_SHA256_RSA2048 0
/* use sha256 for checksum */
#define PACK_SHA256 1
/* no checksum info */
#define PACK_NO_CHECKSUM 2

#define PACK_NO 0
#define PACK_YES 1

/* should define checksum first */
#ifndef PACK_CHECKSUM
#define PACK_CHECKSUM PACK_NO_CHECKSUM
#endif

/* PACK_COMBINE_TO_WRITE_LAST_BLOCK is set, the last writing software will read the not writing data from
flash to combine a entire block, it can save one block to buffer, cause the port write callback will only
write entire block size and need no buffer. but it is not suitable to write to fs system */
#ifndef PACK_COMBINE_TO_WRITE_LAST_BLOCK
#define PACK_COMBINE_TO_WRITE_LAST_BLOCK PACK_NO
#endif


/* package head len should not bigger than this */
#ifndef PACK_MAX_HEAD_LEN
#define PACK_MAX_HEAD_LEN (4 * 1024)
#endif


#if defined(__cplusplus)
extern "C" {
#endif

#define PACK_MALLOC(size) pack_malloc(size)
#define PACK_FREE(ptr) pack_free(ptr)
#define PACK_LOG_ENABLE
#ifdef PACK_LOG_ENABLE
#define PACK_LOG(fmt, ...) \
do\
{\
    pack_params_s *__pack_params__ = pack_get_params();\
    if (__pack_params__->printf != NULL)\
    {\
        (void)__pack_params__->printf("[%s:%d]" fmt "\r\n",  __FUNCTION__, __LINE__,  ##__VA_ARGS__);\
    }\
}while(0)
#else
#define PACK_LOG(fmt, ...) ((void)0)
#endif

#define ASSERT_THIS(do_something) \
        if(NULL == thi)\
        {\
            PACK_LOG("this null pointer");\
            do_something;\
        }

#ifndef MIN
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#endif

#ifndef array_size
#define array_size(a) (sizeof(a)/sizeof(*(a)))
#endif
enum
{
    PACK_OK,
    PACK_ERR
};

typedef struct
{
    ota_opt_s ota_opt;
    void* (*malloc)(size_t size);
    void (*free)(void *ptr);
    int (*printf)(const char *fmt, ...);
}pack_params_s;


typedef struct pack_storage_device_api_tag_s pack_storage_device_api_s;

typedef enum
{
    PACK_DOWNLOAD_OK,
    PACK_DOWNLOAD_FAIL
}pack_download_result_e;
struct pack_storage_device_api_tag_s
{
    int (*write_software)(pack_storage_device_api_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len);
    int (*write_software_end)(pack_storage_device_api_s *thi, pack_download_result_e result, uint32_t total_len);
    int (*active_software)(pack_storage_device_api_s *thi);
};

typedef struct fw_update_record
{
    uint8_t in_use;
    uint8_t block_more;
    uint16_t block_size;
    uint32_t block_num;
    uint32_t block_offset;
    uint32_t uri_len;
    char *uri;
} fw_update_record_t;

typedef struct pack_hardware_tag_s
{
    int (*read_software)(struct pack_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len);
    int (*write_software)(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len);
    void (*set_flash_type)(struct pack_hardware_tag_s *thi, ota_flash_type_e type);
    uint32_t (*get_block_size)(struct pack_hardware_tag_s *thi);
    uint32_t (*get_max_size)(struct pack_hardware_tag_s *thi);
}pack_hardware_s;


/**
 *@ingroup agenttiny
 *@brief get storage device.
 *
 *@par Description:
 *This API is used to get storage device.
 *@attention none.
 *
 *@param none.
 *
 *@retval #pack_storage_device_api_s *     storage device.
 *@par Dependency: none.
 *@see none
 */
pack_storage_device_api_s *pack_get_device(void);

/**
 *@ingroup agenttiny
 *@brief initiate storage device.
 *
 *@par Description:
 *This API is used to initiate storage device.
 *@attention none.
 *
 *@param ato_opt        [IN] Ota option.
 *
 *@retval #int          0 if succeed, or error.
 *@par Dependency: none.
 *@see none
 */
int pack_init_device(const pack_params_s *params);

pack_params_s * pack_get_params(void);
void * pack_malloc(size_t size);
void pack_free(void *ptr);

#if defined(__cplusplus)
}
#endif

#endif //PACKAGE_H



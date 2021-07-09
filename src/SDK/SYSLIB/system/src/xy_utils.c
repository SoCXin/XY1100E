/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "xy_utils.h"

/*******************************************************************************
 *						Global function implementations						   *
 ******************************************************************************/

uint64_t xy_rand_next = 0;

unsigned int xy_chksum(const void *dataptr, int len)  __attribute__((section(".ramtext")));


int xy_srand(uint32_t key)
{
  xy_rand_next = (uint64_t)key;
  return XY_OK;
}

/**
* @brief xy_rand must less than XY_RAND_MAX
*/
#define XY_RAND_MAX    0x7fffffff

int xy_rand()
{
  xy_rand_next = xy_rand_next * 6364136223846793005LL + 1;
  return (int)((xy_rand_next >> 32) & XY_RAND_MAX);
}

//net_tool_standard_chksum
unsigned int xy_chksum(const void *dataptr, int len)
{
    uint32_t acc;
    uint16_t src;
    uint8_t *octetptr;

    acc = 0;
    octetptr = (uint8_t*)dataptr;
    while (len > 1) {
        src = (*octetptr) << 8;
        octetptr++;
        src |= (*octetptr);
        octetptr++;
        acc += src;
        len -= 2;
    }
    if (len > 0) {
        src = (*octetptr) << 8;
        acc += src;
    }

    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }

     return ~acc;
}



/* "AB235E"---->AB235E(3 BYTES) */
int hexstr2bytes(char* src, int src_len, char* dst, int dst_size)
{
	int i;

	if (src ==  NULL || dst == NULL || src_len < 0 || dst_size < (src_len + 1) / 2) {
		xy_assert(0);
	}

	for (i = 0; i < src_len; i += 2) {
		if(*src >= 'a' && *src <='f')
			*dst = ((*src - 'a') + 10) << 4;
		else if (*src >= '0' && *src <= '9') {
			*dst = (*src - '0') << 4;
		} else if (*src >= 'A' && *src <= 'F') {
			*dst = ((*src - 'A') + 10) << 4;
		} else {
			return -1;
		}

		src++;
		if(*src >= 'a' && *src <= 'f')
			*dst |= ((*src - 'a') + 10);
		else if (*src >= '0' && *src <= '9') {
			*dst |= (*src - '0');
		} else if (*src >= 'A' && *src <= 'F') {
			*dst |= ((*src - 'A') + 10);
		} else {
			return -1;
		}

		src++;
		dst++;
	}

	return src_len / 2;
}

/* AB235E(3 BYTES)---->"AB235E" */
int bytes2hexstr(unsigned char* src, signed long src_len, char* dst, signed long dst_size)
{
	const char tab[] = "0123456789ABCDEF";
	signed long i;

	if (src ==  NULL || dst == NULL || src_len < 0 || dst_size <= src_len * 2) {
		xy_assert(0);
	}

	for (i = 0; i < src_len; i++) {
		*dst++ = tab[*src >> 4];
		*dst++ = tab[*src & 0x0f];
		src++;
	}

	*dst = '\0';

	return src_len * 2;
}

extern unsigned char guid[16];
unsigned char *xy_getGUID()
{
    return guid;
}

int xy_rearrange_DWORD(unsigned char *data, int data_len)
{
	int i, round;
	unsigned char temp_char;

	if (data_len % 4 != 0)
		return XY_ERR;
	
	round = data_len / 4;
	for (i = 0; i < round; i++)
	{
		temp_char = data[i * 4 + 0];
		data[i * 4 + 0] = data[i * 4 + 3];
		data[i * 4 + 3] = temp_char;

		temp_char = data[i * 4 + 1];
		data[i * 4 + 1] = data[i * 4 + 2];
		data[i * 4 + 2] = temp_char;
	}
	return XY_OK;
}

void xy_queue_clear(osMessageQueueId_t pMsgQueueId)
{
	void *elem = NULL;
	
    xy_assert (pMsgQueueId != NULL);

    while (osMessageQueueGet(pMsgQueueId, &elem, NULL, 0) == osOK)
    {
    	xy_free(elem);
    }
}

void xy_queue_Delete(osMessageQueueId_t pMsgQueueId)
{
	void *elem = NULL;
	
    xy_assert (pMsgQueueId != NULL);

    while (osMessageQueueGet(pMsgQueueId, &elem, NULL, 0) == osOK)
    {
    	xy_free(elem);
    }
	osMessageQueueDelete(pMsgQueueId);
}

osStatus_t osDelay_stict(uint32_t ticks)
{
	osStatus_t ret;
	osThreadId_t  cur_id;
	osLowPowerType_t power_type;

	cur_id = osThreadGetId();
	power_type = osThreadGetLowPowerFlag(cur_id);
	
	if(osLowPowerPermit != power_type)
	{
		osThreadSetLowPowerFlag(cur_id,osLowPowerPermit);
		ret = osDelay(ticks);
		osThreadSetLowPowerFlag(cur_id,power_type);
		return  ret;
	}
	else
		return  osDelay(ticks);
}



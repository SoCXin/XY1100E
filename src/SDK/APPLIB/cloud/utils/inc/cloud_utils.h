#ifndef _CLOUD_UTILS__H
#define _CLOUD_UTILS__H
#define MUTEX_LOCK_INFINITY 0xFFFFFFFF

#include "lwip/sockets.h"

uint8_t cloud_get_IMEI(char *buffer, uint32_t maxlen);
uint8_t cloud_get_IMSI(char* buffer,uint32_t maxlen);
uint64_t cloud_gettime_ms(void);
unsigned int cloud_get_ResveredMem();
int cloud_mutex_create(osMutexId_t *pMutexId);
int cloud_mutex_destroy(osMutexId_t *pMutexId);
int cloud_mutex_lock(osMutexId_t *pMutexId, uint32_t timeout);
int cloud_mutex_unlock(osMutexId_t *pMutexId);
void cloud_fota_init(void);
#endif /* _CLOUD_UTILS__H */

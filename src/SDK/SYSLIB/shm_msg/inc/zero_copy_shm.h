
#ifndef __ZERO_COPY_SHM_H__
#define __ZERO_COPY_SHM_H__


extern osMutexId_t shm_mem_alloc_m;

typedef struct _T_zero_copy
{
	int         len;
    void   		*buf;
} T_zero_copy;

void shmmem_give_msg_process(int addr);

int shm_zero_copy(void* buf, int len, unsigned int msg_type);

void shmmem_malloc_msg_process(int ulSize);

#endif

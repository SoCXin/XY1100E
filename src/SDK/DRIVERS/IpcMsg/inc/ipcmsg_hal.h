#ifndef _IPCMSG_HAL_H
#define _IPCMSG_HAL_H

#include "osal.h"


#define IPC_WAITFROEVER  osWaitForever

typedef struct
{
    osMutexId_t pMutex;
} IpcMsg_MUTEX;

typedef struct
{
    osSemaphoreId_t pSemaphore;
}IpcMsg_SEMAPHORE;

long IpcMsg_Mutex_Create(IpcMsg_MUTEX *pMutexId);

long IpcMsg_Mutex_Destroy(IpcMsg_MUTEX *pMutexId);
long IpcMsg_Mutex_Lock(IpcMsg_MUTEX *pMutexId,unsigned long  xTicksToWait);

long IpcMsg_Mutex_Unlock(IpcMsg_MUTEX *pMutexId);


long IpcMsg_Semaphore_Create(IpcMsg_SEMAPHORE *pSemaphoreId);


long IpcMsg_Semaphore_Destroy(IpcMsg_SEMAPHORE *pSemaphoreId);

long IpcMsg_Semaphore_Take(IpcMsg_SEMAPHORE *pSemaphoreId,unsigned long  xTicksToWait);

long IpcMsg_Semaphore_Give(IpcMsg_SEMAPHORE *pSemaphoreId);

void IpcMsg_Semaphore_Give_ISR(IpcMsg_SEMAPHORE *pSemaphoreId);
int Ipc_SetInt(char ch_id);

#endif


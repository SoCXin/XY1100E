/* Copyright (C) 2012 mbed.org, MIT License
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**********************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which
 * might include those applicable to Huawei LiteOS of U.S. and the country in which you
 * are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
 * with such applicable export control laws and regulations.
 **********************************************************************************/

/* lwIP includes. */
#include "arch/sys_arch.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/opt.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"

struct timeoutlist
{
	struct sys_timeouts timeouts;
	osThreadId_t pid;
};

/* This is the number of threads that can be started with sys_thread_new() */

#ifdef SYS_THREAD_USED
#define SYS_THREAD_MAX 6
static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];
static u16_t s_nextthread = 0;
#endif
/*lint -e**/
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	(void)size;
	*mbox = osMessageQueueNew(archMESG_QUEUE_LENGTH, sizeof(void *));
#if SYS_STATS
	++lwip_stats.sys.mbox.used;
	if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used)
	{
		lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	}
#endif
	if (*mbox == NULL)
		return ERR_MEM;
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t *mbox)
{
	if (osMessageQueueGetCount(*mbox) != 0)
	{
#if SYS_STATS
		lwip_stats.sys.mbox.err++;
#endif
	}

	osMessageQueueDelete(*mbox);

#if SYS_STATS
	--lwip_stats.sys.mbox.used;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
	while (osMessageQueuePut(*mbox, &data, 0, osWaitForever) != osOK)
	{
	}
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t result;

	if (osMessageQueuePut(*mbox, &msg, 0, osNoWait) == osOK)
	{
		result = ERR_OK;
	}
	else
	{
		result = ERR_MEM;

#if SYS_STATS
		lwip_stats.sys.mbox.err++;
#endif
	}

	return result;
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
  return sys_mbox_trypost(mbox, msg);
}

u32_t sys_arch_mbox_fetch_ext(sys_mbox_t *mbox, void **msg, u32_t timeout, u8_t ignore_timeout)
{
	(void) mbox;
	(void) msg;
	(void) timeout;
	(void) ignore_timeout;

	return 0;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	// portTickType StartTime, EndTime, Elapsed;
	u32_t StartTime, EndTime, Elapsed;

	StartTime = osKernelGetTickCount();

	if (msg == NULL)
	{
		msg = &dummyptr;
	}

	if (timeout != 0)
	{
		if (osOK == osMessageQueueGet(*mbox, &(*msg), NULL, timeout / OS_TICK_PERIOD_MS))
		{
			EndTime = osKernelGetTickCount();
			Elapsed = (EndTime - StartTime) * OS_TICK_PERIOD_MS;

			return (Elapsed);
		}
		else
		{
			*msg = NULL;

			return SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		while (osOK != osMessageQueueGet(*mbox, &(*msg), NULL, osWaitForever))
		{
		}
		EndTime = osKernelGetTickCount();
		Elapsed = (EndTime - StartTime) * OS_TICK_PERIOD_MS;

		return (Elapsed);
	}
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	void *dummyptr;

	if (msg == NULL)
	{
		msg = &dummyptr;
	}

	if (osOK == osMessageQueueGet(*mbox, &(*msg), NULL, osNoWait))
	{
		return ERR_OK;
	}
	else
	{
		return SYS_MBOX_EMPTY;
	}
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
	if (*mbox == SYS_MBOX_NULL)
		return 0;
	else
		return 1;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = SYS_MBOX_NULL;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
#ifdef SYS_THREAD_USED
void sys_init(void)
{
	int i;

	for (i = 0; i < SYS_THREAD_MAX; i++)
	{
		s_timeoutlist[i].pid = 0;
		s_timeoutlist[i].timeouts.next = NULL;
	}

	s_nextthread = 0;
}

struct sys_timeouts *sys_arch_timeouts(void)
{
	int i;
	osThreadId_t pid;
	struct timeoutlist *tl;

	pid = osThreadGetId();

	for (i = 0; i < s_nextthread; i++)
	{
		tl = &(s_timeoutlist[i]);
		if (tl->pid == pid)
		{
			return &(tl->timeouts);
		}
	}

	return NULL;
}
#endif

#if LWIP_COMPAT_MUTEX == 0
err_t sys_mutex_new(sys_mutex_t *mutex)
{
	*mutex = (sys_mutex_t)osMutexNew(osMutexPrioInherit);
	if (*mutex == NULL)
	{
#if SYS_STATS
		++lwip_stats.sys.mutex.err;
#endif
		return ERR_MEM;
	}

#if SYS_STATS
	++lwip_stats.sys.mutex.used;
	if (lwip_stats.sys.mutex.max < lwip_stats.sys.mutex.used)
	{
		lwip_stats.sys.mutex.max = lwip_stats.sys.mutex.used;
	}
#endif
}

void sys_mutex_free(sys_mutex_t *mutex)
{
#if SYS_STATS
	--lwip_stats.sys.mutex.used;
#endif

	osMutexDelete(*mutex);
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
	sys_arch_sem_wait(*mutex, 0);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
	osMutexRelease(*mutex);
}

#endif //LWIP_COMPAT_MUTEX

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      OS.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t
sys_arch_protect(void)
{
	if(osKernelGetState() != osKernelInactive)
		return osKernelLock();
	else
		return -1;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an OS.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect(sys_prot_t pval)
{
	if(osKernelGetState() != osKernelInactive)
		osKernelRestoreLock(pval);
}

u32_t sys_now(void)
{
	/* Lwip docs mentioned like wraparound is not a problem in this funtion */
	return (u32_t)osKernelGetTickCount();
}

#ifdef SYS_THREAD_USED
sys_thread_t
sys_thread_new(char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	osThreadId_t createdTask = NULL;

	if (s_nextthread < SYS_THREAD_MAX)
	{
		createdTask = osThreadNew((osThreadFunc_t)(thread), arg, (signed char *)name, stacksize, prio);

		if (createdTask != NULL)
		{
			s_timeoutlist[s_nextthread++].pid = createdTask;
			return createdTask;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
#endif

#ifdef LWIP_DEBUG

/** \brief  Displays an error message on assertion

    This function will display an error message on an assertion
    to the dbg output.

    \param[in]    msg   Error message to display
    \param[in]    line  Line number in file with error
    \param[in]    file  Filename with error
 */
void assert_printf(char *msg, int line, char *file)
{
	if (msg)
	{
		LWIP_DEBUGF(LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
					("%s:%d in file %s", msg, line, file));
		return;
	}
	else
	{
		LWIP_DEBUGF(LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
					("LWIP ASSERT"));
		return;
	}
}
#endif /* LWIP_DEBUG */

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	*sem = osSemaphoreNew(0xFFFF, count);
	if (*sem == NULL)
	{
#if SYS_STATS
		++lwip_stats.sys.sem.err;
#endif
		return ERR_MEM;
	}

#if SYS_STATS
	++lwip_stats.sys.sem.used;
	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used)
	{
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif

	return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	// portTickType StartTime, EndTime, Elapsed;
	u32_t StartTime, EndTime, Elapsed;
	StartTime = osKernelGetTickCount();

	if (timeout != 0)
	{
		if (osSemaphoreAcquire(*sem, timeout / OS_TICK_PERIOD_MS) == osOK)
		{
			EndTime = osKernelGetTickCount();
			Elapsed = (EndTime - StartTime) * OS_TICK_PERIOD_MS;
			return (Elapsed);
		}
		else
		{
			return SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		while (osSemaphoreAcquire(*sem, osWaitForever) != osOK)
		{
		}
		EndTime = osKernelGetTickCount();
		Elapsed = (EndTime - StartTime) * OS_TICK_PERIOD_MS;
		return (Elapsed);
	}
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *sem)
{
	osSemaphoreRelease(*sem);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/

void sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
	--lwip_stats.sys.sem.used;
#endif
	osSemaphoreDelete(*sem);
}

int sys_sem_valid(sys_sem_t *sem)
{
	if (*sem == SYS_SEM_NULL)
		return 0;
	else
		return 1;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = SYS_SEM_NULL;
}

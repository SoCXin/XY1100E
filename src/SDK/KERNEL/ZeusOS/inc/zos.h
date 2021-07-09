/*
 * Copyright (c) 2019-2029, ZeusOS Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-03-23     longmain       first version
 */

#ifndef __ZOS_H__
#define __ZOS_H__

#include "zos_def.h"
#include "zos_log.h"



//引用硬件驱动头文件
#include "zos_pin.h"
#include "zos_i2c.h"
#include "zos_spi.h"
#include "zos_uart.h"
#include "zos_pwm.h"
#include "zos_flash.h"
#include "zos_rtc.h"
#include "zos_adc.h"
#include "nb.h"


#include "xy_api.h"


/**
 * @brief    进入临界区，并临时关闭中断 
 * @warning  关闭中断时间不宜过长，否则会造成底层驱动工作异常，如丢数据等
 */
extern  void zos_enter_critical(void);

/**
 * @brief    退出临界区，并恢复中断 
 * @warning  
 */
extern  void zos_exit_critical(void);

/**
 * @brief    锁住内核调度，返回当前内核状态，以便后续恢复
 * 返回以前的锁定状态（1-已锁定，0-未锁定，如果为负，则返回错误代码）。
 * @warning  一般与osKernelRestoreLock配对使用，而非osKernelUnlock
 */
extern  zos_int32_t zos_kernel_lock (void);
 
//解锁RTOS内核调度程序。
//返回以前的锁定状态（1-已锁定，0-未锁定，如果为负，则返回错误代码）。
extern  zos_int32_t zos_kernel_unlock (void);
 
//还原RTOS内核调度程序锁定状态。
//param[in]锁定状态由 zos_kernel_lock或zos_kernel_unlock获得。
//返回新的锁定状态（1-已锁定，0-未锁定，如果为负，则返回错误代码）。
extern  zos_int32_t zos_kernel_restoreLock (int32_t lock);
 
/**
 * @brief    获取系统当前的tick数，常用于计时，单位为1毫秒，具体值参看osKernelGetTickFreq返回值
 * @warning  osKernelLock锁调度期间，tick数不会被累加
 */
extern  zos_uint32_t zos_kernel_get_tick_count (void);
 
/**
 * @brief    tick的频率，目前为1毫秒
 * @warning  
 */
extern  zos_uint32_t zos_kernel_get_tick_freq (void);
 

 


/*
 * clock & timer interface
 */

/**
 * @brief   创建一个软件定时器
 * 
 * @param name       [IN] 软件定时器的名称
 * @param func       [IN] 软件定时器的超时回调函数，该回调函数内不得阻塞，且代码不能太长，以免影响其他timer的运行时序
 * @param parameter   [IN] 超时函数的参数，这里是无效参数，填NULL即可，内部不会使用
 * @param flag       [IN] 软件定时器的类型，是只超时一次，还是自动重装

 * @return   创建的软件定时器的id，创建失败则返回NULL
 * @warning  系统默认该接口的timer参与standby睡眠唤醒，即会准时从standby唤醒。 \n
 * 如果设置的周期过短，造成standby唤醒过于频繁，可以通过设置osTimerSetLowPowerFlag接口来避免频繁的standby唤醒。 
 * @attention  超时回调函数不得使用阻塞机制，且代码不能运行太长事件，以免影响其他timer的运行时序
 */
extern  zos_timer_t zos_timer_create(const char *name,
                           void (*timeout)(void *parameter),
                           void       *parameter,
                           zos_uint8_t  flag);

/**
 * @brief    删除某ID
 * @warning  不能在中断或临界区中调用
 */
extern  zos_err_t zos_timer_delete(zos_timer_t timer);

/**
 * @brief    开启某timer
 * @param timer      [IN] 软定时器ID
 * @param ticks      [IN] tick数，目前单位为毫秒
 * @warning  不能在中断或临界区中调用
 * @attention  超时回调函数不得使用阻塞机制，且代码不能运行太长事件，以免影响其他timer的运行时序
 */
extern  zos_err_t zos_timer_start(zos_timer_t timer,zos_tick_t tick);

/**
 * @brief    停止某timer
 * @warning  不能在中断或临界区中调用
 */
extern  zos_err_t zos_timer_stop(zos_timer_t timer);




//  ==== task Management Functions ====

/**
 * @brief   创建一个任务
 * 
 * @param name      [IN] 任务的入口名字
 * @param func      [IN] 任务的入口函数
 * @param argument  [IN] 任务入口函数的参数
 * @param stack_sz  [IN] 为任务分配的栈空间大小，单位为byte
 * @param prio      [IN] 任务的优先级
 * @return   创建的任务的id，返回NULL表示创建失败
 * @warning  用户新增的任务优先级建议设置为osPriorityNormal，不得过高！ \n
 * 任务默认不受standby睡眠影响，即会准时进行超时调度。如果客户任务中存在高频率的超时轮询机制，如5秒，则会大大影响功耗，请与骑士FAE联系。 
 */
extern  zos_task_t zos_task_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             zos_uint32_t stack_size,
                             zos_uint8_t  priority);


/**
 * @brief    强行结束某个指定的线程
 * @param thread_id   [IN] 线程句柄ID
 * @return 结果码
 * @code
    void  main()
    {
	    ............
	    if(g_wdt_TskHandle != NULL)
		{
			zos_task_delete(g_wdt_TskHandle);
		}
	}
   @endcode
 * @attention   由于操作系统内部不能销毁指定线程的关联资源，请慎用！
 * @warning  异步强行结束指定线程，需要调用者确保调用之前已经释放完该指定线程的堆内存、队列、信号量、互斥量、timer等全局资源，否则会造成系统异常  
 */
extern  zos_err_t zos_task_delete(zos_task_t task);

/**
 * @brief    获取当前工作线程的ID
 * @warning  
 */
extern  zos_task_t zos_task_self(void);

/**
 * @brief    通过名字获取线程的ID
 * @warning  
 */
extern  zos_task_t zos_task_find(char *name);

/**
 * @brief    主动触发一次线程调度，调度完后可能仍然在当前线程执行
 * @warning  不建议使用
 */
extern  zos_err_t zos_task_yield(void);

/**
 * @brief    挂起某个线程ID，并触发一次调度
 * @warning  不能在中断或临界区中调用，与osThreadResume配套使用
 */
extern  zos_err_t zos_task_suspend(zos_task_t task);

/**
 * @brief    恢复某线程ID，并触发一次调度
 * @warning  不能在中断或临界区中调用，不建议使用
 */
extern  zos_err_t zos_task_resume(zos_task_t task);

/**
 * @brief    动态设置某ID线程的调度优先级
 * @param   task   [in]  task ID obtained by \ref zos_task_create or \ref zos_task_self.
 * @param   priority    [in]  new priority value for the task function.
 * @warning  普通客户线程，建议设置不高于osPriorityNormal
 */
extern  zos_err_t zos_task_set_priority(zos_task_t task,int priority);
/**
 * @brief    根据ID获取某线程当前的状态
 * @warning  
 */
extern  task_state_t zos_task_get_state(zos_task_t task);


/**
 * @brief    根据ID获取某线程栈使用的历史峰值，单位为字节
 * @warning  
 */
extern  zos_uint32_t zos_task_get_stack_space(zos_task_t task);

/**
 * @brief    根据ID获取某线程栈的当前优先级
 * @warning  不能在中断或临界区中调用
 */
extern  task_priority_t zos_task_get_priority(zos_task_t task);

/**
 * @brief    延迟若干tick后继续执行当前线程，等同于sleep。目前单位为毫秒
 * @warning  1. 不能在中断或临界区中调用 \n
 *           2. 不能在操作系统启动前调用，即只能在线程中调用 \n
 *           3. 不能在idle线程中调用
 */
extern  zos_err_t zos_task_delay(zos_tick_t tick);

/**
 * @brief    延迟若干tick后继续执行当前线程，等同于sleep。目前单位为毫秒
 * @warning  1. 不能在中断或临界区中调用 \n
 *           2. 不能在操作系统启动前调用，即只能在线程中调用 \n
 *           3. 不能在idle线程中调用
 */
extern  zos_err_t zos_task_mdelay(zos_int32_t ms);

/**
 * @brief    延迟若干tick后继续执行当前线程，系统内部保证准时调度
 * @warning  会额外增加系统调度负荷，不建议使用  \n
 *           1. 不能在中断或临界区中调用 \n
 *           2. 不能在操作系统启动前调用，即只能在线程中调用 \n
 *           3. 不能在idle线程中调用
 */
extern  zos_err_t zos_task_mdelay_Until(zos_int32_t ms);


/*
 * semaphore interface
 */

/**
 * @brief    新建一个信号量
 * @param	 max_count      [IN]  信号量的最大个数，为1时即二值信号量，建议设置为0XFFFF 
 * @param	 initial_count  [IN]  信号量的初始值，通常设为0
 * @warning  不能在中断或临界区中调用
 */
zos_sem_t zos_sem_create(uint32_t max_count, uint32_t initial_count);

/**
 * @brief    删除某信号量
 * @warning  不能在中断或临界区中调用
 */
zos_err_t zos_sem_delete(zos_sem_t sem);

/**
 * @brief    超时等待某信号量
 * @param  sem    [in]  sem_id
 * @param  time   [in]  超时tick数，目前单位为毫秒
 * @warning  1. 在操作系统启动前调用时，超时时间必须为0 \n
 *           2. 在idle线程中调用时，超时时间必须为0
 */
zos_err_t zos_sem_wait(zos_sem_t sem, zos_int32_t time);

/**
 * @brief    释放某信号量
 * @warning  
 */
zos_err_t zos_sem_post(zos_sem_t sem);


/*
 * mutex interface
 */

/**
 * @brief    新建一个互斥量，入参建议为mutex_flag_PrioInherit
 * @warning  不能在中断或临界区中调用
 */
extern  zos_mutex_t zos_mutex_create(zos_uint8_t flag);

/**
 * @brief    删除某互斥量
 * @warning  不能在中断或临界区中调用
 */
extern  zos_err_t zos_mutex_delete(zos_mutex_t mutex);

/**
 * @brief    超时等待某互斥量
 * @param  mutex  [in]  互斥量ID
 * @param  time   [in]  超时tick数，目前单位为毫秒
 * @warning  1. 不能在中断或临界区中调用 \n
 *           2. 不能在操作系统启动前调用，即只能在线程中调用 \n
 *           3. 在idle线程中调用时，超时时间必须为0
 */
extern  zos_err_t zos_mutex_lock(zos_mutex_t mutex, zos_int32_t time);

/**
 * @brief    释放某互斥量
 * @warning  1. 不能在中断或临界区中调用 \n
 *           2. 不能在操作系统启动前调用，即只能在线程中调用
 */
extern  zos_err_t zos_mutex_unlock(zos_mutex_t mutex);


#ifdef ZOS_USING_EVENT
/*
 * event interface
 */
extern  zos_err_t zos_event_init(zos_event_t event, const char *name, zos_uint8_t flag);
extern  zos_err_t zos_event_detach(zos_event_t event);
extern  zos_event_t zos_event_create(const char *name, zos_uint8_t flag);
extern  zos_err_t zos_event_delete(zos_event_t event);

extern  zos_err_t zos_event_send(zos_event_t event, zos_uint32_t set);
extern  zos_err_t zos_event_recv(zos_event_t   event,
                       zos_uint32_t  set,
                       zos_uint8_t   opt,
                       zos_int32_t   timeout,
                       zos_uint32_t *recved);
extern  zos_err_t zos_event_control(zos_event_t event, int cmd, void *arg);
#endif


/*
 * message queue interface
 */

/**
 * @brief    新建一个消息队列
 * @param	 max_msgs   [IN]  每次发送和接收消息体大小，建议始终设为sizeof(void *)
 * @param	 msg_size  [IN]  最大消息缓存个数
 * @code
    void  init_user_task()
    {
	   zos_mq_t user_mq = ZOS_NULL;
	   user_mq = zos_mq_create( sizeof(void *),16);
	}
 * @endcode
 * @note  该消息队列建议传递消息体的地址，需要保证发送方申请消息体内存，接收方释放消息体内存
 * @warning  不能在中断或临界区中调用
 */
extern  zos_mq_t zos_mq_create(zos_size_t   msg_size, zos_size_t   max_msgs);

extern  zos_err_t zos_mq_delete(zos_mq_t mq);

/**
 * @brief    写一个消息到队列中
 * @param	 mq         [IN]  队列ID
 * @param	 buffer     [IN]  消息体的地址，建议使用双指针，队列管道传递的是消息体的地址
 * @param	 buff_size  [IN]  目前未使用
 * @param	 timeout    [IN]  等待写完成的时间，单位为tick数，目前为毫秒；0表示一旦发现队列满，立即返回错误；ZOS_WAITING_FOREVER表示死等，确保消息入队；建议使用ZOS_WAITING_FOREVER
 * @code
    void  user_send_msg()
    {
		struct user_msg *msg =NULL;

		msg = zos_zalloc(sizeof(struct user_msg));
		//set msg param value
		ret = zos_mq_send(user_mq, (void *)&msg, 0, ZOS_WAITING_FOREVER);
	   zos_assert(ret == ZOS_EOK);
	}
 * @endcode
 * @note  该消息队列建议传递消息体的地址，需要保证发送方申请消息体内存，接收方释放消息体内存
 * @warning  1. 在操作系统启动前调用时，超时时间必须为0 \n
 *           2. 在idle线程中调用时，超时时间必须为0
 */
extern  zos_err_t zos_mq_send(zos_mq_t *mq, void *buffer, zos_size_t buff_size, zos_tick_t timeout);

/**
 * @brief    超时等待获取一个消息体
 * @param	 mq        [IN]  队列ID
 * @param	 buffer   [IN]  消息体的地址，建议使用双指针，队列管道传递的是消息体的地址
 * @param	 size  [IN]  目前未使用
 * @param	 timeout   [IN]  等待时长，单位为tick数，目前为毫秒；0表示不等待，若无消息，立即返回错误；用户按照需要设置超时等待时长，不建议设为0
 * @code
    void  proc_user_msg_task()
	{
		struct user_msg *msg = NULL;
		while(1) 
		{	
			zos_mq_recv(user_mq, (void *)(&msg), NULL, ZOS_WAITING_FOREVER);
			zos_assert(msg != NULL);
			
			switch(msg->msg_id)
			{
				case :
			}
			zos_free(msg);
			msg = NULL;
		}
	}
 * @endcode
 * @note  该消息队列建议传递消息体的地址，需要保证发送方申请消息体内存，接收方释放消息体内存
 * @warning  1. 在操作系统启动前调用时，超时时间必须为0
 *           2. 在idle线程中调用时，超时时间必须为0
 */
extern  zos_err_t zos_mq_recv(zos_mq_t    mq,
                    void      *buffer,
                    zos_size_t  size,
                    zos_int32_t timeout);


/* ==== Memory Management Functions ==== */


extern void *zos_malloc(zos_size_t nbytes);
extern void *zos_zalloc(zos_size_t nbytes);
extern void *zos_malloc_Align(zos_size_t nbytes);
extern void *zos_zalloc_Align(zos_size_t nbytes);
extern void zos_free(void *ptr);


extern  void zos_show_version(void);

#endif //__OC_H__

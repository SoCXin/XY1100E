#ifndef __NB_MASTER_INIT_H
#define __NB_MASTER_INIT_H

#include "xy_api.h"


#define NB_MASTER_SPI_MOSI_PIN       HAL_GPIO_PIN_NUM_6
#define NB_MASTER_SPI_MISO_PIN       HAL_GPIO_PIN_NUM_7
#define NB_MASTER_SPI_SCLK_PIN       HAL_GPIO_PIN_NUM_8
#define NB_MASTER_SPI_CS_PIN         HAL_GPIO_PIN_NUM_9
#define NB_MASTER_IN_MSG_PIN         HAL_GPIO_PIN_NUM_13

#define NB_MASTER_RECV_TASK_PRIORITY      9
#define NB_MASTER_RECV_TASK_STACK_SIZE    1024
#define NB_MASTER_PROC_TASK_PRIORITY      10
#define NB_MASTER_PROC_TASK_STACK_SIZE    1024


extern osThreadId_t  nb_master_recv_TskHander;
extern osThreadId_t  nb_master_proc_TskHander;
extern osMessageQueueId_t nb_master_proc_Queue;
extern osSemaphoreId_t nb_master_Semaphore;
extern osMutexId_t nb_master_Send_Mutex;

extern HAL_SPI_HandleTypeDef nb_spi_master;

/*****************************************************************************************/
/**
  * @func   void nb_master_proc_task(void)
  * @brief  nb主机处理线程，将解析完成的数据进行相应处理
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void nb_master_proc_task(void);

/*****************************************************************************************/
/**
  * @func   void nb_master_spi_init(void)
  * @brief  nb主机spi硬件初始化
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void nb_master_spi_init(void);
/*****************************************************************************************/
/**
  * @func   void bms_slave_task_init(void)
  * @brief  nb主机任务初始化
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void nb_master_task_init(void);


#endif	/* __NB_MASTER_INIT_H */

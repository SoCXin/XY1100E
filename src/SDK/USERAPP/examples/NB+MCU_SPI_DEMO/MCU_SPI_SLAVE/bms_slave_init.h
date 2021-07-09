#ifndef __BMS_SLAVE_INIT_H
#define __BMS_SLAVE_INIT_H

#include "xy_api.h"

#define BMS_SLAVE_SPI_MOSI_PIN       HAL_GPIO_PIN_NUM_6
#define BMS_SLAVE_SPI_MISO_PIN       HAL_GPIO_PIN_NUM_7
#define BMS_SLAVE_SPI_SCLK_PIN       HAL_GPIO_PIN_NUM_8
#define BMS_SLAVE_SPI_CS_PIN         HAL_GPIO_PIN_NUM_9
#define BMS_SLAVE_OUT_MSG_PIN        HAL_GPIO_PIN_NUM_13
#define BMS_SLAVE_WAKEUP_PIN         HAL_GPIO_PIN_NUM_20

#define BMS_SLAVE_RECV_TASK_PRIORITY      9
#define BMS_SLAVE_RECV_TASK_STACK_SIZE    1024
#define BMS_SLAVE_PROC_TASK_PRIORITY      10
#define BMS_SLAVE_PROC_TASK_STACK_SIZE    1024



extern osThreadId_t  bms_slave_recv_TskHander;
extern osThreadId_t  bms_slave_proc_TskHander;
extern osMessageQueueId_t bms_slave_proc_Queue;
extern osSemaphoreId_t bms_slave_Semaphore;

extern HAL_SPI_HandleTypeDef bms_spi_slave;

/*****************************************************************************************/
/**
  * @func   void bms_slave_proc_task(void)
  * @brief  bms从机处理线程，将解析完成的数据进行相应处理
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_proc_task(void);

/*****************************************************************************************/
/**
  * @func   void bms_slave_spi_init(void)
  * @brief  bms从机spi硬件初始化
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_spi_init(void);
/*****************************************************************************************/
/**
  * @func   void bms_slave_task_init(void)
  * @brief  bms从机任务初始化
  * @param  none
  * @retval none
  */
/*****************************************************************************************/
void bms_slave_task_init(void);


#endif	/* __BMS_SLAVE_INIT_H */

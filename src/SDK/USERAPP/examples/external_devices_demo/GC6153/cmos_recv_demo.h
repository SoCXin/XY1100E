/**
* @file        cmos_recv_demo.h
* @brief       This file is the CMOS recive demo API that users can call.
* @attention
* User can not call the function in flash when excute interrupt handler, include the subfunction, if necessary,
* put this function in ram, just declare this function with a suffix " __attribute__((section(".ramtext"))) "
*/
#include "cmos_gc6153.h"
#include "xy_api.h"

/**
 * @brief This function is the SPI initialization function used to receive CMOS data.
 * 
 * @return int32_t 
 */
int32_t CMOS_SPI_Init(void);

/**
 * @brief This function is the initialization function used to generate CMOS working clock.
 * 
 */
void CMOS_PWM_Init(void);

/**
 * @brief This function is used to control the initialization function of CMOS startup sequence.
 * 
 * @return int32_t 
 */
int32_t CMOS_GPIO_Init(void);

/**
 * @brief This function is a thread initialization function used to receive CMOS data and forward DEMO through the serial port.
 * 
 * @return  
 */
void CMOS_RECV_TASK_INIT(void);

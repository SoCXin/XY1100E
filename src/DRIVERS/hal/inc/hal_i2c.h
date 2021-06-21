/**
  ******************************************************************************
  * @file      hal_i2c.h
  * @ingroup   外设
  * @brief     I2C外设有两路，I2C1和I2C2，传输速度可以为100kbit/s或400kbit/s，可以设置主模式或从模式，从机地址可以设置成7位或10位.
  ******************************************************************************
  */

/* 防止递归包含 -----------------------------------------------------------------*/
#pragma once

/* 引用头文件 ------------------------------------------------------------------*/
#include "hal_cortex.h"
#include "hw_memmap.h"


/** 
  * @brief  I2C工作状态枚举
  */
typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< I2C外设在初始化状态			   */
  HAL_I2C_STATE_READY             = 0x01U,   /*!< I2C外设已初始化并就绪                                    */
  HAL_I2C_STATE_BUSY              = 0x02U,   /*!< I2C外设处于忙状态                                             */
  HAL_I2C_STATE_RXRW              = 0x08U,   /*!< I2C外设处于读写状态                                         */
  HAL_I2C_STATE_RXDV              = 0x20U,   /*!< I2C外设中有有效数据处于接收状态              */
  HAL_I2C_STATE_TXDV              = 0x40U,   /*!< I2C外设中有有效数据处于发送状态               */
  HAL_I2C_STATE_RXOVF             = 0x80U,   /*!< I2C外设接收溢出        			   */
  HAL_I2C_STATE_BA                = 0x100U,  /*!< I2C外设总线正常			   */
  HAL_I2C_STATE_FIFO_OVF          = 0x101U,  /*!< I2C外设FIFO溢出                                             */
  HAL_I2C_STATE_TIMEOUT           = 0x102U,  /*!< I2C外设接收或发送超时                                    */
  HAL_I2C_STATE_ERROR             = 0x104U   /*!< I2C外设状态错误                                                  */
} HAL_I2C_StateTypeDef;

/**
  * @brief I2C寄存器结构体
  */
typedef struct
{
  volatile uint32_t CONTROL;                         /*!< I2C控制寄存器         */
  volatile uint32_t STATUS;                          /*!< I2C状态寄存器         */
  volatile uint32_t ADDRESS;                         /*!< I2C地址寄存器         */
  volatile uint32_t DATA;                            /*!< I2C传输数据寄存器 */
  volatile uint32_t INT_STATUS;                      /*!< I2C中断状态寄存器 */
  volatile uint32_t TRAN_SIZE;                       /*!< I2C传输长度寄存器 */
  volatile uint32_t SLAVE_MONITOR_PAUSE;             /*!< I2C检测停止寄存器 */
  volatile uint32_t TIMEOUT;                         /*!< I2C超时寄存器          */
  volatile uint32_t INT_MASK;                        /*!< I2C中断掩码寄存器 */
  volatile uint32_t INT_ENABLE;                      /*!< I2C中断使能寄存器 */
  volatile uint32_t INT_DISABLE;                     /*!< I2C中断失能寄存器 */
} HAL_I2C_TypeDef;

/** 
  * @brief  I2C传输速度枚举
  */
typedef enum {
  HAL_I2C_CLK_SPEED_100K = 100000U,           /*!< I2C传输速度100K   */
  HAL_I2C_CLK_SPEED_400K = 400000U            /*!< I2C传输速度400K   */
} HAL_I2C_CLK_SpeedTypeDef;

/** 
  * @brief  I2C传输模式枚举
  */
typedef enum
{
  HAL_I2C_MODE_SLAVE     = 0x00U,             /*!< I2C从模式   */
  HAL_I2C_MODE_MASTER    = 0x02U,             /*!< I2C主模式   */
  HAL_I2C_MODE_NONE      = 0x04U              /*!< I2C模式空   */
}HAL_I2CModeTypeDef;

/** 
  * @brief  I2C传输地址模式枚举，7位或10位地址，当地址为最后一位为读写位时，需要用户将该地址右移一位
  */
typedef enum
{
  HAL_I2C_ADDRESS_10BITS = 0x00U,             /*!< I2C从机地址为10位   */
  HAL_I2C_ADDRESS_7BITS  = 0x04U              /*!< I2C从机地址为7位      */
}HAL_I2C_AddressModeTypeDef;

/** 
  * @brief  I2C初始化结构体
  */
typedef struct
{
  HAL_I2CModeTypeDef Mode;                   /*!< I2C传输模式，详见    @brf  HAL_I2CModeTypeDef           */

  HAL_I2C_CLK_SpeedTypeDef ClockSpeed;       /*!< I2C传输速度，详见    @brf  HAL_I2C_CLK_SpeedTypeDef     */

  uint32_t OwnAddress;                       /*!< I2C传输地址                                                                                                                                           */

  HAL_I2C_AddressModeTypeDef AddressingMode; /*!< I2C传输地址模式，详见    @brf HAL_I2C_AddressModeTypeDef */
} HAL_I2C_InitTypeDef;

/**
  * @brief  I2C中断类型枚举
  */
typedef enum {
  HAL_I2C_INTTYPE_COMP                      = 0x01U,/*!< I2C传输完成中断              */
  HAL_I2C_INTTYPE_DATA                      = 0x02U,/*!< I2C无应答中断                  */
  HAL_I2C_INTTYPE_NACK                      = 0x04U,/*!< I2C接收到数据中断         */
  HAL_I2C_INTTYPE_TO                        = 0x08U,/*!< I2C传输超时中断             */
  HAL_I2C_INTTYPE_SLV_RDY                   = 0x10U,/*!< I2C监测从机就绪中断    */
  HAL_I2C_INTTYPE_RX_OVF                    = 0x20U,/*!< I2C接收溢出中断             */
  HAL_I2C_INTTYPE_TX_OVF                    = 0x40U,/*!< I2C发送溢出中断             */
  HAL_I2C_INTTYPE_RX_UNF                    = 0x80U,/*!< I2C接收下溢中断             */
  HAL_I2C_INTTYPE_ARB_LOST                  = 0x200U/*!< I2C仲裁丢失中断             */
} HAL_I2C_Int_TypeDef;

/** 
  * @brief  I2C控制结构体
  */
typedef struct __I2C_HandleTypeDef
{
  HAL_I2C_TypeDef             *Instance;      /*!< I2C外设寄存器基地址                  */

  HAL_I2C_InitTypeDef         Init;           /*!< I2C初始化配置参数                     */

  uint8_t                     *pBuffPtr;      /*!< I2C数据传输地址                          */

  volatile uint16_t           XferSize;       /*!< I2C数据传输大小                          */

  volatile uint16_t           XferCount;      /*!< I2C数据传输大小计数                */

  HAL_LockTypeDef             Lock;           /*!< I2C设备锁                                      */

  volatile HAL_I2C_StateTypeDef  State;       /*!< I2C传输状态                                 */

  volatile HAL_I2CModeTypeDef    Mode;        /*!< I2C传输模式                                 */

  volatile uint32_t           ErrorCode;      /*!< I2C错误码                                     */

  volatile uint32_t           Devaddress;     /*!< I2C目标设备地址                        */

} HAL_I2C_HandleTypeDef;

/** 
  * @brief  I2C错误码类型枚举
  */
typedef enum
{
	HAL_I2C_ERROR_NONE		= 0x00U,	/*!<  I2C无错误			*/
	HAL_I2C_ERROR_TIMEOUT,				/*!<  I2C超时错误			*/
	HAL_I2C_ERROR_RX_OVF	= 0x20U,	/*!<  I2C接收溢出错误		*/
	HAL_I2C_ERROR_TX_OVF	= 0x40U,	/*!<  I2C发送溢出错误	    */
	HAL_I2C_ERROR_RX_UNF	= 0x80U,	/*!<  I2C接收下溢错误	    */
	HAL_I2C_ERROR_ARB_LOST	= 0x200U	/*!<  I2C仲裁丢失错误		*/
}HAL_I2C_ErrorCodeTypeDef;





/** @brief  使能I2C中断.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __INTERRUPT__：I2C中断类型，详见枚举类型  @ref HAL_I2C_Int_TypeDef.
  * @retval 无
  */
#define HAL_I2C_ENABLE_IT(__HANDLE__,__INTERRUPT__) SET_BIT((__HANDLE__)->Instance->INT_ENABLE,(__INTERRUPT__))

/** @brief  失能I2C中断.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __INTERRUPT__：I2C中断类型，详见枚举类型  @ref HAL_I2C_Int_TypeDef.
  * @retval 无
  */
#define HAL_I2C_DISABLE_IT(__HANDLE__,__INTERRUPT__) SET_BIT((__HANDLE__)->Instance->INT_DISABLE,(__INTERRUPT__))

/** @brief  清除I2C中断标志位.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __INTERRUPT__：I2C中断类型，详见枚举类型  @ref HAL_I2C_Int_TypeDef.
  * @retval 无
  */
#define HAL_I2C_CLEAR_IT(__HANDLE__,__INTERRUPT__) SET_BIT((__HANDLE__)->Instance->INT_STATUS,(__INTERRUPT__))

/** @brief  获取I2C状态标志.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __FLAG__：I2C状态， 详见  @ref HAL_I2C_StateTypeDef.
  * @retval 无.
  */
#define HAL_I2C_GET_FLAG(__HANDLE__,__FLAG__) (((((__HANDLE__)->Instance->STATUS) & (__FLAG__)) == (__FLAG__)) ? HAL_SET : HAL_RESET)

/** @brief  清除I2C状态标志.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __FLAG__：I2C状态， 详见  @ref HAL_I2C_StateTypeDef.
  * @retval 无
  */
#define HAL_I2C_CLEAR_FLAG(__HANDLE__,__FLAG__) (((__HANDLE__)->Instance->STATUS) &= (~(__FLAG__)))

/** @brief  设置I2C地址宽度，7位还是10位，详见  @ref HAL_I2C_AddressModeTypeDef.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_ADDR_MODE(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_NEA_Msk,((__HANDLE__)->Init.AddressingMode)))

/** @brief  设置I2C作为从机时的地址.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_SLAVE_OWN_ADDR(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->ADDRESS,0x3FF,((__HANDLE__)->Init.OwnAddress)))

/** @brief  设置I2C作为主机时目标从机的地址，若地址中最低位表示读写位，则需将该地址右移一位.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __ADDR__：需要设置的地址.
  * @retval 无
  */
#define HAL_I2C_SET_MASTER_DIR_ADDR(__HANDLE__,__ADDR__) (MODIFY_REG((__HANDLE__)->Instance->ADDRESS,0x3FF,(__ADDR__)))

/** @brief  清空I2C的FIFO.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_CLEAN_FIFO(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_CLR_FIFO_Msk,I2C_CTL_CLR_FIFO_EN))

/** @brief  设置I2C时钟分频系数.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  __VAL__：I2C分频系数.
  * @retval 无
  */
#define HAL_I2C_SET_EXP_CLK(__HANDLE__,__VAL__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_DIVISOR_Msk,(__VAL__)))

/** @brief  设置I2C主从模式.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_MODE(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_MS_Msk,(__HANDLE__)->Init.Mode))

/** @brief  使能主模式下的传输应答模式.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_ACKMODE(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_ACKEN_Msk,I2C_CTL_ACKEN_EN))

/** @brief  清除I2C控制寄存器.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_CLEAR_CONFIG(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,0xFFFF,0x0))

/** @brief  设置I2C为写方向。
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_TRAN_DIRECTION_WRITE(__HANDLE__) (MODIFY_REG((__HANDLE__)->Instance->CONTROL,I2C_CTL_M_RW_Msk,I2C_CTL_M_WRITE))

/** @brief  设置I2C为读方向.
  * @param  __HANDLE__：I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
#define HAL_I2C_SET_TRAN_DIRECTION_READ(__HANDLE__) (SET_BIT((__HANDLE__)->Instance->CONTROL,I2C_CTL_M_READ))


/**
  * @brief  获取I2C工作状态.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 返回I2C工作状态. 详见  @ref HAL_I2C_StateTypeDef.
  */
HAL_I2C_StateTypeDef HAL_I2C_GetState(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/**
  * @brief  获取I2CHandleStruct结构体成员ErrorCode的值.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 返回I2C错误码.详见  @ref HAL_I2C_ErrorCodeTypeDef。
  */
uint32_t HAL_I2C_GetError(HAL_I2C_HandleTypeDef *I2CHandleStruct);

/**
  * @brief  获取I2CHandleStruct结构体成员Mode的值.即I2C的模式是主模式还是从模式还是无。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval I2C工作模式。详见  @ref HAL_I2CModeTypeDef。
  */
HAL_I2CModeTypeDef HAL_I2C_GetMode(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/** @brief  使能I2C外设时钟。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无。
  */
void HAL_I2C_ENABLE(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/** @brief  失能I2C外设时钟。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无。
  */
void HAL_I2C_DISABLE(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/**
  * @brief  初始化I2C.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_I2C_Init(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/**
  * @brief  去初始化I2C.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示去初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_I2C_DeInit(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/**
  * @brief  I2C主模式下的发送阻塞函数，Timeout为阻塞时间.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  DevAddress：目标从机地址，7位地址时低7位有效，10位地址时低10位有效，若最后一位为R/W位，则需用户将该地址右移一位。
  * @param  pData：要发送的数据的存储地址。
  * @param  Size：要发送的数据的长度。
  * @param  Timeout：发送超时时间，0为立即退出，HAL_MAX_DELAY为永久阻塞.
  * @retval HAL状态.详见  @ref HAL_StatusTypeDef。
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(HAL_I2C_HandleTypeDef* I2CHandleStruct, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  I2C主模式下的接收阻塞函数，Timeout为阻塞时间.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  DevAddress：目标从机地址，7位地址时低7位有效，10位地址时低10位有效，若最后一位为R/W位，则需用户将该地址右移一位。
  * @param  pData：要接收的数据的存储地址。
  * @param  Size：要接收的数据的长度。
  * @param  Timeout：接收超时时间，0为立即退出，HAL_MAX_DELAY为永久阻塞.
  * @retval HAL状态.详见  @ref HAL_StatusTypeDef。
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive(HAL_I2C_HandleTypeDef* I2CHandleStruct, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  I2C从机模式下发送数据阻塞函数，Timeout为阻塞时间。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  pData：要发送的数据的存储地址。
  * @param  Size：要发送的数据的长度。
  * @param  Timeout：发送超时时间，0为立即退出，HAL_MAX_DELAY为永久阻塞.
  * @retval HAL状态.详见  @ref HAL_StatusTypeDef。
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(HAL_I2C_HandleTypeDef* I2CHandleStruct, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  I2C从机模式下接收数据阻塞函数，Timeout为阻塞时间。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  pData：要接收的数据的存储地址。
  * @param  Size：要接收的数据的长度。
  * @param  Timeout：接收超时时间，0为立即退出，HAL_MAX_DELAY为永久阻塞.
  * @retval HAL状态.详见  @ref HAL_StatusTypeDef。
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive(HAL_I2C_HandleTypeDef* I2CHandleStruct, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  I2C从机模式下接收数据中断模式。
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @param  pData：要接收的数据的存储地址。
  * @param  Size：要接收的数据的长度。
  * @retval HAL状态.详见  @ref HAL_StatusTypeDef。
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(HAL_I2C_HandleTypeDef *I2CHandleStruct, uint8_t *pData, uint16_t Size);

/** @brief  获取I2C中断使能状态.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval I2C中断使能结果.详见  @ref HAL_I2C_Int_TypeDef.
  */
uint32_t HAL_I2C_GetITEnable(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/** @brief  获取I2C中断状态.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval I2C当前中断状态.详见  @ref HAL_I2C_Int_TypeDef.
  */
uint32_t HAL_I2C_GetITState(HAL_I2C_HandleTypeDef* I2CHandleStruct);

/**
  * @brief  I2C错误中断回调函数.在对应的CSP中断处理函数中调用.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
void HAL_I2C_ErrorCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct) __RAM_FUNC;

/**
  * @brief  I2C接收完成回调函数.在对应的CSP中断处理函数中调用.
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
void HAL_I2C_RxCpltCallback(HAL_I2C_HandleTypeDef *I2CHandleStruct) __RAM_FUNC;

/**
  * @brief  I2C中断处理函数.在对应的中断服务函数中调用..
  * @param  I2CHandleStruct. 详见结构体定义  @ref HAL_I2C_HandleTypeDef.
  * @retval 无
  */
void I2C_IRQHandler(HAL_I2C_HandleTypeDef *I2CHandleStruct) __RAM_FUNC;

/**
  * @brief  I2C1中断服务函数.I2C1中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_I2C1_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  I2C2中断服务函数.I2C2中断触发时调用此函数.
  * @param  None
  * @retval None
  */
void HAL_I2C2_IRQHandler(void) __RAM_FUNC;


/**
* @brief  I2C 外设.
*/
#define HAL_I2C1               ((HAL_I2C_TypeDef *)I2C1_BASE)  /*!< I2C1外设基地址   */
#define HAL_I2C2               ((HAL_I2C_TypeDef *)I2C2_BASE)  /*!< I2C2外设基地址   */





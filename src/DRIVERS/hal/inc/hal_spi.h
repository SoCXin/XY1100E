/**
  ******************************************************************************
  * @file      hal_spi.h
  * @ingroup   外设
  * @brief     SPI外设只有一路，相对于SPI的时钟pclk，一般为39.168MHz，全局为g_XY_PCLK，最高频率为2分频，最低频率为256分频，\n
  *            SPI可以设置成主模式或从模式，数据宽度可以设置成8位、16位或32位，工作模式可以设置成工作模式0、工作模式1、工作模式2、\n
  *            工作模式3，数据发送方式可以选择最高位有效或最低位有效.
  ******************************************************************************
  */

/* 防止递归包含 -----------------------------------------------------------------*/
#pragma once

/* 引用头文件 ------------------------------------------------------------------*/
#include "hal_cortex.h"
#include "hw_memmap.h"

/**
  * @brief SPI主从模式枚举，主机模式或从机模式
  */
typedef enum
{
  HAL_SPI_MODE_SLAVE       = 0x00U,    /*!< SPI外设从机模式                                */
  HAL_SPI_MODE_MASTER      = 0x01U     /*!< SPI外设主机模式                                */
} HAL_SPI_ModeTypeDef;

/**
  * @brief SPI传输数据宽度类型枚举
  */
typedef enum
{
  HAL_SPI_DATASIZE_8BIT       = 0x00U,    /*!< SPI外设传输数据宽度8位                                   */
  HAL_SPI_DATASIZE_16BIT      = 0x40U,    /*!< SPI外设传输数据宽度16位                                */
  HAL_SPI_DATASIZE_32BIT      = 0xc0U     /*!< SPI外设传输数据宽度32位                                */
} HAL_SPI_Data_SizeTypeDef;

/**
  * @brief SPI工作模式枚举
  * 时钟的极性（CPOL）用来决定在总线空闲时，同步时钟（SCK）信号线上的电位是高电平还是低电平。当时钟极性为0时（CPOL=0），SCK信号线在空闲时为低电平；当时钟极性为1时（CPOL=1），SCK信号线在空闲时为高电平\n
  * 时钟的相位（CPHA）用来决定何时进行信号采样，当时钟相位为0时（CPHA=0），在SCK信号线的第一个跳变沿进行采样当时钟相位为1时（CPHA=1），在SCK信号线的第二个跳变沿进行采样\n
  * 工作模式0： CPHA = 0 CPOL = 0\n
  * 工作模式1： CPHA = 0 CPOL = 1\n
  * 工作模式2： CPHA = 1 CPOL = 0\n
  * 工作模式3： CPHA = 1 CPOL = 1
  */
typedef enum
{
  HAL_SPI_WORKMODE_0      = 0x00U,    /*!< SPI外设工作模式0        */
  HAL_SPI_WORKMODE_1      = 0x04U,    /*!< SPI外设工作模式1        */
  HAL_SPI_WORKMODE_2      = 0x02U,    /*!< SPI外设工作模式2        */
  HAL_SPI_WORKMODE_3      = 0x06U     /*!< SPI外设工作模式3        */
} HAL_SPI_Work_ModeTypeDef;

/**
  * @brief	SPI的NSS管理模式枚举
  */
typedef enum
{
  HAL_SPI_NSS_SOFT         = 0x00U,    /*!< SPI外设软件控制NSS        */
  HAL_SPI_NSS_HARD         = 0x01U,    /*!< SPI外设硬件控制NSS        */
}HAL_SPI_NSS_managementTypeDef;

/**
  * @brief	SPI时钟分频系数枚举
  */
typedef enum {
  HAL_SPI_CLKDIV_2   = 0x0000U,     /*!< SPI外设时钟(pclk)分频为2分频                           */
  HAL_SPI_CLKDIV_4   = 0x0008U,     /*!< SPI外设时钟(pclk)分频为4分频                           */
  HAL_SPI_CLKDIV_8   = 0x0010U,     /*!< SPI外设时钟(pclk)分频为8分频                           */
  HAL_SPI_CLKDIV_16  = 0x0018U,     /*!< SPI外设时钟(pclk)分频为16分频                        */
  HAL_SPI_CLKDIV_32  = 0x0020U,     /*!< SPI外设时钟(pclk)分频为32分频                        */
  HAL_SPI_CLKDIV_64  = 0x0028U,     /*!< SPI外设时钟(pclk)分频为64分频                       */
  HAL_SPI_CLKDIV_128 = 0x0030U,     /*!< SPI外设时钟(pclk)分频为128分频                   */
  HAL_SPI_CLKDIV_256 = 0x0038U      /*!< SPI外设时钟(pclk)分频为256分频                   */
} HAL_SPI_ClkDiv_TypeDef;

/**
  * @brief	SPI数据传输模式枚举
  */
typedef enum
{
  HAL_SPI_FIRSTBIT_MSB      = 0x00U,     /*!< SPI外设数据最高位有效                           */
  HAL_SPI_FIRSTBIT_LSB      = 0x01U      /*!< SPI外设数据最低位有效                           */
} HAL_SPI_MSB_LSB_transmissionTypeDef;

/**
  * @brief  SPI初始化结构体
  */
typedef struct
{
  HAL_SPI_ModeTypeDef Mode;                     /*!< SPI模式，详见    @brf HAL_SPI_ModeTypeDef                          */

  HAL_SPI_Data_SizeTypeDef DataSize;            /*!< SPI数据大小，详见    @brf HAL_SPI_Data_SizeTypeDef                   */

  HAL_SPI_Work_ModeTypeDef WorkMode;         	/*!< SPI工作模式，详见    @brf HAL_SPI_Work_ModeTypeDef                   */

  HAL_SPI_NSS_managementTypeDef NSS;            /*!< SPI的NSS管理，详见    @brf HAL_SPI_NSS_managementTypeDef            */

  HAL_SPI_ClkDiv_TypeDef SPI_CLK_DIV;  		    /*!< SPI时钟分频系数，详见    @brf HAL_SPI_ClkDiv_TypeDef                  */

  HAL_SPI_MSB_LSB_transmissionTypeDef FirstBit; /*!< SPI数据传输模式，详见    @brf HAL_SPI_MSB_LSB_transmissionTypeDef     */

} HAL_SPI_InitTypeDef;

/**
  * @brief  SPI工作状态枚举
  */
typedef enum
{
  HAL_SPI_STATE_RESET      = 0x00U,    /*!< SPI外设正在初始化                                */
  HAL_SPI_STATE_READY      = 0x01U,    /*!< SPI外设已初始化并就绪                       */
  HAL_SPI_STATE_BUSY       = 0x02U,    /*!< SPI外设正在使用中，处于忙状态     */
  HAL_SPI_STATE_BUSY_TX    = 0x03U,    /*!< SPI外设正处在发送忙状态                   */
  HAL_SPI_STATE_BUSY_RX    = 0x04U,    /*!< SPI外设正处在接收忙状态                  */
  HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< SPI外设正处在收发忙状态                  */
  HAL_SPI_STATE_ERROR      = 0x06U,    /*!< SPI外设错误                                              */
  HAL_SPI_STATE_ABORT      = 0x07U,    /*!< SPI外设传输中止或取消                        */
} HAL_SPI_StateTypeDef;

/**
  * @brief SPI寄存器结构体
  */
typedef struct
{
  volatile uint32_t CONFIG;                     /*!< SPI控制寄存器                */
  volatile uint32_t INT_STATUS;                 /*!< SPI中断状态寄存器        */
  volatile uint32_t IEN;                        /*!< SPI中断使能寄存器        */
  volatile uint32_t IDIS;                       /*!< SPI中断失能寄存器        */
  volatile uint32_t IMASK;                      /*!< SPI中断掩码寄存器        */
  volatile uint32_t ENABLE;                     /*!< SPI使能寄存器                 */
  volatile uint32_t DELAY;                      /*!< SPI时钟同步延时             */
  union
  {
	  struct{
		  volatile uint8_t TXD_8;               /*!< SPI发送缓冲数据寄存器,8位    */
		  uint8_t TXD_RESERVED1[3];};
	  struct{
		  volatile uint16_t TXD_16;             /*!< SPI发送缓冲数据寄存器,16位 */
		  uint16_t TXD_RESERVED2;};
	  volatile uint32_t TXD;                    /*!< SPI发送缓冲数据寄存器,32位 */
  };
  union
  {
	  struct{
		  volatile uint8_t RXD_8;               /*!< SPI接收缓冲数据寄存器,8位     */
		  uint8_t RXD_RESERVED1[3];};
	  struct{
		  volatile uint16_t RXD_16;             /*!< SPI接收缓冲数据寄存器,16位 */
		  uint16_t RXD_RESERVED2;};
	  volatile uint32_t RXD;                    /*!< SPI接收缓冲数据寄存器,32位 */
  };
  volatile uint32_t SIC;                        /*!< SPI从机时钟空闲计数寄存器,主机提供时钟时,从机检测到设置的空闲周期值后开始处理数据 */
  volatile uint32_t TX_THRESH;                  /*!< SPI发送阈值设置寄存器                */
  volatile uint32_t RX_THRESH;                  /*!< SPI接收阈值寄存器                         */
  volatile uint32_t TX_FIFO_OP;                 /*!< SPI发送FIFO选择寄存器           */
  volatile uint32_t RX_FIFO_OP;                 /*!< SPI接收FIFO选择寄存器           */
  volatile uint32_t TXFIFO_STATUS;              /*!< SPI发送FIFO状态寄存器           */
  volatile uint32_t RXFIFO_STATUS;              /*!< SPI接收FIFO状态寄存器           */
  volatile uint32_t MOD_ID;                     /*!< SPI模块ID寄存器                           */
} HAL_SPI_TypeDef;

/**
  * @brief  SPI控制结构体
  */
typedef struct __SPI_HandleTypeDef
{
  HAL_SPI_TypeDef               *Instance;      /*!< SPI外设寄存器基地址         */

  HAL_SPI_InitTypeDef           Init;           /*!< SPI初始化配置参数            */

  uint8_t                       *pTxBuffPtr;    /*!< SPI发送数据存储地址        */

  volatile uint16_t             TxXferSize;     /*!< SPI发送数据大小                 */

  volatile uint16_t             TxXferCount;    /*!< SPI发送数据计数                 */

  uint8_t                       *pRxBuffPtr;    /*!< SPI接收数据存储地址        */

  volatile uint16_t             RxXferSize;     /*!< SPI接收数据大小                 */

  volatile uint16_t             RxXferCount;    /*!< SPI接收数据计数                 */

  HAL_LockTypeDef               Lock;           /*!< SPI设备锁                              */

  volatile HAL_SPI_StateTypeDef     State;      /*!< SPI工作状态                          */

  volatile uint32_t             ErrorCode;      /*!< SPI错误码                              */
} HAL_SPI_HandleTypeDef;

/**
  * @brief	SPI错误码类型枚举
  */
typedef enum
{
 HAL_SPI_ERROR_NONE			= 0x00U,      /*!< SPI无错误                                            */
 HAL_SPI_ERROR_REV_OVERFLOW	= 0x01U,      /*!< SPI接收溢出错误码                          */
 HAL_SPI_ERROR_MODE_FAIL	= 0x02U,      /*!< SPI模式失败错误码                          */
 HAL_SPI_ERROR_TX_UNDERFLOW	= 0x40U       /*!< SPI发送下溢错误码                          */
}HAL_SPI_Error_CodeTypeDef;

/**
  * @brief	SPI发送FIFO状态标志枚举
  */
typedef enum
{
 HAL_SPI_TXFIFO_DATA_LEN    = 0xffU,      /*!< SPI发送FIFO剩余长度,当满状态时,该长度为0      */
 HAL_SPI_TXFIFO_TX_FULL     = 0x100U,     /*!< SPI发送FIFO满状态,当满状态时,剩余长度为0      */
 HAL_SPI_TXFIFO_TX_EMPTY    = 0x200U      /*!< SPI发送FIFO空状态                                                                            */
}HAL_SPI_TXFIFO_FlagTypeDef;

/**
  * @brief	SPI接收FIFO状态标志枚举
  */
typedef enum
{
 HAL_SPI_RXFIFO_DATA_LEN    = 0xffU,      /*!< SPI接收FIFO剩余长度,当满状态时,该长度为0     */
 HAL_SPI_RXFIFO_RX_FULL     = 0x100U,     /*!< SPI接收FIFO满状态,当满状态时,剩余长度为0     */
 HAL_SPI_RXFIFO_RX_EMPTY    = 0x200U      /*!< SPI接收FIFO空状态                                                                          */
}HAL_SPI_RXFIFO_FlagTypeDef;

/**
  * @brief	SPI片选类型枚举
  */
typedef enum {
  HAL_SPI_CS_PIN_1 = 0x3800,      /*!< SPI片选引脚为 CS_PIN_1      */
  HAL_SPI_CS_PIN_2 = 0x3400,      /*!< SPI片选引脚为 CS_PIN_2      */
  HAL_SPI_CS_NONE  = 0x3C00       /*!< SPI片选引脚为 空                                            */
} HAL_SPI_ChipSelTypeDef;

/**
  * @brief	SPI中断类型枚举
  */
typedef enum
{
  HAL_SPI_IT_REV_OVERFLOW      = 0x01U,      /*!< SPI接收溢出中断                    */
  HAL_SPI_IT_MODE_FAIL         = 0x02U,      /*!< SPI模式失败中断                    */
  HAL_SPI_IT_TX_NOT_FULL       = 0x04U,      /*!< SPI发送FIFO非满中断      */
  HAL_SPI_IT_TX_FULL           = 0x08U,      /*!< SPI发送FIFO满中断          */
  HAL_SPI_IT_RX_NOT_EMPTY      = 0x10U,      /*!< SPI接收FIFO非空中断      */
  HAL_SPI_IT_RX_FULL           = 0x20U,      /*!< SPI发送FIFO满中断           */
  HAL_SPI_IT_TX_UNDERFLOW      = 0x40U       /*!< SPI发送FIFO下溢中断      */
} HAL_SPI_Interrupt_definitionTypeDef;



/** @brief  失能SPI外设.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_DISABLE(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->ENABLE,1)

/** @brief  使能SPI外设.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_ENABLE(__HANDLE__) SET_BIT((__HANDLE__)->Instance->ENABLE,1)

/** @brief  使能SPI外设中断.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  __INTERRUPT__：SPI中断类型，详见  @ref HAL_SPI_Interrupt_definitionTypeDef。
  * @retval 无
  */
#define HAL_SPI_ENABLE_IT(__HANDLE__,__INTERRUPT__) SET_BIT((__HANDLE__)->Instance->IEN,(__INTERRUPT__))

/** @brief  失能SPI外设中断.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  __INTERRUPT__：SPI中断类型，详见  @ref HAL_SPI_Interrupt_definitionTypeDef。
  * @retval 无
  */
#define HAL_SPI_DISABLE_IT(__HANDLE__,__INTERRUPT__) SET_BIT((__HANDLE__)->Instance->IDIS,(__INTERRUPT__))

/** @brief  SPI主模式配置 .
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_CONFIG_EXP_CLK_MASTER(__HANDLE__) \
	(MODIFY_REG((__HANDLE__)->Instance->CONFIG,(SPI_CONFIG_MODE_Msk | SPI_CONFIG_CPOL_Msk | SPI_CONFIG_CPHA_Msk | SPI_CONFIG_CLK_DIV_Msk | SPI_CONFIG_WORD_SIZE_Msk),\
			(((__HANDLE__)->Init.SPI_CLK_DIV)|((__HANDLE__)->Init.WorkMode)|((__HANDLE__)->Init.Mode)|((__HANDLE__)->Init.DataSize))))

/** @brief  SPI从模式配置 .
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_CONFIG_EXP_CLK_SLAVE(__HANDLE__) \
	(MODIFY_REG((__HANDLE__)->Instance->CONFIG,(SPI_CONFIG_MODE_Msk | SPI_CONFIG_CPOL_Msk | SPI_CONFIG_CPHA_Msk | SPI_CONFIG_CLK_DIV_Msk | SPI_CONFIG_WORD_SIZE_Msk),\
			((HAL_SPI_CLKDIV_2)|((__HANDLE__)->Init.WorkMode)|((__HANDLE__)->Init.Mode)|((__HANDLE__)->Init.DataSize))))

/** @brief  清空SPI发送FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_TXFIFO_FLUSH(__HANDLE__) SET_BIT((__HANDLE__)->Instance->TX_FIFO_OP,SPI_FIFO_OP_RESET_Msk)

/** @brief  初始化SPI发送FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_TXFIFO_NORMAL(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->TX_FIFO_OP,SPI_FIFO_OP_RESET_Msk)

/** @brief  清空SPI接收FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_RXFIFO_FLUSH(__HANDLE__) SET_BIT((__HANDLE__)->Instance->RX_FIFO_OP,SPI_FIFO_OP_RESET_Msk)

/** @brief  初始化SPI接收FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_RXFIFO_NORMAL(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->RX_FIFO_OP,SPI_FIFO_OP_RESET_Msk)

/** @brief  开始SPI发送FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_TXFIFO_START(__HANDLE__) SET_BIT((__HANDLE__)->Instance->TX_FIFO_OP,SPI_FIFO_OP_START_Msk)

/** @brief  停止SPI发送FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_TXFIFO_STOP(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->TX_FIFO_OP,SPI_FIFO_OP_START_Msk)

/** @brief  开始SPI接收FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_RXFIFO_START(__HANDLE__) SET_BIT((__HANDLE__)->Instance->RX_FIFO_OP,SPI_FIFO_OP_START_Msk)

/** @brief  停止SPI接收FIFO.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
#define HAL_SPI_RXFIFO_STOP(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->RX_FIFO_OP,SPI_FIFO_OP_START_Msk)

/** @brief  设置SPI发送FIFO阈值.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef.
  * @param  __VAL__：需要设置的阈值.
  * @retval 无
  */
#define HAL_SPI_SET_TXFIFO_THD(__HANDLE__,__VAL__) MODIFY_REG((__HANDLE__)->Instance->TX_THRESH,0xffffffff,(__VAL__))

/** @brief  设置SPI接收FIFO阈值.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef.
  * @param  __VAL__：需要设置的阈值.
  * @retval 无
  */
#define HAL_SPI_SET_RXFIFO_THD(__HANDLE__,__VAL__) MODIFY_REG((__HANDLE__)->Instance->RX_THRESH,0xffffffff,(__VAL__))

/** @brief  使能SPI软件片选.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef.
  * @retval 无
  */
#define HAL_SPI_MANUAL_CS_ENABLE(__HANDLE__) SET_BIT((__HANDLE__)->Instance->CONFIG,SPI_CONFIG_MANUAL_CS_Msk)

/** @brief  失能SPI软件片选.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef.
  * @retval 无
  */
#define HAL_SPI_MANUAL_CS_DISABLE(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->CONFIG,SPI_CONFIG_MANUAL_CS_Msk)

/** @brief  设置SPI无外设片选.
  * @param  __HANDLE__：SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef.
  * @retval 无
  */
#define HAL_SPI_CS_NOHARDWARE(__HANDLE__) SET_BIT((__HANDLE__)->Instance->CONFIG,0x3C00)

/**
  * @brief  获取SPI工作状态.
  * @param  SPIHandleStruct. 详见  @ref HAL_SPI_HandleTypeDef。
  * @retval SPI工作状态.详见  @ref HAL_SPI_StateTypeDef
  */
HAL_SPI_StateTypeDef HAL_SPI_GetState(HAL_SPI_HandleTypeDef *SPIHandleStruct);

/**
  * @brief  获取SPIHandleStruct结构体中ErrorCode错误码参数.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval SPI结构体中ErrorCode错误码参数.详见  @ref HAL_SPI_Error_CodeTypeDef。
  */
uint32_t HAL_SPI_GetError(HAL_SPI_HandleTypeDef *SPIHandleStruct);

/**
  * @brief  清空SPI发送FIFO.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
void HAL_SPI_Clean_TxFIFO(HAL_SPI_HandleTypeDef *SPIHandleStruct);

/**
  * @brief  清空SPI接收FIFO.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
void HAL_SPI_Clean_RxFIFO(HAL_SPI_HandleTypeDef *SPIHandleStruct);

/** @brief  检查SPI发送FIFO参数状态.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  TxFIFO_Flag：SPI发送FIFO参数状态类型.详见  @ref HAL_SPI_TXFIFO_FlagTypeDef.
  * @retval 当TxFIFO_Flag参数为\n
  *            @arg HAL_SPI_TXFIFO_DATA_LEN 时: 返回值为发送FIFO剩余数据长度。\n
  *            @arg HAL_SPI_TXFIFO_TX_FULL 时: HAL_SET：发送FIFO满状态，HAL_RESET：发送FIFO非满状态。\n
  *            @arg HAL_SPI_TXFIFO_TX_EMPTY 时: HAL_SET：发送FIFO空状态，HAL_RESET：发送FIFO非空状态。
  */
uint8_t HAL_SPI_Get_TxFIFO_Flag(HAL_SPI_HandleTypeDef* SPIHandleStruct,HAL_SPI_TXFIFO_FlagTypeDef TxFIFO_Flag);

/** @brief  检查SPI接收FIFO参数状态.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  RxFIFO_Flag：SPI发送FIFO参数状态类型.详见  @ref HAL_SPI_RXFIFO_FlagTypeDef。
  * @retval 当RxFIFO_Flag参数为：\n
  *            @arg HAL_SPI_RXFIFO_DATA_LEN 时: 返回值为接收FIFO剩余数据长度。\n
  *            @arg HAL_SPI_RXFIFO_RX_FULL 时: HAL_SET：接收FIFO满状态，HAL_RESET：接收FIFO非满状态。\n
  *            @arg HAL_SPI_RXFIFO_RX_EMPTY 时: HAL_SET：接收FIFO空状态，HAL_RESET：接收FIFO非空状态。
  */
uint8_t HAL_SPI_Get_RxFIFO_Flag(HAL_SPI_HandleTypeDef* SPIHandleStruct,HAL_SPI_RXFIFO_FlagTypeDef RxFIFO_Flag);

/**
  * @brief  设置SPI片选引脚.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  CS_Pin：可以选择的SPI片选引脚，详见  @ref HAL_SPI_ChipSelTypeDef.
  * @retval 无
  */
void HAL_SPI_ChipSelect(HAL_SPI_HandleTypeDef *SPIHandleStruct, HAL_SPI_ChipSelTypeDef CS_Pin);

/**
  * @brief  初始化SPI.
  * @param  SPIHandleStruct. 详见结构体定义   @ref HAL_SPI_HandleTypeDef。
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_SPI_Init(HAL_SPI_HandleTypeDef* SPIHandleStruct);

/**
  * @brief  去初始化SPI.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示去初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_SPI_DeInit(HAL_SPI_HandleTypeDef* SPIHandleStruct);

/**
  * @brief  SPI发送阻塞函数,timeout为阻塞时间.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  pData:发送数据的存储地址。
  * @param  Size:发送数据的大小。
  * @param  Timeout:超时时间. 0 是不阻塞，发送FIFO满后立刻退出; HAL_MAX_DELAY 是一直阻塞，直到发送完指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据发送完成的最长时间，超时则退出函数，SPIHandleStruct结构体中的TxXferCount表示实际发送的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示发送数据成功，表示指定时间内成功发送指定数量的数据
  *       @retval  HAL_ERROR    ：表示入参错误
  *       @retval  HAL_BUSY     ：表示外设正在使用中
  *       @retval  HAL_TIMEOUT  ：超时时间不为0时会返回，表示指定时间内未能成功发送指定数量的数据
  *       @retval  HAL_LOST     ：超时时间为0时会返回，表示未能成功发送指定数量的数据
  */
HAL_StatusTypeDef HAL_SPI_Transmit(HAL_SPI_HandleTypeDef* SPIHandleStruct, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  SPI接收阻塞函数,timeout为阻塞时间.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  pData:接收数据的存储地址。
  * @param  Size:接收数据的大小。
  * @param  Timeout:超时时间. 0 是不阻塞，接收FIFO空后立刻退出; HAL_MAX_DELAY 是一直阻塞，直到接收到指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据接收完成的最长时间，超时则退出函数，SPIHandleStruct结构体中的RxXferCount表示实际接收的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：超时时间不为0时，表示指定时间内成功接收指定数量的数据；超时时间为0时，也会返回，但实际接收数据的数量通过hcsp结构体中的RxXferCount来确定
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  *       @retval  HAL_TIMEOUT  ：超时时间不为0时会返回，表示指定时间内未能成功接收指定数量的数据
  */
HAL_StatusTypeDef HAL_SPI_Receive(HAL_SPI_HandleTypeDef *SPIHandleStruct, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  SPI收发函数.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  pTxData:发送数据的存储地址。
  * @param  pRxData:接收数据的存储地址。
  * @param  Size:收发数据的大小。
  * @param  Timeout:超时时间. 0 是不阻塞，发送FIFO满或接收FIFO空后立刻退出; HAL_MAX_DELAY 是一直阻塞，直到发送和接收到指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据发送接收完成的最长时间，超时则退出函数，SPIHandleStruct结构体中的TxXferCount表示实际发送的字节数，RxXferCount表示实际接收的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：发送和接收数据成功，表示指定时间内成功发送和接收指定数量的数据
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  *       @retval  HAL_TIMEOUT  ：超时时间不为0时会返回，表示指定时间内未能成功发送和接收指定数量的数据
  *       @retval  HAL_LOST     ：超时时间为0时会返回，表示未能成功发送和接收指定数量的数据
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(HAL_SPI_HandleTypeDef *SPIHandleStruct, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,uint32_t Timeout);

/**
  * @brief  SPI中断方式接收数据.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @param  pData:接收数据的存储地址。
  * @param  Size:接收数据的大小。
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：中断接收函数执行成功，等待在中断中接收指定数量的的数据
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  */
HAL_StatusTypeDef HAL_SPI_Receive_IT(HAL_SPI_HandleTypeDef *SPIHandleStruct, uint8_t *pData, uint16_t Size);

/** @brief  获取SPI中断使能状态.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval SPI中断使能状态.详见  @ref HAL_SPI_Interrupt_definitionTypeDef。
  */
uint32_t HAL_SPI_GetITEnable(HAL_SPI_HandleTypeDef* SPIHandleStruct);

/** @brief  获取SPI中断状态.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval SPI中断状态.详见  @ref HAL_SPI_Interrupt_definitionTypeDef。
  */
uint32_t HAL_SPI_GetITState(HAL_SPI_HandleTypeDef* SPIHandleStruct);

/**
  * @brief  SPI错误中断回调函数.在对应的CSP中断处理函数中调用.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
void HAL_SPI_ErrorCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct) __RAM_FUNC;

/**
  * @brief  SPI接收完成回调函数.在对应的CSP中断处理函数中调用.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
void HAL_SPI_RxCpltCallback(HAL_SPI_HandleTypeDef *SPIHandleStruct) __RAM_FUNC;

/**
  * @brief  SPI中断处理函数.在对应的中断服务函数中调用.
  * @param  SPIHandleStruct. 详见结构体定义  @ref HAL_SPI_HandleTypeDef。
  * @retval 无
  */
void SPI_IRQHandler(HAL_SPI_HandleTypeDef *SPIHandleStruct) __RAM_FUNC;

/**
  * @brief  SPI中断服务函数.SPI中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_SPI_IRQHandler(void) __RAM_FUNC;


/**
* @brief  SPI 外设.
*/
#define HAL_SPI               ((HAL_SPI_TypeDef *)SPI_BASE)   /*!< SPI 外设基地址   */





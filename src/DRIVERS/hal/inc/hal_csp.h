/**
  ******************************************************************************
  * @file	hal_csp.h
  * @brief	此文件包含CSP外设的变量，枚举，结构体定义，函数声明等.
  * @note	芯片共有4个CSP外设。CSP1默认用于SIMCARD，CSP2默认用于AT，CSP3默认用于LOG，CSP4可以自由使用。\n
  * 		CSP配置为SPI时不支持从机模式；CSP配置为UART时仅支持无校验
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "hal_def.h"
#include "hw_memmap.h"

/**
* @brief CSP 工作状态枚举
* @note  CSP状态由2个不同的子状态 gState 和 RxState 组成. \n
*        - gState 包含控制结构体管理和Tx操作的相关信息. \n
*          gState 的bitmap如下所示 : \n
*          b7-b6  错误信息 \n
*             00 : 无错误 \n
*             01 : (未使用) \n
*             10 : 超时 \n
*             11 : 错误 \n
*          b5     外设初始化状态 \n
*             0  : 未初始化		(外设未初始化) \n
*             1  : 初始化完成	(外设未初始化，但初始化函数已经调用) \n
*          b4-b3  (未使用) \n
*             xx : 应该设置为 00 \n
*          b2     内在进度状态 \n
*             0  : 准备好 \n
*             1  : 忙		(外设正在进行配置或者内部操作) \n
*          b1     (未使用) \n
*             x  : 应该设置为 0 \n
*          b0     发送状态 \n
*             0  : 准备好	(没有发送操作在进行中) \n
*             1  : 忙		(发送操作正在进行中) \n
*        - RxState 包含Rx操作的相关信息 \n
*          RxState 的bitmap如下所示 : \n
*          b7-b6  (未使用) \n
*             xx : 应该设置为 00 \n
*          b5     外设初始化状态 \n
*             0  : 未初始化		(外设未初始化) \n
*             1  : 初始化完成	(外设未初始化，但初始化函数已经调用) \n
*          b4-b2  (未使用) \n
*            xxx : 应该设置为 000 \n
*          b1     接收状态 \n
*             0  : 准备好	(没有Rx操作在进行中) \n
*             1  : 忙		(Rx接收操作正在进行中) \n
*          b0     (未使用) \n
*             x  : 应该设置为 0.
*/
typedef enum
{
	HAL_CSP_STATE_RESET			= 0x00U,	/*!< 外设未初始化。此值可用于 gState 和 RxState                  */
	HAL_CSP_STATE_READY			= 0x20U,	/*!< 外设已初始化，准备好正常使用。此值可用于 gState 和 RxState   */
	HAL_CSP_STATE_BUSY			= 0x24U,	/*!< 一个线程正在运行中。				                         */
	HAL_CSP_STATE_BUSY_TX		= 0x21U,    /*!< 数据发送线程正在进行中。此值仅用于gState                     */
	HAL_CSP_STATE_BUSY_RX		= 0x22U,    /*!< 数据接收线程正在进行中。此值仅用于RxState                    */
	HAL_CSP_STATE_BUSY_TX_RX	= 0x23U,	/*!< 数据发送和接收都在进行中。                                   */
	HAL_CSP_STATE_TIMEOUT		= 0xA0U,    /*!< 超时状态。                                 				 */
	HAL_CSP_STATE_ERROR			= 0xE0U		/*!< 错误状态。                                  				 */
}HAL_CSP_StateTypeDef;

/**
  * @brief	CSP错误类型枚举.
  */
typedef enum
{
	HAL_CSP_ERROR_NONE			= 0x00000000U,  /*!< 无错误			*/
	HAL_CSP_ERROR_RX_OFLOW		= 0x00000004U,  /*!< 接收溢出错误	*/
	HAL_CSP_ERROR_TX_UFLOW		= 0x00000008U,  /*!< 发送溢出错误	*/
	HAL_CSP_ERROR_UART_FRM_ERR	= 0x00000400U   /*!< 帧错误			*/
}HAL_CSP_Error_CodeTypeDef;

/**
* @brief CSP初始化结构体定义.
*/
typedef struct
{
	uint32_t CSP_WorkMode;			/*!< CSP工作模式，可配置为UART模式或者SPI模式	*/

	uint32_t SPI_Mode;				/*!< CSP配置为SPI时的主从模式，可配置为Master模式或者Slave模式	*/

	uint32_t SPI_WorkMode;			/*!< CSP配置为SPI时的工作模式，可配置为0，1，2，3	*/

	uint32_t SPI_Clock;				/*!< CSP配置为SPI时的时钟频率	*/

	uint32_t UART_BaudRate;			/*!< CSP配置为UART时的波特率	*/

	uint32_t UART_DataBits;			/*!< CSP配置为UART时的数据位	*/
							 
	uint32_t UART_StopBits;			/*!< CSP配置为UART时的停止位	*/
}HAL_CSP_InitTypeDef;

/**
* @brief  CSP 控制结构体定义.
*/
typedef struct __CSP_HandleTypeDef
{
	uint32_t Instance;				                /*!< CSP 寄存器基地址		*/

	HAL_CSP_InitTypeDef Init;					    /*!< CSP 初始化结构体		*/

	uint8_t *pTxBuffPtr;					        /*!< CSP 发送缓冲区指针		*/

	volatile uint16_t TxXferSize;					/*!< CSP 发送数据长度		*/

	volatile uint16_t TxXferCount;			        /*!< CSP 实际发送数据长度	*/

	uint8_t *pRxBuffPtr;					        /*!< CSP 接收缓冲区指针		*/

	volatile uint16_t RxXferSize;					/*!< CSP 接收数据长度		*/

	volatile uint16_t RxXferCount;			      	/*!< CSP 实际接收数据长度	*/

	HAL_LockTypeDef Lock;					        /*!< CSP 设备锁			*/

	volatile HAL_CSP_StateTypeDef gState;			/*!< CSP 控制结构体管理和Tx相关的状态信息. 详情参考 @ref HAL_CSP_StateTypeDef	*/

	volatile HAL_CSP_StateTypeDef RxState;			/*!< CSP Rx相关的状态信息. 详情参考 @ref HAL_CSP_StateTypeDef	*/

	volatile uint32_t ErrorCode;			        /*!< CSP 错误码				*/

}HAL_CSP_HandleTypeDef;

/**
* @brief  CSP 外设枚举.
*/
typedef enum {
	HAL_CSP1 = (uint32_t)CSP1_BASE,	/*!< CSP1 基地址 */
	HAL_CSP2 = (uint32_t)CSP2_BASE, /*!< CSP2 基地址 */
	HAL_CSP3 = (uint32_t)CSP3_BASE, /*!< CSP3 基地址 */
	HAL_CSP4 = (uint32_t)CSP4_BASE  /*!< CSP4 基地址 */
}HAL_CSP_TypeDef;

/**
* @brief  CSP UART 数据位枚举.详情参考标准UART数据位.
*/
typedef enum {
	HAL_CSP_DATA_BITS_6 = 6, /*!< 6个数据位 */
	HAL_CSP_DATA_BITS_7 = 7, /*!< 7个数据位 */
	HAL_CSP_DATA_BITS_8 = 8  /*!< 8个数据位 */
} HAL_CSPDataBits_TypeDef;

/**
* @brief  CSP UART 停止位枚举.详情参考标准UART停止位.
*/
typedef enum {
	HAL_CSP_STOP_BITS_1 = 1, /*!< 1个停止位 */
	HAL_CSP_STOP_BITS_2 = 2  /*!< 2个停止位 */
}HAL_CSPStopBits_TypeDef;

/**
* @brief  CSP 发送接收使能枚举.
*/
typedef enum {
	HAL_CSP_DIS_RX_TX  = 0x0, /*!< 失能 Rx 和 Tx */
	HAL_CSP_EN_TX_ONLY = 0x1, /*!< 仅使能 Tx */
	HAL_CSP_EN_RX_ONLY = 0x2, /*!< 仅使能 Rx */
	HAL_CSP_EN_RX_TX   = 0x3  /*!< 使能 Rx 和 Tx */
}HAL_CSPRxTx_TypeDef;

/**
* @brief  CSP FIFO状态枚举.
*/
typedef enum {
  	HAL_CSP_TX_FIFO_LEVEL  = 0x0000, /*!< Tx FIFO 剩余长度*/
  	HAL_CSP_TX_FIFO_FULL   = 0x0080, /*!< Tx FIFO 满*/
  	HAL_CSP_TX_FIFO_EMEPTY = 0x0100, /*!< Tx FIFO 空*/
  	HAL_CSP_RX_FIFO_LEVEL  = 0x1000, /*!< Rx FIFO 剩余长度*/
  	HAL_CSP_RX_FIFO_FULL   = 0x1080, /*!< Rx FIFO 满*/
  	HAL_CSP_RX_FIFO_EMEPTY = 0x1100  /*!< Rx FIFO 空*/
}HAL_CSPFIFO_TypeDef;

/**
* @brief  CSP 中断类型枚举.
*/
typedef enum {
 	HAL_CSP_INTTYPE_RX_DONE          = 0x00000001, /*!< CSP 接收完成中断 */
 	HAL_CSP_INTTYPE_TX_DONE          = 0x00000002, /*!< CSP 发送完成中断 */
 	HAL_CSP_INTTYPE_RX_OFLOW         = 0x00000004, /*!< CSP 接收溢出中断 */
 	HAL_CSP_INTTYPE_TX_UFLOW         = 0x00000008, /*!< CSP 发送溢出中断 */
 	HAL_CSP_INTTYPE_DMA_IO_RX_DONE   = 0x00000010, /*!< CSP DMA 接收完成中断 */
 	HAL_CSP_INTTYPE_DMA_IO_TX_DONE   = 0x00000020, /*!< CSP DMA 发送完成中断 */
 	HAL_CSP_INTTYPE_RXFIFO_FULL      = 0x00000040, /*!< CSP Rx FIFO满中断 */
 	HAL_CSP_INTTYPE_TXFIFO_EMPTY     = 0x00000080, /*!< CSP Tx FIFO空中断 */
 	HAL_CSP_INTTYPE_RXFIFO_THD_REACH = 0x00000100, /*!< CSP Rx FIFO阈值中断 */
 	HAL_CSP_INTTYPE_TXFIFO_THD_REACH = 0x00000200, /*!< CSP Tx FIFO阈值中断 */
 	HAL_CSP_INTTYPE_UART_FRM_ERR     = 0x00000400, /*!< CSP UART 帧错误中断 */
 	HAL_CSP_INTTYPE_RX_TIMEOUT       = 0x00000800, /*!< CSP 接收超时中断 */
 	HAL_CSP_INTTYPE_TX_ALLOUT        = 0x00001000, /*!< CSP 发送FIFO和移位寄存器中的数据都发送完成中断 */
 	HAL_CSP_INTTYPE_INT_ALL          = 0x00001FFF  /*!< CSP 所有中断 */
}HAL_CSPInt_TypeDef;

/**
* @brief  CSP SPI 时钟分频枚举.CSP在PCLK的基础上进行分频.
*/
typedef enum{
  	HAL_CLK_DIV_2,		/*!< 2分频 */
  	HAL_CLK_DIV_4,		/*!< 4分频 */
  	HAL_CLK_DIV_8,		/*!< 8分频 */
  	HAL_CLK_DIV_16,		/*!< 16分频 */
  	HAL_CLK_DIV_32,		/*!< 32分频 */
  	HAL_CLK_DIV_64,		/*!< 64分频 */
  	HAL_CLK_DIV_128,	/*!< 128分频 */
  	HAL_CLK_DIV_256		/*!< 256分频 */
}HAL_CSPClkDiv_TypeDef;

/**
* @brief  CSP SPI 通信模式枚举.详情参考标准SPI通信模式.
*/
typedef enum{
  	HAL_MODE_0,	/*!< CSP SPI 模式0 */
  	HAL_MODE_1,	/*!< CSP SPI 模式1 */
  	HAL_MODE_2,	/*!< CSP SPI 模式2 */
  	HAL_MODE_3	/*!< CSP SPI 模式3 */
}HAL_CSPCommunicationMode_TypeDef;

/**
* @brief  CSP SPI 数据宽度枚举.详情参考标准SPI数据宽度.
*/
typedef enum{
  	HAL_DATA_WIDTH_8,	/*!< CSP SPI 8位数据宽度 */
  	HAL_DATA_WIDTH_16,	/*!< CSP SPI 16位数据宽度 */
  	HAL_DATA_WIDTH_32	/*!< CSP SPI 32位数据宽度 */
}HAL_CSPDataWidth_TypeDef;

/**
* @brief  CSP SPI 工作模式枚举.
*/
typedef enum{
  	HAL_MASTER_MODE,	/*!< CSP SPI 主机模式 */
}HAL_CSPWorkMode_TypeDef;


/**
  * @brief  获取 CSP 工作状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval HAL state.详情参考 @ref HAL_CSP_StateTypeDef
  */
HAL_CSP_StateTypeDef HAL_CSP_GetState(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  获取 CSP 错误状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval CSP 错误码
  */
uint32_t HAL_CSP_GetError(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  CSP的SPI模式下，使能NSS的手动控制模式.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  NewState
  * 	@arg	HAL_ENABLE	使能NSS为手动控制模式
  * 	@arg	HAL_DISABLE	失能NSS为手动控制模式，即NSS由硬件自动控制，无需用户通过API拉高拉低NSS
  * @retval 无
  */
void HAL_CSP_SPI_SetNSSManual(HAL_CSP_HandleTypeDef *hcsp, HAL_FunctionalState NewState);

/**
  * @brief  设置CSP的SPI模式下，发送帧的配置参数.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  tx_data_length		一帧的数据长度.一般默认为 8 即可.
  * @param  tx_delay	接收延时.一般默认为 1 即可.
  * @param  idle		空闲延时.一般默认为 1 即可.
  * @retval 无
  */
void HAL_CSP_SPI_SetTxFrame(HAL_CSP_HandleTypeDef *hcsp, uint8_t tx_data_length, uint8_t tx_delay, uint8_t idle);

/**
  * @brief  设置CSP的SPI模式下，接收帧的配置参数.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  rx_data_length	一帧的数据长度.一般默认为 8 即可.
  * @param  rx_delay	接收延时.一般默认为 1 即可.
  * @param  idle		空闲延时.一般默认为 1 即可.
  * @retval 无
  */
void HAL_CSP_SPI_SetRxFrame(HAL_CSP_HandleTypeDef *hcsp, uint8_t rx_data_length, uint8_t rx_delay, uint8_t idle);

/**
  * @brief  设置CSP的UART模式下，Timeout中断的超时时间.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  timeout	超时时间
  * @retval 无
  */
void HAL_CSP_UART_SetTimeout(HAL_CSP_HandleTypeDef *hcsp, uint16_t timeout);

/**
  * @brief  设置CSP发送和接收的FIFO深度.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  TxLevel	发送深度
  * @param  RxLevel	接收深度
  * @retval 无
  */
void HAL_CSP_SetFIFOLevel(HAL_CSP_HandleTypeDef *hcsp, uint32_t TxLevel, uint32_t RxLevel);

/**
  * @brief  获取CSP FIFO状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param	FIFO_State	FIFO状态类型.详情参考 @ref CSPFIFO_TypeDef
  * @retval FIFO_State 为CSP_TX_FIFO_LEVEL和CSP_RX_FIFO_LEVEL时返回FIFO内数据长度，其余则返回FIFO_State真假状态
  */
uint8_t HAL_CSP_GetFIFOState(HAL_CSP_HandleTypeDef *hcsp, HAL_CSPFIFO_TypeDef FIFO_State);

/**
  * @brief  CSP发送和接收使能控制.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  TxRxEn	TX和RX使能控制
  * 	@arg	CSP_DIS_RX_TX，失能TX和RX
  *		@arg	CSP_EN_TX_ONLY，只使能TX
  *		@arg	CSP_EN_RX_ONLY，只使能RX
  *		@arg	CSP_EN_RX_TX，使能TX和RX
  * @retval 无
  */
void HAL_CSP_CtrlTxRx(HAL_CSP_HandleTypeDef *hcsp, HAL_CSPRxTx_TypeDef TxRxEn);

/**
  * @brief  CSP使能控制.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  NewState
  * 	@arg	HAL_ENABLE	使能CSP
  * 	@arg	HAL_DISABLE	失能CSP
  * @retval 无
  */
void HAL_CSP_Ctrl(HAL_CSP_HandleTypeDef *hcsp, HAL_FunctionalState NewState);

/**
  * @brief  CSP中断使能控制.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param	IntType		中断类型.详情参考 @ref HAL_CSPInt_TypeDef
  * @param  NewState	使能状态
  * 	@arg	HAL_ENABLE	使能中断
  * 	@arg	HAL_DISABLE	失能中断
  * @retval 无
  */
void HAL_CSP_CtrlInt(HAL_CSP_HandleTypeDef *hcsp, HAL_CSPInt_TypeDef IntType, HAL_FunctionalState NewState);

/**
  * @brief  清除CSP中断状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param	IntType		中断类型.详情参考 @ref HAL_CSPInt_TypeDef
  * @retval 无
  */
void HAL_CSP_ClearIntState(HAL_CSP_HandleTypeDef *hcsp, HAL_CSPInt_TypeDef IntType);

/**
  * @brief  初始化CSP为UART.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_CSP_UART_Init(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  初始化CSP为SPI.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_CSP_SPI_Init(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  去初始化CSP.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示去初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_CSP_DeInit(HAL_CSP_HandleTypeDef* hcsp);

/**
  * @brief  发送数据.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  pData 发送数据缓冲区指针.
  * @param  Size  发送数据字节长度.
  * @param  Timeout 超时时间（单位ms）; 如果Time_out取值为HAL_MAX_DELAY，则是一直阻塞，等待发送完指定数量的字节才退出；.\n
  * 		如果Timeout设置为非0非HAL_MAX_DELAY时，表示函数等待数据发送完成的最长时间，超时则退出函数，hcsp结构体中的TxXferCount表示实际发送的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示发送数据成功，表示指定时间内成功发送指定数量的数据
  *       @retval  HAL_ERROR    ：表示入参错误
  *       @retval  HAL_BUSY     ：表示外设正在使用中
  *       @retval  HAL_TIMEOUT  ：表示指定时间内未能成功发送指定数量的数据
  */

HAL_StatusTypeDef HAL_CSP_Transmit(HAL_CSP_HandleTypeDef *hcsp, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  接收数据.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  pData 接收数据缓冲区指针.
  * @param  Size  接收数据字节长度.
  * @param  Timeout 超时时间（单位ms）. 0 是不阻塞，接收FIFO空后立刻退出; HAL_MAX_DELAY 是一直阻塞，直到接收到指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据接收完成的最长时间，超时则退出函数，hcsp结构体中的RxXferCount表示实际接收的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：超时时间不为0时，表示指定时间内成功接收指定数量的数据；超时时间为0时，也会返回，但实际接收数据的数量通过hcsp结构体中的RxXferCount来确定
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  *       @retval  HAL_TIMEOUT  ：超时时间不为0时会返回，表示指定时间内未能成功接收指定数量的数据
  */
HAL_StatusTypeDef HAL_CSP_Receive(HAL_CSP_HandleTypeDef *hcsp, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  发送和接收数据.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  pTxData 发送缓冲区指针
  * @param  pRxData 接收缓冲区指针
  * @param  Size 发送和接收数据字节长度
  * @param  Timeout 超时时间.  HAL_MAX_DELAY 是一直阻塞，直到发送和接收到指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据发送接收完成的最长时间，超时则退出函数，hcsp结构体中的TxXferCount表示实际发送的字节数，RxXferCount表示实际接收的字节数.
  *
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：发送和接收数据成功，表示指定时间内成功发送和接收指定数量的数据
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  *       @retval  HAL_TIMEOUT  ：表示指定时间内未能成功发送和接收指定数量的数据
  */
HAL_StatusTypeDef HAL_CSP_SPI_TransmitReceive(HAL_CSP_HandleTypeDef *hcsp, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  中断接收数据.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @param  pData 接收数据缓冲区指针.
  * @param  Size  接收数据字节长度.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：中断接收函数执行成功，等待在中断中接收指定数量的的数据
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  */
HAL_StatusTypeDef HAL_CSP_Receive_IT(HAL_CSP_HandleTypeDef *hcsp, uint8_t *pData, uint16_t Size);

/**
  * @brief  获取CSP中断使能状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval CSP中断使能状态
  */
uint32_t HAL_CSP_GetITEnable(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  获取CSP中断状态.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval CSP中断状态
  */
uint32_t HAL_CSP_GetITState(HAL_CSP_HandleTypeDef *hcsp);

/**
  * @brief  CSP错误中断回调函数.在对应的CSP中断处理函数中调用
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval 无
  */
void HAL_CSP_ErrorCallback(HAL_CSP_HandleTypeDef *hcsp) __RAM_FUNC;

/**
  * @brief  CSP接收完成回调函数.在对应的CSP中断处理函数中调用.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval 无
  */
void HAL_CSP_RxCpltCallback(HAL_CSP_HandleTypeDef *hcsp) __RAM_FUNC;

/**
  * @brief  CSP中断处理函数.在对应的中断服务函数中调用.
  * @param  hcsp 指向一个包含CSP具体配置信息的HAL_CSP_HandleTypeDef 结构体的指针.详情参考 @ref HAL_CSP_HandleTypeDef.
  * @retval 无
  */
void CSP_IRQHandler(HAL_CSP_HandleTypeDef *hcsp) __RAM_FUNC;

/**
  * @brief  CSP1 中断服务函数.CSP1 中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_CSP1_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  CSP2 中断服务函数.CSP2 中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_CSP2_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  CSP3 中断服务函数.CSP3 中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_CSP3_IRQHandler(void) __RAM_FUNC;

/**
  * @brief  CSP4 中断服务函数.CSP4 中断触发时调用此函数.
  * @param  无
  * @retval 无
  */
void HAL_CSP4_IRQHandler(void) __RAM_FUNC;





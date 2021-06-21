/**
  ******************************************************************************
  * @file    hal_uart.h
  * @brief   此文件包含UART外设的变量，枚举，结构体定义，函数声明等.\n
  *   	 本产品拥有一个UART（Universal Asynchronous Reciver and Transmitter）通用异步收发器模
  *   	 块.UART 作为异步串口通信协议的一种，是在应用程序开发过程中使用频率最高的数据总线。UART串口的特点是将数据一位
  *   	 一位地顺序传送，只要 2 根传输线就可以实现双向通信，一根线发送数据的同时用另一根线接收数据（另需要一根地线）。
  *   	UART通信以帧格式传输数据，每帧包含起始信号、数据信息、停止信息，可能还有校验信息，因此UART 串口通信有几个重
  *   	要的参数，分别是波特率、起始位、数据位、停止位和奇偶检验位。对于两个使用 UART 串口通信的端口，这些参数必须匹配，
  *   	否则通信将无法正常完成。UART 对这些传输参数有具体规定，当然也不是只有唯一的取值，很多参数值都可以自定义设置，
  *   	只是增强它的兼容性。\n
  *
  *   	XY1100的UART模块支持传输的数据帧格式如下所示：\n
		起始位			数据位			校验位			停止位\n
		1 bit		6~8bit		0/1 bit		1/1.5/2 bit\n

  *   	 起始位：表示数据传输的开始，电平逻辑为 “0” ;\n
  *   	 数据位：支持6、7、8，表示传输这几个 bit 位数据。一般取值为 8，因为一个 ASCII 字符值为 8 位国；\n
  *   	 奇偶校验位：用于接收方对接收到的数据进行校验，校验 “1” 的位数为偶数(偶校验)或奇数(奇校验)，以此来校验数据传送的正确性，\n
  *   	 		使用时不需要此位也可以；\n
  *   	 停止位： 表示一帧数据的结束。电平逻辑为 “1”；\n
  *   	 波特率：UART通信时的速率，它用单位时间内传输的二进制代码的有效位(bit)数来表示，其单位为每秒比特数 bit/s(bps)。\n
  *   	 	      常见的波特率值有 4800、9600、14400、38400、115200等，数值越大数据传输的越快，波特率为 115200\n
  *   	 	       表示每秒钟传输 115200 位数据。\n
  *
  *
  *   	 UART的主要特性\n
  *   	- 1个全双工异步通信接口\n
  *   	 -   可编程波特率，支持多种波特率，最高达921600 bits/s\n
  *   	 -   可编程数据字长度(6位、7位或8位)\n
  *   	 -   可配置的停止位，支持1、1.5或者2个停止位\n
  *   	 -   单独的发送器和接收器使能位\n
  *   	 -  校验控制\n
  *   	 	。发送校验位\n
  *   	  	。对接收数据进行校验\n
  *   	- 4个错误检测标志\n
  *   	             。 接收溢出错误\n
  *   	             。 发送溢出错误\n
  *   	             。帧错误\n
  *   	             。校验错误\n
  *   	 - 13个带标志的中断源\n
  *   	 	。 接收FIFO阈值中断，接收完成\n
  *   	              。 接收FIFO空中断\n
  *   	 	。 接收FIFO满中断\n
  *   	              。 发送FIFO空中断\n
  *   	              。 发送FIFO满中断\n
  *   	             。 接收FIFO溢出中断\n
  *   	             。接收数据帧错误中断\n
  *   	             。 接收数据校验错误中断\n
  *   	             。 接收数据超时中断\n
  *   	             。调制解调器指示器屏蔽中断\n
  *   	             。发送FIFO阈值中断，发送完成\n
  *   	             。发送FIFO几乎满状态\n
  *   	              。发送FIFO溢出中断\n
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "hal_cortex.h"
#include "hw_memmap.h"
#include "hw_uart.h"


/**
* @brief UART 工作状态枚举
* @note  UART状态由2个不同的子状态 gState 和 RxState 组成. \n
*        - gState 包含控制结构体和Tx操作的相关信息. \n
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
*             xx : 默认置为 00 \n
*          b2     内在进度状态 \n
*             0  : 准备好 \n
*             1  : 忙		(外设正在进行配置或者内部操作) \n
*          b1     (未使用) \n
*             x  : 默认置为 0 \n
*          b0     发送状态 \n
*             0  : 准备好	(没有发送操作在进行中) \n
*             1  : 忙		(发送操作正在进行中) \n
*        - RxState 包含Rx操作的相关信息 \n
*          RxState 的bitmap如下所示 : \n
*          b7-b6  (未使用) \n
*             xx : 默认置为 00 \n
*          b5     外设初始化状态 \n
*             0  : 未初始化		(外设未初始化) \n
*             1  : 初始化完成	(外设未初始化，但初始化函数已经调用) \n
*          b4-b2  (未使用) \n
*            xxx : 默认置为 000 \n
*          b1     接收状态 \n
*             0  : 准备好	(没有Rx操作在进行中) \n
*             1  : 忙		(Rx接收操作正在进行中) \n
*          b0     (未使用) \n
*             x  : 默认置为 0.
*/
  typedef enum
  {
	HAL_UART_STATE_RESET = 0x00U,      /*!< 外设未初始化。此值可用于 gState 和 RxState                  */
	HAL_UART_STATE_READY = 0x20U,      /*!< 外设已初始化，准备好正常使用。此值可用于 gState 和 RxState   */
	HAL_UART_STATE_BUSY = 0x24U,       /*!< 一个线程正在运行中。				                        */
	HAL_UART_STATE_BUSY_TX = 0x21U,    /*!< 数据发送线程正在进行中。此值仅用于gState                     */
	HAL_UART_STATE_BUSY_RX = 0x22U,    /*!< 数据接收线程正在进行中。此值仅用于RxState                    */
	HAL_UART_STATE_BUSY_TX_RX = 0x23U, /*!< 数据发送和接收都在进行中。                                   */
	HAL_UART_STATE_TIMEOUT = 0xA0U,    /*!< 超时状态。                                 				 	*/
	HAL_UART_STATE_ERROR = 0xE0U       /*!< 错误状态。                                  				*/
  } HAL_UART_StateTypeDef;

/**
 * @brief UART寄存器结构体
 */
  typedef struct
  {
	volatile uint32_t HAL_UART_CONTROL;     /*!< UART 控制寄存器        */
	volatile uint32_t HAL_UART_MODE;        /*!< UART 模式寄存器        */
	volatile uint32_t HAL_UART_INT_ENA;     /*!< UART 中断使能控制寄存器 */
	volatile uint32_t HAL_UART_INT_DIS;     /*!< UART 中断失能控制寄存器 */
	volatile uint32_t HAL_UART_INT_MASK;    /*!< UART 中断使能状态寄存器 */
	volatile uint32_t HAL_UART_INT_STAT;    /*!< UART 中断状态寄存器     */
	volatile uint32_t HAL_UART_BAUD_RATE;   /*!< UART 波特率控制寄存器   */
	volatile uint32_t HAL_UART_RX_TIMEOUT;  /*!< UART 接收超时控制寄存器 */
	volatile uint32_t HAL_UART_RX_TRIGGER;  /*!< UART 接收触发控制寄存器 */
	volatile uint32_t HAL_UART_MODEM_CTRL;  /*!< UART 模式控制寄存器     */
	volatile uint32_t HAL_UART_MODEM_STAT;  /*!< UART 模式状态寄存器     */
	volatile uint32_t HAL_UART_CHAN_STAT;   /*!< UART 通道状态寄存器     */
	volatile uint32_t HAL_UART_FIFO_DATA;   /*!< UART FIFO数据寄存器     */
	volatile uint32_t HAL_UART_BAUD_DIV;    /*!< UART 波特率分频寄存器    */
	volatile uint32_t HAL_UART_FLOW_DELAY;  /*!< UART 寄存器              */
	volatile uint32_t HAL_UART_IRX_PW;      /*!< UART IR接收寄存器        */
	volatile uint32_t HAL_UART_ITX_PW;      /*!< UART IR发送寄存器        */
	volatile uint32_t HAL_UART_TX_TRIGGER;  /*!< UART 发送FIFO触发寄存器   */
  } HAL_UART_TypeDef;

/**
* @brief UART 初始化结构体
*/
  typedef struct
  {
	uint32_t BaudRate;		/*!< 波特率		  */
	uint32_t WordLength;	/*!< 数据位长度 */
	uint32_t StopBits;		/*!< 停止位长度 */
	uint32_t Parity;      /*!< 校验模式   */
  } HAL_UART_InitTypeDef;

/**
* @brief  UART 控制结构体
*/
  typedef struct __UART_HandleTypeDef
  {
	HAL_UART_TypeDef *Instance;				/*!< UART 寄存器基地址		*/

	HAL_UART_InitTypeDef Init;				/*!< UART 初始化结构体		*/

	uint8_t *pTxBuffPtr;					/*!< UART 发送缓冲区指针	*/

	volatile uint16_t TxXferSize;			/*!< UART 发送数据长度		*/

	volatile uint16_t TxXferCount;			/*!< UART 实际发送数据长度	*/

	uint8_t *pRxBuffPtr;					/*!< UART 接收缓冲区指针	*/

	volatile uint16_t RxXferSize;			/*!< UART 接收数据长度		*/

	volatile uint16_t RxXferCount;			/*!< UART 实际接收数据长度	*/

	HAL_LockTypeDef Lock;					/*!< UART 设备锁			*/

	volatile HAL_UART_StateTypeDef gState;	/*!< UART 控制结构体管理和Tx相关的状态信息. 详情参考 @ref HAL_UART_StateTypeDef	*/

	volatile HAL_UART_StateTypeDef RxState;	/*!< UART Rx相关的状态信息. 详情参考 @ref HAL_UART_StateTypeDef	*/

	volatile uint32_t ErrorCode;			/*!< UART 错误码			*/

  } HAL_UART_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/
#define UART_IT_MASK 			0x00001FFFU

#define HAL_UART_ERROR_NONE		0x00000000U		/*!< UART 无错误 */
#define HAL_UART_ERROR_ROVR 	0x00000020U		/*!< UART 接收溢出错误 */
#define HAL_UART_ERROR_FRAME 	0x00000040U		/*!< UART 帧错误 */
#define HAL_UART_ERROR_PARE 	0x00000080U		/*!< UART 校验错误 */
#define HAL_UART_ERROR_TOVR 	0x00001000U		/*!< UART 发送溢出错误 */

#define UART_WORDLENGTH_6B 		0x00000006U		/*!< UART 6个数据位 */
#define UART_WORDLENGTH_7B 		0x00000004U		/*!< UART 7个数据位 */
#define UART_WORDLENGTH_8B 		0x00000000U		/*!< UART 8个数据位 */

#define UART_STOPBITS_1 		0x00000000U		/*!< UART 1个停止位 */
#define UART_STOPBITS_1_5 		0x00000040U		/*!< UART 1.5个停止位 */
#define UART_STOPBITS_2 		0x00000080U		/*!< UART 2个停止位 */

#define UART_PARITY_EVEN		0x00000000U		/*!< UART 偶校验 */
#define UART_PARITY_ODD 		0x00000008U		/*!< UART 奇校验 */
#define UART_PARITY_ZERO 		0x00000010U		/*!< UART 零校验 */
#define UART_PARITY_ONE 		0x00000018U		/*!< UART 一校验 */
#define UART_PARITY_NONE 		0x00000020U		/*!< UART 无校验 */

#define UART_CHNSTA_RTRIG 		0x00000001U		/*!< UART 接收FIFO阈值触发状态 */
#define UART_CHNSTA_REMPTY 		0x00000002U		/*!< UART 接收FIFO空状态 */
#define UART_CHNSTA_RFUL 		0x00000004U		/*!< UART 接收FIFO满状态 */
#define UART_CHNSTA_TEMPTY 		0x00000008U		/*!< UART 发送FIFO空状态 */
#define UART_CHNSTA_TFUL 		0x00000010U		/*!< UART 发送FIFO满状态 */
#define UART_CHNSTA_ROVR 		0x00000020U		/*!< UART 接收溢出状态 */
#define UART_CHNSTA_FRAME		0x00000040U		/*!< UART 接收数据帧错误状态 */
#define UART_CHNSTA_PARE 		0x00000080U		/*!< UART 接收数据校验错误状态 */
#define UART_CHNSTA_TIMEOUT 	0x00000100U		/*!< UART 接收数据超时状态 */
#define UART_CHNSTA_DMSI 		0x00000200U		/*!< UART Δ调制解调器指示器中断屏蔽状态 */
#define UART_CHNSTA_RACTIVE 	0x00000400U		/*!< UART 接收状态机激活状态 */
#define UART_CHNSTA_TACTIVE 	0x00000800U		/*!< UART 发送状态机激活状态 */
#define UART_CHNSTA_FDELT 		0x00001000U		/*!< UART 接收流延时状态 */
#define UART_CHNSTA_TTRIG 		0x00002000U		/*!< UART 发送FIFO 触发状态 */
#define UART_CHNSTA_TNFUL 		0x00004000U		/*!< UART 发送FIFOF几乎满状态 */

#define UART_INTTYPE_RTRIG 		0x00000001U		/*!< UART 接收FIFO阈值中断 */
#define UART_INTTYPE_REMPTY 	0x00000002U		/*!< UART 接收FIFO空中断 */
#define UART_INTTYPE_RFUL 		0x00000004U		/*!< UART 接收FIFO满中断 */
#define UART_INTTYPE_TEMPTY 	0x00000008U		/*!< UART 发送FIFO空中断 */
#define UART_INTTYPE_TFUL 		0x00000010U		/*!< UART 发送FIFO满中断 */
#define UART_INTTYPE_ROVR 		0x00000020U		/*!< UART 接收FIFO溢出中断 */
#define UART_INTTYPE_FRAME 		0x00000040U		/*!< UART 接收数据帧错误中断 */
#define UART_INTTYPE_PARE 		0x00000080U		/*!< UART 接收数据校验错误中断 */
#define UART_INTTYPE_TIMEOUT 	0x00000100U		/*!< UART 接收数据超时中断 */
#define UART_INTTYPE_DMSI 		0x00000200U		/*!< UART Δ调制解调器指示器屏蔽中断 */
#define UART_INTTYPE_TTRIG 		0x00000400U		/*!< UART 发送FIFO阈值中断 */
#define UART_INTTYPE_TNFUL 		0x00000800U		/*!< UART 发送FIFO几乎满状态 */
#define UART_INTTYPE_TOVR 		0x00001000U		/*!< UART 发送FIFO溢出中断 */
#define UART_INTTYPE_ALL		0x00001FFFU		/*!< UART 所有中标志位 */


/** @brief  检测 UART 的状态.
  * @param  __HANDLE__ 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  __FLAG__ 需要检测的状态.
  * @return UART状态.
  * 	 返回可能是以下值:
  *				@retval 0:	UART不处于此状态
  *				@retval 1:	UART处于此状态
  */
#define HAL_UART_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->HAL_UART_CHAN_STAT & (__FLAG__)) == (__FLAG__))

/** @brief  使能 UART中断
  * @param  __HANDLE__ 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  __INTERRUPT__ 待使能的中断类型.
  * @retval None
  */
#define HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->HAL_UART_INT_ENA |= ((__INTERRUPT__)&UART_IT_MASK))

/** @brief  禁用 UART 中断.
  * @param  __HANDLE__ 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  __INTERRUPT__ 待禁用 的中断类型.
  * @retval None
  */
#define HAL_UART_DISABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->HAL_UART_INT_DIS |= ((__INTERRUPT__)&UART_IT_MASK))

/** @brief   使能 UART.
  * @param  __HANDLE__ 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval None
  */
#define HAL_UART_ENABLE(__HANDLE__)                                                                                                                        \
  do                                                                                                                                                         \
  {                                                                                                                                                          \
	(__HANDLE__)->Instance->HAL_UART_CONTROL = ((__HANDLE__)->Instance->HAL_UART_CONTROL | (UART_CTRL_TXDIS | UART_CTRL_RXDIS | UART_CTRL_TXRES | UART_CTRL_RXRES)); \
	uint32_t data = (__HANDLE__)->Instance->HAL_UART_CONTROL;                                                                                                    \
	while ((data & (UART_CTRL_TXRES | UART_CTRL_RXRES)) != 0)                                                                                                \
	{                                                                                                                                                        \
	  data = (__HANDLE__)->Instance->HAL_UART_CONTROL;                                                                                                           \
	}                                                                                                                                                        \
	(__HANDLE__)->Instance->HAL_UART_CONTROL = (UART_CTRL_TXEN | UART_CTRL_RXEN);                                                                                \
  } while (0U)

/** @brief   禁用 UART.
  * @param  __HANDLE__ 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval None
  */
#define HAL_UART_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->HAL_UART_CONTROL |= (UART_CTRL_TXDIS | UART_CTRL_RXDIS))


/**
  * @brief  获取 UART 工作状态.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval HAL state.详情参考 @ref HAL_UART_StateTypeDef.
  */
HAL_UART_StateTypeDef HAL_UART_GetState(HAL_UART_HandleTypeDef *huart);

/**
  * @brief 获取 UART 错误状态.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval UART 错误码.
  */
uint32_t HAL_UART_GetError(HAL_UART_HandleTypeDef *huart);

/**
  * @brief  设置 UART 接收的超时时间.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  timeout 超时时间值.
  * @retval none
  */
void HAL_UART_SetTimeout(HAL_UART_HandleTypeDef *huart, uint16_t timeout);

/**
  * @brief  设置 UART 发送FIFO阈值.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  TxLevel 发送FIFO阈值
  * @retval none
  */
void HAL_UART_SetTxFIFOLevel(HAL_UART_HandleTypeDef *huart, uint32_t TxLevel);

/**
  * @brief  设置 UART 接收FIFO阈值.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  RxLevel 接收FIFO阈值
  * @retval none
  */
void HAL_UART_SetRxFIFOLevel(HAL_UART_HandleTypeDef *huart, uint32_t RxLevel);

/**
  * @brief  初始化 UART.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_UART_Init(HAL_UART_HandleTypeDef *huart);

/**
  * @brief  去初始化 UART.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示去初始化外设成功
  *       @retval  HAL_ERROR    ：表示入参错误
  */
HAL_StatusTypeDef HAL_UART_DeInit(HAL_UART_HandleTypeDef *huart);

/**
  * @brief  发送数据.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  pData 发送数据缓冲区指针.
  * @param  Size  发送数据字节长度.
  * @param  Timeout 超时时间（单位ms）; 如果Time_out取值为HAL_MAX_DELAY，则是一直阻塞，等待发送完指定数量的字节才退出；.\n
  * 		如果Timeout设置为非0非HAL_MAX_DELAY时，表示函数等待数据发送完成的最长时间，超时则退出函数，huart结构体中的TxXferCount表示实际发送的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：表示发送数据成功，表示指定时间内成功发送指定数量的数据
  *       @retval  HAL_ERROR    ：表示入参错误
  *       @retval  HAL_BUSY     ：表示外设正在使用中
  *       @retval  HAL_TIMEOUT  ：表示指定时间内未能成功发送指定数量的数据
  */
HAL_StatusTypeDef HAL_UART_Transmit(HAL_UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  接收数据.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  pData 接收数据缓冲区指针.
  * @param  Size  接收数据字节长度.
  * @param  Timeout 超时时间（单位ms）. 0 是不阻塞，检测到接收FIFO空后，则立刻退出; HAL_MAX_DELAY 是一直阻塞，直到接收到指定数量的字节后才退出.\n
  * 		非0非HAL_MAX_DELAY时，表示函数等待数据接收完成的最长时间，超时则退出函数，huart结构体中的RxXferCount表示实际接收的字节数.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：当Timeout未置为0时，表示指定时间内成功接收指定数量的数据；当Timeout置为0时，也会返回OK，但实际接收数据的数量通过huart结构体中的RxXferCount来确定
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  *       @retval  HAL_TIMEOUT  ：针对Timeout未置为0时，表示指定时间内未能成功接收指定数量的数据
  */
HAL_StatusTypeDef HAL_UART_Receive(HAL_UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
  * @brief  接收中断配置 .
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @param  pData 接收数据缓冲区指针.
  * @param  Size  接收数据字节长度.
  * @return 函数执行状态.详情参考 @ref HAL_StatusTypeDef
  *   返回值可能是以下类型：
  *       @retval  HAL_OK       ：接收中断配置函数执行成功，等待在中断中接收指定数量的的数据
  *       @retval  HAL_ERROR    ：入参错误
  *       @retval  HAL_BUSY     ：外设正在使用中
  */
HAL_StatusTypeDef HAL_UART_Receive_IT(HAL_UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/** 
  * @brief  获取 UART 中断使能状态.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval UART 中断使能状态.
  */
uint32_t HAL_UART_GetITEnable(HAL_UART_HandleTypeDef* huart);

/** 
  * @brief  获取 UART 中断状态.（UART中断为读清）
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval UART 中断状态.
  */
uint32_t HAL_UART_GetITState(HAL_UART_HandleTypeDef* huart);

/**
  * @brief  UART 错误中断回调函数.在对应的UART中断处理函数中调用.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval None
  */
void HAL_UART_ErrorCallback(HAL_UART_HandleTypeDef* huart) __RAM_FUNC;

/**
  * @brief  UART 接收完成回调函数.在对应的UART中断处理函数中调用.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval None
  */
void HAL_UART_RxCpltCallback(HAL_UART_HandleTypeDef *huart) __RAM_FUNC;

/**
  * @brief  UART 中断处理函数.在UART中断服务中调用.
  * @param  huart 指向一个包含UART具体配置信息的 HAL_UART_HandleTypeDef 结构体的指针.详情参考 @ref HAL_UART_HandleTypeDef.
  * @retval None
  */
void UART_IRQHandler(HAL_UART_HandleTypeDef *huart) __RAM_FUNC;

/**
  * @brief  UART 中断服务函数. UART中断触发时调用此函数.
  * @param  None
  * @retval None
  */
void HAL_UART_IRQHandler(void) __RAM_FUNC;






/**
* @brief  UART 外设.
*/
#define HAL_UART1   (HAL_UART_TypeDef *)UART1_BASE





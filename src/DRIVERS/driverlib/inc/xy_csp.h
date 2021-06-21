/** 
* @file        xy_csp.h
* @brief       This file is the csp driver API that users can call.
*
* @attention  User can not call the function in flash when excute interrupt handler, include the subfunction, if necessary,
*             put this function in ram, just declare this function with a suffix " __attribute__((section(".ramtext"))) ",
*             do not change the declare in xy_peripheral.h
*
* @par initialize spi device example

* @par step1: Open clock for device
* @code
  xy_PRCM_ClockCmd(CSP_UART_PRCM_CLK, ENABLE);
  xy_PRCM_ClockCmd(PRCM_CKG_CTL_GPIO, ENABLE);
* @endcode

* @par step2: Clean the pinmux I/O register 
* @code
  xy_GPIO_ClearConfig(CSP_UART_TX_PIN);
  xy_GPIO_ClearConfig(CSP_UART_RX_PIN);
* @endcode
*

* @par step3: Pinmux
* @code
  xy_GPIO_Remapping(CSP_UART_TX_PIN, CSP_UART_REMAP_TX);
  xy_GPIO_Remapping(CSP_UART_RX_PIN, CSP_UART_REMAP_RX);
* @endcode

* @par step4: Peripheral auto-configuration mode, when GPIO is used as a mapping for other peripherals, 
              this mode does not require the user to manually specify the GPIO input and output mode
* @code
  xy_GPIO_SetIOMode(CSP_UART_TX_PIN, GPIO_MODE_HW_PER);
  xy_GPIO_SetIOMode(CSP_UART_RX_PIN, GPIO_MODE_HW_IN_PU);
* @endcode

* @par step5: Initialize csp to uart mode
* @code
  xy_CSP_UART_Init(CSP_UART_PORT, 9600);
* @endcode

* @par step6: Register interrupt if needed
* @code
  NVIC_IntRegister(CSP_UART_INT_VECTOR, csp_int_handler, 1);
* @endcode

* @par step7: Set fifo thread for interrupt if needed
* @code
  xy_CSP_UART_FIFO_Level_Set(CSP_UART_PORT, 128, 128);
* @endcode

* @par step8: Open interrupt if needed
* @code
  xy_CSP_UART_IntCmd(CSP_UART_PORT, CSP_INTTYPE_RX_DONE, ENABLE);
* @endcode

* @par step9: Open device
* @code
  xy_CSP_UART_Cmd(CSP_UART_PORT, ENABLE);
* @endcode
**********************************************************************************
*/
#pragma once

#if BLE_ENABLE

#include "xinyi_hardware.h"
#include "csp.h"
#include "xinyi_hardware.h"

typedef enum {
  CSP1 = (int)CSP1_BASE,
  CSP2 = (int)CSP2_BASE,
  CSP3 = (int)CSP3_BASE,
  CSP4 = (int)CSP4_BASE
} CSP_TypeDef;

typedef enum {
  CSP_DATA_BITS_6 = 6,
  CSP_DATA_BITS_7 = 7,
  CSP_DATA_BITS_8 = 8
} CSPDataBits_TypeDef;

typedef enum {
  CSP_STOP_BITS_1 = 1,
  CSP_STOP_BITS_2 = 2
}CSPStopBits_TypeDef;

typedef enum {
  CSP_DIS_RX_TX  = 0x0,
  CSP_EN_TX_ONLY = 0x1,
  CSP_EN_RX_ONLY = 0x2,
  CSP_EN_RX_TX   = 0x3
}CSPRxTx_TypeDef;

typedef enum {
  CSP_TX_FIFO_LEVEL  = 0x0000,
  CSP_TX_FIFO_FULL   = 0x0080,
  CSP_TX_FIFO_EMEPTY = 0x0100,
  CSP_RX_FIFO_LEVEL  = 0x1000,
  CSP_RX_FIFO_FULL   = 0x1080,
  CSP_RX_FIFO_EMEPTY = 0x1100
}CSPFIFO_TypeDef;

typedef enum {
  CSP_INTTYPE_RX_DONE          = 0x00000001,
  CSP_INTTYPE_TX_DONE          = 0x00000002,
  CSP_INTTYPE_RX_OFLOW         = 0x00000004,
  CSP_INTTYPE_TX_UFLOW         = 0x00000008,
  CSP_INTTYPE_DMA_IO_RX_DONE   = 0x00000010,
  CSP_INTTYPE_DMA_IO_TX_DONE   = 0x00000020,
  CSP_INTTYPE_RXFIFO_FULL      = 0x00000040,
  CSP_INTTYPE_TXFIFO_EMPTY     = 0x00000080,
  CSP_INTTYPE_RXFIFO_THD_REACH = 0x00000100,
  CSP_INTTYPE_TXFIFO_THD_REACH = 0x00000200,
  CSP_INTTYPE_UART_FRM_ERR     = 0x00000400,
  CSP_INTTYPE_RX_TIMEOUT       = 0x00000800,
  CSP_INTTYPE_TX_ALLOUT        = 0x00001000,
  CSP_INTTYPE_INT_ALL          = 0x00001FFF
} CSPInt_TypeDef;

typedef enum{
  CLK_DIV_2,
  CLK_DIV_4,
  CLK_DIV_8,
  CLK_DIV_16,
  CLK_DIV_32,
  CLK_DIV_64,
  CLK_DIV_128,
  CLK_DIV_256 
}CSPClkDiv_TypeDef;

typedef enum{
  MODE_0,
  MODE_1,
  MODE_2,
  MODE_3 
}CSPCommunicationMode_TypeDef;

typedef enum{
  DATA_WIDTH_8,
  DATA_WIDTH_16,
  DATA_WIDTH_32
}CSPDataWidth_TypeDef;

typedef enum{
  MASTER_MODE,
}CSPWorkMode_TypeDef;

/**
 * @brief Initialize CSP, support configuration data bit, stop bit, do not support configuration flow control, default no flow control.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param Baudrate   [IN]  Serial port baud rate
 * @param DataBits   [IN]  Serial port data bits @ref CSPDataBits_TypeDef
 * @param StopBits   [IN]  Serial port stop bits @ref CSPStopBits_TypeDef
 */
void xy_CSP_UART_Init(CSP_TypeDef CSPx, uint32_t Baudrate, CSPDataBits_TypeDef DataBits, CSPStopBits_TypeDef StopBits);

/**
 * @brief Initialize CSP to SPI mode, support configuration Work Mode, stop Communication Mode, clock.
 * 
 * @param CSPx                [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param WorkMode            [IN]  SPI work mode, master only now. @ref SPIWorkMode_TypeDef
 * @param CommunicationMode   [IN]  SPI communication mode, there are mode0~mode3 optional. @ref SPIWorkMode_TypeDef
 * @param SCLK                [IN]  SPI Speed. Max Frequency 1/6 I/O clock
 */
void xy_CSP_SPI_Init(CSP_TypeDef CSPx, CSPWorkMode_TypeDef WorkMode, CSPCommunicationMode_TypeDef CommunicationMode, unsigned long SCLK);

/**
 * @brief This function can hand over the SPI chip select pin automatically controlled by the hardware to the user control, the original hardware control is invalid.
 * 
 * @warning This function needs to be called after the init function.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param NewState   [IN]  The state you want. @ref FunctionalState.
 */
void xy_CSP_SPI_NSS_ManualContorlSet(CSP_TypeDef CSPx,FunctionalState NewState);

/**
 * @brief SPI transmission frame control
 * 
 * @param CSPx             [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param tx_data_length   [IN]  actual data length.
 * @param tx_delay         [IN]  Delay from pull-down of chip select pin to the first data output.
 * @param idle             [IN]  Chip select high level duration between each frame of data.
 */
void xy_CSP_SPI_TxFRAME_CTRL(CSP_TypeDef CSPx, uint8_t tx_data_length, uint8_t tx_delay, uint8_t idle);

/**
 * @brief SPI recieved frame control
 * 
 * @param CSPx             [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param rx_data_length   [IN]  actual data length.
 * @param rx_delay         [IN]  Delay from pull-down of chip select pin to the first data output.
 * @param idle             [IN]  Chip select high level duration between each frame of data.
 */
void xy_CSP_SPI_RxFRAME_CTRL(CSP_TypeDef CSPx, uint8_t rx_data_length, uint8_t rx_delay, uint8_t idle);

/**
 * @brief SPI read write data.Syntactic sugar.
 * 
 * @param CSPx    [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param txBuf   [IN]  Transmit data storage location.
 * @param rxBuf   [IN]  Receive data storage location.
 * @param len     [IN]  Length of sent and received data, in bytes
 */
void xy_CSP_SPI_WriteRead(CSP_TypeDef CSPx, uint8_t *txBuf, uint8_t * rxBuf, uint32_t len);

/**
 * @brief CSP interrupt enable and disable control.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param IntType    [IN]  The type of SPI interrupt that needs to be operated. @ref CSPInt_TypeDef.
 * @param NewState   [IN]  The interrupt state you want. @ref FunctionalState.
 */
void xy_CSP_IntCmd(CSP_TypeDef CSPx, CSPInt_TypeDef IntType, FunctionalState NewState);

/**
 * @brief CSP device enable and disable control.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param NewState   [IN]  The device state you want. @ref FunctionalState.
 */
void xy_CSP_Cmd(CSP_TypeDef CSPx, FunctionalState NewState);

/**
 * @brief Set the FIFO depth that triggers the interrupt.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param TxLevel    [IN]  TX FIFO depth
 * @param RxLevel    [IN]  RX FIFO depth
 * @warning Neither TxLevel nor RxLevel can be set to exceed 128 or 0.
 */
void xy_CSP_FIFO_Level_Set(CSP_TypeDef CSPx, uint32_t TxLevel, uint32_t RxLevel);

/**
 * @brief Set the time to trigger the receiving timeout.
 * @warning only valid for CSP_INTTYPE_RX_TIMEOUT interrupt.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param timeout    [IN]  This unit is the time occupied by one bit under the current UART port baud rate condition.
 * @attention The timeout mechanism is activated when the UART starts to receive.
 */
void xy_CSP_UART_TIMEOUT_Set(CSP_TypeDef CSPx, uint16_t timeout);

/**
 * @brief CSP device TX or RX enable and disable control.
 * @param CSPx       [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param TxRxEn     [IN]  The device state you want. @ref CSPRxTx_TypeDef.
 */
void xy_CSP_TxRxCmd(CSP_TypeDef CSPx, CSPRxTx_TypeDef TxRxEn);


/**
 * @brief Get the CSP FIFO Status
 * @param CSPx         [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param FIFO_State   [IN]  Need to get the status of the SPI FIFO. @ref CSPFIFO_TypeDef.
 * @return uint8_t
 * @par
 * <table>
 * <tr>     <th>FIFO type       <th>Value   <th>Meaning </tr>
 * <tr>  <td>CSP_TX_FIFO_LEVEL  <td> --     <td>Number of unsent bytes in the FIFO </tr>
 * <tr>  <td>CSP_TX_FIFO_FULL   <td> 1/0    <td>FIFO  full / not full </tr>
 * <tr>  <td>CSP_TX_FIFO_EMEPTY <td> 1/0    <td>FIFO  empty / not empty </tr>
 * <tr>  <td>CSP_RX_FIFO_LEVEL  <td> --     <td>Number of unread bytes in the FIFO </tr>
 * <tr>  <td>CSP_RX_FIFO_FULL   <td> 1/0    <td>FIFO  full / not full </tr>
 * <tr>  <td>CSP_RX_FIFO_EMEPTY <td> 1/0    <td>FIFO  empty / not empty </tr>
 */

uint8_t xy_CSP_GetFIFOState(CSP_TypeDef CSPx, CSPFIFO_TypeDef FIFO_State);

/**
 * @brief Get the enable flag bit of the interrupt type specified by CSP
 * @param CSPx         [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param IntType      [IN]  CSP interrupt category. @ref CSPInt_TypeDef.
 * @return @ref FlagStatus 
 * @retval SET
 * Already Enable
 * @retval RESET
 * Disable
 */
FlagStatus xy_CSP_IntEnableGet(CSP_TypeDef CSPx, CSPInt_TypeDef IntType);

/**
 * @brief Get the flag bit of the interrupt type specified by CSP
 * @param CSPx         [IN]  CSP number to be operated. @ref CSP_TypeDef. 
 * @param IntType      [IN]  CSP interrupt category. @ref CSPInt_TypeDef. 
 * @return @ref FlagStatus 
 * @retval SET
 * Already Set
 * @retval RESET
 * Unset
 */
FlagStatus xy_CSP_IntStateGet(CSP_TypeDef CSPx, CSPInt_TypeDef IntType);

/**
 * @brief Clear the flag bit of the interrupt type specified by CSP
 * @param CSPx         [IN]  CSP number to be operated. @ref CSP_TypeDef. 
 * @param IntType      [IN]  CSP interrupt category. @ref CSPInt_TypeDef. 
 */
void xy_CSP_IntStateClear(CSP_TypeDef CSPx, CSPInt_TypeDef IntType);

/**
 * @brief Read one byte of data from RX FIFO.
 * @param CSPx         [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @retval uint8_t
 */
uint8_t xy_CSP_ReadChar(CSP_TypeDef CSPx);

/**
 * @brief Read multiple data from the RX FIFO. 
 *        When the specified length of data is read or the FIFO is empty, the reading will stop and the number of bytes read will be returned.
 * @param CSPx          [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param pBuf          [IN]  Receive data storage location.
 * @param len           [IN]  Length of received data, in bytes
 * @return Length of data read
 * @retval uint32_t
 */
uint32_t xy_CSP_ReadBuf(CSP_TypeDef CSPx, uint8_t *pBuf, uint32_t len);

/**
 * @brief Write a data to TX FIFO.
 * @param CSPx          [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param data          [IN]  Data to be written
 */
void xy_CSP_WriteChar(CSP_TypeDef CSPx, uint8_t data);

/**
 * @brief Write multiple data to the RX FIFO. 
 *        Write the specified length of data to the TX FIFO, write the specified data length, it will stop writing.
 * @param CSPx          [IN]  CSP number to be operated. @ref CSP_TypeDef.
 * @param pBuf          [IN]  Receive data storage location.
 * @param len           [IN]  Length of received data, in bytes
 * @return Length of data written
 * @retval uint32_t
 */
uint32_t xy_CSP_WriteBuf(CSP_TypeDef CSPx, uint8_t *pBuf, uint32_t len);

#endif

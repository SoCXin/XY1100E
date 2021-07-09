/**
  ******************************************************************************
  * @file      startup_cm3.s
  * @author    xinyi platform software team
  * @version   V1.0
  * @date      15-June-2020
  * @brief     xinyi 1100plus vector table for RIDE7 toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */
.word  __data_start__
/* end address for the .data section. defined in linker script */
.word  __data_end__
/* start address for the .bss section. defined in linker script */
.word  __bss_start__
/* end address for the .bss section. defined in linker script */
.word  __bss_end__
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:

/* Disable all interrupt before os start */
  cpsid i

/* Copy the data segment initializers from flash to SRAM */
/*  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4

LoopCopyDataInit:
  ldr  r0, =__data_start__
  ldr  r3, =__data_end__
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit*/
  ldr  r2, =__bss_start__
  b  LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4

LoopFillZerobss:
  ldr  r3, =__bss_end__
  cmp  r2, r3
  bcc  FillZerobss

/* Special word fill the main stack. */
  ldr  r2, =__Main_Stack_Limit
  b  LoopFillMainStack
FillMainStack:
  ldr  r3, =0xA5A5A5A5
  str  r3, [r2], #4

LoopFillMainStack:
  ldr  r3, = __stack
  cmp  r2, r3
  bcc  FillMainStack
  
/* Set the SP to use secondary boot's stack area */
  ldr sp, =__stack
/* Call the clock system intitialization function.*/
  bl  SystemInit
/* Call the application's entry point.*/
  bl  main
  bx  lr
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None
 * @retval None
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  __stack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word  CSP1_Handler
  .word  I2C1_Handler
  .word  SPI1_Handler
  .word  TIM1_Handler
  .word  UART1_Handler
  .word  UTC_Handler
  .word  WDT_Handler
  .word  MCNT_Handler
  .word  TIM2_Handler
  .word  CSP2_Handler
  .word  XTAL_Handler
  .word  PLL_Handler
  .word  QSPI_Handler
  .word  SEMAPHORE_Handler
  .word  TIM3_Handler
  .word  TIM4_Handler
  .word  DMAC0_Handler
  .word  DMAC1_Handler
  .word  DMAC2_Handler
  .word  DMAC3_Handler
  .word  WAKEUP_Handler
  .word  GPIO_Handler
  .word  I2C2_Handler
  .word  CSP3_Handler
  .word  CSP4_Handler
  .word  DFEBUF_Handler
  .word  IRQ26_Handler
  .word  IRQ27_Handler
  .word  IRQ28_Handler
  .word  IRQ29_Handler
  .word  IRQ30_Handler
  .word  IRQ31_Handler
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak  NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak  HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak  MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler

  .weak  BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak  UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak  SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak  DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak  PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak  SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak  CSP1_Handler
  .thumb_set CSP1_Handler,Default_Handler

  .weak  I2C1_Handler
  .thumb_set I2C1_Handler,Default_Handler

  .weak  SPI1_Handler
  .thumb_set SPI1_Handler,Default_Handler

  .weak  TIM1_Handler
  .thumb_set TIM1_Handler,Default_Handler

  .weak  UART1_Handler
  .thumb_set UART1_Handler,Default_Handler

  .weak  UTC_Handler
  .thumb_set UTC_Handler,Default_Handler

  .weak  WDT_Handler
  .thumb_set WDT_Handler,Default_Handler

  .weak  MCNT_Handler
  .thumb_set MCNT_Handler,Default_Handler

  .weak  TIM2_Handler
  .thumb_set TIM2_Handler,Default_Handler

  .weak  CSP2_Handler
  .thumb_set CSP2_Handler,Default_Handler

  .weak  XTAL_Handler
  .thumb_set XTAL_Handler,Default_Handler

  .weak  PLL_Handler
  .thumb_set PLL_Handler,Default_Handler

  .weak  QSPI_Handler
  .thumb_set QSPI_Handler,Default_Handler

  .weak  SEMAPHORE_Handler
  .thumb_set SEMAPHORE_Handler,Default_Handler

  .weak  TIM3_Handler
  .thumb_set TIM3_Handler,Default_Handler

  .weak  TIM4_Handler
  .thumb_set TIM4_Handler,Default_Handler

  .weak  DMAC0_Handler
  .thumb_set DMAC0_Handler,Default_Handler

  .weak  DMAC1_Handler
  .thumb_set DMAC1_Handler,Default_Handler

  .weak  DMAC2_Handler
  .thumb_set DMAC2_Handler,Default_Handler

  .weak  DMAC3_Handler
  .thumb_set DMAC3_Handler,Default_Handler

  .weak  WAKEUP_Handler
  .thumb_set WAKEUP_Handler,Default_Handler

  .weak  GPIO_Handler
  .thumb_set GPIO_Handler,Default_Handler

  .weak  I2C2_Handler
  .thumb_set I2C2_Handler,Default_Handler

  .weak  CSP3_Handler
  .thumb_set CSP3_Handler,Default_Handler

  .weak  CSP4_Handler
  .thumb_set CSP4_Handler,Default_Handler

  .weak  DFEBUF_Handler
  .thumb_set DFEBUF_Handler,Default_Handler

  .weak  IRQ26_Handler
  .thumb_set IRQ26_Handler,Default_Handler

  .weak  IRQ27_Handler
  .thumb_set IRQ27_Handler,Default_Handler

  .weak  IRQ28_Handler
  .thumb_set IRQ28_Handler,Default_Handler

  .weak  IRQ29_Handler
  .thumb_set IRQ29_Handler,Default_Handler

  .weak  IRQ30_Handler
  .thumb_set IRQ30_Handler,Default_Handler

  .weak  IRQ31_Handler
  .thumb_set IRQ31_Handler,Default_Handler

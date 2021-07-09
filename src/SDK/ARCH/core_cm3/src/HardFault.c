#include "HardFault.h"

#include "dump.h"
#include "sys_debug.h"

#if ( USE_HardFault_DUMP )

#ifdef __CC_ARM
/******************************************************************************
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 *****************************************************************************/
__asm void HardFault_Handler(void)
{
	/* get the SP, is MSP or PSP, as a parameter to jump to Dump_Handler_in_Fault */
	TST    LR, #4				// test current sp is MSP or PSP
	ITE    EQ
	MRSEQ  R0, MSP
	MRSNE  R0, PSP				// current SP may as a parameter of Register_Dump_from_Interrupt
	/* jump to Dump_Handler_in_Fault */
	B      Dump_Handler_in_Fault
);

/* in the end, R0 is also equal to SP, that is the stack point before trigger hardfault,
* it also can be a parameter of a function.
*/
}
#endif

#ifdef __GNUC__
/******************************************************************************
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 *****************************************************************************/
void __attribute__ (( naked )) HardFault_Handler(void)
{
	__asm__ __volatile__ (
		/* get the sp, is msp or psp, as a parameter to jump to Dump_Handler_in_Fault */
		"	tst    lr, #4						\n"    // test current sp is msp or psp
		"	ite    eq							\n"
		"	mrseq  r0, msp						\n"
		"	mrsne  r0, psp						\n"    // current sp may as a parameter of Register_Dump_from_Interrupt
		"	b      Dump_Handler_in_Fault		\n"
		"	.align 4							\n"
	);

/* in the end, r0 is also equal to sp, that is the stack point before trigger hardfault,
 * it also can be a parameter of a function.
 */
}
#endif

#else

/******************************************************************************
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 *****************************************************************************/
void HardFault_Handler(void)
{
	Sys_Assert(0);
	while(1);
}

#endif


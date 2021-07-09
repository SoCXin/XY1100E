#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__

//*****************************************************************************
//
// The following are defines for the Timer register offsets.
//
//*****************************************************************************
#define TIMER_CNT               0x00000000  // Timer current count value
#define TIMER_RLD               0x00000004  // Timer reload value; In compare mode, this is the comparing value
#define TIMER_PWM               0x00000008  // Timer controls PWM modulation; Store the Capture values for the CAPTURE and CAPTURE/COMPARE modes.
#define TIMER_CTL               0x0000000C  // Timer control

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_CNT register.
//
//*****************************************************************************
#define TIMER_CNT_COUNT_Pos     0
#define TIMER_CNT_COUNT_Msk     0x0000FFFF


//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_RLD register.
//
//*****************************************************************************
#define TIMER_RLD_RELOAD_Pos    0
#define TIMER_RLD_RELOAD_Msk    0x0000FFFF


//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_PWM register.
//
//*****************************************************************************
#define TIMER_PWM_PWM_Pos       0
#define TIMER_PWM_PWM_Msk       0x0000FFFF


//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_CTL register.
//
//*****************************************************************************
#define TIMER_CTL_INPCAP_Pos            0
#define TIMER_CTL_INPCAP_Msk            (1UL << TIMER_CTL_INPCAP_Pos)
#define TIMER_CTL_PWMD_Pos              1
#define TIMER_CTL_PWMD_Msk              (7UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_NO_DELAY         (0UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_2_CYCLE    (1UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_4_CYCLE    (2UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_8_CYCLE    (3UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_16_CYCLE   (4UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_32_CYCLE   (5UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_64_CYCLE   (6UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_PWMD_DELAY_128_CYCLE  (7UL << TIMER_CTL_PWMD_Pos)
#define TIMER_CTL_RSTCNT_Pos            4
#define TIMER_CTL_RSTCNT_Msk            (1UL << TIMER_CTL_RSTCNT_Pos)
#define TIMER_CTL_RSTCNT_EN             (1UL << TIMER_CTL_RSTCNT_Pos)
#define TIMER_CTL_RSTCNT_DIS            (0UL << TIMER_CTL_RSTCNT_Pos)
#define TIMER_CTL_TICONFIG_Pos          5
#define TIMER_CTL_TICONFIG_Msk          (3UL << TIMER_CTL_TICONFIG_Pos)
#define TIMER_CTL_TICONFIG_OUTER_EVENT  (2UL << TIMER_CTL_TICONFIG_Pos)
#define TIMER_CTL_TICONFIG_INNER_EVENT  (3UL << TIMER_CTL_TICONFIG_Pos)
#define TIMER_CTL_TICONFIG_ALL_EVENT    (0UL << TIMER_CTL_TICONFIG_Pos)
#define TIMER_CTL_TEN_Pos               7
#define TIMER_CTL_TEN_Msk               (1UL << TIMER_CTL_TEN_Pos)
#define TIMER_CTL_TEN_EN                (1UL << TIMER_CTL_TEN_Pos)
#define TIMER_CTL_TEN_DIS               (0UL << TIMER_CTL_TEN_Pos)
#define TIMER_CTL_TMODE_Pos             8
#define TIMER_CTL_TMODE_Msk             (15UL << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_ONE_SHOT        (0UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_CONTINUOUS      (1UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_COUNTER         (2UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_PWM_SINGLE      (3UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_CAPTURE         (4UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_COMPARE         (5UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_GATED           (6UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_CAP_AND_CMP     (7UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_TMODE_PWM_DUAL        (8UL  << TIMER_CTL_TMODE_Pos)
#define TIMER_CTL_PRES_Pos              12
#define TIMER_CTL_PRES_Msk              (7UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_1         (0UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_2         (1UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_4         (2UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_8         (3UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_16        (4UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_32        (5UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_64        (6UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_PRES_DIVIDE_128       (7UL << TIMER_CTL_PRES_Pos)
#define TIMER_CTL_TPOL_Pos              15
#define TIMER_CTL_TPOL_Msk              (1UL << TIMER_CTL_TPOL_Pos)
#define TIMER_CTL_TPOL_TRUE             (1UL << TIMER_CTL_TPOL_Pos)
#define TIMER_CTL_TPOL_FALSE            (0UL << TIMER_CTL_TPOL_Pos)


#endif // __HW_TIMER_H__

#ifndef __HW_UTC_H__
#define __HW_UTC_H__

//*****************************************************************************
//
// The following are defines for the UTC register offsets.
//
//*****************************************************************************
#define UTC_CTRL                    0x00000000  //
#define UTC_HOUR_MODE               0x00000004  //
#define UTC_TIMER                   0x00000008  //
#define UTC_CAL                     0x0000000C  //
#define UTC_ALARM_TIMER             0x00000010  //
#define UTC_ALARM_CAL               0x00000014  //
#define UTC_ALARM_EN                0x00000018  //
#define UTC_INT_STAT                0x0000001C  //
#define UTC_INT_EN                  0x00000020  //
#define UTC_INT_DIS                 0x00000024  //
#define UTC_INT_MSK                 0x00000028  //
#define UTC_STATUS                  0x0000002C  //
#define UTC_KEEP_UTC                0x00000030  //
#define UTC_CLK_CNT                 0x00000034  //
#define UTCALARMCLKCNT_CFG			0x00000038  //
//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_CTRL register.
//
//*****************************************************************************
#define UTC_CTRL_TIMER_STOP_Pos     0
#define UTC_CTRL_TIMER_STOP         (1UL << UTC_CTRL_TIMER_STOP_Pos)

#define UTC_CTRL_CAL_STOP_Pos       1
#define UTC_CTRL_CAL_STOP           (1UL << UTC_CTRL_CAL_STOP_Pos)

#define UTC_CTRL_DIVCNT_Pos         2
#define UTC_CTRL_DIVCNT             (1UL << UTC_CTRL_DIVCNT_Pos)

//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_HOUR_MODE register.
//
//*****************************************************************************
#define UTC_HOUR_MODE_Pos           0
#define UTC_HOUR_MODE_Msk           (1UL << UTC_HOUR_MODE_Pos)
#define UTC_HOUR_MODE_12H           (1UL << UTC_HOUR_MODE_Pos)
#define UTC_HOUR_MODE_24H           (0UL << UTC_HOUR_MODE_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_TIMER register.
//
//*****************************************************************************
#define UTC_TIMER_HU_Pos            0
#define UTC_TIMER_HU_Msk            (0xF << UTC_TIMER_HU_Pos)
#define UTC_TIMER_HT_Pos            4
#define UTC_TIMER_HT_Msk            (0xF << UTC_TIMER_HT_Pos)
#define UTC_TIMER_SU_Pos            8
#define UTC_TIMER_SU_Msk            (0xF << UTC_TIMER_SU_Pos)
#define UTC_TIMER_ST_Pos            12
#define UTC_TIMER_ST_Msk            (0x7 << UTC_TIMER_ST_Pos)
#define UTC_TIMER_MU_Pos            16
#define UTC_TIMER_MU_Msk            (0xF << UTC_TIMER_MU_Pos)
#define UTC_TIMER_MT_Pos            20
#define UTC_TIMER_MT_Msk            (0x7 << UTC_TIMER_MT_Pos)
#define UTC_TIMER_MRU_Pos           24
#define UTC_TIMER_MRU_Msk           (0xF << UTC_TIMER_MRU_Pos)
#define UTC_TIMER_MRT_Pos           28
#define UTC_TIMER_MRT_Msk           (0x3 << UTC_TIMER_MRT_Pos)
#define UTC_TIMER_PM_Pos            30
#define UTC_TIMER_PM_Msk            (1UL << UTC_TIMER_PM_Pos)
#define UTC_TIMER_PM                (1UL << UTC_TIMER_PM_Pos)
#define UTC_TIMER_CH_Pos            31
#define UTC_TIMER_CH_Msk            (1UL << UTC_TIMER_CH_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_CAL register.
//
//*****************************************************************************
#define UTC_CAL_DAY_Pos             0
#define UTC_CAL_DAY_Msk             (0x7 << UTC_CAL_DAY_Pos)
#define UTC_CAL_MU_Pos              3
#define UTC_CAL_MU_Msk              (0xF << UTC_CAL_MU_Pos)
#define UTC_CAL_MT_Pos              7
#define UTC_CAL_MT_Msk              (1UL << UTC_CAL_MT_Pos)
#define UTC_CAL_DU_Pos              8
#define UTC_CAL_DU_Msk              (0xF << UTC_CAL_DU_Pos)
#define UTC_CAL_DT_Pos              12
#define UTC_CAL_DT_Msk              (3UL << UTC_CAL_DT_Pos)
#define UTC_CAL_YU_Pos              16
#define UTC_CAL_YU_Msk              (0xF << UTC_CAL_YU_Pos)
#define UTC_CAL_YT_Pos              20
#define UTC_CAL_YT_Msk              (0xF << UTC_CAL_YT_Pos)
#define UTC_CAL_CU_Pos              24
#define UTC_CAL_CU_Msk              (0xF << UTC_CAL_CU_Pos)
#define UTC_CAL_CT_Pos              28
#define UTC_CAL_CT_Msk              (3UL << UTC_CAL_CT_Pos)
#define UTC_CAL_CH_Pos              31
#define UTC_CAL_CH_Msk              (1UL << UTC_CAL_CH_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_ALARM_TIMER register.
//
//*****************************************************************************
#define UTC_ALARM_TIMER_HU_Pos      0
#define UTC_ALARM_TIMER_HU_Msk      (0xF << UTC_ALARM_TIMER_HU_Pos)
#define UTC_ALARM_TIMER_HT_Pos      4
#define UTC_ALARM_TIMER_HT_Msk      (0xF << UTC_ALARM_TIMER_HT_Pos)
#define UTC_ALARM_TIMER_SU_Pos      8
#define UTC_ALARM_TIMER_SU_Msk      (0xF << UTC_ALARM_TIMER_SU_Pos)
#define UTC_ALARM_TIMER_ST_Pos      12
#define UTC_ALARM_TIMER_ST_Msk      (0x7 << UTC_ALARM_TIMER_ST_Pos)
#define UTC_ALARM_TIMER_MU_Pos      16
#define UTC_ALARM_TIMER_MU_Msk      (0xF << UTC_ALARM_TIMER_MU_Pos)
#define UTC_ALARM_TIMER_MT_Pos      20
#define UTC_ALARM_TIMER_MT_Msk      (0x7 << UTC_ALARM_TIMER_MT_Pos)
#define UTC_ALARM_TIMER_MRU_Pos     24
#define UTC_ALARM_TIMER_MRU_Msk     (0xF << UTC_ALARM_TIMER_MRU_Pos)
#define UTC_ALARM_TIMER_MRT_Pos     28
#define UTC_ALARM_TIMER_MRT_Msk     (0x3 << UTC_ALARM_TIMER_MRT_Pos)
#define UTC_ALARM_TIMER_PM_Pos      30
#define UTC_ALARM_TIMER_PM_Msk      (1UL << UTC_ALARM_TIMER_PM_Pos)
#define UTC_ALARM_TIMER_PM          (1UL << UTC_ALARM_TIMER_PM_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_ALARM_CAL register.
//
//*****************************************************************************
#define UTC_ALARM_CAL_MU_Pos        3
#define UTC_ALARM_CAL_MU_Msk        (0xF << UTC_ALARM_CAL_MU_Pos)
#define UTC_ALARM_CAL_MT_Pos        7
#define UTC_ALARM_CAL_MT_Msk        (1UL << UTC_ALARM_CAL_MT_Pos)
#define UTC_ALARM_CAL_DU_Pos        8
#define UTC_ALARM_CAL_DU_Msk        (0xF << UTC_ALARM_CAL_DU_Pos)
#define UTC_ALARM_CAL_DT_Pos        12
#define UTC_ALARM_CAL_DT_Msk        (3UL << UTC_ALARM_CAL_DT_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_ALARM_EN register.
//
//*****************************************************************************
#define UTC_ALARM_MIN_SEC           0x00000001
#define UTC_ALARM_SEC               0x00000002
#define UTC_ALARM_MIN               0x00000004
#define UTC_ALARM_HOUR              0x00000008
#define UTC_ALARM_DATE              0x00000010
#define UTC_ALARM_MONTH             0x00000020
#define UTC_ALARM_ALL               0x0000003F


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_INT_XXX register.
//
//*****************************************************************************
#define UTC_INT_MIN_SEC             0x00000001
#define UTC_INT_SEC                 0x00000002
#define UTC_INT_MIN                 0x00000004
#define UTC_INT_HOUR                0x00000008
#define UTC_INT_DATE                0x00000010
#define UTC_INT_MONTH               0x00000020
#define UTC_INT_ALARM               0x00000040
#define UTC_INT_ALL                 0x0000007F


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_STATUS register.
//
//*****************************************************************************
#define UTC_STATUS_VALID_TIMER_Pos          0
#define UTC_STATUS_VALID_TIMER_Msk          (1UL << UTC_STATUS_VALID_TIMER_Pos)
#define UTC_STATUS_VALID_CAL_Pos            1
#define UTC_STATUS_VALID_CAL_Msk            (1UL << UTC_STATUS_VALID_CAL_Pos)
#define UTC_STATUS_VALID_ALARM_TIMER_Pos    2
#define UTC_STATUS_VALID_ALARM_TIMER_Msk    (1UL << UTC_STATUS_VALID_ALARM_TIMER_Pos)
#define UTC_STATUS_VALID_ALARM_CAL_Pos      3
#define UTC_STATUS_VALID_ALARM_CAL_Msk      (1UL << UTC_STATUS_VALID_ALARM_CAL_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_KEEP_UTC register.
//
//*****************************************************************************
#define UTC_KEEP_UTC_Pos            0
#define UTC_KEEP_UTC_Msk            (1UL << UTC_KEEP_UTC_Pos)


//*****************************************************************************
//
// The following are defines for the bit fields in the UTC_CLK_CNT register.
//
//*****************************************************************************
#define UTC_CLK_CNT_Pos             0
#define UTC_CLK_CNT_Msk             (0x7FFF << UTC_CLK_CNT_Pos)


#endif // __HW_UTC_H__

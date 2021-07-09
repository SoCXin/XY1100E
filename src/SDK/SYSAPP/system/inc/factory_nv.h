#ifndef _FACTORY_NV_H_
#define _FACTORY_NV_H_
#include  <stdint.h>

#define FAC_NV_LOAD_ADDR 0x27003000
#define FAC_NV_EXEC_ADDR 0x27003000

#define G_LRRC_SUPPORT_BAND_MAX_NUM      19
#define D_NVM_MAX_SIZE_IMEI              9
#define D_NVM_MAX_SIZE_IMEI_SV           10

#define NVM_MAX_ADD_SPECTRUM_EMISSION_MAX_NUM        5

#define NVM_MAX_DCN_ID_NUM                 32 
#define NVM_MAX_CANDIDATE_FREQ_NUM         32
#define NVM_MAX_OPERATOR_NUM               3 
#define NVM_MAX_OPERATOR_PLMN_NUM          10
#define NVM_MAX_EPLMN_NUM                  16

#define NVM_MAX_PRE_EARFCN_NUM             G_LRRC_SUPPORT_BAND_MAX_NUM
#define NVM_MAX_PRE_BAND_NUM               5
#define NVM_MAX_LOCK_FREQ_NUM              20
#define NVM_MAX_SN_LEN                     64

#define NAS_IMSI_LTH   9
#define G_LRRC_PRE_EARFCN_INVALID           1111111


typedef struct
{
    unsigned char    ucProfile0x0002SupportFlg;
    unsigned char    ucProfile0x0003SupportFlg;
    unsigned char    ucProfile0x0004SupportFlg;
    unsigned char    ucProfile0x0006SupportFlg;
    unsigned char    ucProfile0x0102SupportFlg;
    unsigned char    ucProfile0x0103SupportFlg;
    unsigned char    ucProfile0x0104SupportFlg;
}NVM_ROHC_PROFILE_STRU;

typedef enum
{
    LRRC_MAX_ROHC_SESSION_CS2 = 0, 
    LRRC_MAX_ROHC_SESSION_CS4,
    LRRC_MAX_ROHC_SESSION_CS8,
    LRRC_MAX_ROHC_SESSION_CS12
}LRRC_MAX_ROHC_SESSION_VALUE;

typedef struct
{    
    unsigned char     ucSuppBand;
	unsigned char     ucPowerClassNB20dBmR13Flg;
    unsigned char     ucAddSpectrumEmissionNum;
    unsigned char     aucAddSpectrumEmission[NVM_MAX_ADD_SPECTRUM_EMISSION_MAX_NUM];
}LRRC_SUPPORT_BAND_STRU;

typedef struct
{
    unsigned char     ucSuppBand;
    unsigned char     ucEnable;        /* 0:not Valid 1:Valid */
    unsigned short    usOffset;        /*EndFreq = ulStartFreq + usOffset*/
    unsigned long     ulStartFreq;
}LRRC_PRE_BAND_STRU;

typedef struct
{
    unsigned char    ucApn[22];
    unsigned char    auPlmnList[NVM_MAX_OPERATOR_PLMN_NUM][3];      /* Byte Order: MCC2 MCC1 | MNC3 MCC3 | MNC2 MNC1 */
    unsigned long    aulPreEarfcn[NVM_MAX_PRE_EARFCN_NUM];
    LRRC_PRE_BAND_STRU    aPreBand[NVM_MAX_PRE_BAND_NUM];
}LNB_NAS_OPERATOR_CFG_DATA_STUR;

typedef struct {
    unsigned char    aucImei[D_NVM_MAX_SIZE_IMEI]; /* IMEI */               //##8,74,85,2,1,51,69,147,9##&&
    unsigned char    ucReserved1;                                           //##0##
    unsigned char    aucImeisv[D_NVM_MAX_SIZE_IMEI_SV];/* IMEI(SV) */       //##9,67,85,2,1,51,69,147,121,240##&&
}T_ImeiInfo;

typedef struct
{
#define     D_AUTH_PROT_NONE            0
#define     D_AUTH_PROT_PAP             1
#define     D_AUTH_PROT_CHAP            2
    unsigned short                      ucUseFlg:1;                        //##0##
    unsigned short                      ucUsernameLen:5;                   //##0##
    unsigned short                      ucPasswordLen:5;                   //##0##
    unsigned short                      ucAuthProt:3;                      //##0##
    unsigned short                      ucPadding:2;                       //##0##
    unsigned short                      ucPadding2;                        //##0##
#define     D_PCO_AUTH_MAX_LEN          16 
    unsigned char                       aucUsername[D_PCO_AUTH_MAX_LEN];   //##0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0##
    unsigned char                       aucPassword[D_PCO_AUTH_MAX_LEN];   //##0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0##
}T_PCO_AUTH_INFO_STRU;

typedef struct
{
    unsigned char    ucUsed:1;                                              //##0##
    unsigned char    ucPdpType:3;                                           //##0##
    unsigned char    ucAtCid:4;                                             //##0##

    unsigned char    ucNSLPI:1;                                             //##0##
    unsigned char    ucP_cid:1;                                             //##0##, cid index
    unsigned char    ucApnLen:6;                                            //##0##
    
#define FACTORY_USER_SET_APN_LEN    38
    unsigned char    ucApn[FACTORY_USER_SET_APN_LEN];                       //##0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0##
    T_PCO_AUTH_INFO_STRU tAuthInfo; //40 byte
}LNB_NAS_USER_SET_CID_INFO;

typedef struct
{
    unsigned char    ucAcsStratumRelType:4;                                 //##0##
	unsigned char    ucMultiToneSupportFlg:1;                               //##1##
    unsigned char    ucMultiDRBsupportFlg:1;                                //##1##
	unsigned char    ucMaxROHCContextSessions:2;                            //##0##

    unsigned char    ucDataInactMonR14SupFlg:1;                            //##0##
    unsigned char    ucRaiSupptR14SupFlg:1;                                //##0##
    unsigned char    ucMulCarrNprachR14SupFlg:1;                           //##0##
    unsigned char    ucTwoHarqProR14SupFlg:1;                              //##0##
    unsigned char    ucPwrClassNb14dBmR14SupFlg:1;                         //##0##
    unsigned char    ucMulCarrPagSupportFlg:1;                             //##0##
	unsigned char    ucMultiCarrierSupportFlg:1;                           //##0##
	unsigned char    ucMultiNSPmaxSupportFlg:1;                            //##0##
	
    unsigned char    ucLRrcPdcpParamFlg:1;                                 //##1##
	unsigned char    ucProfile0x0002SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0003SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0004SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0006SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0102SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0103SupportFlg:1;                          //##0##
	unsigned char    ucProfile0x0104SupportFlg:1;                          //##0##
	
    unsigned char    ucUeCategory:2;                                       //##0##
	unsigned char    ucInterferenceRandomisationR14:1;                     //##0##
	unsigned char    ucEarlyContentionResolutionR14:1;                     //##0##

    unsigned char    ucInterFreqRSTDmeasFlg:1;                             //##1##
    unsigned char    ucAddNeighbCellInfoListFlg:1;                         //##1##
    unsigned char    ucPrsIdFlg:1;                                         //##1##
    unsigned char    ucTpSeparationViaMutingFlg:1;                         //##0##

	
	unsigned char    ucSupportBandNum;                                     //##3##

    unsigned char    ucAddPrsCfgFlg:1;                                     //##0##
    unsigned char    ucPrsBasedTbsFlg:1;                                   //##1##
    unsigned char    ucAddPathReportFlg:1;                                 //##1##
    unsigned char    ucDensePrsCfgFlg:1;                                   //##0##  
    unsigned char    ucPrsOccGroupFlg:1;                                   //##0##
    unsigned char    ucPrsFreqHoppingFlg:1;                                //##0##
    unsigned char    ucPeriodicalReportingFlg:1;                           //##1##
    unsigned char    ucMultiPrbNprsFlg:1;                                  //##1##

    unsigned char    ucIdleStateForMeasFlg:1;                              //##1##
    unsigned char    ucMaxSupptPrsBandwidthFlg:1;                          //##0##
    unsigned char    ucMaxSupptPrsBandwidth:3;                             //##0##
    unsigned char    ucMaxSupptPrsCfgFlg:1;                                //##0##
    unsigned char    ucMaxSupptPrsCfg:2;                                   //##0##1/2    

    unsigned char    ucCrtdcpRptIndFlg:1;                                  //##0##
    unsigned char    ucPadding2:7;                                         //##0##
	LRRC_SUPPORT_BAND_STRU    aSupportBandInfo[G_LRRC_SUPPORT_BAND_MAX_NUM];
}T_UeCapa;

typedef struct
{
    unsigned char    ucATLockEarfcnFlg;
    unsigned char    ucATLockCellIDFlg;
    unsigned short   usATLockCellID;

    unsigned long    ulATLockEarfcn;
}T_AtLockEarfcnCellInfo;
typedef struct
{
    unsigned char    ucMsgService:1;
    unsigned char    ucCmmsN:2;
    unsigned char    ucScaLen:4;
    unsigned char    ucPadding:1;
    unsigned char    ucToSca;
#define              D_SMS_SCA_SIZE_MAX          12 
    unsigned char    aucSca[D_SMS_SCA_SIZE_MAX];
}T_SMS_CFG_INFO;

typedef struct
{
	unsigned char    ucNfCapabilitySupportFlg:1;                           //##0##
	unsigned char    ucS1UDataSupportFlg:1;                                //##1##
	unsigned char    ucERwOpdnSupportFlg:1;                                //##0##
	unsigned char    ucExtPeriodicTimersSupportFlg:1;                      //##1##
	unsigned char    ucEdrxSupportFlg:1;                                   //##1##
	unsigned char    ucPsmSupportFlg:1;                                    //##1##
	unsigned char    ucAUTV:1;                                             //##1##
	unsigned char    ucCpBackOffSupportFlg:1;                              //##0##

	unsigned char    ucRestrictECSupportFlg:1;                             //##0##
	unsigned char    uc3gppsDataOffSupportFlg:1;                           //##0##
	unsigned char    ucRdsSupportFlg:1;                                    //##0##
	unsigned char    ucApnRateControlSupportFlg:1;                         //##1##
	unsigned char    ucPsmEnableFlg:1;                                     //##1##&&
    unsigned char    ucEdrxEnableFlg:1;                                    //##0##&&
	unsigned char    ucAutoConnectFlg:1;                                   //##1##&&
	unsigned char    ucL2THPFlag:1;                                        //##0##

	unsigned char    ucActTimeBakFlg:1;                                    //##0##
	unsigned char    ucEdrxsValBakFlg:1;                                   //##0##
	unsigned char    ucTraceLevel:1;                                       //##0##
	unsigned char    ucPowerTest:1;                                        //##0##
	unsigned char    aucBandScan:2;                                        //##0##
	unsigned char    ucFreqScanType:2;                                     //##0##

	unsigned char    ucPreferCiotOpt:2;                                    //##1##
	unsigned char    ucDataInactTimer:5;                                   //##0##
    unsigned char    ucPSRptInfoFlg:1;                                     //##0##

	unsigned char    ucReqPeriodiTauValue;                                 //##72##&&
	unsigned char    ucReqActTimeValue;                                    //##33##&&
	unsigned char    ucReqEdrxValue;                                       //##2##&&
	unsigned char    ucSinrOffset;                                         //##0##

	unsigned char    ucPowOffSleep:1;                                      //##0##
	unsigned char    ucUpCIoTSupportFlg:1;                                 //##0##
	unsigned char    ucSuppBandNum:6;                                      //##2##&&
	unsigned char    aucSupportBandList[G_LRRC_SUPPORT_BAND_MAX_NUM];      //##5,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0##&&

	LNB_NAS_OPERATOR_CFG_DATA_STUR      aOperatorCfgData[NVM_MAX_OPERATOR_NUM];

    LNB_NAS_USER_SET_CID_INFO tCidInfo0; //cid index = 0
    unsigned short      usOosTimerLen;                                     //##60##&&
    unsigned char    ucSecAlg;
	unsigned char    ucPsTestTauLen;                                       //##0##

    unsigned long    aulLockFreqList[NVM_MAX_LOCK_FREQ_NUM];          //##1200,1201,1575,1949,2400,2401,2525,2649,3450,3451,3625,3799,0,0,0,0,0,0,0,0##

	unsigned char    ucLockFreqNum;                                   //##12##
    unsigned char    ucUpReqFlag:1;//1                                                    //##0##&&
    unsigned char    ucCellReselForbitFlg:1;  /* 0: can cell reselection; 1: can not  */  //##0##
    unsigned char    ucUpRptFlag:1; //####&& /*0: Up Capa not support for Pulbic Net;  1: Up Report depend on Nv*/
    unsigned char    ucPsSpeedFlag:1; /*0:    1: update vrr=vrh when appear SnGap*/ 
    unsigned char    ucPsmReqFlag:1;
    unsigned char    ucWakeupInitUsimFlg:1;
    unsigned char    ucCciotoptN:2;                                        //##0##
    unsigned char    ucLockFreqListStat:1;
    unsigned char    ucNoSimSlpTimeLen:7;                                 //##127##&&
    unsigned char    ucPTWValue;                                          //##3##&&

    unsigned char    ucUlRateCtrlBufSize;
    unsigned char    ucERegMode:3;                                         //##0##&&
    unsigned char    ucCsconN:1;                                           //##0##
    unsigned char    ucCgerepMode:1;                                       //##1##
    unsigned char    ucCrtdcpRepValue:1;                                   //##0##
    unsigned char    ucEDrxMode:2;                                         //##0##

    unsigned char    ucCtzrReport:2;                                       //##3##
    unsigned char    ucStorageOperType:4;                                  //##0##
    unsigned char    ucPadding6:2;                                         //##0##
   
    unsigned char    ucNgactrN:1;                                          //##0##
    unsigned char    ucNptwEDrxMode:2;                                     //##0##
    unsigned char    ucNotSinglBand:1;                                     //##0##
    unsigned char    ucPadding4:4;                                         //####&&

    LNB_NAS_USER_SET_CID_INFO tCidInfo1; //cid index = 1

    T_AtLockEarfcnCellInfo tAtLockEarfcnCell;

    unsigned char    ucReqPeriodiTauValue_Bak;                             //##0##
    unsigned char    ucReqActTimeValue_Bak;                                //##0##
    unsigned char    ucReqEdrxValue_Bak;                                   //##0##
    unsigned char    ucPTWValue_Bak;                                       //##0##

    unsigned short   usSyncTimePeriod;                                     //##0##
    unsigned short   usBarringReleaseDelay:11;                             //##0##
    unsigned char    ucEpcoSupportFlg:1;                                   //##1##
    unsigned char    ucNonIpNoSmsFlg:1;                                    //##0##
    unsigned char    ucT3412ExtChgReportFlg:1;                             //##1##
    unsigned char    ucPadding5:2;                                         //##0##
    unsigned char    ucDiscLen;                                            //##0##
    T_SMS_CFG_INFO   tSmsCfgInfo;
    unsigned char  aucPadding3[565];/* in all :1536 */
}T_NasNvInfo;

typedef struct
{
    int  magic_num;       //erase flash magic num,such as 5A5A5A5A
    T_UeCapa            tUeCapa;
    T_NasNvInfo         tNasNv;
}T_PsNvInfo;

enum Peri_Remap_Type{
	REMAP_SWD = 0,
 	REMAP_CSP2,
	REMAP_UART,
	REMAP_CSP3,
	REMAP_I2C1,
	REMAP_SPI,
	REMAP_TMR1,
	REMAP_JTAG,
	REMAP_LED,
	REMAP_RFSWITCH,
	REMAP_SIM,
	
	REMAP_PERI_MAX
};

typedef struct
{
	unsigned short txDcQ_LP;
	unsigned short txDcI_LP;
	unsigned short txDcQ_HP;
	unsigned short txDcI_HP;
}rf_mt_dc_t;

typedef struct
{
	rf_mt_dc_t 	txDC_LB;
	rf_mt_dc_t 	txDC_HB;
	unsigned short		txIqAmp_LB;
	unsigned short		txIqAmp_HB;
}rf_mt_dc_iq_t;

typedef struct
{
	rf_mt_dc_iq_t 	txDcIqNv_LLT;
	rf_mt_dc_iq_t 	txDcIqNv_HLT;
	rf_mt_dc_iq_t 	txDcIqNv_LMT;
	rf_mt_dc_iq_t 	txDcIqNv_HMT;
	rf_mt_dc_iq_t 	txDcIqNv_LHT;
	rf_mt_dc_iq_t 	txDcIqNv_HHT;
	
	unsigned char	txDcIqNvFlag_LLT;
	unsigned char	txDcIqNvFlag_HLT;
	unsigned char	txDcIqNvFlag_LMT;
	unsigned char	txDcIqNvFlag_HMT;
	unsigned char	txDcIqNvFlag_LHT;
	unsigned char	txDcIqNvFlag_HHT;
	unsigned char	padding[2];
}rf_dciq_t;

//size is 560 bytes,user can use 1800 bytes in tail 
typedef struct {
    char  deepsleep_enable;    //##1##&& go into SoC DEEPSLEEP,will write back NV to flash , maybe special user not need to deep sleep
    char  work_mode;           //##0##&& CPU core power mode;0:DSP and M3 POWER ON,must use worklock;1:only M3 POWER ON by use PLL,2:only M3 POWER ON by use XTAL
    char  ver_type;            //##2## software version type;1:telecom operators version,not use WORKLOCK; 2: SDK version;4:jinka SDK;8:CMIOT;16:ZeusOS
    char  set_tau_rtc;         //##1##&& set TAU RTC or not when into DEEPSLEEP by PSM mode
 	 
    char  lpm_standby_enable;        //##1## go into SoC standby sleep,AT uart will close BBPLL,so "+ERROR:8003" frequently 4
    char  wfi_enable;                //##1## go into CPU wfi sleep
    char  offtime;                   //##0##&& unused. indicate whether do AT+OFFTIME or not when POWER ON by POWER OFF mode;if close,POWER ON will do attach
    char  padding10;                //##0##&& 

	char  g_NPSMR_enable;           //##0##&&if 0,+POWERDOWN:<time> +POWERON:<type>;if 1,+NPSMR:1,+NPSMR:0 8
	char  pin_reset;                //##0##&& PIN reset switch,if 0,do nothing;if 1,willerase all NV,and restore factory NV
	char  keep_retension_mem;       //##0##&& when support power off,retension memory must off;else user can set here,if period time < 1 hour,must set 1,else maybe flash bad
	uint8_t deepsleep_delay;		//##30##&& wait some seconds for user to lock by sending AT command when system power on by EXTPIN_WAKEUP

	uint8_t ext_lock_disable;		 //##0## 0:ext mcu input atcommand will add ext worklock automatically, 1:ext mcu input do not add ext worklock
	uint8_t demotest;				 //##0## bit map,only for test,see user_app_init
	char  down_data;                 //##1##&& 0,will stop T3324,and into PSM immediately;1,will process normally
	char  off_debug;                 //##0##&& open 1 when product testing,assert will soft reset
	
	char  save_cloud;                //##0## save cloud state info into flash or not,for power saving
	char  keep_cloud_alive;          //##0## Used to control whether the cloud is always online. If receiving downlink packet when DRX/eDRX, must set 1
	char  fota_period;               //##0##days,Detect the cycle of upgrade events,always used for terminal UE triggered upgrade,such as HTTP(abup);
	char  fota_mode;				 //##0##&& fota mode, 0:enable; 1:disable

    char  watchdog_enable;          //##1##&& enable hard watchdog or not. 0: software assert will not reset; 1: software assert will reset by watchdog
	char  hard_dog_mode;            //##0##&& unused. if 0,The system automatically feeds watchdog;1 The system will not automatically feed the dog.
	uint8_t  hard_dog_time;            //##0##&& unused.  minutes,set watchdog reload value.if hard_dog_mode = 1,will reset SoC when timeout 
	uint8_t  user_dog_time;            //##0##&& minutes,set soft timer when system poweron,and will call user_dog_timeout_hook when timeout, to avoid work too long

	
    char  cdp_register_mode;        //##0## cdp regist mode;0:manual mode by AT req;1:automatic mode;2:disable 24
	char  g_NITZ;                   //##1##&& Current Date and Time get mode;1,update by 3GPP;0,update by user AT+CCLK=<yy/MM/dd,hh:mm:ss>[<+-zz>]
	char  open_log;    		        //##1##&& log output switch;0,non log;1,all core log;2,only M3 core log;3,only M3 user log
	char  sim_vcc_ctrl;             //##2## bit 0-2(0,always 3V;1,always 1.8V;2,first 1.8V;3,first 3V) bit 3(retry interval between 3v and 1v8,0:20s,1:1s) bit 4-6(current result,2 is 3V;4 is 1.8V)
	
	char  padding22;                 
	char  high_freq_data;           //##0## Highly volatile user data(g_user_vol_data),period < 1 hour,so need save in retension memory
	char  g_drop_up_dbg;            //##0##  will delete,drop uplink packet for test
	char  g_drop_down_dbg;          //##0##  will delete,drop downlink packet for test

	char  test;                     //##0##bit 0,xy_zalloc too big will assert;
	char  padding2[3];

	char  close_urc;                //##0## when power test,can set 1
	char  ring_enable;              //##0## Enable/Disable RI indication, default 0.
	char  padding12;                //##0## 
	char  padding13;				//##0## 
	
	char  close_rai;                //##0## Whether to send RAI automatically.if 0,will send by AT+WORKLOCK=0;if 1,user can call AT+XYRAI to send
	char  open_rtc_urc;             //##0## if 1,will send URC when USER RTC wakeup
	char  min_mah;                  //##0## *10 mAh. min mAh required for normal operation,such as FOTA;if current capacity lower than this val*10,will give up FOTA
	char  min_mVbat;                //##0## *100 mv. min mv required for normal operation,such as FOTA;if current voltage lower than  this val*100,will give up FOTA
	
	uint16_t    backup_threshold; //##0## second,backup memory wirte back to flash time threshold,if 0,not write back 44
	char  open_tau_urc;				//##0## if 1,will send URC when TAU wakeup
	char  padding14;                //##0## 
	
	char  padding15[2];             //##0## 
	char  fota_res;					//##0## fota update res;0,init; 1,success;2,fail
	char  padding16;                //##0##

	char  padding17;                //##0##
	char  padding7;
	uint16_t  uart_rate; 		   //##4##&& low 9 bits:AT uart rate ,baudrate is uart_rate*2400;high 7 bits:uartrate got by autobaud
	
	uint8_t dump_mem_into_flash;   //##0##&& 0: close     1:open  dump mem into flash 56
	uint8_t  aon_uvlo_en;          //##0##&& Deepsleep 1uA:0 ;  Deepsleep75nA:1	, power down need discharge.
	uint8_t hclk_div;              //##10## Can be 4, 5, 6, 7, 8, 10, 12, 16
	uint8_t pclk_div;              //##1##  Can be 1, 2, 3, 4
	
	char  deepsleep_threshold;      //##0## seconds,for test drx/eDRX into deepsleep
	char  padding23;
	char  xtal_32k;                 //##0## 0,use 32K xtalcclock;1,use RC clock,and must close deepslee/standby/wfi
	char  ps_utcwkup_dis;		//0,allow ps utc wakeup;1,disable utc wakeup in deepsleep mode,only when use rc32k

	uint32_t  rc32k_freq;		   //##0## 0,10 *rc32k frequence

	char  user_log_size;			//##0##&& USER log limit;0,5K;other,1024 * N
	char  close_at_uart;            //##0##  unused,AT uart data mode. if 0,used for AT;if 1,used for user's OPENCPU   
	char  padding8;                 //##0##&& 
	char  resetctl;                 //##0##&& 0,RESET PIN 20MS;1,RESET PIN 6S

	char  log_size;					//##0##&& used by DSP log;0,20K;other,1024 * N
	char  cmee_mode;				//##1##  0:\r\nERROR\r\n  1:\r\nCME ERROR:ERROR_CODE\r\n  2:\r\nCME ERROR:ERROR_STRING\r\n
	char  dsp_ext_pool;             //##20##&& DPS external POOL,1024 * N
	uint8_t  binding_mode;          //##0## bindingmode of lwm2m 1:UDP mode  2:UDP queue mode
	
	uint32_t cdp_lifetime;          //##0## OceanConnect IoT lifetime: 0-30*86400
	char  padding4[8];


	uint32_t dm_reg_time;          //##2592000## unicom dm register overdue time,default 2592000 sec(30 days)
	
 	uint16_t peri_remap_enable;    //##1931##&& default: 0x078B		for each bit,0:disable 1:enable    92
 									//bit0:SWD(M3)	bit1:CSP2(AT UART); bit2:UART(USER UART); bit3:CSP3(LOG UART); 
									//	 bit4:I2C1 		bit5:SPI; 		 bit6:TMR1(PWM); 	  bit7:JTAG(DSP); 	 
									//   bit8:LED;   bit9:RF switch      bit10:SIM
	uint8_t swd_swclk_pin;          //##2##&&  ARM swd for JLINK
	uint8_t swd_swdio_pin;          //##3##&&  ARM swd for JLINK
	
	uint8_t csp2_txd_pin;           //##15##&&  used for AT        96
	uint8_t csp2_rxd_pin;           //##16##&&  used for AT 
	uint8_t uart_txd_pin;           //##255##&&
	uint8_t uart_rxd_pin;           //##255##&&
	
	uint8_t csp3_txd_pin;          //##14##&&   used for log      100
	uint8_t csp3_rxd_pin;          //##17##&&   used for log 
	uint8_t i2c1_sclk_pin;         //##255##&&
	uint8_t i2c1_sda_pin;          //##255##&&

	uint8_t spi_sclk_pin;         //##255##&&                    104
	uint8_t spi_mosi_pin;         //##255##&&
	uint8_t spi_miso_pin;         //##255##&&
	uint8_t spi_ss_n_pin;         //##255##&&

	uint8_t tmr1_pwm_outp_pin;    //##255##&&               108
	uint8_t tmr1_pwm_outn_pin;    //##255##&&
	uint8_t jtag_jtdi_pin;        //##18##&&  dsp JTAG
	uint8_t jtag_jtms_pin;        //##19##&&  dsp JTAG
	
	uint8_t jtag_jtdo_pin;       //##20##&&  dsp JTAG                   112
	uint8_t jtag_jtck_pin;       //##21##&&  dsp JTAG
	uint8_t led_pin;             //##5##&&  used for debug DSP,0XFF is close LED debug

	uint8_t rf_switch_v0_pin;    //##0## 
	uint8_t rf_switch_v1_pin;    //##1## 
	uint8_t rf_switch_v2_pin;    //##4## 

	uint8_t sim_clk_pin;        //##22##&&
	uint8_t sim_rst_pin;        //##23##&&
	uint8_t sim_data_pin;       //##24##&&  
	char  padding9[3];

	uint32_t dm_inteval_time;   //##0## cmcc dm register overdue time,default 1440 minutes(24 hours)    116


	uint8_t pmu_ioldo_sel;      //##0##
	uint8_t cdp_dtls_switch;//##0## cdp cloud sever dtls security switch:0,close;1,open
	uint8_t cdp_dtls_nat_type;//##0## cdp cloud sever dtls nat type switch:0,enable,30/s;1,disable
	uint8_t cdp_dtls_max_timeout;//##0## cdp cloud sever dtls max timeout
	
    uint8_t need_start_dm;     //##0## will delete,switch DM,0:close all dm,1:open all dm,and according to operator getted from PS,choose correct dm  132
    uint8_t dm_retry_num;      //##5## if fail,retry dm count,default 3
    uint16_t dm_retry_time;    //##3600## if fail,retry dm interval time,default 3600 sec
    
    uint16_t cloud_server_port;  //##0## cloud server port;cdp and onenet share  136
    uint8_t regver;             //##2## dm register version
    uint8_t onenet_config_len;  //##0## the length of onenet default config hex

	uint8_t cloud_server_auth[16]; //##""##&& cloud server authcode;or cdp dtls psk                                                     140
    uint8_t product_ver[28];    //##""##&& product version,tail char must is '\0'                                                          156
    uint8_t modul_ver[20];     //##"XYS-XYM110"##&& module version,tail char must is '\0'                                                  184
    uint8_t hardver[20];      //##"XYM110_HW_V1.0"##&& hardware version,tail char must is '\0'                                             204
    uint8_t versionExt[28];     //##"XYM110_SW_V0.3.7"##&& external version,tail char must is '\0'                                       224
    uint8_t ref_dns[32];       //##"114.114.114.114"##&& refer DNS,such as "114.114.114.114,119.29.29.29",tail char must is '\0'           252
    uint8_t cloud_server_ip[16];  //##""##&& cloud server ip;cdp and onenet share                                                         284
    uint8_t onenet_config_hex[120]; //##""##&& onenet default config hex                                                                 300
    uint8_t dm_app_key[64];   //##"M100000329"## dm appkey                                                                                 420                                                   
    uint8_t dm_app_pwd[64];   //##"43648As94o1K8Otya74T2719D51cmy58"## dm password for dm appkey                                           484
	
    uint8_t padding19;                                                                                                                                                   
    uint8_t tx_low_power_flag;//##0##&&
    uint8_t tx_multi_tone_power_flag;	//##1##&& RF Tx 15kHz@12Tone reduce 1dB 
    char  padding1;  

    uint8_t rf_switch_mode_txl;//##5##&& GPIO4:1 GPIO1:0 GPIO0:1                      552
    uint8_t rf_switch_mode_rxl;//##7##&& GPIO4:1 GPIO1:1 GPIO0:1
    uint8_t rf_switch_mode_txh;//##6##&& GPIO4:1 GPIO1:1 GPIO0:0
    uint8_t rf_switch_mode_rxh;//##4##&& GPIO4:1 GPIO1:0 GPIO0:0

	uint8_t rfWaittingTime;//##122##&&                                                       556
	uint8_t xtal_type;//##0##&& TCXO:0   XO:1	
	uint8_t rfInsertData;//##5##
	uint8_t dm_serv_flag;

	uint32_t  xtal_freq;//##38400000##&&                                               560
	uint32_t  bb_threshold_1Tone;//##18000##&&
	uint32_t  bb_threshold_3Tone;//##18000##&&
	uint32_t  bb_threshold_6Tone;//##18000##&&
	uint32_t  bb_threshold_12Tone;//##18000##&&

	//only used for USER!!!XinYi can not add new fac param after here!!!
	uint8_t  user_reserved[92];		//##0## reserverd for user,xinyi can not use here

}softap_fac_nv_t;

//use  2332 bytes,all is 4080 bytes
typedef struct {
	T_PsNvInfo    tNvData; /*1536 Byte,must cache aligned*/
	rf_dciq_t     rf_dciq_nv;/*128 Byte,must cache aligned*/
	softap_fac_nv_t  softap_fac_nv;   //668 BYTES,begin from 0X680,platform factory nv
	
}factory_nv_t;

typedef struct
{
	T_ImeiInfo			tImei;
	unsigned char	 	ucSnLen;
	unsigned char       padding[3];
	unsigned char	 	aucSN[NVM_MAX_SN_LEN]; 
	unsigned int 		chksum;
	//unsigned int 		rfITflag;
}T_PsImeiSnInfo;

//extern T_PsImeiSnInfo 	gPsImeiSnInfo;
extern softap_fac_nv_t *g_softap_fac_nv;

#define POWER_CNT 68

typedef struct
{
	unsigned int txCaliFreq;
	char	txCaliTempera;
	char	txCaliVolt;	
	char 	reserved[2];
	short	bandPower[POWER_CNT];	
}rf_tx_calibration_t;

typedef struct
{
	unsigned int txFlatFreq;
	char	txCaliTempera;
	char	txCaliVolt;	
	char 	reserved[2];
	short	txFlatLPower;     	
	short	txFlatMPower;		
	short	txFlatHPower;		
	short   padding;
}rf_tx_flatten_t;

typedef struct
{
	unsigned int 	version;
	unsigned int 	nvFlag0;
	int 			freqErr;
	rf_tx_calibration_t txCaliLB;
	rf_tx_calibration_t txCaliHB;
	rf_tx_flatten_t		txFlatLB[12];   
	rf_tx_flatten_t		txFlatHB[12];	
	unsigned short 	txFactorLB;
	unsigned short 	txFactorHB;
	unsigned short 	txFactorLT;
	unsigned short 	txFactorHT;
	unsigned short 	txFactorVolt[8];
	unsigned int 	reserved[4];
	unsigned int 	nvFlag1;
	unsigned int	tSensorValue;
}rf_mt_nv_t;

typedef enum
{
	RF_CALI_NV = 0,
	RF_PSIMEI_SN  =0x400,
	RF_GOLDMACHINE_NV = 0x800,
	RF_STATE_RFIT = 0xA00,
	RF_UPDATE =0xFFF,
}rf_info_type;

typedef struct
{
	unsigned int msg_len;
	unsigned int msg_addr;
}rf_info_t;

#endif

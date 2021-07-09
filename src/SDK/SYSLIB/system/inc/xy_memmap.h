/** 
* @file     	xy_memmap.h
* @brief     	flash和RAM的内存分布
* @attention  	用户可用的flash空间仅能从USER_FLASH_BASE开始，大小为USER_FLASH_LEN_MAX
*
*/

#pragma once
#include "softap_macro.h"


/*****************************************D R A M ***********************************************
						DRAM,only uesd for DSP
						 _ _ _ _ _ _ ____________0x3ffc0000
                	    |			|
						|	0x13800 |DSP heap1
						|_ _ _ _ _ _|____________0x3ffd3800
				   256K	|			|
						|	0x2C800 |DSP rodata/data/bss       remain 52000
			 			|_ _ _ _ _ _|____________0x40000000

*****************************************D R A M ***********************************************/
#define  DSP_HEAP_DRAM_START					0x3ffc0000UL        //M3 can not use
#define  DSP_HEAP_DRAM_LEN						0x13800             //M3 can not use


#define  DSP_DATA_DRAM_START					0x3ffd3800UL        //M3 can not use
#define  DSP_DATA_DRAM_LEN						0x2c800             //M3 can not use


/*****************************************I R A M ***********************************************
							IRAM,only uesd for DSP
						_ _ _ _ _ _	_____________0x40000000
						|			|
				   256K |	0x40000 |DSP  text     remain 2500
						|_ _ _ _ _ _|____________0x40040000

*****************************************I R A M ***********************************************/


/*******************************R e t e n s i o n   M e m o r y ********************************
						retension memory(DEEP SLEEP can keep power on)
						 _ _ _ _ _ _ ____________0x24000000
						|			|
						|	0x1000	| back up mem(DSP volatile NV+g_softap_var_nv)  
						|_ _ _ _ _ _|____________0x24001000

*******************************R e t e n s i o n   M e m o r y ********************************/


/*****************************************S R A M ***********************************************
						SRAM,shared by DSP and M3
				    _ _ __ _ _ _ _ _ ____________0x20000000
						|			|
						|	0xC000  | DSP data/text/bss    remian 6000
				     	|_ _ _ _ _ _|____________0x2000C000    
						|			|                          
						|	0xDF00	| ARM heap2
				 _ _ __ |_ _ _ _ _ _|____________0x20019F00
				        |			|
						|	0x1000	| InVAR NV ram
						|_ _ _ _ _ _|____________0x2001AF00    
						|			|
						|	0x1000	| factory NV ram
						|_ _ _ _ _ _|____________0x2001BF00
						|			|
						|	0xE00	| SHM RAM(ipc memory)
						|_ _ _ _ _ _|____________0x2001CD00
						|			|
						|	0x300	| SHM RAM(state shark)
				 _ _ __ |_ _ _ _ _ _|____________0x2001D000 
				    |   |			|
					|	|			| ARM  text            total 327K remain 188K
				   268K	|	0x43000	| ARM  heap  
				 _ _|__ |_ _ _ _ _ _|____________0x20060000 
			
*****************************************S R A M ***********************************************/
#if IS_DSP_CORE
#define  SRAM_OFFSET_BASE                   (0x30000000)
#else
#define  SRAM_OFFSET_BASE                   (0)
#endif
#define  SRAM_BASE					        (0x20000000+SRAM_OFFSET_BASE)    //0x20000000

#define  SRAM_LENGTH			    		(0x60000)

#define  SRAM_M3_DSP_OFFSET			        0x30000000

#define  DSP_DATA_SRAM_START				(0x20000000+SRAM_OFFSET_BASE)     //0x20000000    
#define  DSP_DATA_SRAM_LEN					0xC000  

#define  ARM_RAM_HEAP2_BASE                 (0x2000C000+SRAM_OFFSET_BASE)  //0x2000c000  
#define  ARM_RAM_HEAP2_LEN                  0xDF00

#define  RAM_INVAR_NV_BASE             	    (0x20019F00+SRAM_OFFSET_BASE)         //0x20019F00,only used for DSP core,==g_invar_nv
#define  RAM_INVAR_NV_MAXLEN				0x1000          

#define  RAM_FACTORY_NV_BASE             	(0x2001AF00+SRAM_OFFSET_BASE)         //0x2001AF00,only used for DSP core
#define  RAM_FACTORY_NV_MAXLEN				0x1000          

#define  M3_SRAM_BASE					    (0x2001D000+SRAM_OFFSET_BASE)        //text+heap1

#define  M3_SRAM_END						(0x20060000+SRAM_OFFSET_BASE)

/*****IPC  SHM  RAM*****/
#define  RAM_ICM_BUF_START                  (0x2001BF00+SRAM_OFFSET_BASE)         //0x2001BF00
#define  RAM_ICM_BUF_LEN				    0xe00                                  //0x900 used

#define  RAM_XTEND_SBUF_START			    (RAM_ICM_BUF_START+RAM_ICM_BUF_LEN)    //0x2001CD00, dedicated to shark with other core
#define  RAM_XTEND_SBUF_LEN				    0x300

//must cache line align,32BYTES
#define  PS_START_SYNC					    (RAM_XTEND_SBUF_START)  				//0x2001CD00,indicate DSP work mode(campon/attach),only linx/yus can vary
#define  SYS_UP_REASON			    	    (RAM_XTEND_SBUF_START+0x20)				//0x2001CD20,system on reason,see SYS_START_STAT
#define  SOFT_RESET_REASON			    	(RAM_XTEND_SBUF_START+0x24)             //0x2001CD24,soft reset reason,see RB_TYPE_E

#define  AT_UART_BAUD			    	    (RAM_XTEND_SBUF_START+0x40)

#define  DSP_SUSPEND			    	    (RAM_XTEND_SBUF_START+0x60)				//0x2001CD60,not  used
#define  DSP_WATCHDOG_UPDATE			   	(RAM_XTEND_SBUF_START+0x80)				//0x2001CD80,DSP set this value++,ARM read this to confirm whether DSP is dump

#define  ARM_STANDBY_ENTER_XIP				(RAM_XTEND_SBUF_START+0xa0)				//0x2001CDA0, ARM standby wakeup enter XIP mode

#define  AP_ALARM_VALUE						(RAM_XTEND_SBUF_START+0xc0)			    //0x2001CDC0 AP ALARM CNT,UINT64 type
#define  CP_ALARM_VALUE						(RAM_XTEND_SBUF_START+0xe0)			    //0x2001CDE0 CP ALARM CNT,UINT64 type

#define  ARM_DSP_MSG_STATE				    (RAM_XTEND_SBUF_START+0x100)			//0x2001CE00,AP write,DSP read,dynamic write flash,to shark with other core 
#define  DSP_ARM_MSG_STATE				    (RAM_XTEND_SBUF_START+0x120)			//0x2001CE20,DSP write,AP read,dynamic write flash,to shark with other core 

#define  PUBLIC_KEY                         (RAM_XTEND_SBUF_START+0x140)            //0x2001CE40,sec boot read form OTP,user key

#define  FREELOCK_FLAG                      (RAM_XTEND_SBUF_START+0x160)            //0x2001CE60

#define  OTPINFO_RAM_ADDR                   (RAM_XTEND_SBUF_START+0x180)            //0x2001CE80,include ADC,Calibartion,FlashUID
#define  OTPINFO_RAM_LEN                    (92)      

#define  CLOCK_DIV_VALUE					(RAM_XTEND_SBUF_START+0x1e0)            //0x2001CEE0,int bit0-7:HCLK_DIV,bit8-15:PCLK_DIV

#define  AP_LPM_STATE			    	    (RAM_XTEND_SBUF_START+0x200)            //0x2001CF00
#define  DSP_LPM_STATE			    	    (RAM_XTEND_SBUF_START+0x220)	        //0x2001CF20,DSP core lpm state,only linx/yus can vary
#define  DSP_LPM_MSG			    	    (RAM_XTEND_SBUF_START+0x240)	        //0x2001CF40
#define  FLASH_DYN_SYNC					    (RAM_XTEND_SBUF_START+0x260)            //0x2001CF60 flash dynamic write or erase sync.bit 0,if 0,do dynamic write;bit 1,if 1,DSP proxy task have run
#define  DUMP_FLASH_SYNC					(RAM_XTEND_SBUF_START+0x280)
#define  FLASH_NOTICE_FLAG                  (RAM_XTEND_SBUF_START+0x2A0)
#define  DSP_EXT_POOL                       (RAM_XTEND_SBUF_START+0x2C0)            //0x2001CFC0 DSP external  POOL and log mem
/************************END  0x2001D000***********************/


#if IS_DSP_CORE
#define  AES_START							(0x80020000)
#else
#define  AES_START							(0x26000000)
#endif

#define  AES_LENGTH							(0x20000)
#define  AES_OFFSET							(0x5A020000)

/**********************************************************************************************
				Bak	Memory	  RAM 		  config
**********************************************************************************************/
#if IS_DSP_CORE
#define  BACKUP_MEM_BASE				    0x50100000UL
#else
#define  BACKUP_MEM_BASE				    0x24000000UL
#endif

#define  BACKUP_MEM_OFFSET                  0x2C100000          //usable size is XY_FTL_AVAILABLE_SIZE


#define  BACKUP_MEM_CHECKSUM				(BACKUP_MEM_BASE+EFTL_CSM_OFFSET)  //0x24000FF0
#define  BACKUP_MEM_FLAG					(BACKUP_MEM_CHECKSUM+4)    //0x24000FF4,retension memory state flag
#define  BACKUP_MEM_RESET_FLAG				(BACKUP_MEM_CHECKSUM+8)    //0x24000FF8,softreset  flag,and do NV restore
#define  BACKUP_MEM_RF_MODE 				(BACKUP_MEM_CHECKSUM+12)   //0x24000FFC, RF工作模式 0为正常模式， 1为校准模式

#define  RAM_NV_VOLATILE_BASE				BACKUP_MEM_BASE        //0x24000000,read and write by M3,not DSP
#define  RAM_NV_VOLATILE_PS_START			RAM_NV_VOLATILE_BASE   //0x24000000
#define  RAM_NV_VOLATILE_PS_LEN				(1024*3)
#define  RAM_NV_VOLATILE_PHY_START		    (RAM_NV_VOLATILE_PS_START+RAM_NV_VOLATILE_PS_LEN)    //0x24000c00
#define  RAM_NV_VOLATILE_PHY_LEN			608
#define  RAM_NV_VOLATILE_LPM_START		    (RAM_NV_VOLATILE_PHY_START+RAM_NV_VOLATILE_PHY_LEN)   //0x24000E60
#define  RAM_NV_VOLATILE_LPM_LEN			64

#define  DSP_VOLATILE_ALL_LEN               (RAM_NV_VOLATILE_PS_LEN+RAM_NV_VOLATILE_PHY_LEN+RAM_NV_VOLATILE_LPM_LEN)  //0xea0

#define  RAM_NV_VOLATILE_SOFTAP_START		(RAM_NV_VOLATILE_LPM_START+RAM_NV_VOLATILE_LPM_LEN)   //0x24000EA0,see g_softap_var_nv    
#define  RAM_NV_VOLATILE_SOFTAP_LEN			96               

#define  RAM_NV_VOLATILE_RTCLIST_START		(RAM_NV_VOLATILE_SOFTAP_START+RAM_NV_VOLATILE_SOFTAP_LEN)   //0x24000F00,see  rtclist_var_nv_t
#define  RAM_NV_VOLATILE_RTCLIST_LEN		236              

/**********************************************************************************************
			XY	FLASH  config
			 _ _ _ _ _ _ ____________0x2700_0000
			|			|
			|	 0x2000 |sec bootloader header info and backup
			|_ _ _ _ _ _|____________0x2700_2000
			|			|
			|	 0x1000 |image info                     //版本的分区信息，二级boot加载大版本时读取
			|_ _ _ _ _ _|____________0x2700_3000
			|			|
			|	 0x1000 |main factory NV                 //原始版本的出厂NV，客户可选择是否重新擦写,但永远不能修改
			|_ _ _ _ _ _|____________0x2700_4000
			|			|
			|	 0x2000 |working factory NV              //工作态的出厂NV，一旦原始出厂NV重新擦写，该区域必须擦除；否则不得擦除
			|_ _ _ _ _ _|____________0x2700_6000
			|			|
			|	 0x3000 |DSP volatile NV+g_softap_var_nv=back memory    //易变NV，升级或烧录时擦除
			|_ _ _ _ _ _|____________0x2700_9000
			|			|
			|	 0x2000 |  DSP NON-volatile NV                  //非易变NV升级或烧录时擦除
			|_ _ _ _ _ _|____________0x2700_B000
			|			|
			|	0x2000	|  FOTA cloud_info and result           //FOTA云信息和升级结果保存区域
			|_ _ _ _ _ _|____________0x2700_D000
			|			|
			|	0x2000	|  RF calibration NV                    //射频校准NV，升级时不擦除；烧录时根据工具配置是否重新擦写
			|_ _ _ _ _ _|____________0x2700_F000
			|			|
			|	0x3000	|  yun NET NV for deepsleep             //升级或烧录时擦除
			|_ _ _ _ _ _|____________0x2701_2000
			|			|
			|	0xC000	|  secondary bootloader prime
			|_ _ _ _ _ _|____________0x2701_E000
			|			|
			|	0xC000	|  secondary bootloader backup
			|_ _ _ _ _ _|____________0x2702_A000
			|			|
			|	0x85000 | 	DSP flash code          remain 8080
			|_ _ _ _ _ _|____________0x270A_F000
			|			|
			|	0x24000 |	DSP DRAM  text/data     remain 17472
			|_ _ _ _ _ _|____________0x270D_3000
			|			|
			|	0x40000 |	DSP IRAM  text/data     remain 2448
			|_ _ _ _ _ _|____________0x2711_3000
			|			|
			|	0xC000  |	DSP SRAM  text/data     remain 5576
			|_ _ _ _ _ _|____________0x2711_F000
			|			|
			|  0xCD000  |   ARM(text/rodata/FOTA packet)   //FOTA差分包保存的首地址决定于ARM版本大小；可用AT+NUESTATS=ALLMEM查询剩余大小
			|_ _ _ _ _ _|____________0x271E_C000
			|			|
			|	0x10000 |  FOTA back up                   //二级boot进行差分升级时用的备份区，以支持断电续升
			|_ _ _ _ _ _|____________0x271F_C000
			|			|
			|	0x4000  |  user NV                        //该区域供用户保存私有数据，FOTA升级时不擦除，不升级
			|_ _ _ _ _ _|____________0x2720_0000

**********************************************************************************************/

#if IS_DSP_CORE
#define  FLASH_OFFSET_BASE                      (0x2A000000)
#else
#define  FLASH_OFFSET_BASE                      (0)
#endif

#define  FLASH_BASE								(0x27000000+FLASH_OFFSET_BASE)
#define  FLASH_M3_DSP_OFFSET                    0x2A000000
#define  FLASH_LENGTH							(0x200000)

#define  SEC_BOOT_HEADER_BASE					(0x27000000+FLASH_OFFSET_BASE)

#define  OTPINFO_FLASH_ADDR                     (0x27000080+FLASH_OFFSET_BASE)            //flash,include ADC,Calibartion,FlashUID

#define  SEC_BOOT_HEADER_LEN			        0x2000	 

#define	 PARTITION_INFO_FLASH_BASE		        (0x27002000+FLASH_OFFSET_BASE)	          
#define  PARTITION_INFO_LEN		 	            0x1000 

#define  NV_FLASH_MAIN_FACTORY_BASE      	    (0x27003000+FLASH_OFFSET_BASE)
#define  NV_FLASH_FACTORY_BASE           	    (0x27004000+FLASH_OFFSET_BASE)  //back-up factory memory,reset or fota need erase,write by DSP
#define  NV_FLASH_FACTORY_LEN			        0x2000		


#define	 NV_FLASH_DSP_VOLATILE_BASE		 		(0x27006000+FLASH_OFFSET_BASE)	          
#define  NV_FLASH_DSP_VOLATILE_LEN		 	    0x3000    

#define	 NV_FLASH_DSP_NON_VOLATILE_BASE		    (0x27009000+FLASH_OFFSET_BASE)	 	 
#define  NV_FLASH_DSP_NON_VOLATILE_LEN		    0x2000   

#define  FOTA_STATEINFO_FLASH_BASE              (0x2700B000+FLASH_OFFSET_BASE)   //保存CDP等FOTA状态信息
#define  FOTA_STATEINFO_FLASH_MAXLEN            0x1000                    

#define  FOTA_RESULT_FLASH_BASE                 (FOTA_STATEINFO_FLASH_BASE+FOTA_STATEINFO_FLASH_MAXLEN) //升级过程中header信息PARTITION_INFO_FLASH_BASE备份区
#define  FOTA_RESULT_FLASH_MAXLEN               0x1000                      

#define  NV_FLASH_RF_BASE                       (0x2700D000+FLASH_OFFSET_BASE)	 	  
#define  NV_FLASH_RF_LEN                        0x1000

#define  NV_FLASH_RF_BAKUP_BASE                 (0x2700E000+FLASH_OFFSET_BASE)	 	  
#define  NV_FLASH_RF_BAKUP_LEN                  0x1000

#define  NV_FLASH_NET_BASE                      (0x2700F000+FLASH_OFFSET_BASE)	 	      
#define  NV_FLASH_NET_LEN                       0x3000


/******************************芯翼自研FOTA使用的flash地址信息*****************************
*******************************************************************************************/
#define  FOTA_UPDATE_INFO_ADDR                  (0x270000D8+FLASH_OFFSET_BASE)  // save fota_flash_addr and fota flag
#define  VERSION_MIMG_SHA                       (FOTA_UPDATE_INFO_ADDR+8)  // version ming sha
#define  VERSION_MIMG_SHA_LEN                   0x14                       // 20byte


#define  ARM_FLASH_BASE_ADDR              	    (0x2711F000+FLASH_OFFSET_BASE)	      // ARM核使用的flash开始地址
#define  ARM_FLASH_BASE_LEN                     0xCD000                    			  // ARM核中text/data/fota共用的flash总长度

#define  FOTA_BACKUP_BASE                       (0x271EC000+FLASH_OFFSET_BASE)   //升级过程备份区，以实现断电续升
#define  FOTA_BACKUP_LEN_MAX                    0x10000                          // fota backup area len 64K

//The first flash address available to the user.
#define  USER_FLASH_BASE                         0x271FC000

//Maximun flash size available to users.
#define  USER_FLASH_LEN_MAX                      0x4000 ///<81,920



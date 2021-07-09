#ifndef _IPCMSG_H
#define _IPCMSG_H
#include "ipcmsg_hal.h"



#define 	IPCMSG_BUF_BASE_ADDR        RAM_ICM_BUF_START
#define 	IPCMSG_BUF_LEN				RAM_ICM_BUF_LEN



#define 	IPCMSG_MSGHEAD_FLAG 		0x5656
#define 	IPCMSG_ALIGN 				0x04



//#define TRUE 1
//#define FALSE 0


#define ALIGN_IPCMSG(size,align) (((unsigned int)size + align - 1)&(~(align - 1)))

typedef enum
{
	ICM_ETH = 0,
	ICM_LPM,  //DSP核进入深睡，通过该消息告知M3核深睡的时长等信息
	ICM_AT,   //两个核间进行AT命令交互，零拷贝机制
	ICM_FREE,  //两个核间进行内存的跨核释放
	ICM_SHMMEM_ALLOC,  //目前仅用于DSP向M3核申请堆内存，通常用于特殊调试时申请大内存
	ICM_SHMMEM_GIVE,    //目前仅用于DSP向M3核申请堆内存，通常用于特殊调试时申请大内存
	ICM_IPDATA,         //核间传递数据包，内部使用零拷贝
	ICM_SOCKPROXY,     //DSP核通过该消息实现socket功能
	ICM_SMARTCARDPROXY,  //DSP核的PS通过该消息使用M3核的SIM卡驱动
	ICM_WIRESHARKDATA,   //M3核抓包数据零拷贝传给DSP核从log口输出
	ICM_SMSDATA,         //M3核收到的短信内容通过该消息零拷贝传给DSP核
	ICM_FLASHWRITE,      //M3核通过DSP核执行写flash动作，并挂起M3核
	ICM_FLASHWRT_NOTICE,  //DSP核执行flash本地写操作之前，通过该消息通知M3核挂起，以防止退出XIP模式时M3核发生异常
	ICM_APP_SHM_MSG_REQ,  //两个核间通过该消息传递跨核请求消息，对方处理完毕后，可以通过ICM_APP_SHM_MSG_RSP消息应答对方
	ICM_APP_SHM_MSG_RSP,  //两个核间通过ICM_APP_SHM_MSG_REQ消息传递跨核请求消息，对方处理完毕后，可以通过该消息应答对方，以实现同步跨核API接口syn_send_shmmsg_req
	ICM_AP_LOG_PRINT,     //M3向DSP输出log数据，零拷贝机制
	ICM_MAX,

}ICM_flag;

typedef enum
{
	IpcMsg_LOG = 0,
	IpcMsg_Normal,//see  ICM_flag
	IpcMsg_Dump,
	IpcMsg_Flash, //for FLASH WRITE
	IpcMsg_Channel_MAX,
}IpcMsg_ChID;

typedef struct _T_IpcMsg_Head
{	
	unsigned short  data_len;
	unsigned short 	data_flag;
}T_IpcMsg_Head;


typedef struct _T_IpcMsg_Resource
{
	IpcMsg_ChID		Ch_ID;
	unsigned int 	block_size;
	unsigned int 	block_cnt;
	unsigned int    direction:2; //lowest order bit indicate whether exist downlink direction(DSP->M3) or not,secondary low order bit indicate whether exist uplink direction(M3->DSP) or not;1:open,0:close
	unsigned int    reserved:30;
}T_IpcMsg_Resource;

typedef struct _T_RingBuf
{
	
	unsigned int	size;
	unsigned int 	Write_Pos;	
	unsigned int 	base_addr;
	unsigned char 	align1[20];
	unsigned int 	Read_Pos;
	unsigned char 	align2[28];
	
}T_RingBuf;

typedef struct _T_RingBuf_Ctl
{	
	T_RingBuf		send_crl;
	T_RingBuf		rcv_crl;
}T_RingBuf_Ctl;


typedef struct _T_IpcMsg
{
    T_RingBuf* 		RingBuf_send;
    T_RingBuf* 		RingBuf_rcv;
    unsigned int 	flag;
	IpcMsg_MUTEX		write_mutex;
	IpcMsg_MUTEX		read_mutex;
	IpcMsg_SEMAPHORE	read_sema;
}T_IpcMsg_ChInfo;

typedef struct _T_IpcMsg_Msg
{    
    unsigned char			chID;
    unsigned int 		flag;
    void  				*buf;
    unsigned int 		len;
} T_IpcMsg_Msg;
int IpcMsg_Read(T_IpcMsg_Msg *pMsg,unsigned long xTicksToWait);
int IpcMsg_Write(T_IpcMsg_Msg *pMsg);	
int Is_Ringbuf_Empty(T_RingBuf *ringbuf);
void ipcmsg_ringbuf_read(T_RingBuf* RingBuf_rcv, void *__dest, unsigned int __data_len,unsigned int __buf_len);
int Is_Ringbuf_ChFreeSpace(T_RingBuf *ringbuf_send, unsigned int size);
void ipcmsg_ringbuf_write(T_RingBuf* RingBuf_send, void *__src, unsigned int __n);
int icpmsg_ringbuf_write_remain_size(int ch_id);

#endif


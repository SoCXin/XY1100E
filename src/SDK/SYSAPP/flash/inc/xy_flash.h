/**
* @file         xy_flash.h
* @brief        flash操作接口，含磨损均衡算法接口
* @attention    1.凡是使用FTL接口进行磨损和容错机制的，用户可使用的空间大小皆为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
* 		        2.用户根据写flash的频繁度，决定size的大小，若size=FLASH_SECTOR_LENGTH，则代表放弃磨损均衡，仅使用芯翼的头尾保护机制，以解决异常断电的flash容错。 \n
* 			    3.客户完全可以自行设计磨损和内容保护机制，可以调用xy_Flash_Read和xy_Flash_Write接口。 \n
* 			    4.由于实时写flash造成系统会被锁中断一定时间，当系统高负荷运转时可能达到秒级别锁中断，进而可能造成用户业务的延迟。 \n
*               5.芯翼提供了一种后台写flash的操作，在设备进入深睡或软重启时，通过回调函数user_hook_deepsleep_before执行用户flash动作。\n
*               6.芯片支持工作期间实时写flash，但是由于执行写flash操作时，会退出XIP模式，进而不能运行flash上的代码。所以要求所有的中断服务程序中不得调用flash上的代码。 \n
*               7.针对6中的情况，可以用__attribute__((section(".ramtext")))方式把某些函数放在RAM上。 \n
*               8.平台提供了从USER_FLASH_BASE开始，大小为USER_FLASH_LEN_MAX字节的flash空间供用户保存数据到flash中。 \n
*               9.用户不得使用超出设计范围的flash空间，否则会造成未知错误。具体使用指导参看《芯翼XY1100平台开发指南》的“NV与flash的使用”章节。 \n
*               10.擦写等接口返回bool值，当为XY_ERR时，一般为电压过低。为了防止低电压造成的flash芯片的数据异常。
*
* @note   目前，平台使用的Flash是兆易公司生产的NOR-flash，擦写上限次数为10万次，以10年使用寿命来计算，擦写频率不能小于1小时一次。 \n
*         Flash的sector扇区单位为4K字节，擦除或写入一次耗时50毫秒左右；读取很快，小于1毫秒。
* @warning  write和erase接口不得在开机初始化时调用，此时核间通道尚未准备完毕，会造成堵死。\n
*           换言之，擦写等动作，仅建议客户在自己的用户线程中调用！
* @attention   双核运行状态下，擦写flash接口中会锁调度，直到flash操作完成。由于仅锁调度不屏蔽中断，此时中断还会得到响应， \n
***  这就需要保证中断handler及其调用的所有函数中不涉及任何的flash读写，具体限制体现为：
***     ​1.  不能执行flash上的代码（text）\n
***     2.  不能读取任何位于flash上的常量 （rodata）\n
***     3.  不能调用flash相关api  \n
​***  如果将锁调度替换成锁中断，则没有上面的几个限制，但有一定的负面影响，具体表现为： \n
​*** flash操作期间，所有中断得不到响应，无论中断触发多少次，等恢复全局中断以后都只会执行一次中断处理， \n
​*** 典型的如系统的1ms tick中断，在整个flash操作过程中，tick都不会增长，直到恢复全局中断以后才会增长一次， \n
​*** 这就导致系统维护的tick定时和真实时间产生偏离，软件定时器就会受此影响导致真实响应时间晚于预期的时间，其时间差即为flash操作耗时。 \n
​*** 其他依赖中断次数的场景皆有类似问题。
*/

#pragma once
#include <stdint.h>


/**
 * @brief  Size of each sector
 */
#define  FLASH_SECTOR_LENGTH                       EFTL_PAGE_SIZE         //0x1000	4096Byte


/**
 * @brief  使用芯翼的FTL磨损算法机制，用户可用的有效空间，需要去除头尾保护字段
 */
#define  XY_FTL_AVAILABLE_SIZE                   (FLASH_SECTOR_LENGTH-4-sizeof(struct eftl_page_tag)) // 0x1000-4-16

/**
 * @brief  供用户高频率使用的RAM长度，仅在user_task_volatile_demo.c中使用，其它地方不得使用。
 * @attention   如果使用，必须联系芯翼的FAE确认。
 */
#define  USER_VOL_DATA_LEN                DSP_VOLATILE_ALL_LEN  ///<3744 bytes, if need to use, must contact our FAE.

/**
 * @brief  少数客户有高频率写小数据到flash中的需求，如每两秒写4字节到flash中。由于flash寿命只有10万次，这样很容易造成坏块。 \n
 * 芯翼平台复用4K retension mem来解决高频率写flash问题，用户仅需把私有小数据保存到该全局中即可，芯翼平台后台会保证不丢失数据。具体请参考user_task_volatile_demo.c
 * @attention   如果使用，必须联系芯翼的FAE确认。 \n
 *  用户能保存的长度上限为USER_VOL_DATA_LEN，否则会造成系统各种异常。
*/
extern  void *g_user_vol_data;

/**
 * @brief  双核模式时本地写标志。 \n

*/
extern  uint8_t g_have_suspend_dsp;


#define EFTL_PAGE_SIZE         	(0x1000)
#define PAGE_AVAILABLE_SIZE     XY_FTL_AVAILABLE_SIZE  //0XFEC,4076 bytes
#define EFTL_CSM_OFFSET         (EFTL_PAGE_SIZE-sizeof(struct eftl_page_tag))  //0XFF0
#define EFTL_WRITE_NUM_OFFSET   (EFTL_PAGE_SIZE-sizeof(struct eftl_page_tag)+4)  //0XFF4,if erase,will is 0/FF all


#define  IS_CACHE_ALIGNED(a)        ((((uint32_t)a)&(CACHE_LINE-1))==0)
#define  IS_FLASH_ALIGNED(a)    ((((uint32_t)a)&(EFTL_PAGE_SIZE-1))==0)

struct eftl_page_tag {
	uint32_t checksum;
	uint32_t write_num; //nv/user write frequency for bad page,offset is 0XFCC
	char padding[8];    //unused
};

struct eftl_cfg {
	uint32_t addr;//flash start addr
	uint32_t tpage_total;//true pages,such as 5
};




uint32_t ftl_read_write_num(uint32_t addr);


/**
 * @brief  read data from flash。一个扇区4K字节耗时183us
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to ram.
 * @param size     [IN]The length of data.
 */
void xy_flash_read(uint32_t addr, void *data, uint32_t size);

/**
 * @brief  Erase and write new data to flash. 一个扇区4K字节耗时54ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to flash.Must cache aligned
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention      由于异常断电会造成flash内容损坏，所以客户必须自行添加头尾校验位来确保内容的有效性。 
 * @warning        该接口会执行擦除和写入两个动作，对于非整个扇区写入的场景，建议客户统一先执行xy_flash_erase，再多次调用xy_flash_write_no_erase接口，以降低坏块风险！
 */
int xy_flash_write(uint32_t addr,void *data, uint32_t size);

/**
 * @brief   Write new data to flash without erase. such as  FOTA packet download。一个扇区4K字节耗时9ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]The first address of the data to be written to flash.Must cache aligned
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention      由于异常断电会造成flash内容损坏，所以客户必须自行添加头尾校验位来确保内容的有效性。 
 * @warning        使用该接口的前提是能够保证待写入的flash空间已被擦除为全FF，否则会造成脏数据。所以必须与xy_Flash_Erase接口配套使用，即先擦除一大段flash，再一小段一小段分时写入。
 */
int xy_flash_write_no_erase(uint32_t addr,void *data, uint32_t size);

/**
 * @brief   Erease block.一个扇区4K字节耗时45ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 */
int xy_flash_erase(uint32_t addr, uint32_t size);

/**
 * @brief   检查flash地址是否为FTL磨损的flash地址范围
 * 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 */
int is_ftl_flash_addr(uint32_t addr);

/**
 * @brief  磨损均衡的写接口，支持单个参数的修改，其他值不变。一个扇区4K字节耗时58ms
 * 
 * @param addr     [IN]磨损扇区的flash首地址，4K字节对齐
 * @param offset   [IN]待写入的内容偏移，常为结构体某变量在结构体中的偏移
 * @param data     [IN]The first address of the data to be written to flash.
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention   每个4K的flash区域中用户可用的空间为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *        支持用户按照偏移进行部分读写，以方便结构体具体参数的部分修改。
 */
int xy_ftl_write(uint32_t addr, int offset, uint8_t *data, uint32_t size);

/**
 * @brief  磨损均衡的读接口，支持偏移读取。一个扇区4K字节耗时1.2ms
 * 
 * @param addr     [IN]The address where the data is stored in the flash.
 * @param data     [IN]ram address
 * @param size     [IN]The length of the data. 
 * @return BOOL, see  @ref  xy_ret_Status_t.
 * @attention   每个4K的flash区域中用户可用的空间为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *    支持用户按照偏移进行部分读写，即addr&0xFFF为地址偏移值。
 */
int xy_ftl_read(uint32_t addr, uint8_t *data, uint32_t size);



/** 
 * @brief   供用户进行磨损均衡的flash注册
 *
 * @param addr   必须为4K字节对齐的，即尾部必须为0X000。
 * @param addr   必须为4K的整数倍，即必须是整的sector，一个sector为4K字节。
 * @attention    凡是使用该接口进行磨损和容错机制的，用户可使用的空间大小皆为XY_FTL_AVAILABLE_SIZE，其中头和尾部添加了保护字段。 \n
 *           用户根据写flash的频繁度，决定size的大小，若size=FLASH_SECTOR_LENGTH，则代表放弃磨损均衡，仅使用芯翼的头尾保护机制，以解决异常断电的flash容错。 \n
 *           客户完全可以自行设计磨损和内容保护机制，可以调用xy_Flash_Read和xy_Flash_Write接口。 \n
 *           目前XY1100的flash寿命为10万次，当写频率小于一小时，必须考虑磨损机制，否则无法达到10年使用期限。
 */
void xy_regist_ftl_req(uint32_t addr, uint32_t size);



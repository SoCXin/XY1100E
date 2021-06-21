
#include  "zos.h"

#ifdef BOARD_USING_UART_DEMO


osThreadId_t  hal_uart_IT_demo_TskHandle = NULL;


void uart_callback(zos_uint8_t *rev_data, zos_uint16_t len)
{
	char *str_out = NULL;
	str_out = xy_zalloc(200);
	snprintf(str_out, 200, "uart_callback revdata:%s: len:%d\n", rev_data,len);
	send_debug_str_to_at_uart(str_out);
	xy_free(str_out);
    //回写收到的数据
	zos_uart_write(ZOS_UART_BUS_1,rev_data,len);

}
/**
 * @brief	此demo使用中断方式接收指定长度数据，fifo中收到数据就触发中断开始接收。接收到指定长度数据则将数据回写至uart；
* 			如果没接收到指定长度数据，则将实际接收长度的数据回写至uart\n
 */
void hal_uart_IT_demo_task(void)
{
	ZOS_UART_CONFIG uart_cfg;
	uart_cfg.baud_rate=9600;
	uart_cfg.data_bits=ZOS_UART_DATA_BITS_8;
	uart_cfg.parity=ZOS_UART_PARITY_NONE;
	uart_cfg.stop_bits=ZOS_UART_STOP_BITS_1;
	zos_uart_init(ZOS_UART_BUS_1,&uart_cfg,uart_callback);

	while(1)
	{
		osDelay(1000);

	}
}

/**
 * @brief 任务创建
 * 
 */
void hal_uart_IT_demo_task_init(void)
{
	hal_uart_IT_demo_TskHandle = osThreadNew((osThreadFunc_t)hal_uart_IT_demo_task, NULL, "hal_uart_IT_demo_task", 1024, osPriorityNormal);
}


#endif //BOARD_USING_UART_DEMO

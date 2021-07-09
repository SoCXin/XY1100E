/**
* @file			adc_demo.c
* @ingroup		peripheral
* @brief		ADC外设Demo
* @attention	当量程设置为1V时（目前仅支持1V量程）：
*         		ADC参考电压是1V，精度12bit, 采样频率有240K和480K可选。校准后的偏差+-3mv。
*         		ADC支持双端模式，双端模式需要两个电压输入信号，双端模式的电压是差分电压。
*        		当量程设置为1V时（目前仅支持1V量程）：
*         		双端模式，P端接地，量程为-1V~0V，输入正电压读数均为负数；
*         		双端模式，N端接地，量程为0~1V；
*         		双端模式，两端接信号，量程为 -1V~1V。
*         		内部电压模式，量程为0-5V。无需接线，测量结果为芯片内部VBAT电压。
***********************************************************************************/

#if DEMO_TEST

#include "xy_api.h"

//任务参数配置
#define ADC_WORK_MODE     HAL_ADC_MODE_TYPE_VBAT
#define ADC_WORK_SPEED    HAL_ADC_SPEED_TYPE_240K

//任务全局变量
osThreadId_t  hal_adc_TskHandle = NULL;
HAL_ADC_HandleTypeDef ADC_HandleStruct;


/**
 * @brief 任务线程
 *
 */
void hal_adc_test_task()
{
	short real_val;       //从寄存器获取的真实值
	short convert_val;    //转换值
	short no_cal_vol;     //利用转换值转为未校准的电压值
	short round_val;      //利用转换值进行校准得到圆整值
	short cal_vol;        //API内部利用转换值进行校准得到圆整值，利用圆整值转换为校准后的电压值

	int average;
	
	char *str_out = NULL;
	ADC_HandleStruct.Init.Mode = ADC_WORK_MODE;
	ADC_HandleStruct.Init.Speed = ADC_WORK_SPEED;
	ADC_HandleStruct.Init.ADCRange = HAL_ADC_RANGE_TYPE_1V;
	while(1)
	{
		average = 0;

		//初始化ADC
		HAL_ADC_Init(&ADC_HandleStruct);
		//从寄存器获取真实值10次取平均值
		for(uint8_t i = 0; i < 10; i++)
		{
			real_val = HAL_ADC_GetValue(&ADC_HandleStruct);
			average += real_val;
		}
		//去初始化ADC
		HAL_ADC_DeInit(&ADC_HandleStruct);

		if(average >= 0)
			real_val = (average + 5) / 10;
		else
			real_val = (average - 5) / 10;

		//获取转换值
		convert_val = HAL_ADC_GetConverteValue(&ADC_HandleStruct, real_val);
		//利用转换值转为未校准的电压值
		no_cal_vol  = HAL_ADC_GetVoltageValue(&ADC_HandleStruct,convert_val);
		//利用转换值进行校准得到圆整值		
		round_val   = HAL_ADC_GetRoundingValue(&ADC_HandleStruct,convert_val);		
		//API内部利用转换值进行校准得到圆整值，利用圆整值转换为校准后的电压值
		cal_vol     = HAL_ADC_GetVoltageValueWithCal(&ADC_HandleStruct,convert_val);	
		
		//通过AT口打印信息，用于调试
		//if (no_cal_vol > 400)
		{
			str_out = xy_zalloc(200);
			snprintf(str_out, 200, "\r\n"
				"real_val    = %d\r\n"
				"convert_val = %d\r\n"
				"no_cal_vol  = %d\r\n"
				"round_val   = %d\r\n"
				"cal_vol     = %d\r\n",
				real_val, convert_val, no_cal_vol, round_val, cal_vol);
				send_debug_str_to_at_uart(str_out);
			xy_free(str_out);
		}
		//通过osDelay释放线程控制权
		osDelay(2000);
	}
}


/**
 * @brief 任务创建
 */
void hal_adc_test_task_init(void)
{
	hal_adc_TskHandle = osThreadNew((osThreadFunc_t)(hal_adc_test_task), NULL, "hal_adc_test_task", 512, osPriorityNormal);
}

#endif

#pragma once


extern int  g_normal_wakeup;


/**
 * @brief dynamic load dsp core when user needs to use NB stack
 * @param isDump	 when user calls this function,isDump should be 0.
 * @note  单核模式下动态加载启动dsp核，用户调用时入参为0。
 */
void dynamic_load_up_dsp(int isDump);

void set_dsp_no_work();

/**
 * @brief 配置DSP核工作模式
 * 
 */
void set_DSP_work_mode();

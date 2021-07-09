/**
 * @file xy_lib_stub.h
 * @brief 用于解决封库模块与外部模块的耦合
 * @version 1.0
 * @date 2021-02-26
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
/**
 * xdsend命令的处理，解决封库的at_ctl模块与socket模块的耦合
 */
bool do_xdsend_cmd(char *at_data);

int find_xdsend_cmd(char *at_cmd);
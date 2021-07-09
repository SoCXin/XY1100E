/*
 *   Record all at command header and global variable
 */
#pragma once

/*******************************************************************************
 *							 Include header files							   *
 ******************************************************************************/
#include "at_com.h"

/*******************************************************************************
 *                       Global variable declarations                          *
 ******************************************************************************/
//pointer to xy basic at request command,can see @at_basic_req array
extern struct at_serv_proc_e *g_at_basic_req;
//pointer to xy basic at urc command,can see @at_basic_info array
extern struct at_inform_proc_e* g_at_basic_info;

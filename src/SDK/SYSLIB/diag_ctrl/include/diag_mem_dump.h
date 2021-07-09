#ifndef _DIAG_MEMORY_DUMP_H_
#define _DIAG_MEMORY_DUMP_H_
#include "basetype.h"
#include "TypeDefine.h"

void send_mem_ready();

void set_pc_info(unsigned char ready);

//return 1 pc is ready,otherewise 0
unsigned char  query_pc_info();

void mem_dump_info(char *mem_file, unsigned int mem_addr, unsigned int mem_size);

void mem_dump_info_ram(char *mem_file, unsigned int mem_addr, unsigned int mem_size);

void mem_dump_info_flash(char *mem_file, unsigned int mem_addr, unsigned int mem_size);


#endif


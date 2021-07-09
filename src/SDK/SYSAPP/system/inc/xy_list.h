#pragma once

#include "xy_log.h"
#include "xy_nbiot_module_define.h"
#include "factory_nv.h"

typedef struct PNODE
{ 
  struct PNODE* next;
  int val; 

} PNODE_t; 

void insert_list(PNODE_t **pHead, PNODE_t* new_block);

PNODE_t* findByVal(PNODE_t *pHead, int val);

void deleteByVal(PNODE_t **pHead, int val);

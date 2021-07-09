#ifndef _XY_LIST
#define _XY_LIST

#include "xy_utils.h"
#include "xy_system.h"
#include "hw_types.h"
#include "xy_list.h"
volatile char hjmm = 0;
void insert_list(PNODE_t **pHead, PNODE_t* new_block)
{
	PNODE_t *tmp_xBlock = *pHead;

	if(*pHead == NULL)
	{
		new_block->next = NULL;
		*pHead= new_block;

	}
	else
	{			
		while(tmp_xBlock->next != NULL)
		{
			tmp_xBlock = tmp_xBlock->next;
		}
		tmp_xBlock->next = new_block;
	}
	hjmm = 1;
}

PNODE_t* findByVal(PNODE_t *pHead, int val)
{
	vTaskSuspendAll();
	while (pHead != NULL && pHead->val != val) 
	{ 
		pHead = pHead->next; 
	} 
	xTaskResumeAll();
  return pHead; 
} 


void deleteByVal(PNODE_t **pHead, int val)
{ 
  PNODE_t *t = *pHead; 
  PNODE_t *n;
  if (*pHead == NULL)
  { 
    return; 
  } 
  else
  { 
    if ((*pHead)->val == val)
    { 
		*pHead = (*pHead)->next; 
		xy_free(t); 
    } 
    else
    { 
      while (t->next != NULL && t->next->val != val)
      { 
        t = t->next; 
      } 
      if (t->next)
      { 
        n = t->next->next;
        xy_free(t->next); 
        t->next = n; 
      } 
    } 
  } 
}
#endif

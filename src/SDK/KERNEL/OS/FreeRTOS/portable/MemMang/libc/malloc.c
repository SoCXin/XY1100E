#include "xy_utils.h"

/*****************************************************************************
Function         :      free
Description      :      Deallocates the memory previously allocated by a call to calloc, malloc, or
                realloc. The argument ptr points to the space that was previously allocated.
                If ptr points to a memory block that was not allocated with calloc, malloc,
                or realloc, or is a space that has been deallocated, then the result is
                undefined.
Input            :      [1] void *ptr, pointed to the memory need to free.
Output           :      nothing.
Return           :      No value is returned.
*****************************************************************************/
void free(void *ptr)
{
    if (ptr == NULL)
        return;

    vPortFree(ptr);/*lint !e534*/
}


/*****************************************************************************
Function         :      malloc
Description      :      Allocates the requested memory and returns a pointer to it. The requested
                size is size bytes. The value of the space is indeterminate.
Input            :      [1] size_t size, spcified the size need to allocate.
Output           :      nothing.
Return           :      On success a pointer to the requested space is returned.
                On failure a null pointer is returned.
*****************************************************************************/
void *malloc(size_t size) /*lint !e31 !e10*/
{
    void *ptr = NULL; /*lint !e64 !e10*/

    if (size == 0)
        return NULL; /*lint !e64*/

    ptr = pvPortMalloc((uint32_t)size);

    return ptr;
}


void *zalloc(size_t size) /*lint !e10*/
{
    void *ptr = malloc (size);

    if (ptr != NULL)
    {
        memset((void *)ptr, (int)0, size);
    }

    return ptr;
}


/*****************************************************************************
Function         :      calloc
Description      :    Allocates the requested memory and returns a pointer to it. The requested
                size is nitems each size bytes long (total memory requested is nitems*size).
                The space is initialized to all zero bits.
Input            :      [1] size_t nitems,
                [2] size_t size,
Output           :      nothing.
Return           :      On success a pointer to the requested space is returned.
                On failure a null pointer is returned.
*****************************************************************************/
void *calloc(size_t nitems, size_t size) /*lint !e578*/
{
    return zalloc(nitems * size);
}


/*****************************************************************************
Function         :      memalign
Description      :      allocates a block of size bytes whose address is a multiple of boundary.
               The boundary must be a power of two!
Input            :      [1] size_t size, spcified the size need to allocate.
               [2] size_t boundary, the returned memory address will be a multiple of boundary.
               This argument must be a power of two. This property is not checked by
               memalign, so misuse may result in random runtime errors.
Output           :      nothing.
Return           :      On success a pointer to the requested space is returned.
               On failure a null pointer is returned.
*****************************************************************************/
//void *memalign (size_t boundary, size_t size)  /*lint !e18 !e578*/
//{
//    /*lint !e18 !e578*/
//    void *ptr = NULL;
//
//    if(size == 0)
//        return NULL; /*lint !e64*/
//
//    ptr = LOS_MemAllocAlign((void *)OS_SYS_MEM_ADDR, (UINT32)size, (UINT32)boundary);
//
//    return ptr; /*lint !e64*/
//}


/*****************************************************************************
Function         :      realloc
Description      :      Attempts to resize the memory block pointed to by ptr that was previously
                allocated with a call to malloc or calloc. The contents pointed to by ptr are
                unchanged. If the value of size is greater than the previous size of the
                block, then the additional bytes have an undeterminate value. If the value
                of size is less than the previous size of the block, then the difference of
                bytes at the end of the block are freed. If ptr is null, then it behaves like
                malloc. If ptr points to a memory block that was not allocated with calloc
                or malloc, or is a space that has been deallocated, then the result is
                undefined. If the new space cannot be allocated, then the contents pointed
                to by ptr are unchanged. If size is zero, then the memory block is completely
                freed.
Input            :      [1] void *ptr, pointed to the memory which need to remalloc.
                [2] size_t size, specified the size to remalloc.
Output           :      nothing.
Return           :      On success a pointer to the memory block is returned (which may be in a
                different location as before).
               On failure or if size is zero, a null pointer is returned.
*****************************************************************************/
void *realloc(void *ptr, size_t size)
{
    void *p = NULL;
	if (ptr == NULL)
    {
        return malloc(size); /*lint !e64*/
    }

    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    return pvPortRealloc((void *)ptr, (uint32_t)size);
}


/* EOF malloc.c */

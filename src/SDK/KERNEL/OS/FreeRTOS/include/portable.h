/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Portable layer API.  Each function must be defined for each port.
 *----------------------------------------------------------*/

#ifndef PORTABLE_H
#define PORTABLE_H

/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
pre-processor definition was used to ensure the pre-processor found the correct
portmacro.h file for the port being used.  That scheme was deprecated in favour
of setting the compiler's include path such that it found the correct
portmacro.h file - removing the need for the constant and allowing the
portmacro.h file to be located anywhere in relation to the port being used.
Purely for reasons of backward compatibility the old method is still valid, but
to make it clear that new projects should not use it, support for the port
specific constants has been moved into the deprecated_definitions.h header
file. */
#include "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
did not result in a portmacro.h header file being included - and it should be
included here.  In this case the path to the correct portmacro.h header file
must be set in the compiler's include path. */
#ifndef portENTER_CRITICAL
	#include "portmacro.h"
#endif

#if portBYTE_ALIGNMENT == 32
	#define portBYTE_ALIGNMENT_MASK ( 0x001f )
#endif

#if portBYTE_ALIGNMENT == 16
	#define portBYTE_ALIGNMENT_MASK ( 0x000f )
#endif

#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#if portBYTE_ALIGNMENT == 4
	#define portBYTE_ALIGNMENT_MASK	( 0x0003 )
#endif

#if portBYTE_ALIGNMENT == 2
	#define portBYTE_ALIGNMENT_MASK	( 0x0001 )
#endif

#if portBYTE_ALIGNMENT == 1
	#define portBYTE_ALIGNMENT_MASK	( 0x0000 )
#endif

#ifndef portBYTE_ALIGNMENT_MASK
	#error "Invalid portBYTE_ALIGNMENT definition"
#endif

#ifndef portNUM_CONFIGURABLE_REGIONS
	#define portNUM_CONFIGURABLE_REGIONS 1
#endif

#ifndef portHAS_STACK_OVERFLOW_CHECKING
	#define portHAS_STACK_OVERFLOW_CHECKING 0
#endif

#ifndef portARCH_NAME
	#define portARCH_NAME NULL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#if( portUSING_MPU_WRAPPERS == 1 )
	#if( portHAS_STACK_OVERFLOW_CHECKING == 1 )
		StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, StackType_t *pxEndOfStack, TaskFunction_t pxCode, void *pvParameters, BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
	#else
		StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters, BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
	#endif
#else
	#if( portHAS_STACK_OVERFLOW_CHECKING == 1 )
		StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, StackType_t *pxEndOfStack, TaskFunction_t pxCode, void *pvParameters ) PRIVILEGED_FUNCTION;
	#else
		StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters ) PRIVILEGED_FUNCTION;
	#endif
#endif

/* Used by heap_5.c, heap_6.c and heap_7.c to define the start address and size of each memory region
that together comprise the total FreeRTOS heap space. */
typedef struct HeapRegion
{
	uint8_t *pucStartAddress;
	size_t xSizeInBytes;
} HeapRegion_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
typedef struct xHeapStats
{
	size_t xAvailableHeapSpaceInBytes;		/* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
	size_t xSizeOfLargestFreeBlockInBytes; 	/* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
	size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
	size_t xNumberOfFreeBlocks;				/* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
	size_t xMinimumEverFreeBytesRemaining;	/* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
	size_t xNumberOfSuccessfulAllocations;	/* The number of calls to pvPortMalloc() that have returned a valid memory block. */
	size_t xNumberOfSuccessfulFrees;		/* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;

/* Used to memory allocated align of pvPortMalloc(), use for heap_6.c and heap_7.c */
typedef enum
{
	eHeapAlign_1 = 1,
	eHeapAlign_2 = 2,
	eHeapAlign_4 = 4,
	eHeapAlign_8 = 8,
	eHeapAlign_16 = 16,
	eHeapAlign_32 = 32,
	eHeapAlign_64 = 64,
	eHeapAlign_128 = 128
} eHeapAlign;

/*
 * heap regions type
 */
typedef enum
{
	LargeSize_SlowAccess = 0,
	SmallSize_SlowAccess,
	SmallSize_FastAccess,
	HEAP_REGION_MAX
} eHeapRegions;

/* Used by heap_6.c to define the allocate memory sequence */
typedef struct HeapMallocSequnce
{
	uint8_t sequnce[ HEAP_REGION_MAX ][ HEAP_REGION_MAX ];
} HeapMallocSequnce_t;

/*
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc() - not creating a task,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc being called.
 *
 * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
 * defines a region of memory that can be used as the heap.  The array is
 * terminated by a HeapRegions_t structure that has a size of 0.  The region
 * with the lowest start address must appear first in the array.
 */
#if( configMALLOC_USE_HEAP_7 == 1 )
  void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions, const HeapMallocSequnce_t * const pxMallocSequence ) PRIVILEGED_FUNCTION;
#else
  void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions ) PRIVILEGED_FUNCTION;
#endif

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
void vPortGetHeapStats( HeapStats_t *pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */

#if( configMALLOC_USE_HEAP_6 == 1 )
  #if( configUSE_HEAP_MALLOC_DEBUG == 1 )
    void *pvPortMallocWithAlignDebug( size_t xSize, eHeapAlign eAlignment, char *pcFile, int xLine ) PRIVILEGED_FUNCTION;
    #define pvPortMallocWithAlign( xSize, eAlignment ) pvPortMallocWithAlignDebug( xSize, eAlignment, __FILE__, __LINE__ )
    #define pvPortMalloc( xSize ) pvPortMallocWithAlign( xSize, eHeapAlign_8 )
    #define pvPortMallocNormal( xSize, pcFile, xLine ) pvPortMallocWithAlignDebug( xSize, eHeapAlign_4, pcFile, xLine )
    #define pvPortMallocAlign( xSize, pcFile, xLine ) pvPortMallocWithAlignDebug( xSize, eHeapAlign_32, pcFile, xLine )

    void *pvPortReallocWithAlignDebug( void *pv, size_t size, eHeapAlign eAlignment, char *pcFile, int xLine ) PRIVILEGED_FUNCTION;
    #define pvPortReallocWithAlign( pv, xSize, eAlignment ) pvPortReallocWithAlignDebug( pv, xSize, eAlignment, __FILE__, __LINE__ )
    #define pvPortRealloc( pv, xSize ) pvPortReallocWithAlign( pv, xSize, eHeapAlign_8 )
    #define pvPortReallocNormal( pv, xSize, pcFile, xLine ) pvPortReallocWithAlignDebug( pv, xSize, eHeapAlign_4, pcFile, xLine )
    #define pvPortReallocAlign( pv, xSize, pcFile, xLine ) pvPortReallocWithAlignDebug( pv, xSize, eHeapAlign_32, pcFile, xLine )
  #else
    void *pvPortMallocWithAlign( size_t xSize, eHeapAlign eAlign ) PRIVILEGED_FUNCTION;
    #define pvPortMalloc( xSize ) pvPortMallocWithAlign( xSize, eHeapAlign_8 )
    #define pvPortMallocNormal( xSize ) pvPortMallocWithAlign( xSize, eHeapAlign_4 )
    #define pvPortMallocAlign( xSize ) pvPortMallocWithAlign( xSize, eHeapAlign_32 )

    void *pvPortReallocWithAlign( void *pv, size_t xSize, eHeapAlign eAlign ) PRIVILEGED_FUNCTION;
    #define pvPortRealloc( pv, xSize ) pvPortReallocWithAlign( pv, xSize, eHeapAlign_8 )
    #define pvPortReallocNormal( pv, xSize ) pvPortReallocWithAlign( pv, xSize, eHeapAlign_4 )
    #define pvPortReallocAlign( pv, xSize ) pvPortReallocWithAlign( pv, xSize, eHeapAlign_32 )
  #endif
    void vPortFree( void *pv ) PRIVILEGED_FUNCTION;
#elif( configMALLOC_USE_HEAP_7 == 1 )
  #if( configUSE_HEAP_MALLOC_DEBUG == 1 )
    void *pvPortMallocWithAlignDebug( size_t xSize, eHeapRegions eRegion, eHeapAlign eAlignment, char *pcFile, int xLine ) PRIVILEGED_FUNCTION;
    #define pvPortMallocWithAlign( xSize, eRegion, eAlignment ) pvPortMallocWithAlignDebug( xSize, eRegion, eAlignment, __FILE__, __LINE__ )
    #define pvPortMalloc( xSize, eSelectHeap ) pvPortMallocWithAlign( xSize, eSelectHeap, eHeapAlign_8 )
    #define pvPortMallocNormal( xSize, pcFile, xLine ) pvPortMallocWithAlignDebug( xSize, LargeSize_SlowAccess, eHeapAlign_4, pcFile, xLine )
    #define pvPortMallocAlign( xSize, pcFile, xLine ) pvPortMallocWithAlignDebug( xSize, LargeSize_SlowAccess, eHeapAlign_32, pcFile, xLine )

    void *pvPortReallocWithAlignDebug( void *pv, size_t xSize, eHeapRegions eRegion, eHeapAlign eAlignment, char *pcFile, int xLine ) PRIVILEGED_FUNCTION;
    #define pvPortReallocWithAlign(pv, xSize, eRegion, eAlignment ) pvPortReallocWithAlignDebug(pv, xSize, eRegion, eAlignment, __FILE__, __LINE__ )
    #define pvPortRealloc( pv, xSize, eSelectHeap ) pvPortReallocWithAlign( pv, xSize, eSelectHeap, eHeapAlign_8 )
    #define pvPortReallocNormal( pv, xSize, pcFile, xLine ) pvPortReallocWithAlignDebug( pv, xSize, LargeSize_SlowAccess, eHeapAlign_4, pcFile, xLine )
    #define pvPortReallocAlign( pv, xSize, pcFile, xLine ) pvPortReallocWithAlignDebug( pv, xSize, LargeSize_SlowAccess, eHeapAlign_32, pcFile, xLine )
  #else
    void *pvPortMallocWithAlign( size_t xSize, eHeapRegions xSelectHeap, eHeapAlign eAlignment ) PRIVILEGED_FUNCTION;
    #define pvPortMalloc( xSize, eSelectHeap ) pvPortMallocWithAlign( xSize, eSelectHeap, eHeapAlign_8 )
    #define pvPortMallocNormal( xSize ) pvPortMallocWithAlign( xSize, LargeSize_SlowAccess, eHeapAlign_4 )
    #define pvPortMallocAlign( xSize ) pvPortMallocWithAlign( xSize, LargeSize_SlowAccess, eHeapAlign_32 )

    void *pvPortReallocWithAlign( void *pv, size_t size, eHeapRegions eRegion, eHeapAlign eAlignment ) PRIVILEGED_FUNCTION;
    #define pvPortRealloc( pv, xSize, eSelectHeap ) pvPortReallocWithAlignDebug( pv, xSize, eSelectHeap, eHeapAlign_8 )
    #define pvPortReallocNormal( pv, xSize ) pvPortReallocWithAlignDebug( pv, xSize, LargeSize_SlowAccess, eHeapAlign_4 )
    #define pvPortReallocAlign( pv, xSize ) pvPortReallocWithAlignDebug( pv, xSize, LargeSize_SlowAccess, eHeapAlign_32 )
  #endif
    void vPortFree( void *pv ) PRIVILEGED_FUNCTION;
#else
  void *pvPortMalloc( size_t xSize ) PRIVILEGED_FUNCTION;
  void vPortFree( void *pv ) PRIVILEGED_FUNCTION;
#endif

void vPortInitialiseBlocks( void ) PRIVILEGED_FUNCTION;
size_t xPortGetFreeHeapSize( void ) PRIVILEGED_FUNCTION;
size_t xPortGetMinimumEverFreeHeapSize( void ) PRIVILEGED_FUNCTION;

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
BaseType_t xPortStartScheduler( void ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
void vPortEndScheduler( void ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#if( portUSING_MPU_WRAPPERS == 1 )
	struct xMEMORY_REGION;
	void vPortStoreTaskMPUSettings( xMPU_SETTINGS *xMPUSettings, const struct xMEMORY_REGION * const xRegions, StackType_t *pxBottomOfStack, uint32_t ulStackDepth ) PRIVILEGED_FUNCTION;
#endif

#ifdef __cplusplus
}
#endif

#endif /* PORTABLE_H */


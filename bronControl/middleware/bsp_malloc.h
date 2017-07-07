#ifndef BSP_MALLOC_H_
#define BSP_MALLOC_H_
/******************************************************************************
 * (c) COPYRIGHT 2014 by NetModule AG, Switzerland.  All rights reserved.
 *
 * The program(s) may only be used and/or copied with the written permission
 * from NetModule AG or in accordance with the terms and conditions stipulated
 * in the agreement contract under which the program(s) have been supplied.
 *
 * PACKAGE : BSP
 *
 * ABSTRACT:
 *  dynamic memory implementation
 *
 * HISTORY:
 *  Date      Author       Description
 *  20140821  AHu          created
 *
 *****************************************************************************/

/*** includes ****************************************************************/
//#include <stdlib.h>
/*** defines *****************************************************************/

/*** types *******************************************************************/

/*** function prototypes *****************************************************/

/**
 * Wrapper for calloc.
 */
#ifdef Q_SPY
void* bsp_calloc(size_t _num, size_t _size);
#else
#define bsp_calloc calloc
#endif

/**
 * Wrapper for malloc.
 */
#ifdef Q_SPY
void* bsp_malloc(size_t _size);
#else
#define bsp_malloc malloc
#endif

/**
 * Wrapper for realloc.
 */
#ifdef Q_SPY
void* bsp_realloc(void *_ptr, size_t _size);
#else
#define bsp_realloc realloc
#endif

/**
 * Wrapper for free.
 */
#ifdef Q_SPY
void bsp_free(void *_ptr);
#else
#define bsp_free free
#endif

#endif /* BSP_MALLOC_H_ */

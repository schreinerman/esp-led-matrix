/**
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2022 IO-Expert.com. All rights reserved.
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this condition and the following disclaimer.
 **
 ** This software is provided by the copyright holder and contributors "AS IS"
 ** and any warranties related to this software are DISCLAIMED.
 ** The copyright owner or contributors be NOT LIABLE for any damages caused
 ** by use of this software.

 *******************************************************************************
 */

/**
 *******************************************************************************
 **\file timeouts.cpp
 **
 ** Handle Timeouts
 ** A detailed description is available at
 ** @link TimeoutsGroup file description @endlink
 **
 ** History:
 ** - 2022-12-30  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__TIMEOUTS_H__)
#define __TIMEOUTS_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup TimeoutsGroup Handle Timeouts
 **
 ** Provided functions of Timeouts:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page timeouts_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "timeouts.h"
** @endcode
**
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** (Global) Include files
 *******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>

/**
 *******************************************************************************
 ** Global pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global type definitions ('typedef') 
 *******************************************************************************
 */

typedef enum en_timeouts_type
{
    TimeoutsTypeInt = 0,
    TimeoutsTypeUInt8,
    TimeoutsTypeInt8,
    TimeoutsTypeUInt16,
    TimeoutsTypeInt16,
    TimeoutsTypeUInt32,
    TimeoutsTypeInt32,
} en_timeouts_type_t;

#if !defined(BOOLEAN_T)
   #define BOOLEAN_T 1
   typedef bool boolean_t;
   #define TRUE true
   #define FALSE false
#endif

/**
 *******************************************************************************
 ** Global variable declarations ('extern', definition in C source)
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global function prototypes ('extern', definition in C source) 
 *******************************************************************************
 */



uint32_t Timeouts_GetDiff(uint32_t u32TicksLastUpdate, uint32_t u32TicksNow);
boolean_t Timeouts_Decrease(void* pHandle,en_timeouts_type_t enType,uint32_t u32Diff,uint32_t u32Min);
boolean_t Timeouts_Increase(void* pHandle,en_timeouts_type_t enType,uint32_t u32Diff,uint32_t u32Max);


//@} // TimeoutsGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMEOUTS_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */


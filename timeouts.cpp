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

#define __TIMEOUTS_CPP__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */


#include <string.h> //required also for memset, memcpy, etc.
#include <stdint.h>
#include <stdbool.h>
#include "timeouts.h"

/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global variable definitions (declared in header file with 'extern') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local type definitions ('typedef') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local variable definitions ('static') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */


uint32_t Timeouts_GetDiff(uint32_t u32TicksLastUpdate, uint32_t u32TicksNow)
{
    uint32_t u32Diff = u32TicksNow;
    if (u32Diff < u32TicksLastUpdate)
    {
       u32Diff = 0xFFFFFFFFul - u32TicksLastUpdate + u32Diff;
    } else
    {
       u32Diff = u32Diff - u32TicksLastUpdate;
    }
    return u32Diff;   
}

/**
 * @brief Update a timeout variable by decreasing by u32Diff
 * 
 * @param pHandle Pointer to variable
 * @param enType Type of variable
 * @param u32Diff Difference to decrease
 * @param u32Min Minimum value (minimal value can be as lowest as 0)
 * @return boolean_t TRUE if variable got "0", else FALSE
 */
boolean_t Timeouts_Decrease(void* pHandle,en_timeouts_type_t enType,uint32_t u32Diff,uint32_t u32Min)
{
    uint32_t u32Tmp = 0;
    boolean_t result = FALSE;

    if (pHandle == NULL)
    {
        return FALSE;
    }
    if (u32Diff == 0)
    {
        return FALSE;
    }

    switch(enType)
    {
        case TimeoutsTypeInt:
            u32Tmp = *((int*)pHandle);
            break;
        case TimeoutsTypeUInt8:
            u32Tmp = *((uint8_t*)pHandle);
            break;
        case TimeoutsTypeInt8:  
            u32Tmp = *((int8_t*)pHandle);
            break;
        case TimeoutsTypeUInt16:
            u32Tmp = *((uint16_t*)pHandle);
            break;
        case TimeoutsTypeInt16: 
            u32Tmp = *((int16_t*)pHandle);
            break;
        case TimeoutsTypeUInt32:
            u32Tmp = *((uint32_t*)pHandle);
            break;
        case TimeoutsTypeInt32: 
            u32Tmp = *((int32_t*)pHandle);
            break;
    }

    if ((u32Diff >= (u32Tmp - u32Min)) || (u32Tmp < u32Min))
    {
        u32Tmp = u32Min;
        result = TRUE;
    } else
    {
        u32Tmp -= u32Diff;
    }

    switch(enType)
    {
        case TimeoutsTypeInt:
            *((int*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt8:
            *((uint8_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt8:  
            *((int8_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt16:
            *((uint16_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt16: 
            *((int16_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt32:
            *((uint32_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt32: 
            *((int32_t*)pHandle) = u32Tmp;
            break;
    }
    return result;
}

/**
 * @brief Update a timeout variable by increasing by u32Diff
 * 
 * @param pHandle Pointer to variable
 * @param enType Type of variable
 * @param u32Diff Difference to increase
 * @param u32Max Maximum value
 * @return boolean_t TRUE if variable got "0", else FALSE
 */
boolean_t Timeouts_Increase(void* pHandle,en_timeouts_type_t enType,uint32_t u32Diff,uint32_t u32Max)
{
    uint32_t u32Tmp = 0;
    boolean_t result = FALSE;

    if (pHandle == NULL)
    {
        return FALSE;
    }
    if (u32Diff == 0)
    {
        return FALSE;
    }

    switch(enType)
    {
        case TimeoutsTypeInt:
            u32Tmp = *((int*)pHandle);
            break;
        case TimeoutsTypeUInt8:
            u32Tmp = *((uint8_t*)pHandle);
            break;
        case TimeoutsTypeInt8:  
            u32Tmp = *((int8_t*)pHandle);
            break;
        case TimeoutsTypeUInt16:
            u32Tmp = *((uint16_t*)pHandle);
            break;
        case TimeoutsTypeInt16: 
            u32Tmp = *((int16_t*)pHandle);
            break;
        case TimeoutsTypeUInt32:
            u32Tmp = *((uint32_t*)pHandle);
            break;
        case TimeoutsTypeInt32: 
            u32Tmp = *((int32_t*)pHandle);
            break;
    }

    if (((u32Diff + u32Tmp) >= u32Max) || (u32Tmp > u32Max))
    {
        u32Tmp = u32Max;
        result = TRUE;
    } else
    {
        u32Max += u32Diff;
    }

    switch(enType)
    {
        case TimeoutsTypeInt:
            *((int*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt8:
            *((uint8_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt8:  
            *((int8_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt16:
            *((uint16_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt16: 
            *((int16_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeUInt32:
            *((uint32_t*)pHandle) = u32Tmp;
            break;
        case TimeoutsTypeInt32: 
            *((int32_t*)pHandle) = u32Tmp;
            break;
    }
    return result;
}


/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */


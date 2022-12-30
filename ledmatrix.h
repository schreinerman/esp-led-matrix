/**
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2022 io-expert.com. All rights reserved.
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
 **\file ledmatrix.cpp
 **
 ** LED Matrix API
 ** A detailed description is available at
 ** @link LedMatrixGroup file description @endlink
 **
 ** History:
 ** - 2022-12-26  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__LEDMATRIX_H__)
#define __LEDMATRIX_H__


/**
 *******************************************************************************
 ** \defgroup LedMatrixGroup LED Matrix API
 **
 ** Provided functions of LedMatrix:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page ledmatrix_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "ledmatrix.h"
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
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <WiFiClient.h>
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
#else
  #include <WebServer.h>
#endif

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

#if defined(ARDUINO_ARCH_ESP8266)
  void LedMatrix_Init(ESP8266WebServer* pWebServer);
#else
  void LedMatrix_Init(WebServer* pWebServer);
#endif
int LedMatrix_Deinit(void);

void LedMatrix_Update(void);

//@} // LedMatrixGroup

#endif /* __LEDMATRIX_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */


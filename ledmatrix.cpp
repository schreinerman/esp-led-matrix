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

#define __LEDMATRIX_CPP__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */


#include <string.h> //required also for memset, memcpy, etc.
#include <stdint.h>
#include <stdbool.h>
#include "appconfig.h"
#include "ledmatrix.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FastLED.h"
#include "timeouts.h"

/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#define MATRIX_WIDTH u16MatrixWidth
#define MATRIX_HEIGHT u16MatrixHeight
#define NUM_LEDS MATRIX_WIDTH * MATRIX_HEIGHT
#define DATA_PIN 5

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

#if defined(ARDUINO_ARCH_ESP8266)
static ESP8266WebServer* pServer;
#else
static WebServer* pServer;
#endif

static CRGB leds[1024] = {
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0xFF,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0xFF), CRGB(0x00,0x00,0x00), 
CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), CRGB(0x00,0x00,0x00), 
};

static bool bUpdate = true;

static volatile uint32_t u32LastUpdateFromApi = millis();

static volatile uint32_t u32LastRefresh = millis();

static volatile uint8_t u8Refreshs = 3;

static volatile uint32_t u32SplashScreen = 10000;

static volatile uint32_t u32Fadeout = 0;

static uint32_t u32LastUpdate = millis();

const uint16_t u16MatrixWidth = AppConfig_GetWidth();
const uint16_t u16MatrixHeight = AppConfig_GetHeight();

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

static uint8_t hexToDec(const char* hex)
{
    if ((*hex >= 'a') && (*hex <= 'f'))
    {
        return (uint8_t)(*hex - 'a')+10;
    } else if ((*hex >= 'A') && (*hex <= 'F'))
    {
        return (uint8_t)(*hex - 'A')+10;
    } else if ((*hex >= '0') && (*hex <= '9'))
    {
        return (uint8_t)(*hex - '0');
    }
    return 0;
}

static void handleAPI() {
    int x,y,pos,i;
    uint8_t r,g,b;
    const char* color = NULL;
    const char* command = NULL;
    StaticJsonDocument<128> doc;
    pServer->sendHeader("Connection", "close");
    if (pServer->method() != HTTP_POST) {
        pServer->send(405, "text/plain", "Method Not Allowed");
    } else {
    if (pServer->args() == 1)
    {
        if (pServer->arg(0) != NULL)
        {
            deserializeJson(doc, pServer->arg(0));
            command = doc["i"];
            switch(command[0])
            {
                case 'd':
                    x = doc["x"];
                    y = doc["y"];
                    color = doc["c"];
                    pos = y * MATRIX_WIDTH;
                    if ((y % 2) == 0)
                    {
                        pos = pos + (MATRIX_WIDTH-1) - x;
                    } else
                    {
                        pos = pos + x;
                    }
                    if ((pos >= 0) && (pos < NUM_LEDS) && (color != NULL))
                    {
                        if (*color == '#')
                        {
                            color++;
                        }
                        r = hexToDec(color + 0) << 4 | hexToDec(color + 1);
                        g = hexToDec(color + 2) << 4 | hexToDec(color + 3);
                        b = hexToDec(color + 4) << 4 | hexToDec(color + 5);
                        leds[pos].r = r;
                        leds[pos].g = g;
                        leds[pos].b = b;
                        u32LastUpdateFromApi = millis();
                        bUpdate = true;
                    }
                    break;
                case 'c':
                    for (i=0;i < NUM_LEDS;i++)
                    {
                        leds[i].r = 0;
                        leds[i].g = 0;
                        leds[i].b = 0;
                    } 
                    u32LastUpdateFromApi = millis();
                    bUpdate = true;
                    break;
            }
            pServer->send(200, "text/plain", "ok");
        } else
        {
            pServer->send(405, "text/plain", "error");
        }

    } else
    {
        pServer->send(405, "text/plain", "error");
    }
  }
}

#if defined(ARDUINO_ARCH_ESP8266)
  void LedMatrix_Init(ESP8266WebServer* pWebServer)
#else
  void LedMatrix_Init(WebServer* pWebServer)
#endif
{
    pServer = pWebServer;
    pServer->on("/api/matrix/draw", handleAPI);
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(32);
}

int LedMatrix_Deinit(void)
{
}

void LedMatrix_Update(void)
{
    uint32_t u32Millis = millis();
    uint32_t u32DiffApiUpdate = Timeouts_GetDiff(u32LastUpdateFromApi,u32Millis);
    uint32_t u32Diff = Timeouts_GetDiff(u32LastUpdate,u32Millis);

    u32LastUpdate = u32Millis;

    if (Timeouts_Decrease((void*)&u32SplashScreen,TimeoutsTypeUInt32,u32Diff,0))
    {
        u32Fadeout = 10000;
    }
    if (u32Fadeout > 0)
    {
        u8Refreshs = 1;
        leds[random(0, NUM_LEDS)] = CRGB(0,0,0);
    } 
    if (Timeouts_Decrease((void*)&u32Fadeout,TimeoutsTypeUInt32,u32Diff,0))
    {
        for(int i = 0;i < NUM_LEDS;i++)
        {
            leds[i] = CRGB(0,0,0);
        }
        u8Refreshs = 3;
    }
    if ((bUpdate) && (u32DiffApiUpdate > 200))
    {
        u8Refreshs = 3;
        bUpdate = false;
    }
    if ((u8Refreshs > 0) || ((bUpdate) && ((u32Millis - u32LastRefresh) > 1000)))
    {
        FastLED.show();
        delay(1);
        u8Refreshs--;
        u32LastRefresh = u32Millis;
    }

    /*if (!bUpdate)
    {
        u32LastRefresh = u32Millis;
    }*/
}

/**
 *******************************************************************************
 ** EOF (not truncated)
 *****************AA**************************************************************
 */


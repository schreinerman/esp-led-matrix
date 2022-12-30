/**
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2021 io-expert.com. All rights reserved.
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
 **\file appconfig.c
 **
 ** App Configuration
 ** A detailed description is available at
 ** @link AppConfigGroup file description @endlink
 **
 ** History:
 ** - 2021-2-3  1.00  Manuel Schreiner
 *******************************************************************************
 */

#define __APPCONFIG_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */

#include <Arduino.h>
#include "stdint.h"
#include "appconfig.h"
#include "webconfig.h"

#if defined(ARDUINO_ARCH_ESP8266)
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include "FS.h"
#include "SPIFFS.h"
#define USE_SPIFFS
#include <ESPmDNS.h>
#else
#error Not supported architecture
#endif

/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#pragma GCC optimize ("-O3")

#define FORMAT_SPIFFS_IF_FAILED true


/**
 *******************************************************************************
 ** Global variable definitions (declared in header file with 'extern') 
 *******************************************************************************
 */


stc_appconfig_t stcAppConfig = {
  INITIAL_SSID_STATION_MODE,
  INITIAL_PASSORD_STATION_MODE,
  INITIAL_WWW_NAME,
  INITIAL_WWW_PASS,
  
  16, // Width
  16, // Height
  5, // DOUT

  0xCFDFAABB
};

stc_webconfig_description_t astcAppConfigDescription[] = {
    {enWebConfigTypeStringLen32,"ssid","Wifi-SSID"},
    {enWebConfigTypeStringLen32,"password","Wifi-PSK"},
    {enWebConfigTypeStringLen32,"wwwuser","WWW-User"},
    {enWebConfigTypeStringLen32,"wwwpass","WWW-Pass"},
    
    {enWebConfigTypeUInt16,"Width","Width"},
    {enWebConfigTypeUInt16,"Height","Height"},
    {enWebConfigTypeUInt8,"DOUT","DOUT"},

};

stc_webconfig_handle_t stcWebConfig = {
  (uint8_t*)&stcAppConfig,
  (uint32_t)sizeof(stcAppConfig),
  (uint32_t)((sizeof(astcAppConfigDescription)/sizeof(astcAppConfigDescription[0]))),
  (stc_webconfig_description_t*)astcAppConfigDescription
};

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

static bool bInitDone = false;
static bool bWebServerInitDone = false;
static bool bLockWrite = false;
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

static bool InitData()
{
  static bool bInitalized = false;
  if (!bInitalized)
  {
    #if defined(USE_SPIFFS)
      if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
      {
        bInitalized = true;
      } else
      {
        SPIFFS.format();
        if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
        {
          bInitalized = true;
        }
      }
    #else
      EEPROM.begin(512);
      bInitalized = true;
    #endif
  }
  return bInitalized;
}
static void ReadData()
{
  #if defined(USE_SPIFFS)
    if (SPIFFS.exists("/config.bin"))
    {
      File file = SPIFFS.open("/config.bin",FILE_READ);
      file.readBytes((char*)&stcAppConfig,sizeof(stcAppConfig));
      file.close();
    }
  #else
    EEPROM.get(0,stcAppConfig);
  #endif
}

static void WriteData()
{
  #if defined(USE_SPIFFS)
    File file = SPIFFS.open("/config.bin",FILE_WRITE);
    if(file){
      file.write((uint8_t*)&stcAppConfig,sizeof(stcAppConfig));
      file.close();
    }
  #else
    EEPROM.put(0,stcAppConfig);
    EEPROM.commit();
  #endif
}

/*********************************************
 * Init App Configuration
 * 
 * pWebServerHandle Webserver handle
 * 
 ********************************************* 
 */
#if defined(ARDUINO_ARCH_ESP8266)
void AppConfig_Init(ESP8266WebServer* pWebServerHandle)
#else
void AppConfig_Init(WebServer* pWebServerHandle)
#endif
{
  if (bInitDone == false)
  {
    bInitDone = true;
    memset(&stcAppConfig,0,sizeof(stcAppConfig));
    if (!InitData())
    {
      return;
    }
    ReadData();
    if (stcAppConfig.u32magic != 0xCFDFAABB)
    {
      bLockWrite = true;
      memset(&stcAppConfig,0,sizeof(stcAppConfig));
      stcAppConfig.u32magic = 0xCFDFAABB;
      AppConfig_SetStaSsid(INITIAL_SSID_STATION_MODE);
      AppConfig_SetStaPassword(INITIAL_PASSORD_STATION_MODE);
      AppConfig_SetWwwUser(INITIAL_WWW_NAME);
      AppConfig_SetWwwPass(INITIAL_WWW_PASS);
      
      AppConfig_SetWidth(16);
      AppConfig_SetHeight(16);
      AppConfig_SetDOUT(5);

      bLockWrite = false;
      AppConfig_Write();
    }
    
  }
  if ((bWebServerInitDone != true) && (pWebServerHandle != NULL))
  {
    bWebServerInitDone = true;
    WebConfig_Init(pWebServerHandle,&stcWebConfig);
  }
}

/*********************************************
 * Write App Configuration
 * 
 ********************************************* 
 */
void AppConfig_Write(void)
{
  if (bLockWrite == false)
  {
    bLockWrite = true;
    Serial.println("Updating App Configuration...");

    WriteData();
    
    bLockWrite = false;
  }
}

/*********************************************
 * Get STA-Mode SSID
 * 
 * \return SSID as string
 ********************************************* 
 */
char* AppConfig_GetStaSsid(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.ssidStation;
}


/*********************************************
 * Set STA-Mode SSID
 * 
 * \param ssid SSID
 * 
 ********************************************* 
 */
void AppConfig_SetStaSsid(char* ssid)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  strncpy(stcAppConfig.ssidStation,ssid,32);
  AppConfig_Write();
}

/*********************************************
 * Get STA-Mode Password
 * 
 * \return password as string
 ********************************************* 
 */
char* AppConfig_GetStaPassword(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.passwordStation;
}

/*********************************************
 * Set STA-Mode Password
 * 
 * \param pass Password
 * 
 ********************************************* 
 */
void AppConfig_SetStaPassword(char* pass)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  strncpy(stcAppConfig.passwordStation,pass,32);
  AppConfig_Write();
}


/**********************************************
 * Get WwwUser - WWW-User
 * 
 * \return WwwUser
 **********************************************
 */
char* AppConfig_GetWwwUser(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.wwwUser;
}

/*********************************************
 * Set WwwUser - WWW-User
 * 
 * \param WwwUser WWW-User
 * 
 ********************************************* 
 */
void AppConfig_SetWwwUser(char* WwwUser)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  strncpy(stcAppConfig.wwwUser,WwwUser,32);
  AppConfig_Write();
}
/**********************************************
 * Get WwwPass - WWW-Pass
 * 
 * \return WwwPass
 **********************************************
 */
char* AppConfig_GetWwwPass(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.wwwPass;
}

/*********************************************
 * Set WwwPass - WWW-Pass
 * 
 * \param WwwPass WWW-Pass
 * 
 ********************************************* 
 */
void AppConfig_SetWwwPass(char* WwwPass)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  strncpy(stcAppConfig.wwwPass,WwwPass,32);
  AppConfig_Write();
}


/**********************************************
 * Get Width - Width
 * 
 * \return Width
 **********************************************
 */
uint16_t AppConfig_GetWidth(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.Width;
}

/*********************************************
 * Set Width - Width
 * 
 * \param Width Width
 * 
 ********************************************* 
 */
void AppConfig_SetWidth(uint16_t Width)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  stcAppConfig.Width = Width;
  AppConfig_Write();
}
/**********************************************
 * Get Height - Height
 * 
 * \return Height
 **********************************************
 */
uint16_t AppConfig_GetHeight(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.Height;
}

/*********************************************
 * Set Height - Height
 * 
 * \param Height Height
 * 
 ********************************************* 
 */
void AppConfig_SetHeight(uint16_t Height)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  stcAppConfig.Height = Height;
  AppConfig_Write();
}
/**********************************************
 * Get DOUT - DOUT
 * 
 * \return DOUT
 **********************************************
 */
uint8_t AppConfig_GetDOUT(void)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  return stcAppConfig.DOUT;
}

/*********************************************
 * Set DOUT - DOUT
 * 
 * \param DOUT DOUT
 * 
 ********************************************* 
 */
void AppConfig_SetDOUT(uint8_t DOUT)
{
  if (bInitDone == false)
  {
    AppConfig_Init(NULL);
  }
  stcAppConfig.DOUT = DOUT;
  AppConfig_Write();
}


/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */

/**
 * @file    SDK_EVAL_Config.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file contains SDK EVAL configuration and useful defines.
 * @details
 *
 * This file is used to include all or a part of the SDK Eval
 * libraries into the application program which will be used.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_CONFIG_H
#define __SDK_EVAL_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Number of supported BlueNRG Development Kit platforms */
#define SDK_PLATFORMS_NUMBER     (1)


/* BlueNRG1 Development platform: STEVAL-IDB007VX */
#define STEVAL_IDB007VX 0
/* BlueNRG1 Development platform: STEVAL-IDB007V1 */
#define STEVAL_IDB007V1 1
/* User defined platform */
#define USER_EVAL_PLATFORM 2


#ifndef USER_DEFINED_PLATFORM
#define USER_DEFINED_PLATFORM USER_EVAL_PLATFORM
#endif

#if USER_DEFINED_PLATFORM == USER_EVAL_PLATFORM
/* Adding the symbol USER_DEFINED_PLATFORM=USER_EVAL_PLATFORM
 * An user platform can be supported, at compile time, by following these steps: 
 *  
 * 1) Create a file  "USER_Platform_Configuration.h" with specific user 
 *    platform configuration (USER_Platform_Configuration_STEVAL_IDB007V1.h can be used as reference).
 * 2) Place the "USER_Platform_Configuration.h" on the 
 *    SDK_Eval_BlueNRG1\inc folder. 
 * 3) On the selected EWARM workspace preprocessor options, add this define: 
 *    USER_DEFINED_PLATFORM=USER_EVAL_PLATFORM
 */
#include "USER_Platform_Configuration.h"
 #elif USER_DEFINED_PLATFORM == STEVAL_IDB007V1
/* Adding the symbol USER_DEFINED_PLATFORM=STEVAL_IDB007V1
 * only the STEVAL-IDB007V1 is supported
 */
#include "USER_Platform_Configuration_STEVAL_IDB007V1.h"
#else 
/* This is the header file used for BlueNRG1 kits platforms auto configuration 
   performed at runtime: user must not change it */
#include "USER_Platform_Configuration_auto.h"
#endif 


#include "SDK_EVAL_Button.h"
#include "SDK_EVAL_Com.h"
//#include "SDK_EVAL_I2C.h"
#include "SDK_EVAL_Led.h"
//#include "SDK_EVAL_SPI.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_BlueNRG1
 * @{
 */
    
/** @addtogroup SDK_EVAL_Config         SDK EVAL Config
 * @brief SDK EVAL configuration.
 * @details See the file <i>@ref SDK_EVAL_Config.h</i> for more details.
 * @{
 */

/** @addtogroup SDK_EVAL_Config_Exported_Types          SDK EVAL Config Exported Types
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Exported_Constants        SDK EVAL Config Exported Constants
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Exported_Macros           SDK EVAL Config Exported Macros
 * @{
 */

   
enum {
  ID_UNKNOWN = 0,       /* BlueNRG-1 board not identified */       
  ID_STEVAL_IDB007V1,   /* BlueNRG-1 Development platform: STEVAL-IDB007V1 */
};

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Exported_Functions        SDK EVAL Config Exported Functions
 * @{
 */

void SdkEvalIdentification(void);
uint8_t SdkEvalGetVersion(void);
   
/**
 * @}
 */

/**
 * @}
 */


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

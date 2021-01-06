/**
 * @file    SDK_EVAL_Led.h
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    Febrary 3, 2014
 * @brief   This file contains definitions for Software Development Kit eval board Leds.
 * @details
 *
 * In this module there are API for the management of the leds on the SDK Eval
 * motherboard.
 *
 * @endcode
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
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_LED_H
#define __SDK_EVAL_LED_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L476xx
#include "stm32l476xx.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_bus.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_bus.h"
#endif


#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
 * @{
 */

/** @addtogroup SDK_EVAL_Led            SDK EVAL Led
 * @brief Management of Software Development Kit eval board Leds.
 * @details See the file <i>@ref SDK_EVAL_Led.h</i> for more details.
 * @{
 */

/** @defgroup SDK_EVAL_Led_Exported_Types               SDK EVAL Led Exported Types
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL LEDs
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Exported_Constants                           SDK EVAL Led Exported Constants
 * @{
 */
#define LED_ON                           GPIO_PIN_SET
#define LED_OFF                           GPIO_PIN_RESET 
     
#define USER_LED_GPIO_PIN                LL_GPIO_PIN_5
#define USER_LED_GPIO_PORT               GPIOA
#ifdef STM32L476xx
#define USER_LED_GPIO_CLK_ENABLE()         LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define USER_LED_GPIO_CLK_ENABLE()         LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif


/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_Led_Exported_Macros                       SDK EVAL Led Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_Led_Exported_Functions                   SDK EVAL Led Exported Functions
 * @{
 */

void SdkEvalLedInit(void);
void SdkEvalLedOn(void);
void SdkEvalLedOff(void);
void SdkEvalLedToggle(void);
FlagStatus SdkEvalLedGetState(void);

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

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

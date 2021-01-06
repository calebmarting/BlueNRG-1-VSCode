/**
 * @file    SDK_EVAL_Led.c
 * @author  AMS VMA RF application team
 * @version V1.0.1
 * @date    March 10, 2015
 * @brief   This file provides all the low level API to manage SDK LEDs.
 * @details
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


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Led.h"
/** @addtogroup SDK_EVAL_STM32L
 * @{
 */


/** @addtogroup SDK_EVAL_Led
 * @{
 */

/** @defgroup SDK_EVAL_Led_Private_TypesDefinitions             SDK EVAL Led Private Types Definitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Private_Defines                      SDK EVAL Led Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Led_Private_Macros                       SDK EVAL Led Private Macros
 * @{
 */

/**
 * @}
 */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Led_Private_FunctionPrototypes                    SDK EVAL Led Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Led_Private_Functions                             SDK EVAL Led Private Functions
 * @{
 */



/**
 * @brief  Configures LED GPIO.
 * @param  None.
 * @retval None.
 */
void SdkEvalLedInit(void)
{
  USER_LED_GPIO_CLK_ENABLE();
  
  LL_GPIO_SetPinMode(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN, LL_GPIO_MODE_OUTPUT);
  
}

/**
 * @brief  Turns selected LED On.
 * @param  None.
 * @retval None.
 */
void SdkEvalLedOn(void)
{
   LL_GPIO_SetOutputPin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN);
}

/**
 * @brief  Turns selected LED Off.
 * @param  None.
 * @retval None.
 */
void SdkEvalLedOff(void)
{
   LL_GPIO_ResetOutputPin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN);
}

/**
 * @brief  Toggles the selected LED.
 * @param  None.
 * @retval None.
 */
void SdkEvalLedToggle(void)
{
  LL_GPIO_TogglePin(USER_LED_GPIO_PORT, USER_LED_GPIO_PIN);
}

/**
 * @brief  Returns the status of a specified led.
 * @param  None.
 * @retval FlagStatus return the status of the LED. This parameter can be:
 *         SET or RESET.
 */
FlagStatus SdkEvalLedGetState(void)
{
  if(USER_LED_GPIO_PORT->IDR & USER_LED_GPIO_PIN)
    return SET;
  else
    return RESET;
}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

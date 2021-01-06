/**
 * @file    SDK_EVAL_Led.c
 * @author  AMS VMA RF application team
 * @version V1.1.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage LEDs for eval board.
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Led.h"


/** @addtogroup SDK_EVAL_BlueNRG1           SDK EVAL BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_Led                SDK EVAL Led
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
 * @defgroup SDK_EVAL_Led_Private_FunctionPrototypes            SDK EVAL Led Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Led_Private_Functions                     SDK EVAL Led Private Functions
 * @{
 */


/**
 * @brief  Get the IO pin associate to the required LED
 * @param  xLed: LED name @ref SdkEvalLed
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval led_pin: IO pin name associated to that LED
 */
uint32_t Get_LedGpioPin(SdkEvalLed xLed)
{
  uint32_t led_pin;
  led_pin = GPIO_Pin_14;
//  switch (xLed)
//  {
//    case(LED1):
//      led_pin = SDK_EVAL_LED1_PIN;
//      break;
//    case(LED2):
//      led_pin = SDK_EVAL_LED2_PIN;
//      break;
//    case(LED3):
//      led_pin = SDK_EVAL_LED3_PIN;
//      break;
//    default:
//      led_pin = SDK_EVAL_LED1_PIN;
//      break;
//  }
  return (led_pin);
}

/**
 * @}
 */



/**
 * @defgroup SDK_EVAL_Led_Public_Functions                SDK EVAL Led Public Functions
 * @{
 */


/**
 * @brief  Configures LEDs.
 * @param  xLed Specifies the Led to be configured @ref SdkEvalLed.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval None.
 */
void SdkEvalLedInit(SdkEvalLed xLed)
{
  GPIO_InitType GPIO_InitStructure;
  
  /* Enable the GPIO Clock */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);
  
  /* Configure the LED pin */
  GPIO_InitStructure.GPIO_Pin = Get_LedGpioPin(xLed);
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_Pull = ENABLE;
  GPIO_InitStructure.GPIO_HighPwr = ENABLE;
  GPIO_Init(&GPIO_InitStructure);

  /* Put the LED off */
  SdkEvalLedOff(xLed);
}

/**
 * @brief  Turns selected LED On.
 * @param  xLed Specifies the Led to be set on  @ref SdkEvalLed.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval None.
 */
void SdkEvalLedOn(SdkEvalLed xLed)
{
   GPIO_WriteBit(Get_LedGpioPin(xLed), LED_ON);
}

/**
 * @brief  Turns selected LED Off.
 * @param  xLed Specifies the Led to be set off @ref SdkEvalLed.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval None.
 */
void SdkEvalLedOff(SdkEvalLed xLed)
{
   GPIO_WriteBit(Get_LedGpioPin(xLed), LED_OFF);
}

/**
 * @brief  Toggles the selected LED.
 * @param  xLed Specifies the Led to be toggled @ref SdkEvalLed.@ref SdkEvalLed.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval None.
 */
void SdkEvalLedToggle(SdkEvalLed xLed)
{
  GPIO_ToggleBits(Get_LedGpioPin(xLed));
}

/**
 * @brief  Returns the status of a specified led.
 * @param  xLed Specifies the Led to be read @ref SdkEvalLed.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 *         @arg LED3
 * @retval FlagStatus: status of the LED @ref FlagStatus
 *         This parameter can be: SET or RESET.
 */
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed)
{
  if(GPIO_ReadBit(Get_LedGpioPin(xLed)))
    return RESET;
  else
    return SET;
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



/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

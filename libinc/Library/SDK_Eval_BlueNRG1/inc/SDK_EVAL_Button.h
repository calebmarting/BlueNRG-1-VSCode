/**
 * @file    SDK_EVAL_Button.h
 * @author  AMS VMA RF application team
 * @version V1.1.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage buttons for eval board.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_BUTTON_H
#define __SDK_EVAL_BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_BlueNRG1       SDK EVAL BlueNRG1
 * @{
 */

/** @addtogroup SDK_EVAL_Button         SDK EVAL Button
 * @{
 */

/** @addtogroup SDK_EVAL_Button_Exported_Types          SDK EVAL Button Exported Types
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL Buttons
 */
typedef enum {
  BUTTON_1 = 0,
  BUTTON_2
} SdkEvalButton;


/**
 * @brief  Enumeration of SDK EVAL Buttons IRQ mode
 */
typedef enum {
  IRQ_ON_FALLING_EDGE = 0,
  IRQ_ON_RISING_EDGE,
  IRQ_ON_BOTH_EDGE
} SdkEvalButtonIrq;


/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Constants                SDK EVAL Button Exported Constants
 * @{
 */
#define EVAL_BOARD_BUTTONS          (2)

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Macros           SDK EVAL Button Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Functions        SDK EVAL Button Exported Functions
 * @{
 */
uint32_t Get_ButtonGpioPin(SdkEvalButton xButton);
void SdkEvalPushButtonInit(SdkEvalButton xButton);
void SdkEvalPushButtonIrq(SdkEvalButton xButton, SdkEvalButtonIrq xButtonIrq);
FlagStatus SdkEvalPushButtonGetState(SdkEvalButton xButton);
FlagStatus SdkEvalPushButtonGetITPendingBit(SdkEvalButton xButton);
void SdkEvalPushButtonClearITPendingBit(SdkEvalButton xButton);

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

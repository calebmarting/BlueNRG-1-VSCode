/**
 * @file    SDK_EVAL_Config.c
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage SDK Version identification.
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
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

/** @addtogroup SDK_EVAL_BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_Config
 * @{
 */

/** @defgroup SDK_EVAL_Config_Private_TypesDefinitions             SDK EVAL Config Private Types Definitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Private_Defines                      SDK EVAL Config Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Private_Macros                       SDK EVAL Config Private Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Public_Variables                    SDK EVAL Config Public Variables
 * @{
 */
static volatile uint8_t s_SdkEvalVersion = ID_UNKNOWN;

#if USER_DEFINED_PLATFORM == STEVAL_IDB007VX

/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) Buttons definitions
 */
const uint32_t s_vectnButtonPinVersion[SDK_PLATFORMS_NUMBER][EVAL_BOARD_BUTTONS] = {{GPIO_Pin_13, GPIO_Pin_5}};


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) UART definitions
 */
const uint32_t s_UartTxPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_8};
const uint32_t s_UartTxModeVersion[SDK_PLATFORMS_NUMBER] = {Serial1_Mode};
const uint32_t s_UartRxPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_11};
const uint32_t s_UartRxModeVersion[SDK_PLATFORMS_NUMBER] = {Serial1_Mode};


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) I2C definitions
 */
const I2C_Type* s_I2cPeripheralVersion[SDK_PLATFORMS_NUMBER] = {I2C2};
const uint32_t s_I2cDataPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_5};
const uint32_t s_I2cDataModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint32_t s_I2cClkPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_4};
const uint32_t s_I2cClkPinModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint8_t s_I2cIrqVersion[SDK_PLATFORMS_NUMBER] = {I2C2_IRQn};

const DMA_CH_Type* s_I2cDataDmaTxVersion[SDK_PLATFORMS_NUMBER] = {DMA_CH7};
const uint32_t s_I2cDataDmaRxTcVersion[SDK_PLATFORMS_NUMBER] = {DMA_FLAG_TC6};

const DMA_CH_Type* s_I2cDataDmaRxVersion[SDK_PLATFORMS_NUMBER] = {DMA_CH6};
const uint32_t s_I2cDataDmaTxTcVersion[SDK_PLATFORMS_NUMBER] = {DMA_FLAG_TC7};

const uint32_t s_I2CBaseVersion[SDK_PLATFORMS_NUMBER] = {I2C2_BASE};

/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) LEDs definitions
 */
const uint32_t s_vectnGpioPinVersion[SDK_PLATFORMS_NUMBER][EVAL_BOARD_LEDS] = {{GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_14}};



/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) SPI definitions
 */
const uint32_t s_SpiCs1PinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_1};
const uint32_t s_SpiCs1ModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint32_t s_SpiCs2PinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_11};
const uint32_t s_SpiCs2ModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint32_t s_SpiOutPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_2};
const uint32_t s_SpiOutModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint32_t s_SpiInPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_3};
const uint32_t s_SpiInModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};
const uint32_t s_SpiSclkPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_0};
const uint32_t s_SpiSclkModeVersion[SDK_PLATFORMS_NUMBER] = {Serial0_Mode};

/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007V1) sensor IRQ definitions
 */
const uint32_t s_SpiIrqSensorPinVersion[SDK_PLATFORMS_NUMBER] = {GPIO_Pin_12};

#endif /* #ifndef USER_DEFINED_PLATFORM */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Config_Private_FunctionPrototypes                    SDK EVAL Config Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/**
 * @defgroup SDK_EVAL_Config_Private_Functions                             SDK EVAL Config Private Functions
 * @{
 */

/**
 * @brief  Identify the version of the current BlueNRG1 platform.
 * @param  None.
 * @retval None.
 */
void SdkEvalIdentification(void)
{

#if USER_DEFINED_PLATFORM == STEVAL_IDB007VX

  /* Try to understand if it is possible to have information about the board */
  s_SdkEvalVersion = ID_STEVAL_IDB007V1;    

#elif USER_DEFINED_PLATFORM == STEVAL_IDB007V1
    s_SdkEvalVersion = ID_STEVAL_IDB007V1;
#elif USER_DEFINED_PLATFORM == USER_EVAL_PLATFORM
    s_SdkEvalVersion = ID_STEVAL_IDB007V1;      /* user code */
#endif
  
} /* end SdkEvalIdentification() */

/**
 * @brief  Returns the version of the current BlueNRG1 platform.
 * @param  None.
 * @retval None.
 */
uint8_t SdkEvalGetVersion(void)
{  
    return s_SdkEvalVersion; 
} /* end SdkEvalGetVersion() */

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

/**
 * @file    SDK_EVAL_Uart_Driver.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    February, 28 2017
 * @brief   This file contains definitions for Software Development Kit eval SPI devices
 * @details
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
#ifndef __SDK_EVAL_UART_DRIVER_H
#define __SDK_EVAL_UART_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L476xx
#include "stm32l476xx.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx.h"
#endif

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_UART_STM32L
* @{
*/
   
   
/** @defgroup SPI_Private_TypesDefinitions
* @{
*/

/**
* @}
*/


/** @addtogroup SDK_EVAL_Uart                    SDK EVAL Uart
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library UART interface functions. 
* @{
*/

   /** @addtogroup SDK_EVAL_UART_Exported_Types          SDK EVAL Uart Exported Types
 * @{
 */


/**
* @}
*/

/** @defgroup UART_Private_Defines
* @{
*/


#define DTM_UART_RTS_OUTPUT_LOW()       LL_GPIO_ResetOutputPin(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN)
#define DTM_UART_RTS_OUTPUT_HIGH()      LL_GPIO_SetOutputPin(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN)


/**
* @}
*/


/** @defgroup SDK_EVAL_Uart_Peripheral_Gpio
* @{
*/

/**
* @brief BlueNRG User Platform UART & GPIO lines: to be customized for addressing user platform.
*        Example configuration: STM32L152 Nucleo.
 */
#define DTM_USART                           USART1
#define DTM_USART_IRQn                      USART1_IRQn
#define DTM_USART_CLK_ENABLE()              LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)
#ifdef STM32L476xx
#define DTM_USART_GPIO_CLK_ENABLE()         LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define DTM_USART_GPIO_CLK_ENABLE()         LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif



/* Definition for DTM_USART Pins */
#define DTM_USART_TX_PIN                    LL_GPIO_PIN_9
#define DTM_USART_TX_GPIO_PORT              GPIOA
#define DTM_USART_TX_AF()                   LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_7)
#define DTM_USART_RX_PIN                    LL_GPIO_PIN_10
#define DTM_USART_RX_GPIO_PORT              GPIOA
#define DTM_USART_RX_AF()                   LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_7)

   
#define DTM_USART_CTS_PIN                    LL_GPIO_PIN_11
#define DTM_USART_CTS_GPIO_PORT              GPIOA
#define DTM_USART_CTS_AF()                     LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_11, LL_GPIO_AF_7)
   
#define DTM_USART_RTS_PIN                    LL_GPIO_PIN_12
#define DTM_USART_RTS_GPIO_PORT              GPIOA
#define DTM_USART_RTS_AF()                     LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_12, LL_GPIO_AF_7)

   
#define DTM_USART_EXTI_IRQn                       EXTI15_10_IRQn
#define DTM_USART_EXTI_IRQHandler                 EXTI15_10_IRQHandler
#define DTM_USART_CTS_SYSCFG_PORT                 LL_SYSCFG_EXTI_PORTA
#define DTM_USART_CTS_SYSCFG_LINE                 LL_SYSCFG_EXTI_LINE11
#define DTM_USART_CTS_EXTI_LINE                   LL_EXTI_LINE_11
#define DTM_USART_CTS_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(DTM_USART_CTS_EXTI_LINE)
#define DTM_USART_CTS_EXTI_TRIG1_ENABLE()          LL_EXTI_EnableRisingTrig_0_31(DTM_USART_CTS_EXTI_LINE)
#define DTM_USART_CTS_EXTI_TRIG2_ENABLE()          LL_EXTI_EnableFallingTrig_0_31(DTM_USART_CTS_EXTI_LINE)
   
/**
* @}
*/

/**
* @}
*/

void DTM_IO_Config(void);
void DTM_Config_GPIO_Output_RTS(void);
void DTM_Config_UART_RTS(void);
void DTM_Config_GPIO_InputIrq_CTS(void);
void DTM_Config_UART_CTS(void);
void DTM_UART_CTS_Input(void);


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

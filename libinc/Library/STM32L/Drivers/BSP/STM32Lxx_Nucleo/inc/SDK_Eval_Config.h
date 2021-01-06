/**
* @file    SDK_EVAL_Config.h
* @author  AMS VMA RF application team
* @version V1.0.1
* @date    March 10, 2014
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
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_CONFIG_H
#define __SDK_EVAL_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L476xx
#include "stm32l476xx.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_usart.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_dma.h"
#include "stm32l1xx_ll_spi.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_usart.h"
#endif

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

#include "SDK_EVAL_Com.h"
#include "SDK_EVAL_Spi.h"
#include "SDK_EVAL_Uart.h"
#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_Gpio.h"


#ifdef __cplusplus
extern "C" {
#endif

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

#define SDK_BUFFER_SIZE (265)  /* 258 (HCI header +  payload) + 2 (alignment) + 4 (fifo item length) + 1 spare */

#ifdef UART_INTERFACE
#define Disable_IRQ()      NVIC_DisableIRQ(DTM_USART_IRQn)
#define Enable_IRQ()       NVIC_EnableIRQ(DTM_USART_IRQn)
#else
#define Disable_IRQ()      NVIC_DisableIRQ(DTM_SPI_EXTI_IRQn)
#define Enable_IRQ()       NVIC_EnableIRQ(DTM_SPI_EXTI_IRQn)
#endif


/* USART list */        // rename USART / UART STUFF TO VCOM
#define UART_PORT          USART2
#define UART_IRQ  USART2_IRQn
#define UART_CLOCK() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define UART_BAUDRATE     115200

#ifdef STM32L476xx
#define UART_GPIO_CLK_ENABLE()         LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define UART_GPIO_CLK_ENABLE()         LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif


/* TX GPIO USART */
#define UART_TX_PORT GPIOA
#define UART_TX_PIN LL_GPIO_PIN_2
#define UART_TX_AF()                      LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)

/* RX GPIO USART */
#define UART_RX_PORT GPIOA
#define UART_RX_PIN LL_GPIO_PIN_3
#define UART_RX_AF()   LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)


/**
* @}
*/
  
/** @defgroup SDK_EVAL_Config_Exported_Macros           SDK EVAL Config Exported Macros
* @{
*/


/**
* @}
*/


/** @defgroup SDK_EVAL_Config_Exported_Functions        SDK EVAL Config Exported Functions
* @{
*/

void SystemClock_Config(void);

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

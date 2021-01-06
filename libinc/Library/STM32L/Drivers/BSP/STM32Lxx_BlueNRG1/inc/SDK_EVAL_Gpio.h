/**
 * @file    SDK_EVAL_Gpio.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    March, 10 2014
 * @brief   This file contains definitions for Software Development Kit eval SPI devices
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_GPIO_H
#define __SDK_EVAL_GPIO_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L476xx
#include "stm32l476xx.h"
#include "stm32l4xx_ll_bus.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx.h"
#endif

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
* @{
*/
   
   
/** @defgroup SPI_Private_TypesDefinitions
* @{
*/

/**
* @}
*/


/** @addtogroup SDK_EVAL_Spi                    SDK EVAL Spi
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library SPI interface functions. 
* @{
*/

   /** @addtogroup SDK_EVAL_SPI_Exported_Types          SDK EVAL Spi Exported Types
 * @{
 */


/**
* @}
*/

/** @defgroup SPI_Private_Defines
* @{
*/


/**
* @}
*/


/** @defgroup SDK_EVAL_Spi_Peripheral_Gpio
* @{
*/

/* BlueNRG SW reset line define values */
#define BNRG_RESET_PIN          LL_GPIO_PIN_8
#define BNRG_RESET_PORT         GPIOA 
#ifdef STM32L476xx
#define BNRG_RESET_CLK_ENABLE()     LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define BNRG_RESET_CLK_ENABLE()     LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif

/* BlueNRG BOOTLOADER line define values */
#define BNRG_BOOT_PIN           LL_GPIO_PIN_14
#define BNRG_BOOT_PORT          GPIOB 
#define BNRG_BOOT_CLK_ENABLE()  __GPIOB_CLK_ENABLE() 

/* Defines for MOSI BOOT pin */
#define BNRG_SPI_MOSI_BOOT_PIN             LL_GPIO_PIN_7                 /* PA.7 */
#define BNRG_SPI_MOSI_BOOT_PORT            GPIOA
#ifdef STM32L476xx
#define BNRG_SPI_MOSI_BOOT_CLK_ENABLE()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define BNRG_SPI_MOSI_BOOT_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif

/* Defines for GPIO to control the BlueNRG communication interface */
#define BNRG_SELECT_PIN         LL_GPIO_PIN_12          /* CN10.16 */
#define BNRG_SELECT_PORT        GPIOB
#ifdef STM32L476xx
#define BNRG_SELECT_CLK_ENABLE()     LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#endif
#ifdef STM32L152xE
#define BNRG_SELECT_CLK_ENABLE()     LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)
#endif
   
/* end  */

void BlueNRG_Power_Down(void);
void BlueNRG_Power_Up(void);
void BlueNRG_RST(void);
void Sdk_Eval_Reset_Pin_Init(void);
void BlueNRG_HW_Bootloader(void);
void BlueNRG_HW_Updater(void);
void BlueNRG_Activate_Select_Pin(void);

/**
* @}
*/

/**
* @}
*/

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

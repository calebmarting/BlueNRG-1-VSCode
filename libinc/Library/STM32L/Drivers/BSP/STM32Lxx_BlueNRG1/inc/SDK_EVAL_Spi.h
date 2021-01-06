/**
 * @file    SDK_EVAL_Spi_Driver.h
 * @author  AMS - AAS Division
 * @version V1.0.1
 * @date    March, 10 2014
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
#ifndef __SDK_EVAL_SPI_DRIVER_H
#define __SDK_EVAL_SPI_DRIVER_H

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
* @{
*/

   /** @addtogroup SDK_EVAL_SPI_Exported_Types          SDK EVAL Spi Exported Types
 * @{
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

/**
* @brief BlueNRG User Platform SPI & GPIO lines: to be customized for addressing user platform.
*        Example configuration: STM32L152 Nucleo.
 */
/* BlueNRG SPI Port */
#define DTM_SPI_INSTANCE               SPI1
/* BlueNRG SPI Clock */
#define DTM_SPI_CLK_ENABLE()	       LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1)

// SPI Configuration
#define DTM_SPI_DATASIZE	       LL_SPI_DATAWIDTH_8BIT
#define DTM_SPI_CLKPOLARITY	       LL_SPI_POLARITY_LOW
#define DTM_SPI_CLKPHASE	       LL_SPI_PHASE_2EDGE
#define DTM_SPI_FIRSTBIT	       LL_SPI_MSB_FIRST

/* BlueNRG-1 SPI slave mode maximum baudrate is 1 MHz */
#ifdef STM32L476xx
#define DTM_SPI_BAUDRATEPRESCALER     LL_SPI_BAUDRATEPRESCALER_DIV128
#endif
#ifdef STM32L152xE
#define DTM_SPI_BAUDRATEPRESCALER     LL_SPI_BAUDRATEPRESCALER_DIV32
#endif


/* BlueNRG SPI SCLK define values */
#define DTM_SPI_SCLK_PIN              LL_GPIO_PIN_3
#define DTM_SPI_SCLK_PORT	      GPIOB
#ifdef STM32L476xx
#define DTM_SPI_SCLK_CLK_ENABLE()     LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#endif
#ifdef STM32L152xE
#define DTM_SPI_SCLK_CLK_ENABLE()     LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)
#endif

/* Defines for MISO pin */
#define DTM_SPI_MISO_PIN            LL_GPIO_PIN_6
#define DTM_SPI_MISO_PORT	    GPIOA
#ifdef STM32L476xx
#define DTM_SPI_MISO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define DTM_SPI_MISO_CLK_ENABLE()   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif


/* Defines for MOSI pin */
#define DTM_SPI_MOSI_PIN             LL_GPIO_PIN_7
#define DTM_SPI_MOSI_PORT            GPIOA
#ifdef STM32L476xx
#define DTM_SPI_MOSI_CLK_ENABLE()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define DTM_SPI_MOSI_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif

/* BlueNRG SPI CS define values */
#define DTM_SPI_CS_PIN               LL_GPIO_PIN_1
#define DTM_SPI_CS_PORT              GPIOA
#ifdef STM32L476xx
#define DTM_SPI_CS_CLK_ENABLE()     LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define DTM_SPI_CS_CLK_ENABLE()     LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif

/* BlueNRG SPI IRQ line define values */
#define DTM_SPI_IRQ_PIN              LL_GPIO_PIN_0
#define DTM_SPI_IRQ_PORT	     GPIOA
#ifdef STM32L476xx
#define DTM_SPI_IRQ_CLK_ENABLE()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#endif
#ifdef STM32L152xE
#define DTM_SPI_IRQ_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#endif



// EXTI External Interrupt for SPI
#define DTM_SPI_EXTI_IRQn                       EXTI0_IRQn
#define DTM_SPI_EXTI_IRQHandler                 EXTI0_IRQHandler
#define DTM_SPI_IRQ_SYSCFG_PORT                 LL_SYSCFG_EXTI_PORTA
#define DTM_SPI_IRQ_SYSCFG_LINE                 LL_SYSCFG_EXTI_LINE0
#define DTM_SPI_IRQ_EXTI_LINE                   LL_EXTI_LINE_0
#define DTM_SPI_IRQ_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(DTM_SPI_IRQ_EXTI_LINE)
#define DTM_SPI_IRQ_EXTI_TRIG_ENABLE()          LL_EXTI_EnableRisingTrig_0_31(DTM_SPI_IRQ_EXTI_LINE)


/**
* @}
*/

/**
* @}
*/


/**
* @brief  Initializes the SPI for BlueNRG module
* @param  xSpiMode : Spi mode (Polling or IRQ)
* @retval Status
*/
void SdkEvalSpiInit(void);

uint8_t BlueNRG_SPI_Read(uint8_t *buffer, uint16_t buff_size); 
uint8_t BlueNRG_SPI_Write(uint8_t* data, uint16_t Nb_bytes);

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

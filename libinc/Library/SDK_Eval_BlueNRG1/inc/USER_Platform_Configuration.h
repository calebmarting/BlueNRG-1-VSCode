/**
 * @file    USER_Platform_Configuration.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file contains definitions for BlueNRG-1 Eval Platform 
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
#ifndef __USER_PLATFORM_CONFIGURATION_H
#define __USER_PLATFORM_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"

#ifdef __cplusplus
 extern "C" {
#endif


  
/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007Vx) Buttons definitions
 */
#define PUSH_BUTTON1_PIN                 GPIO_Pin_13
#define PUSH_BUTTON2_PIN                 GPIO_Pin_5


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007Vx) UART definitions
 */
#define SDK_EVAL_UART_TX_PIN          GPIO_Pin_8
#define SDK_EVAL_UART_TX_MODE         Serial1_Mode

#define SDK_EVAL_UART_RX_PIN          GPIO_Pin_11
#define SDK_EVAL_UART_RX_MODE         Serial1_Mode


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007Vx) I2C definitions
 */
#define SDK_EVAL_I2C                  I2C2

#define SDK_EVAL_I2C_DATA_PIN         GPIO_Pin_5
#define SDK_EVAL_I2C_DATA_MODE        Serial0_Mode

#define SDK_EVAL_I2C_CLK_PIN          GPIO_Pin_4
#define SDK_EVAL_I2C_CLK_MODE         Serial0_Mode

#define SDK_EVAL_I2C_IRQ_HANDLER        I2C2_Handler
 
#define SDK_EVAL_I2C_DMA_TX                 DMA_CH7
#define SDK_EVAL_I2C_DMA_TX_ITTC            DMA_FLAG_TC7

#define SDK_EVAL_I2C_DMA_RX                 DMA_CH6
#define SDK_EVAL_I2C_DMA_RX_ITTC            DMA_FLAG_TC6

#define SDK_EVAL_I2C_BASE                   I2C2_BASE


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007Vx) LEDs definitions
 */
#define SDK_EVAL_LED1_PIN                GPIO_Pin_6
#define SDK_EVAL_LED2_PIN                GPIO_Pin_7   
#define SDK_EVAL_LED3_PIN                GPIO_Pin_14 
 


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007Vx) SPI definitions
 */
#define SDK_EVAL_SPI_PERIPH_SENSOR_PIN            GPIO_Pin_1
#define SDK_EVAL_SPI_PERIPH_SENSOR_MODE           Serial0_Mode

#define SDK_EVAL_SPI_PERIPH_MOSI_PIN              GPIO_Pin_2
#define SDK_EVAL_SPI_PERIPH_MOSI_MODE             Serial0_Mode

#define SDK_EVAL_SPI_PERIPH_MISO_PIN              GPIO_Pin_3
#define SDK_EVAL_SPI_PERIPH_MISO_MODE             Serial0_Mode

#define SDK_EVAL_SPI_PERIPH_SCLK_PIN              GPIO_Pin_0
#define SDK_EVAL_SPI_PERIPH_SCLK_MODE             Serial0_Mode

#define SDK_EVAL_IRQ_SENSOR_PIN                   GPIO_Pin_12


#ifdef __cplusplus
}
#endif

#endif /* __USER_PLATFORM_CONFIGURATION_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

/**
 * @file    USER_Platform_Configuration_auto.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file contains definitions for BlueNRG1 kits platforms with auto identification.
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
#ifndef __USER_PLATFORM_CONFIGURATION_AUTO_H
#define __USER_PLATFORM_CONFIGURATION_AUTO_H

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007VX) Buttons definitions
 */

extern const uint32_t s_vectnButtonPinVersion[][2];

#define PUSH_BUTTON1_PIN                    s_vectnButtonPinVersion[SdkEvalGetVersion()-1][0]
#define PUSH_BUTTON2_PIN                    s_vectnButtonPinVersion[SdkEvalGetVersion()-1][1]


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007VX) UART definitions
 */
extern const uint32_t s_UartTxPinVersion[];
extern const uint32_t s_UartTxModeVersion[];
extern const uint32_t s_UartRxPinVersion[];
extern const uint32_t s_UartRxModeVersion[];

#define SDK_EVAL_UART_TX_PIN                s_UartTxPinVersion[SdkEvalGetVersion()-1] 
#define SDK_EVAL_UART_TX_MODE               s_UartTxModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_UART_RX_PIN                s_UartRxPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_UART_RX_MODE               s_UartRxModeVersion[SdkEvalGetVersion()-1]


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007VX) I2C definitions
 */
extern const I2C_Type* s_I2cPeripheralVersion[];
extern const uint32_t s_I2cDataPinVersion[];
extern const uint32_t s_I2cDataModeVersion[];
extern const uint32_t s_I2cClkPinVersion[];
extern const uint32_t s_I2cClkPinModeVersion[];
extern const uint8_t s_I2cIrqVersion[];

extern const DMA_CH_Type* s_I2cDataDmaTxVersion[];
extern const DMA_CH_Type* s_I2cDataDmaRxVersion[];
extern const uint32_t s_I2cDataDmaTxTcVersion[];
extern const uint32_t s_I2cDataDmaRxTcVersion[];

extern const uint32_t s_I2CBaseVersion[];

#define SDK_EVAL_I2C                        s_I2cPeripheralVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_DATA_PIN               s_I2cDataPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_I2C_DATA_MODE              s_I2cDataModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_CLK_PIN                s_I2cClkPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_I2C_CLK_MODE               s_I2cClkPinModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_DMA_TX                 s_I2cDataDmaTxVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_I2C_DMA_TX_ITTC            s_I2cDataDmaTxTcVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_DMA_RX                 s_I2cDataDmaRxVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_I2C_DMA_RX_ITTC            s_I2cDataDmaRxTcVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_BASE                   s_I2CBaseVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_I2C_IRQ                    s_I2cIrqVersion[SdkEvalGetVersion()-1]

/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007VX) LEDs definitions
 */
extern const uint32_t s_vectnGpioPinVersion[][3];

#define SDK_EVAL_LED1_PIN                   s_vectnGpioPinVersion[SdkEvalGetVersion()-1][0]
#define SDK_EVAL_LED2_PIN                   s_vectnGpioPinVersion[SdkEvalGetVersion()-1][1]   
#define SDK_EVAL_LED3_PIN                   s_vectnGpioPinVersion[SdkEvalGetVersion()-1][2] 
 


/**
 * @brief BlueNRG1 Eval Board (STEVAL-IDB007VX) SPI definitions
 */
extern const uint32_t s_SpiCs1PinVersion[];
extern const uint32_t s_SpiCs1ModeVersion[];
extern const uint32_t s_SpiCs2PinVersion[];
extern const uint32_t s_SpiCs2ModeVersion[];
extern const uint32_t s_SpiOutPinVersion[];
extern const uint32_t s_SpiOutModeVersion[];
extern const uint32_t s_SpiInPinVersion[];
extern const uint32_t s_SpiInModeVersion[];
extern const uint32_t s_SpiSclkPinVersion[];
extern const uint32_t s_SpiSclkModeVersion[];
extern const uint32_t s_SpiIrqSensorPinVersion[];

#define SDK_EVAL_SPI_PERIPH_SENSOR_PIN      s_SpiCs1PinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_SPI_PERIPH_SENSOR_MODE     s_SpiCs1ModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_SPI_PERIPH_OUT_PIN        s_SpiOutPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_SPI_PERIPH_OUT_MODE       s_SpiOutModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_SPI_PERIPH_IN_PIN        s_SpiInPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_SPI_PERIPH_IN_MODE       s_SpiInModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_SPI_PERIPH_SCLK_PIN        s_SpiSclkPinVersion[SdkEvalGetVersion()-1]
#define SDK_EVAL_SPI_PERIPH_SCLK_MODE       s_SpiSclkModeVersion[SdkEvalGetVersion()-1]

#define SDK_EVAL_IRQ_SENSOR_PIN             s_SpiIrqSensorPinVersion[SdkEvalGetVersion()-1]

   
#ifdef __cplusplus
}
#endif

#endif /* __USER_PLATFORM_CONFIGURATION_AUTO_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

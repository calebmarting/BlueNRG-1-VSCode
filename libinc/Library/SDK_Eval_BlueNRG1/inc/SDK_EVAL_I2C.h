/**
 * @file    SDK_EVAL_I2C.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage I2C interface for eval board.
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
#ifndef __SDK_EVAL_I2C_H
#define __SDK_EVAL_I2C_H

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

/** @addtogroup SDK_EVAL_I2C            SDK EVAL I2C
 * @{
 */

/** @defgroup SDK_EVAL_I2C_Exported_Types               SDK EVAL I2C Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_I2C_Exported_Constants             SDK EVAL I2C Exported Constants
 * @{
 */


/**
 * @brief  I2C buffer max size
 */
#define I2C_BUFF_SIZE                   (10)

/**
 * @brief  I2C_TX DMA channel
 */
#define DMA_CH_I2C_TX                   SDK_EVAL_I2C_DMA_TX

/**
 * @brief  I2C_TX DMA Transfer Complete Interrupt
 */
#define DMA_CH_I2C_TX_IT_TC             SDK_EVAL_I2C_DMA_TX_ITTC

/**
 * @brief  I2C_RX DMA channel
 */
#define DMA_CH_I2C_RX                   SDK_EVAL_I2C_DMA_RX

/**
 * @brief  I2C_RX DMA Transfer Complete Interrupt
 */
#define DMA_CH_I2C_RX_IT_TC             SDK_EVAL_I2C_DMA_RX_ITTC



/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_I2C_Exported_Macros                       SDK EVAL I2C Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_I2C_Exported_Functions                   SDK EVAL I2C Exported Functions
 * @{
 */

ErrorStatus SdkEvalI2CInit(uint32_t baudrate);
ErrorStatus SdkEvalI2CWrite(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToWrite);
ErrorStatus SdkEvalI2CRead(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToRead);

ErrorStatus SdkEvalI2CDmaInit(uint32_t baudrate);
ErrorStatus SdkEvalI2CDmaWrite(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToWrite);
ErrorStatus SdkEvalI2CDmaRead(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToRead);
void DmaI2CHandlerCallback(void);

ErrorStatus SdkEvalI2CIrqInit(uint32_t baudrate);
ErrorStatus SdkEvalI2CIrqWrite(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToWrite);
ErrorStatus SdkEvalI2CIrqRead(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t NumByteToRead);
void IrqI2CHandlerCallback(void);

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

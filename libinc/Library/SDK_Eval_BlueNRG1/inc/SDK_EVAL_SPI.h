/**
 * @file    SDK_EVAL_SPI.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage SPI interface for eval board.
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
#ifndef __SDK_EVAL_SPI_H
#define __SDK_EVAL_SPI_H

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

/** @addtogroup SDK_EVAL_SPI            SDK EVAL SPI
 * @brief Management of SDK eval board SPI.
 * @{
 */

/** @defgroup SDK_EVAL_SPI_Exported_Types               SDK EVAL SPI Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_SPI_Exported_Constants                           SDK EVAL SPI Exported Constants
 * @{
 */

/**
  * @brief SPI Baudrate
  */
#define SPI_BAUDRATE                    (1000000)
   
/**
 * @brief  SPI DR address
 */
#define SPI_DR_BASE_ADDR                (SPI_BASE + 0x08)

/**
 * @brief  SPI buffer max size
 */
#define SPI_BUFF_SIZE                   (10)

/**
 * @brief  SPI_TX DMA channel
 */
#define DMA_CH_SPI_TX                   (DMA_CH5)

/**
 * @brief  SPI_TX DMA Transfer Complete Interrupt
 */
#define DMA_CH_SPI_TX_IT_TC             (DMA_FLAG_TC5)

/**
 * @brief  SPI_RX DMA channel
 */
#define DMA_CH_SPI_RX                   (DMA_CH4)

/**
 * @brief  SPI_RX DMA Transfer Complete Interrupt
 */
#define DMA_CH_SPI_RX_IT_TC             (DMA_FLAG_TC4)
   
/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_SPI_Exported_Macros                       SDK EVAL SPI Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_SPI_Exported_Functions                   SDK EVAL SPI Exported Functions
 * @{
 */

/**
 * @brief  Put the CS pin high
 */
#define SDK_EVAL_SPI_CS_HIGH()                  GPIO_SetBits(SDK_EVAL_SPI_PERIPH_SENSOR_PIN)

/**
 * @brief  Put the CS pin low
 */
#define SDK_EVAL_SPI_CS_LOW()                   GPIO_ResetBits(SDK_EVAL_SPI_PERIPH_SENSOR_PIN)


ErrorStatus SdkEvalSpiInit(uint32_t baudrate);
ErrorStatus SdkEvalSpiRead(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToRead);
ErrorStatus SdkEvalSpiWrite(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToWrite);

ErrorStatus SdkEvalSpiDmaInit(uint32_t baudrate);
ErrorStatus SdkEvalSpiDmaRead(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToRead);
ErrorStatus SdkEvalSpiDmaWrite(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToWrite);
void DmaSpiHandlerCallback(void);

ErrorStatus SdkEvalSpiIrqInit(uint32_t baudrate);
ErrorStatus SdkEvalSpiIrqRead(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToRead);
ErrorStatus SdkEvalSpiIrqWrite(uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToWrite);
void IrqSpiHandlerCallback(void);

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

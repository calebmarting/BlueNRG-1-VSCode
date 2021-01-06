/**
 * @file    LSM6DS3_hal.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage LSM6DS3 inertial sensor of eval board.
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
#ifndef __LSM6DS3_HAL_H
#define __LSM6DS3_HAL_H

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_SPI.h"
#include "LSM6DS3.h"
   
   
#ifdef __cplusplus
 extern "C" {
#endif

   

/** @addtogroup SDK_EVAL_BlueNRG1   SDK EVAL BlueNRG1
 * @{
 */

/** @addtogroup LSM6DS3            LSM6DS3
 * @{
 */

/** @addtogroup SDK_EVAL_LSM6DS3_HAL            SDK EVAL LSM6DS3_HAL
 * @{
 */
    
/** @defgroup SDK_EVAL_LSM6DS3_HAL_Exported_Types               SDK EVAL LSM6DS3_HAL Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_LSM6DS3_HAL_Exported_Constants                           SDK EVAL LSM6DS3_HAL Exported Constants
 * @{
 */

   
/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_LSM6DS3_HAL_Exported_Macros                       SDK EVAL LSM6DS3_HAL Exported Macros
 * @{
 */

#define LSM6DS3_IRQ_PIN                      SDK_EVAL_IRQ_SENSOR_PIN
#define LSM6DS3_SPI_FREQUENCY                (8000000)

/**
 * @}
 */

/** @defgroup SDK_EVAL_LSM6DS3_HAL_Exported_Functions                   SDK EVAL LSM6DS3_HAL Exported Functions
 * @{
 */

   
   
#ifdef BLUENRG1_SPI_DMA
   
#define LSM6DS3_IO_Init()                                                       (IMU_6AXES_StatusTypeDef)SdkEvalSpiDmaInit(LSM6DS3_SPI_FREQUENCY)
#define LSM6DS3_IO_Write(pBuffer, DeviceAddr, RegisterAddr, NumByteToWrite)     (IMU_6AXES_StatusTypeDef)SdkEvalSpiDmaWrite(pBuffer, RegisterAddr, NumByteToWrite)
#define LSM6DS3_IO_Read(pBuffer, DeviceAddr, RegisterAddr, NumByteToRead)       (IMU_6AXES_StatusTypeDef)SdkEvalSpiDmaRead(pBuffer, RegisterAddr, NumByteToRead)

#else
#ifdef BLUENRG1_SPI_IRQ
   
#define LSM6DS3_IO_Init()                                                       (IMU_6AXES_StatusTypeDef)SdkEvalSpiIrqInit(LSM6DS3_SPI_FREQUENCY)
#define LSM6DS3_IO_Write(pBuffer, DeviceAddr, RegisterAddr, NumByteToWrite)     (IMU_6AXES_StatusTypeDef)SdkEvalSpiIrqWrite(pBuffer, RegisterAddr, NumByteToWrite)
#define LSM6DS3_IO_Read(pBuffer, DeviceAddr, RegisterAddr, NumByteToRead)       (IMU_6AXES_StatusTypeDef)SdkEvalSpiIrqRead(pBuffer, RegisterAddr, NumByteToRead)

#else /* BLUENRG1_SPI_POLL */
   
#define LSM6DS3_IO_Init()                                                       (IMU_6AXES_StatusTypeDef)SdkEvalSpiInit(LSM6DS3_SPI_FREQUENCY)
#define LSM6DS3_IO_Write(pBuffer, DeviceAddr, RegisterAddr, NumByteToWrite)     (IMU_6AXES_StatusTypeDef)SdkEvalSpiWrite(pBuffer, RegisterAddr, NumByteToWrite)
#define LSM6DS3_IO_Read(pBuffer, DeviceAddr, RegisterAddr, NumByteToRead)       (IMU_6AXES_StatusTypeDef)SdkEvalSpiRead(pBuffer, RegisterAddr, NumByteToRead)

#endif /* BLUENRG1_SPI_IRQ */
#endif /* BLUENRG1_SPI_DMA */
   
/* Configures IRQ pin */
#define LSM6DS3_IO_ITConfig()                                               {SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE); \
                                                                             GPIO_Init(&(GPIO_InitType){LSM6DS3_IRQ_PIN, GPIO_Input, DISABLE, DISABLE}); \
                                                                             NVIC_Init(&(NVIC_InitType){GPIO_IRQn, LOW_PRIORITY, ENABLE}); \
                                                                             GPIO_EXTIConfig(&(GPIO_EXTIConfigType){LSM6DS3_IRQ_PIN, GPIO_IrqSense_Edge, IRQ_ON_FALLING_EDGE}); \
                                                                             GPIO_ClearITPendingBit(LSM6DS3_IRQ_PIN); \
                                                                             GPIO_EXTICmd(LSM6DS3_IRQ_PIN, ENABLE);}

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

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

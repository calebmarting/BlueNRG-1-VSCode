/**
 * @file    SDK_EVAL_Io.h
 * @author  AMS VMA RF application team
 * @version V1.0.1
 * @date    March 17 2015
 * @brief   This file contains definitions for Software Development Kit eval board I/O ports.
 * @details
 *
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
#ifndef __SDK_EVAL_IO_H
#define __SDK_EVAL_IO_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L476xx
#include "stm32l476xx.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx.h"
#endif

#ifdef ENABLE_USB
#include "usbd_cdc_interface.h"
#endif
#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
 * @{
 */


/** @addtogroup SDK_EVAL_Io           SDK EVAL IO
 * @brief Management of Software Development Kit eval board I/O ports.
 * @details See the file <i>@ref SDK_EVAL.Io.h</i> for more details.
 * @{
 */


/** @defgroup SDK_EVAL_Io_Exported_Types               SDK EVAL IO Exported Types
 * @{
 */
#ifdef ENABLE_USB
extern USBD_HandleTypeDef USBD_Device;
#endif
   
/**
 * @brief  IO ports for SDK EVAL enumeration
 */
typedef enum
{
  COM1 = 0

} SdkEvalCom;

/**
 * @brief  IO_Receive_Data User callback type 
 */
typedef void (* IO_RECEIVE_DATA_USER_CALLBACK_TYPE) (uint8_t * rx_data, uint16_t data_size);

/**
 * @}
 */

/** @defgroup SDK_EVAL_Io_Exported_Constants           SDK EVAL IO Exported Constants
 * @{
 */


/**
 * @}
 */


/** @defgroup SDK_EVAL_Io_Exported_Macros              SDK EVAL IO Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Io_Exported_Functions           SDK EVAL IO Exported Functions
 * @{
 */
void SdkEval_IO_Uart_Irq_Handler(void);
void SdkEval_IO_Config(IO_RECEIVE_DATA_USER_CALLBACK_TYPE io_receive_function);
void SdkEval_IO_Send_Data(uint8_t read_data);
void SdkEval_IO_Receive_Data(uint8_t * rx_data, uint16_t data_size);
void SdkEval_IO_processInputData(uint8_t * rx_data, uint16_t data_size);
void SdkEvalComUartInit(uint32_t baudrate);

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

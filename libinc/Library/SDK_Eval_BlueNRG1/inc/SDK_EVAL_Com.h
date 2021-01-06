/**
 * @file    SDK_EVAL_Com.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage UART interface for eval board.
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_COM_H
#define __SDK_EVAL_COM_H

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif



/** @addtogroup SDK_EVAL_BlueNRG1               SDK EVAL BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_Com                    SDK EVAL Com
 * @{
 */


/** @defgroup SDK_EVAL_Com_Exported_Types       SDK EVAL Com Exported Types
 * @{
 */


/**
 * @brief  IO_Receive_Data User callback type 
 */
typedef void (* IO_RECEIVE_DATA_USER_CALLBACK_TYPE) (uint8_t * rx_data, uint16_t data_size);


/**
 * @brief  Default value for UART baudrate
 */
#ifndef UART_BAUDRATE
#define UART_BAUDRATE            (115200)
#endif

/**
 * @}
 */

/** @defgroup SDK_EVAL_Com_Exported_Constants        SDK EVAL Com Exported Constants
 * @{
 */


/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Exported_Macros           SDK EVAL Com Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Exported_Functions        SDK EVAL Com Exported Functions
 * @{
 */
    
void SdkEvalComUartInit(uint32_t baudrate);
void SdkEvalComUartIrqConfig(FunctionalState EnableIrq);
void SdkEvalComIOUartIrqHandler(void);
void SdkEvalComIOConfig(IO_RECEIVE_DATA_USER_CALLBACK_TYPE io_receive_function);
void SdkEvalComIOSendData(uint8_t read_data);
void SdkEvalComIOReceiveData(uint8_t * rx_data, uint16_t data_size);
uint8_t SdkEvalComIOTxFifoNotEmpty(void);
uint8_t SdkEvalComUARTBusy(void);
void SdkEvalComIOProcessInputData(uint8_t * rx_data, uint16_t data_size);
uint8_t __io_getcharNonBlocking(uint8_t* data);

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

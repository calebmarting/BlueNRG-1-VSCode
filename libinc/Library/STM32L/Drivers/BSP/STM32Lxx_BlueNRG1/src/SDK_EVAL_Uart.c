/**
* @file    SDK_EVAL_UART_Driver.c
* @author  AMS VMA RF application team
* @version V1.0.0
* @date    February 28, 2017
* @brief   This file provides all the low level UART API to access to BlueNRG module
* @details It uses STM32L1 Cube APIs + Optimized APIs for UART Transmit, Receive, 
*          Transmit & Receive (these APIs are needed for BLE communication
*          throughput) 
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
*
*/

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "osal.h"

/** @addtogroup SDK_EVAL_UART_STM32L
* @{
*/

/** @addtogroup SDK_EVAL_Uart                    SDK EVAL Uart
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library UART interface functions. 
* @{
*/

/** @defgroup STM32_BLUENRG_BLE_Private_Defines 
* @{
*/ 


/**
* @}
*/

/** @defgroup UART_Private_Variables
* @{
*/


uint8_t sdk_buffer[SDK_BUFFER_SIZE];

/**
* @}
*/

/** @defgroup UART_Functions
* @{
*/


/**
 * @brief  Configure the selected I/O communication channel
 * @param  None.
 * @retval None.
 */
void DTM_IO_Config(void)
{  
  DTM_USART_GPIO_CLK_ENABLE();
  
  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(DTM_USART_TX_GPIO_PORT, DTM_USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  DTM_USART_TX_AF();
  LL_GPIO_SetPinSpeed(DTM_USART_TX_GPIO_PORT, DTM_USART_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(DTM_USART_TX_GPIO_PORT, DTM_USART_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(DTM_USART_TX_GPIO_PORT, DTM_USART_TX_PIN, LL_GPIO_PULL_UP);
  
  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(DTM_USART_RX_GPIO_PORT, DTM_USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  DTM_USART_RX_AF();
  LL_GPIO_SetPinSpeed(DTM_USART_RX_GPIO_PORT, DTM_USART_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(DTM_USART_RX_GPIO_PORT, DTM_USART_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(DTM_USART_RX_GPIO_PORT, DTM_USART_RX_PIN, LL_GPIO_PULL_UP);
  
  /* Init USART2 port */
  DTM_USART_CLK_ENABLE();
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  LL_USART_Disable(DTM_USART);
  
  LL_USART_SetTransferDirection(DTM_USART, LL_USART_DIRECTION_TX_RX);
  LL_USART_ConfigCharacter(DTM_USART, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
  
#ifdef DTM_UART_HW_FLOW_CTRL
  LL_USART_SetHWFlowCtrl(DTM_USART, LL_USART_HWCONTROL_RTS_CTS);
#else
  LL_USART_SetHWFlowCtrl(DTM_USART, LL_USART_HWCONTROL_NONE);
#endif
  
  LL_USART_SetBaudRate(DTM_USART, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); 
  
  /* Small delay to avoid issue during reset of the BlueNRG-1 TO CHECK
  * because the UART lines during reset can generates UART Error */
  for(volatile uint32_t i=0; i<0xFFFFF; i++);
  
  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(DTM_USART);
  
  /* NVIC Configuration for USART interrupts */
  NVIC_SetPriority(DTM_USART_IRQn, 4);    // TO CHECK the 4
  NVIC_EnableIRQ(DTM_USART_IRQn);
  
  /* Enable IRQ for reception  */
  LL_USART_EnableIT_RXNE(DTM_USART);
  
#ifdef DTM_UART_HW_FLOW_CTRL
  /* Configure the RTS pin as OUTPUT high level */
  DTM_UART_RTS_OUTPUT_HIGH();
  LL_GPIO_SetPinMode(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_PULL_UP);
  DTM_USART_RTS_AF();
  
  /* Configure the CTS as INPUT IRQ */
  LL_GPIO_SetPinMode(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinOutputType(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_PULL_UP);
  DTM_USART_CTS_AF();
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_SYSCFG_SetEXTISource(DTM_USART_CTS_SYSCFG_PORT, DTM_USART_CTS_SYSCFG_LINE);
  
  DTM_USART_CTS_EXTI_LINE_ENABLE();
  DTM_USART_CTS_EXTI_TRIG1_ENABLE();
  DTM_USART_CTS_EXTI_TRIG2_ENABLE();
  
  NVIC_EnableIRQ(DTM_USART_EXTI_IRQn); 
  NVIC_SetPriority(DTM_USART_EXTI_IRQn, 4);
#endif  
}/* end SdkEval_IO_Config() */


/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void DTM_Config_GPIO_Output_RTS(void)
{
  LL_GPIO_SetPinMode(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_MODE_OUTPUT);
  
}



void DTM_Config_UART_RTS(void)
{
  LL_GPIO_SetPinMode(DTM_USART_RTS_GPIO_PORT, DTM_USART_RTS_PIN, LL_GPIO_MODE_ALTERNATE);
}


/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void DTM_Config_GPIO_InputIrq_CTS(void)
{
  LL_GPIO_SetPinMode(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_MODE_INPUT);
  NVIC_EnableIRQ(DTM_USART_EXTI_IRQn);
}


void DTM_Config_UART_CTS(void)
{
  NVIC_DisableIRQ(DTM_USART_EXTI_IRQn);
  
  LL_GPIO_SetPinMode(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_MODE_ALTERNATE);
}


void DTM_UART_CTS_Input(void)
{  
  LL_GPIO_SetPinMode(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN, LL_GPIO_MODE_INPUT);
}


void Hal_Write_Serial(const void* data1, const void* data2, uint16_t n_bytes1, uint16_t n_bytes2)
{  
  Osal_MemCpy(sdk_buffer, data1, n_bytes1);
  Osal_MemCpy(&sdk_buffer[n_bytes1], data2, n_bytes2);

#ifdef DTM_UART_HW_FLOW_CTRL
  while(LL_GPIO_IsInputPinSet(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN) == 0);
  
  DTM_Config_UART_CTS();
  DTM_UART_RTS_OUTPUT_LOW();
#endif
  
  for(uint16_t i=0; i<(n_bytes1+n_bytes2); i++) {
    LL_USART_TransmitData8(DTM_USART, sdk_buffer[i]);
    while(LL_USART_IsActiveFlag_TXE(DTM_USART) == 0);
  }
  while(LL_USART_IsActiveFlag_TC(DTM_USART) == 0);
  
#ifdef DTM_UART_HW_FLOW_CTRL
  DTM_UART_RTS_OUTPUT_HIGH();
  DTM_UART_CTS_Input();
  
  while(LL_GPIO_IsInputPinSet(DTM_USART_CTS_GPIO_PORT, DTM_USART_CTS_PIN) == 0);
  
  NVIC_EnableIRQ(DTM_USART_EXTI_IRQn);
#endif
}


/**
* @}
*/


/**
* @}
*/


/**
* @}
*/



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

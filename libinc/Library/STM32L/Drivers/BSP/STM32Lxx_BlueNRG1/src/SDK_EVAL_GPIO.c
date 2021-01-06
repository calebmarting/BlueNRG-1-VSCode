/**
* @file    SDK_EVAL_Gpio.c
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
* <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
*
*/

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"

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

/**
* @}
*/

/** @defgroup UART_Functions
* @{
*/


/**
 * @brief  Configure the selected GPIO
 * @param  None.
 * @retval None.
 */
void Sdk_Eval_Reset_Pin_Init(void)
{    
  /* Reset pin */
  BNRG_RESET_CLK_ENABLE();

  LL_GPIO_SetPinMode(BNRG_RESET_PORT, BNRG_RESET_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(BNRG_RESET_PORT, BNRG_RESET_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(BNRG_RESET_PORT, BNRG_RESET_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(BNRG_RESET_PORT, BNRG_RESET_PIN, LL_GPIO_PULL_NO);
  
  BlueNRG_Power_Down();	/*Added to avoid spurious interrupt from the BlueNRG */
  
}


/**
* @brief  Reset the BlueNRG
* @param  None
* @retval None
*/
void BlueNRG_RST(void)
{   
  LL_GPIO_ResetOutputPin(BNRG_RESET_PORT, BNRG_RESET_PIN);
  LL_mDelay(5);
  LL_GPIO_SetOutputPin(BNRG_RESET_PORT, BNRG_RESET_PIN);
  LL_mDelay(150);
  
}


/**
* @brief  Power down the BlueNRG (reset).
* @param  None
* @retval None
*/
void BlueNRG_Power_Down(void)
{
  LL_GPIO_ResetOutputPin(BNRG_RESET_PORT, BNRG_RESET_PIN);
  
}

/**
* @brief  Power up the BlueNRG (release reset).
* @param  None
* @retval None
*/
void BlueNRG_Power_Up(void)
{
  LL_GPIO_SetOutputPin(BNRG_RESET_PORT, BNRG_RESET_PIN);
  LL_mDelay(15);
}


/**
* @brief  Activate internal bootloader using pin.
* @param  None
* @retval None
*/
void BlueNRG_HW_Bootloader(void)
{
  /* Configure MOSI pin for bootloader */
  LL_GPIO_SetPinMode(BNRG_BOOT_PORT, BNRG_BOOT_PIN, LL_GPIO_MODE_OUTPUT);
  
  // high MISO pin and reset BlueNRG-1
  LL_GPIO_SetOutputPin(BNRG_BOOT_PORT, BNRG_BOOT_PIN);

  BlueNRG_RST();
}


/**
* @brief  Activate internal bootloader using pin.
* @param  None
* @retval None
*/
void BlueNRG_HW_Updater(void)
{
#ifdef SPI_INTERFACE
  Disable_IRQ();
#endif
  
  BNRG_SPI_MOSI_BOOT_CLK_ENABLE();
  
  /* Configure MOSI pin for bootloader */
  LL_GPIO_SetPinMode(BNRG_SPI_MOSI_BOOT_PORT, BNRG_SPI_MOSI_BOOT_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(BNRG_SPI_MOSI_BOOT_PORT, BNRG_SPI_MOSI_BOOT_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  
  // high MISO pin and reset BlueNRG-1
  LL_GPIO_SetOutputPin(BNRG_SPI_MOSI_BOOT_PORT, BNRG_SPI_MOSI_BOOT_PIN);
  
  BlueNRG_RST();
  
#ifdef SPI_INTERFACE
  
  /* Configure MOSI Pin connected to pin 29 of CN10 connector */
  LL_GPIO_SetPinMode(DTM_SPI_MOSI_PORT, DTM_SPI_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
  Enable_IRQ();
#endif
  
}


/**
* @brief  Activate the pin BNRG_SELECT_PIN used to indicate the communication
*         interface with the BlueNRG. This feature must be available.
* @param  None
* @retval None
*/
void BlueNRG_Activate_Select_Pin(void)
{
  /* Enable the Select Pin for the communication interface, if supported */
  BNRG_SELECT_CLK_ENABLE();
  LL_GPIO_SetPinMode(BNRG_SELECT_PORT, BNRG_SELECT_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(BNRG_SELECT_PORT, BNRG_SELECT_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(BNRG_SELECT_PORT, BNRG_SELECT_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(BNRG_SELECT_PORT, BNRG_SELECT_PIN, LL_GPIO_PULL_NO);
  
#ifdef SPI_INTERFACE
  LL_GPIO_ResetOutputPin(BNRG_SELECT_PORT, BNRG_SELECT_PIN);
#endif
#ifdef UART_INTERFACE
  LL_GPIO_SetOutputPin(BNRG_SELECT_PORT, BNRG_SELECT_PIN);
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



/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

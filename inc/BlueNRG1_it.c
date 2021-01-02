/**
******************************************************************************
* @file    GPIO/IOToggle/BlueNRG1_it.c 
* @author  RF Application Team
* @version V1.0.0
* @date    September-2015
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and
*          peripherals interrupt service routine.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"


/** @addtogroup BlueNRG1_StdPeriph_Examples
* @{
*/

/** @addtogroup GPIO_Examples
* @{
*/ 

/** @addtogroup GPIO_InputInterrupt
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t lSystickCounter;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
*/
void NMI_Handler(void)
{
  while(1);
}

/**
* @brief  This function handles Hard Fault exception.
*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1);
}


/**
* @brief  This function handles SVCall exception.
*/
void SVC_Handler(void)
{
}


/**
* @brief  This function handles SysTick Handler.
*/
void SysTick_Handler(void)
{
  lSystickCounter++;
}

/******************************************************************************/
/*                 BlueNRG-1 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (system_bluenrg1.c).                                               */
/******************************************************************************/


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : low_power.c
* Author             : AMS - VMA RF Application team 
* Version            : V1.0.0
* Date               : 24-June-2015
* Description        : Functions for low power management
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef SYSCLK_MSI 

/* Includes ------------------------------------------------------------------*/

#include "hal.h"
#include "SDK_EVAL_Config.h"
#include "compiler.h"

WEAK_FUNCTION(void User_Timer_Enter_Sleep(void));
WEAK_FUNCTION(void User_Timer_Exit_Sleep(void));
WEAK_FUNCTION(uint8_t App_SleepMode_Check(void));

/**
  * @brief  System Power Configuration for Low Power mode
  * @note: It configure all the GPIOs on Analog Input mode and disable clocks
  * @retval None
  */
void SystemPower_Config(void)
{
  /* Enable GPIOs clock */
#ifdef STM32L476xx
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
#endif 
#ifdef STM32L152xE
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
#endif 
  
  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOH, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOH, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  
#ifdef STM32L152xE
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOF, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinPull(GPIOG, LL_GPIO_PIN_ALL, LL_GPIO_PULL_NO);
#endif
  
  /* Disable GPIOs clock */
#ifdef STM32L476xx
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
#endif 
#ifdef STM32L152xE
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
#endif
  
}

/**
  * @brief  Configure HCKL
  * @param  RCC_SYSCLK: HCLK frequency , 
  * @retval None
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  
  tmpreg = RCC->CFGR;
  
  /* Clear HPRE[3:0] bits */
  tmpreg &= ~RCC_CFGR_HPRE;
  
  /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
  tmpreg |= RCC_SYSCLK;
  
  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Change MSI frequency
  * @param  freq: frequency range, 
  * @param  div2: if FALSE, HCLK will be SYSCLK, otherwise SYSCLK/2.
  * @retval None
  */
void ChangeMSIClock(uint32_t freq, BOOL div2)
{     
  /* To configure the MSI frequency */
  LL_RCC_MSI_SetRange(freq);
  
  if (div2)
  {
    RCC_HCLKConfig(RCC_CFGR_HPRE_DIV2); //TBR
    
  }
  else {
    RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1); //TBR
  }
}

/**
  * @brief  Put the micro in sleep mode
  * @note   This function puts the micro in a sleep mode without disabling timer
  *         clock. It is called when a fast wake-up is needed.
  *         To be called only inside an atomic section.
  * @retval None
  */
void Enter_Sleep_Mode(void)
{    
  /* Request Wait For Interrupt */
//  LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
  
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP0);
  
  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  
  __WFI();
}


/**
  * @brief  Put the micro in low power sleep mode
  * @note   This function puts the micro in a low power sleep mode without disabling timer
  *         clock. This function assumes that the system clock source is already MSI.
  * @retval None
  */
void Enter_LP_Sleep_Mode(void)
{   
  __disable_irq();
  
#ifndef SENSORDEMO_NWK
  /* Enable the power down mode during Sleep mode */
  LL_FLASH_EnableSleepPowerDown();
#endif

  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
  
#ifndef SENSORDEMO_NWK
  User_Timer_Enter_Sleep();
  
  /* Switch in MSI 65KHz, HCLK 32kHz */
  ChangeMSIClock(LL_RCC_MSIRANGE_0, TRUE);
#endif
  
  /* Enter Sleep Mode for Interrupt  */
  __enable_irq();
  Enter_Sleep_Mode();
  __disable_irq();
  
#ifndef SENSORDEMO_NWK
  /* Switch in MSI 4MHz, HCLK 4MHz */
  ChangeMSIClock(LL_RCC_MSIRANGE_6, FALSE);

  User_Timer_Exit_Sleep();
#endif

  /* Resume Tick interrupt if disabled prior to sleep mode entry */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
  
  __enable_irq();
}

void System_Sleep(void)
{
  if(App_SleepMode_Check()) {
    Enter_LP_Sleep_Mode();
  }
}
#endif /* SYSCLK_MSI */

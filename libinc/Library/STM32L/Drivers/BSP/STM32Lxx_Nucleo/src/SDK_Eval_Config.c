/**
 * @file    SDK_EVAL_Io.c
 * @author  AMS VMA RF application team
 * @version V1.0.1
 * @date    March,17 2015
 * @brief   This file provides all the low level API to manage SDK I/O ports.
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
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "low_power.h"


/** @addtogroup SDK_EVAL_STM32L
 * @{
 */


/** @addtogroup SDK_EVAL_Config
 * @{
 */


/** @defgroup SDK_EVAL_Config_Private_Defines                      SDK EVAL Config Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Private_Macros                       SDK EVAL Config Private Macros
 * @{
 */



/**
 * @}
 */

/** @defgroup SDK_EVAL_Config_Private_Variables                    SDK EVAL Config Private Variables
 * @{
 */



/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Private_FunctionPrototypes                   SDK EVAL Config Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Config_Private_Functions                            SDK EVAL Config Private Functions
 * @{
 */


/**
  * @brief  System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if (HCLK_32MHZ == 1)
  /* Enable ACC64 access and set FLASH latency */
  
#ifdef STM32L152xE
  LL_FLASH_Enable64bitAccess();
#endif
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  LL_RCC_HSI_SetCalibTrimming(0x10);

  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
  
  /* Main PLL configuration and activation */
#ifdef STM32L476xx
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
#endif
#ifdef STM32L152xE
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_12, LL_RCC_PLL_DIV_4); // LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);
#endif
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  
#ifdef STM32L476xx
  SysTick_Config(80000000/1000);
  LL_SetSystemCoreClock(80000000);
#endif
#ifdef STM32L152xE
  SysTick_Config(48000000/1000);   // 32000000/1000
  LL_SetSystemCoreClock(48000000); // 32000000
#endif
#endif
  
  
#if (SYSCLK_MSI == 1)
  /* Set FLASH latency */ 
#ifdef STM32L152xE
  LL_FLASH_Enable64bitAccess();
#endif
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  /* Set MSI Range */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  
  /* Enable MSI if not already activated*/
  if (LL_RCC_MSI_IsReady() == 0)
  {
    /* MSI configuration and activation */
    LL_RCC_MSI_Enable();
    while(LL_RCC_MSI_IsReady() != 1)
    {
    };
  }
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1);

  SysTick_Config(4000000/1000);
  LL_SetSystemCoreClock(4000000);
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


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/

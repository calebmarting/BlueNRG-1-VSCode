/**
******************************************************************************
* @file    clock.c 
* @author  CL
* @version V1.0.0
* @date    04-July-2014
* @brief   
******************************************************************************
* @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "clock.h"

#ifdef STM32L476xx
#include "stm32l4xx_ll_utils.h"
#endif
#ifdef STM32L152xE
#include "stm32l1xx_ll_utils.h"
#endif

const uint32_t CLOCK_SECOND = 1000;
static uint32_t clock_tick = 0;

/**
 * @brief  Clock_Init
 * @param  None
 * @retval None
 */
void Clock_Init(void)
{
  
}

/**
 * @brief  Clock_Time
 * @param  None
 * @retval tClockTime
 */
uint32_t Clock_Time(void)
{
  return clock_tick;
}

/**
 * @brief  Clock_Wait Wait for a multiple of 1 ms.
 * @param  int i
 * @retval None
 */
void Clock_Wait(uint32_t i)
{
  LL_mDelay(i);
}


void Clock_Inc_Tick(void)
{
  clock_tick++;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


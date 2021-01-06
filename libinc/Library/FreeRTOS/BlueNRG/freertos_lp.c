/**
  ******************************************************************************
  * @file    freertos_lp.c 
  * @author  AMS RF Application Team
  * @version V1.0.0
  * @date    October-2018
  * @brief   Support ot tickless idle functionality of FreeRTOS for BlueNRG
  *          application processors.
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
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM CM3 port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "bluenrg1_stack.h"   
#include "sleep.h"

uint8_t gpio_wake_bit_mask = 0;
uint8_t gpio_wake_level_mask = 0;


/* The virtual timer used to wakeup the system. It cannot be used by the application. */
#define VTIMER_NUM  3

void Timer_ProcessQ(void);

/*-----------------------------------------------------------*/
#if configUSE_TICKLESS_IDLE
void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
  eSleepModeStatus eSleepStatus;
  uint32_t timeBeforeSleep, timeAfterSleep;
  
  timeBeforeSleep = HAL_VTimerGetCurrentTime_sysT32();
  
  eSleepStatus = eTaskConfirmSleepModeStatus();
  
  /* Start a timer to wake up the system after the given interval if there
    are tasks waiting for a timeout. Since this may take some time, it is not
    called inside the critical section.  */
  if(eSleepStatus == eStandardSleep)
  {
    HAL_VTimer_Stop(VTIMER_NUM);
    HAL_VTimerStart_ms(VTIMER_NUM, (xExpectedIdleTime*1000)/configTICK_RATE_HZ);
    Timer_ProcessQ();
  }
  
  /* Enter a critical section. */
  __disable_irq();
  
  /* Ensure it is still ok to enter the sleep mode. */
  eSleepStatus = eTaskConfirmSleepModeStatus();
  
  if( eSleepStatus == eAbortSleep )
    {
        /* A task has been moved out of the Blocked state since this macro was
        executed, or a context siwth is being held pending.  Do not enter a
        sleep state.  Exit the critical section. */
        __enable_irq();
        HAL_VTimer_Stop(VTIMER_NUM);
    }
    else
    {
        if( eSleepStatus == eNoTasksWaitingTimeout )
        {
            /* It is not necessary to configure an interrupt to bring the
            microcontroller out of its low power state at a fixed time in the
            future. */
           /* No need to stop the timer since it was started only if eSleepStatus was
            eStandardSleep. It may happen that eSleepStatus becomes eAbortSleep
            after the second call to eTaskConfirmSleepModeStatus(), but it cannot
            happen that eSleepStatus becomes eNoTasksWaitingTimeout if it
            was eStandardSleep. */                    
          BlueNRG_Sleep(SLEEPMODE_NOTIMER, gpio_wake_bit_mask, gpio_wake_level_mask);    
        }
        else
        {
            /* Enter the low power state. */
            /* TODO: Need to set gpioWakeBitMask if user want to wakeup using a GPIO. */ 
            BlueNRG_Sleep(SLEEPMODE_WAKETIMER, gpio_wake_bit_mask, gpio_wake_level_mask);
            
            if(wakeupFromSleepFlag)
            {              
              /* Determine how long the microcontroller was actually in a low power
              state for, which will be less than xExpectedIdleTime if the
              microcontroller was brought out of low power mode by an interrupt
              other than that configured by the vSetWakeTimeInterrupt() call.
              Note that the scheduler is suspended before
              portSUPPRESS_TICKS_AND_SLEEP() is called, and resumed when
              portSUPPRESS_TICKS_AND_SLEEP() returns.  Therefore no other tasks will
              execute until this function completes. */
              timeAfterSleep = HAL_VTimerGetCurrentTime_sysT32();
              
              /* Correct the kernels tick count to account for the time the
              microcontroller spent in its low power state. */
              
              /* TODO: Account for the rounding to millisecond time units.
                Set the systick LOAD register to the appropriate value.  */
              
              vTaskStepTick( HAL_VTimerDiff_ms_sysT32(timeAfterSleep,timeBeforeSleep)/portTICK_PERIOD_MS );
            }
        }

        /* Exit the critical section - it might be possible to do this immediately
        after the prvSleep() calls. */
        __enable_irq();
    }
}
/*-----------------------------------------------------------*/
/* Set wakeup GPIOs to be used when FreeRTOS Idle Task will call BlueNRG sleep function */
void BlueNRG_SetWakeupMask(uint8_t gpioWakeBitMask, uint8_t gpioWakeLevelMask)
{
   gpio_wake_bit_mask = gpioWakeBitMask;
   gpio_wake_level_mask = gpioWakeLevelMask;   
}
#endif

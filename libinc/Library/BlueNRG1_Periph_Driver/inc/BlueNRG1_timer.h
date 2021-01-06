/**
 ******************************************************************************
 * @file    BlueNRG1_timer.h
 * @author  RF Application Team
 * @date    Jan-2020
 * @brief   This file contains all the functions prototypes for the TIMER 
 *          firmware library.
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
 * <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_TIMER_H
#define BLUENRG1_TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include <stdio.h>

/** @defgroup TIMER_Exported_Types Exported Types
  * @{
  */ 

/** 
  * @brief Structure definition of TIMER initialization
  */ 
typedef struct
{
  uint16_t XTAL_StartupTime;      /*!< XTAL startup in 2.44 us unit */
/**
 * Enable initial estimation of the frequency of the Low Speed Oscillator, otherwise it will be assumed fixed at 32.768 kHz.
 * Ignored if periodic calibration is active (TIMER_PeriodicCalibration = TRUE).
 */
  BOOL TIMER_InitialCalibration;
  BOOL TIMER_PeriodicCalibration; /*!< Assume that 32 KHz clock requires periodic calibration, i.e. varies with temperature */
} TIMER_InitType;

typedef struct timer_calibration_s {
  uint64_t last_calibration_time; /** Absolute system time when last calibration was performed */
  uint32_t period; /** Number of 16 MHz clock cycles in (2*(SLOW_COUNT+1) low speed oscillator periods */
  uint32_t freq; /** 2^39/period */
  uint8_t  periodic_calibration; /** Tells whether periodic hardware calibration is needed or not */
  uint32_t last_calibration_machine_time; /** Last machine time when calibration was performed */

} TIMER_CalibrationType;

/**
  * @}
  */ 

/** @defgroup TIMER_Exported_Constants Exported Constants
  * @{
  */
#define TIME_CONVERSION2            0

/** This define represents the marging we take for long sleep to allow the system to
    avoid to have the counter wrapping. It is expressed in machine time, so it is variable
    when using internal RO */
#define TIMER_WRAPPING_MARGIN 4096

/** This #define represents the number of significant bits in the timer */
#define TIMER_BITS 24 //32
#define TIMER_MAX_VALUE (0xFFFFFFFFU >> (32-TIMER_BITS))
#define CALIB_MAX_VALUE TIMER_MAX_VALUE >> 1

/*This threshold should be the time to allow the device to go correctly in sleep*/
#define SLEEP_TIME_MIN 60

/** @defgroup Device_System_Delays System Delays
  * @{
  */

#define DELAY_TX          56 
#define DELAY_TX_NO_CAL   27 
#define DELAY_RX          56 
#define DELAY_RX_NO_CAL   27 

/**
  * @}
  */

#define TIMER1_BUSY       0x1
#define TIMER2_BUSY       0x2
#define WAKEUP_TIMER_BUSY 0x3
   
#define WAKEUP_TIMER_MASK 0x1
#define TIMER1_TIMER_MASK 0x2
#define TIMER2_TIMER_MASK 0x3

/** @defgroup TIMER_Ticks_Definition Ticks Definition
  * @{
  */
#define TIMER_SYSTICK_PER_SECOND 409600
#define TIMER_SYSTICK_PER_10MS   4096

/**
  * @}
  */

/**
  * @}
  */ 

/** @defgroup TIMER_Exported_Macros Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup TIMER_Exported_Functions Exported Functions
  * @{
  */

/**
 * @brief Initialize the TIMER functionality
 * @retval None
 */
void TIMER_Init(TIMER_InitType* TIMER_InitStruct);

/**
 * @brief Perform a low speed clock calibration and store results in the global context.
 *        It updates the XTAL startup time and the interpolator configuration.
 *        It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *        regardless of actual need of periodic calibration.
 * @warning This function is not re-entrant since it updates the context variable storing the system time.
 * @retval None
 */
void TIMER_Calibrate(void);

/**
 * @brief Start the calibration routine.
 * @retval None
 */
void TIMER_StartCalibration(void);

/**
 * @brief Return TRUE if a calibration is on going. It relies on the value of the interrupt status.
 *        Assumption is that no other modules are managing the interrupt status of the calibrator.
 * @retval TRUE if calibration is running, FALSE otherwise.
 */
BOOL TIMER_IsCalibrationRunning(void);

/**
 * @brief Records the result of the last calibration in the internal context.
 *        It updates the XTAL startup time and the interpolator configuration.
 *        It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *        regardless of actual need of periodic calibration.
 * @warning This function is not re-entrant since it updates the context variable storing the system time.
 *          It should be called only in user context and not in interrupt context.
 * @retval None
 */
void TIMER_UpdateCalibrationData(void);

/**
 * @brief Return the current system time in system time unit (STU).
 *        This is a counter that grows since the power up of the system and it never wraps.
 * @return Current system time
 */
uint64_t TIMER_GetCurrentSysTime(void);

/**
 * @brief Set the wakeup time to the specified delay. The delay is converted in machine time and only 20 most significant bits
 *        are taken into account. The XTAL startup time is not taken into account for the wakeup, i.e. the system does not wait for
 *        the XTAL startup time parameter to trigger the interrupt.
 *        The delay is translated into machine time unit (MTU) and it is assigned to the wakeup register.
 * @param delay: Delay from the current time expressed in system time unit (STU). Range is 0 to maximum value of STU. 
 *        The maximum value STU is dependent on the speed of the low speed oscillator.
 *        A value too small will force the timer to wrap, so it is reccommended to use at least 5-10 STU.
 * @param allow_sleep: Setting it to zero will prevent hardware to go to deep sleep,
 *        if other code commands the system to go to deep sleep.
 * @warning This function should be called with interrupts disabled to avoid programming the timer with a value in the past
 * @return Current time in MTU
 */
uint32_t TIMER_SetWakeupTime(uint32_t delay, BOOL allow_sleep);

/**
 * @brief Return the current calibration data.
 * @retval None
 */
void TIMER_GetCurrentCalibrationData(TIMER_CalibrationType *data);

/**
 * @brief Disable Wakeup Timer, Timer1 and Timer2 enabling sleep.
 * @retval None
 */
void TIMER_ClearRadioTimerValue(void);

/**
 * @brief Return the system time referred to the absolute machine time passed as parameter.
 * @param time: Absolute machine time in the past
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return System time value 
 */
uint64_t TIMER_GetPastSysTime(uint32_t time);

/**
 * @brief Return timer capture register value in STU.
 * @return value in STU 
 */
uint64_t TIMER_GetAnchorPoint(void);

/**
 * @brief Program the radio timer (a.k.a Timer1) as close as possible.
 * @retval None
 */
void TIMER_SetRadioCloseTimeout(void);

/**
 * @brief Return the MTU corresponding to the STU passed as parameter.
 * @param time: STU amount to be converted in MTU
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *     user context and not in interrupt context.
 * @return MTU value 
 */
uint32_t TIMER_SysTimeToMachineTime(uint32_t time);

/**
 * @brief Return the STU corresponding to the MTU passed as parameter.
 * @param time: MTU amount to be converted in STU
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *     user context and not in interrupt context.
 * @return STU value 
 */
uint32_t TIMER_MachineTimeToSysTime(uint32_t time);

/**
 * @brief Return the system time referred to the absolute machine time passed as parameter.
 * @param time: Absolute machine time in the future
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return System time value 
 */
uint64_t TIMER_GetFutureSysTime(uint32_t time);

/**
 * @brief Return the system time referred to the time in microseconds passed as parameter.
 * @param time: time in microseconds
 * @return System time value 
 */
uint32_t TIMER_UsToSystime(uint32_t time);

/**
 * @brief Return the machine time referred to the time in microseconds passed as parameter.
 * @param time: time in microseconds
 * @return Machine time value 
 */
uint32_t TIMER_UsToMachinetime(uint32_t time);

/**
 * @brief Program either Timer1 or the Wakeup Timer if the device has enough time to settle after exit from sleep or not.
 *        If the timeout is enough in the future, a preconfigured radio transaction is going to trigger at the specified
 *        timeout. The timeout is translated into machine time unit (MTU) and it is assigned to the wakeup register.
 *        If it is sleeping, the CPU wakes up (no IRQ) when the absolute time mathches the 20 MSB of the timeout
 *        written in the register. The transmission or reception starts when the timer value matches the value of 
 *        timeout plus the time the analog part needs to settle. All the 24 bits are considered in this case.
 *        An IRQ related to this event can be generated only when the transaction is completed.
 *        In order to get a precise timeout, the radio setup delays are taken into account. They differ if
 *        the event is a transmission or reception and if the PLL calibration is requested or not.
 * @param timeout: Absolute time expressed in system time unit (STU).
 *        It must represent a time in the future.
 *        If timeout is less than current time, it is considered as it is in the future (wrapping).
 *        The maximum value in STU depends on the speed of the low speed oscillator.
 * @param  event_type: Specify if it is a TX (1) or RX (0) event.
 * @param  cal_req: Specify if PLL calibration is requested (1) or not (0).
 * @warning This function should be called with interrupts disabled to avoid programming the timer with a value in the past.
 * @retval 0 if a correct timeout has been programmed in the timeout register.
 * @retval 1 if a correct timeout cannot be programmed (the timeout is too close).
 */
uint8_t TIMER_SetRadioTimerValue(uint32_t timeout, BOOL event_type, BOOL cal_req);

/**
 * @brief Update the value of period slow variable in RAM.
 * @retval None
 */
void TIMER_updatePeriodSlow(void);

/**
 * @brief Update the value of wakeup_time_offset variable in RAM.
 * @retval None
 */
void TIMER_updateWkupTimeOffset(void);

/**
 * @brief Return the status of the Radio timers and the last value programmed in the register.
 * @note When Timer2 is on schedule, the time is expressed in microseconds.
 * @param  time: pointer to value which is going to have time value.
 * @retval 0 if no timer has been programmed programmed.
 * @retval 1 if Timer1 has been programmed.
 * @retval 2 if Timer2 has been programmed.
 * @retval 3 if Wakeup Timer has been programmed.
 */
uint8_t TIMER_GetRadioTimerValue(uint32_t* time);

/**
 * @brief Get the radio setup delay in system time.
 * @param event_type: Specify if it is a TX (1) or RX (0) event.
 * @param cal_req: Specify if PLL calibration is requested (1) or not (0).
 * @return Radio setup delay in system time
 */
uint8_t TIMER_GetActivitySysDelay(uint8_t event_type, uint8_t cal_req);

/**
 * @brief Returns the next activities scheduled by the Timer Module.
 * @param nextRadioActivity: pointer to value that is going to have last TIMEOUT value programmed
 * @param nextRadioActivity: pointer to value that is going to have last HOST_WKUP_TIMER value programmed
 * @retval None
 */
void TIMER_GetNextTimerActivity(uint32_t* nextRadioActivity, uint32_t* nextHostActivity);

/**
 * @brief  Get the current absolute time.
 */
__STATIC_INLINE uint32_t TIMER_GetCurrentMachineTime(void) { return BLUE_CTRL->CURRENT_TIME; }

/**
 * @brief  Set the host_wakeup_timer timeout.
 */
__STATIC_INLINE void TIMER_SetHostTimeoutReg(uint32_t value) { BLUE_CTRL->HOST_WKUP_TIMER = value; }

/**
 * @brief  Get the host_wakeup_timer timeout.
 */
__STATIC_INLINE uint32_t TIMER_GetHostTimeoutReg(void) { return BLUE_CTRL->HOST_WKUP_TIMER; }

/**
 * @brief  Get radio controller timer timeout.
 */
__STATIC_INLINE uint32_t TIMER_GetTimeoutReg(void) { return BLUE_CTRL->TIMEOUT; }

/**
 * @brief  Set radio controller timer timeout.
 */
__STATIC_INLINE void TIMER_SetTimeoutReg(uint32_t value) { BLUE_CTRL->TIMEOUT = value; }

/**
 * @brief  Enable BlueIRQ.
 */
__STATIC_INLINE void TIMER_EnableBlueIRQ(void) { NVIC_EnableIRQ(BLUE_CTRL_IRQn); }

/**
 * @brief  Disable BlueIRQ.
 */
__STATIC_INLINE void TIMER_DisableBlueIRQ(void) { NVIC_DisableIRQ(BLUE_CTRL_IRQn); }

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

#endif /* BLUENRG1_TIMER_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

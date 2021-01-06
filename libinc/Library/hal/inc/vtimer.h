/**
 ******************************************************************************
 * @file    vtimer.h
 * @author  RF Application Team
 * @date    Jan-2020
 * @brief   vtimer and Radio timer high level APIs header file
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
#ifndef VTIMER_H
#define VTIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_types.h"
#include "sleep.h"
#include "BlueNRG1_timer.h"

/** @defgroup VTIMER_Exported_Types Exported Types
  * @{
  */ 

typedef enum {
  RADIO_TIMER_OFF = 0,
  RADIO_TIMER_PENDING = 1,
} RadioStatus; 

typedef void (*VTIMER_CallbackType)(void *);
typedef struct VTIMER_HandleTypeS {
	uint64_t expiryTime;
	VTIMER_CallbackType callback;
	BOOL active; /*Managed by the timer tick. User does not have to care about this*/
	struct VTIMER_HandleTypeS *next; /*Managed by the timer tick. User does not have to care about this*/
	void *userData;
} VTIMER_HandleType;

typedef void (*usr_cb_t)(void *);
typedef struct VTIMER_RadioHandleTypeS {
    uint64_t expiryTime; /*When the first bit is transmitted or when the receive window is opened*/
    BOOL cal_req;
    BOOL active; /*Managed by the timer tick. User does not have to care about this*/
    BOOL pending; /*Managed by the timer tick. User does not have to care about this*/    
    BOOL event_type;
} VTIMER_RadioHandleType;

typedef struct HAL_VTIMER_InitS {
  uint16_t XTAL_StartupTime;             /*!< XTAL startup in 2.44 us unit */
/**
 * Enable initial estimation of the frequency of the Low Speed Oscillator, otherwise it will be assumed fixed at 32.768 kHz.
 * Ignored if periodic calibration is active (PeriodicCalibrationInterval != 0).
 */
  BOOL EnableInitialCalibration;
  uint16_t PeriodicCalibrationInterval; /*!< Periodic calibration interval in ms, to disable set to 0 */
} HAL_VTIMER_InitType;

/**
  * @}
  */ 

/** @defgroup Radio_Exported_Constants Exported Constants
* @{
*/
#define HAL_VTIMER_SUCCESS 0
#define HAL_VTIMER_LATE 1
#define HAL_VTIMER_CRITICAL 2

#define RADIO_ACTIVITY_MARGIN 204800

/* Threshold to take into account the calibration duration. TBR */
#define CALIB_SAFE_THR 370

/* Minimum threshold in STU to safely clear radio timers.
The wakeup timer in the worst case triggers about 30us in advance.
This must be considered when the radio timer is cleared.
Then a window of about 30 us is considered as critical, that is 
it is not sure the timer can be cleared properly*/
#define CLEAR_TIME_MIN 14

#define BLUE_SLEEP_ENABLE         (0x10000000)

/** @defgroup HAL_VTimer_SetRadioTimerValue flags
  * @{
  */

#define HAL_VTIMER_PLL_CALIB_REQ            (0x01U)
#define HAL_VTIMER_PLL_CALIB_NOT_REQ        (0x00U)
#define HAL_VTIMER_TX_EVENT                 (0x01U)
#define HAL_VTIMER_RX_EVENT                 (0x00U)

/**
  * @}
  */

/**
* @}
*/ 

/** @defgroup VTIMER_Exported_Functions Exported Functions
  * @{
  */

/**
 * @brief Starts a one-shot virtual timer for the given relative timeout value expressed in ms
 * @param timerHandle: The virtual timer
 * @param msRelTimeout: The relative time, from current time, expressed in ms
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle already started.
 */
int HAL_VTimerStart_ms(VTIMER_HandleType *timerHandle, uint32_t msRelTimeout);

/**
 * @brief Stops the one-shot virtual timer specified if found
 * @param timerHandle: The virtual timer
 * @retval None
 */
void HAL_VTimer_Stop(VTIMER_HandleType *timerHandle);

/**
 * @brief This function return the current reference time expressed in system time units.
 * The returned value can be used as absolute time parameter where needed in the other
 * HAL_VTimer* APIs
 * @return absolute current time expressed in system time units.
 */
uint32_t HAL_VTimerGetCurrentTime_sysT32(void);

/**
 * @brief This function returns the sum of an absolute time and a signed relative time.
 * @param sysTime: Absolute time expressed in internal time units.
 * @param msTime: Signed relative time expressed in ms.
 * @note abs(msTime) <= 5242879
 * @return 32bit resulting absolute time expressed in internal time units.
 */
uint64_t HAL_VTimerAcc_sysT32_ms(uint64_t sysTime, int32_t msTime);

/**
 * @brief Returns the difference between two absolute times: sysTime1-sysTime2.
 * The resulting value is expressed in ms.
 * @param sysTime2: Absolute time expressed in internal time units.
 * @param sysTime1: Absolute time expressed in internal time units.
 * @return resulting signed relative time expressed in ms.
 */
int64_t HAL_VTimerDiff_ms_sysT32(uint64_t sysTime1, uint64_t sysTime2);

/**
 * @brief Starts a one-shot virtual timer for the given absolute timeout value
 *        expressed in internal time units.
 * @param timerHandle: The virtual timer
 * @param time: Absolute time expressed in STU.
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It already started.
 */
int HAL_VTimerStart_sysT32(VTIMER_HandleType *timerHandle, uint64_t time);

/**
 * @brief Returns the absolute expiry time of a running timer expressed in internal time units.
 * @param timerHandle: The virtual timer
 * @param sysTime: Absolute time expressed in internal time units.
 */
int HAL_VTimerExpiry_sysT32(VTIMER_HandleType *timerHandle, uint64_t *sysTime);

/**
 * @brief Initialize the timer module. It must be placed in the initialization
 * section of the application.
 * @retval None
 */
void HAL_VTIMER_Init(HAL_VTIMER_InitType *HAL_TIMER_InitStruct);

/**
 * @brief Timer module state machine. Check and schedule the calibration.
 *        Check expired timers and execute user callback.
 *        It must be placed inside the infinite loop.
 * @retval None
 */
void HAL_VTIMER_Tick(void);

/**
 * @brief Return the consensus of the Virtual timer management to go in sleep.
 * @retval TRUE if all vtimers have been served and the calibration procedure has already finished. 
 * @retval FALSE if the vtimer Tick is still busy.
 */
uint8_t HAL_VTIMER_SleepCheck(void);

/**
 * @brief  Returns the admitted sleep mode according to the next timer activity.
 * @return Sleep mode 
 */
SleepModes HAL_VTIMER_TimerSleepCheck(SleepModes sleepMode);

/**
 * @brief Virtual timer Timeout Callback. It must be called in the Blue handler
 *        if the host wake up timer bit in the radio controller interrupt/status
 *        register is active.
 * @retval None
 */
void HAL_VTIMER_TimeoutCallback(void);

/**
 * @brief Radio activity finished. Not used yet
 * @retval None
 */
void HAL_VTIMER_RadioTimerIsr(void);

/**
 * @brief Returns the number of timers in the queue.
 * @return number of timers in the queue.
 */
uint32_t HAL_VTIMER_GetPendingTimers(void);

/**
 * @brief Schedules a radio activity for the given absolute timeout value expressed in STU
 *        If the calibration of the low speed oscillator is needed, if it is possible,
 *        the radio timer will be programmed with the latest calibration data.
 * @param time: Absolute time expressed in STU.
 * @param event_type: Specify if it is a TX (1) or RX (0) event.
 * @param cal_req: Specify if PLL calibration is requested (1) or not (0).
 * @retval 0 if radio activity has been scheduled succesfully.
 * @retval 1 if radio activity has been rejected (it is too close or in the past).
 */
uint8_t HAL_VTimer_SetRadioTimerValue(uint32_t time, uint8_t event_type, uint8_t cal_req);

/**
 * @brief Disable Timer2. 
 * @retval None
 */
void HAL_VTimer_StopRadioTimer2(void);

/**
 * @brief Return the status of the radio timer.
 *        The timeout of the last radio timer activity taken into account by the Timer Module
 *        is saved in the variable passed as parameter.
 * @param time: Pointer of the variable where the time of the last radio activity scheduled is stored
 *              The time is expressed in STU.
 * @retval 0 if no radio timer is pending.
 * @retval 1 if a radio timer is pending and it is not already expired.
 */
RadioStatus HAL_VTimer_GetRadioTimerValue(uint32_t *time);

/**
 * @brief Clear the last radio activity scheduled disabling the radio timer too.
 *        Furthermore, it returns if the timeout is too close with respect the current time and
 *        the radio activity might not be cleared in time.
 * @retval 0 if the radio activity has been cleared successfully.
 * @retval 2 if it might not be possible to clear the last radio activity.
 */
uint8_t HAL_VTimer_ClearRadioTimerValue(void);

/**
 * @brief Get the last anchorPoint in system time unit.
 * @return TimerCapture register is system time unit.
 */
uint64_t HAL_VTIMER_GetAnchorPoint(void);
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /*VTIMER_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/  

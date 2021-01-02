/**
 ******************************************************************************
 * @file    vtimer.c 
 * @warning This module must not be included in applications based on the 
 *          BLE stack library. In that case, the API implementation is managed by the library itself.
 *          The API calls remain the same. In the future release the BLE stack library will rely on 
 *          the Timer module library.
 * @author  RF Application Team
 * @date    Jan-2020
 * @brief   Virtual timer and Radio timer high level APIs
 * @details This file implements the software layer that provides the virtualization of the
 * resources of a single hardware timer in order to allocate many user virtual timers.
 * Each instance of a virtual timer is placed in an queue ordered by the expiration time
 * and it can be linked to a callback.
 * The timer tick is in charge to execute the callback linked to each virtual timer
 * and to update the hardware timeout to gurantee the expiration of the next virtual
 * timer in the queue.
 * A special virtual timer called calibration/anti-wrapping timer is automatically armed
 * by the software. This timer can address two tasks:
 * - it is in charge to maintain the never wrapping virtual time base.
 * - if the slow clock calibration is enabled, it starts the calibration procedure at each
 * calibration interval specified during the initialization.
 *
 * A timer is intended as an event programmed in the future at a certain absolute expiration time
 * on a time base. In this implementation the time base grows on 64 bits. Then, it never wraps.
 * However, due to hardware timer finite length and in order to maintain the timing coherency, the time base
 * must be maintained at least one time before the hardware timer wraps.  
 * Then even if the slow clock calibration is disabled, the calibration/anti-wrapping timer
 * is always active with the only role to maintain the time base and it will expire
 * at a rate that depends on the hardware timer capability.
 * The time base unit is a STU that is the unit exposed to the user and it is equal to 625/256 us. 
 * The calibration/anti-wrapping mechanism is not managed by the user.
 *
 * This software layer also exposes the possibility to program a radio timer.
 * A radio timer allows the user to trigger an already configured radio event.
 * This layer tries to exploit the last calibration values to program the radio activity
 * in order to improve the accuracy. In this case, the radio event is not immediately programmed 
 * when it is requested, but only when the next calibration values are available.
 * Since the calibration values are available inside the timer tick when the calibration is over,
 * the application must ensure that the timer tick is called after the calibration timer 
 * expiration within a certain margin in order to avoid that the radio event is shifted in the
 * past and cannot be anymore programmed.
 *
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
/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "BlueNRG1_timer.h"
#include "vtimer.h"
#include "sleep.h"

/** @defgroup VTIMER_Private_TypesDefinitions Private Types Definitions
  * @{
  */

#define ATOMIC_SECTION_BEGIN() uint32_t uwPRIMASK_Bit = __get_PRIMASK(); \
                                __disable_irq(); \
/* Must be called in the same scope of ATOMIC_SECTION_BEGIN */
#define ATOMIC_SECTION_END() __set_PRIMASK(uwPRIMASK_Bit)
#define MAX(a,b) ((a) < (b) )? (b) : (a)
#define MIN(a,b) ((a) > (b) )? (b) : (a)
#define DIFF8(a,b) ((a)>=(b) ? ((a)-(b)) : (256-((a)-(b))))
#define INCREMENT_EXPIRE_COUNT_ISR (HAL_VTIMER_Context.expired_count = ((HAL_VTIMER_Context.expired_count + 1) == HAL_VTIMER_Context.served_count) ? HAL_VTIMER_Context.expired_count : (HAL_VTIMER_Context.expired_count + 1))
#define INCREMENT_EXPIRE_COUNT ATOMIC_SECTION_BEGIN(); INCREMENT_EXPIRE_COUNT_ISR ; ATOMIC_SECTION_END();
                                  
/*Minimum programmable value in sys. unit for a radio activity to be in the future*/ 
#if HS_SPEED_XTAL==HS_SPEED_XTAL_32MHZ
#define TIMER1_MARGIN 9 /*TBD*/
#else
#define TIMER1_MARGIN 12 /*TBD*/
#endif

typedef struct HAL_VTIMER_ContextS {
  uint32_t PeriodicCalibrationInterval;  /*!< Periodic calibration interval in ms, to disable set to 0 */
  BOOL calibration_in_progress;  /*!< Flag to indicate that a periodic calibration has been started */
  BOOL enableTimeBase; /*TBR*/
  uint32_t hs_startup_time; /*TBR*/
  VTIMER_HandleType *rootNode;
  uint8_t expired_count; /* Progressive number to indicate expired timers */
  uint8_t served_count; /* Progressive number to indicate served expired timers */
} HAL_VTIMER_ContextType;

/**
  * @}
  */

/** @defgroup VTIMER_Private_Variables Private Variables
  * @{
  */
static HAL_VTIMER_ContextType HAL_VTIMER_Context;
static VTIMER_HandleType calibrationTimer;
static VTIMER_RadioHandleType radioTimer;

/**
  * @}
  */

/** @defgroup VTIMER_Private_Variables Private Variables
  * @{
  */
static TIMER_CalibrationType calibrationData;

/**
  * @}
  */

/** @defgroup VTIMER_Private_Function Private Function 
  * @{
  */
static void calibration_callback (void *handle)
{
  TIMER_StartCalibration();
  HAL_VTIMER_Context.calibration_in_progress = TRUE;
}

static VTIMER_HandleType * _remove_timer_in_queue(VTIMER_HandleType *rootNode, VTIMER_HandleType *handle)
{
  VTIMER_HandleType *current = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  
  while ((current!=NULL) && (current != handle)) {
    prev = current;
    current=current->next;
  }
  
  if (current == NULL) {
    /* Not found */
  }
  else if (current == rootNode) {
    /* New root node */
    returnValue = current->next;
  }
  else {
    prev->next = current->next;
  }
  
  return returnValue;
}

static VTIMER_HandleType * _insert_timer_in_queue(VTIMER_HandleType *rootNode, VTIMER_HandleType *handle)
{
  VTIMER_HandleType *current = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  
  while ((current!=NULL) && (current->expiryTime < handle->expiryTime)) {
    prev = current;
    current=current->next;
  }
  
  handle->next = current;
  
  if (prev == NULL) {
    /* We are the new root */
    returnValue = handle;
  }
  else {
    prev->next = handle;
  }
  
  return returnValue;
}

/* Set timeout and skip non active timers */
static VTIMER_HandleType *_update_user_timeout(VTIMER_HandleType *rootNode, uint8_t *expired)
{
  VTIMER_HandleType *curr = rootNode;
  VTIMER_HandleType *rootOrig = rootNode;
  int32_t delay;
  *expired =0;
  while (curr != NULL) {
    if (curr->active) {
      ATOMIC_SECTION_BEGIN();
      delay = curr->expiryTime-TIMER_GetCurrentSysTime();
    
      if (delay > 0) {
        /* Protection against interrupt must be used to avoid that the called function will be interrupted
          and so the timer programming will happen after the target time is already passed
          leading to a timer expiring after timer wraps, instead of the expected delay */
        
        TIMER_SetWakeupTime(delay, TRUE);
        ATOMIC_SECTION_END();
        break;
      }
      else {
        /* Timeout in the past or very close, what to do ??? */
        /* Set a flag for further processing ? or set a very close timeout ?*/
        *expired = 1;
        ATOMIC_SECTION_END();
        break;
      }
    }
    curr=curr->next;
  }
  if (*expired)
    return rootOrig;
  return curr;
}

/* Check the number of expired timer from rootNode (ordered list of timers) and return the list of expired timers */
static VTIMER_HandleType *_check_callbacks(VTIMER_HandleType *rootNode,VTIMER_HandleType **expiredList)
{
  VTIMER_HandleType *curr = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  *expiredList = rootNode;
  int32_t delay;
  uint32_t expiredCount = 0;
  
  while (curr != NULL) {
    if (curr->active) {
      delay = curr->expiryTime-TIMER_GetCurrentSysTime();
      
      if (delay > 0) {
        /* End of expired timers list*/
        break;
      }
    } 
    prev = curr;
    curr=curr->next;
    expiredCount++;
  }
  
  if (expiredCount) {
    /* Some timers expired */
    prev->next=NULL;
    returnValue = curr;
  }
  else {
    /* No timer expired */
    *expiredList = NULL;
  }
  
  return returnValue;
}

static int _start_timer(VTIMER_HandleType *timerHandle, uint64_t time)
{
  uint8_t expired = 0;
  
  /* The timer is already started*/
  if(timerHandle->active)
  {
    return 1;
  }
  timerHandle->expiryTime = time;
  timerHandle->active = TRUE;
  if (_insert_timer_in_queue(HAL_VTIMER_Context.rootNode, timerHandle) == timerHandle) {
    HAL_VTIMER_Context.rootNode = _update_user_timeout(timerHandle, &expired);
    if (expired) {
      /* A new root timer is already expired, mimic timer expire that is normally signaled
      through the interrupt handler that increase the number of expired timers*/
      INCREMENT_EXPIRE_COUNT;
    }
  }
  return expired;
}

/*Check if it is time to program the pending radio timer (large timeouts).
1) The radio event is before the next calibration. Then the timer must be programmed.
2) The calibration timer expired but the calibration didn't start and the values not updated as expected.
   Then the last calibration time is misleading. The radio timer must be programmed.
3) The radio event is after the next calibration event. Then the timer will be programmed with the latest values.
The check on the last calibration time is made even though the calibration is not needed in order to avoid counter wrapping
with timeouts far in the future.
*/
void _check_radio_activity(VTIMER_RadioHandleType * timerHandle, uint8_t *expired)
{
  uint32_t sys_delay;
  uint64_t nextCalibrationEvent,currentTime; 
                                 
  *expired = 0;
  if (timerHandle->pending){
    nextCalibrationEvent = calibrationData.last_calibration_time + \
                           HAL_VTIMER_Context.PeriodicCalibrationInterval;
    
    ATOMIC_SECTION_BEGIN();
    currentTime = TIMER_GetCurrentSysTime();
    
    if((timerHandle->expiryTime < (nextCalibrationEvent + RADIO_ACTIVITY_MARGIN)) \
                                    || \
      (currentTime > (nextCalibrationEvent + CALIB_SAFE_THR)))
     {
      sys_delay = TIMER_GetActivitySysDelay(timerHandle->event_type,timerHandle->cal_req);
      if (timerHandle->expiryTime > currentTime + sys_delay + TIMER1_MARGIN){
        *expired = TIMER_SetRadioTimerValue(timerHandle->expiryTime, timerHandle->event_type,timerHandle->cal_req);
        ATOMIC_SECTION_END();
        timerHandle->pending = FALSE; /*timer has been served. No more pending*/
        timerHandle->active = TRUE; /*timer has been programmed and it becomes ACTIVE*/
      }
      else{
        ATOMIC_SECTION_END();
        *expired = 1;
      }
    }
    else{
      ATOMIC_SECTION_END();
      TIMER_SetTimeoutReg(1<<28);
    }
  }
}

static void _virtualTimeBaseEnable(FunctionalState state)
{
  if(state != DISABLE)
  {
    if (HAL_VTIMER_Context.enableTimeBase == FALSE)
    {
      calibration_callback(&calibrationTimer);
      HAL_VTIMER_Context.enableTimeBase = TRUE;
    }
  }
  else
  {
    HAL_VTimer_Stop(&calibrationTimer);
    HAL_VTIMER_Context.enableTimeBase = FALSE;
  }
}

/**
  * @}
  */


/** @defgroup VTIMER_Public_Function Public Function
  * @{
  */

/**
 * @brief Return the status of the radio timer.
 *        The timeout of the last radio timer activity taken into account by the Timer Module
 *        is saved in the variable passed as parameter.
 * @param time: Pointer of the variable where the time of the last radio activity scheduled is stored
 *              The time is expressed in STU.
 * @retval 0 if no radio timer is pending.
 * @retval 1 if a radio timer is pending and it is not already expired.
 */
RadioStatus HAL_VTimer_GetRadioTimerValue(uint32_t *time)
{
  *time = radioTimer.expiryTime;
  if (radioTimer.pending || radioTimer.active)
    return RADIO_TIMER_PENDING;
  else
    return RADIO_TIMER_OFF;
}

/**
 * @brief Clear the last radio activity scheduled disabling the radio timer too.
 *        Furthermore, it returns if the timeout is too close with respect the current time and
 *        the radio activity might not be cleared in time.
 * @retval 0 if the radio activity has been cleared successfully.
 * @retval 2 if it might not be possible to clear the last radio activity.
 */
uint8_t HAL_VTimer_ClearRadioTimerValue(void)
{
  int64_t time_diff;
  uint8_t retVal;
  
  TIMER_ClearRadioTimerValue();
  radioTimer.active = FALSE;
  radioTimer.pending = FALSE;

  ATOMIC_SECTION_BEGIN();
  time_diff = radioTimer.expiryTime \
              - TIMER_GetCurrentSysTime() \
              - TIMER_GetActivitySysDelay(radioTimer.event_type,radioTimer.cal_req) \
              - HAL_VTIMER_Context.hs_startup_time;
  ATOMIC_SECTION_END();
 /*
  if(time_diff <= 0)
    retVal = HAL_VTIMER_LATE;
*/
  if (time_diff < CLEAR_TIME_MIN)
    retVal = HAL_VTIMER_CRITICAL;
  else{
    retVal = HAL_VTIMER_SUCCESS;
  }
  return retVal;
}

/**
 * @brief Disable Timer2. 
 * @retval None
 */
void HAL_VTimer_StopRadioTimer2(void)
{
  TIMER_ClearRadioTimerValue();
}

/**
 * @brief Virtual timer Timeout Callback. It must be called in the Blue handler
 *        if the host wake up timer bit in the radio controller interrupt/status
 *        register is active.
 * @retval None
 */
void HAL_VTIMER_TimeoutCallback(void)
{
  /* Disable host timer and enable the sleep*/
  TIMER_SetHostTimeoutReg(1<<28);
  INCREMENT_EXPIRE_COUNT_ISR;
}

/**
 * @brief Radio activity finished. Not used yet
 * @retval None
 */
void HAL_VTIMER_RadioTimerIsr(void)
{
  radioTimer.active = FALSE;
}

/**
 * @brief Starts a one-shot virtual timer for the given absolute timeout value
 *        expressed in internal time units.
 * @param timerHandle: The virtual timer
 * @param time: Absolute time expressed in STU.
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It is already started.
 */
int HAL_VTimerStart_sysT32(VTIMER_HandleType *timerHandle, uint64_t time)
{
  uint8_t retVal;
  retVal = _start_timer(timerHandle, time);
  _virtualTimeBaseEnable(ENABLE);
  return retVal;
}

/**
 * @brief Starts a one-shot virtual timer for the given relative timeout value expressed in ms
 * @param timerHandle: The virtual timer
 * @param msRelTimeout: The relative time, from current time, expressed in ms
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It is already started.
 */
int HAL_VTimerStart_ms(VTIMER_HandleType *timerHandle, uint32_t msRelTimeout)
{
  uint64_t temp = msRelTimeout;
  uint8_t retVal;
  retVal = _start_timer(timerHandle, TIMER_GetCurrentSysTime() + (temp*TIMER_SYSTICK_PER_10MS)/10);
  _virtualTimeBaseEnable(ENABLE);
  return retVal; 
}

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
uint8_t HAL_VTimer_SetRadioTimerValue(uint32_t time, uint8_t event_type, uint8_t cal_req)
{
  uint8_t retVal = 0;
  
  radioTimer.event_type = event_type;
  radioTimer.cal_req = cal_req;
  radioTimer.expiryTime = calibrationData.last_calibration_time + (uint32_t)(time - (uint32_t)calibrationData.last_calibration_time);
  radioTimer.active = FALSE;
  radioTimer.pending = TRUE;
  
  _check_radio_activity(&radioTimer, &retVal);
  
  if (retVal){
    radioTimer.pending = FALSE;
  }
  _virtualTimeBaseEnable(ENABLE);
  return retVal; 
}

/**
 * @brief Stops the one-shot virtual timer specified if found
 * @param timerHandle: The virtual timer
 * @retval None
 */
void HAL_VTimer_Stop(VTIMER_HandleType *timerHandle)
{
  VTIMER_HandleType *rootNode = _remove_timer_in_queue(HAL_VTIMER_Context.rootNode, timerHandle);
  uint8_t expired = 0;
  timerHandle->active=FALSE;
  if (HAL_VTIMER_Context.rootNode != rootNode && rootNode != NULL) {
    HAL_VTIMER_Context.rootNode = _update_user_timeout(rootNode, &expired);
    if (expired) {
      /* A new root timer is already expired, mimic timer expire */
      INCREMENT_EXPIRE_COUNT;
    }
  }
  else
     HAL_VTIMER_Context.rootNode = rootNode;
}	

/**
 * @brief This function return the current reference time expressed in system time units.
 * The returned value can be used as absolute time parameter where needed in the other
 * HAL_VTimer* APIs
 * @return absolute current time expressed in system time units.
 */
uint32_t HAL_VTimerGetCurrentTime_sysT32(void)
{
  return ((uint32_t ) TIMER_GetCurrentSysTime());
}

/**
 * @brief Get the last anchorPoint in system time unit.
 * @return TimerCapture register in system time unit.
 */
uint64_t HAL_VTIMER_GetAnchorPoint(void)
{
  return TIMER_GetAnchorPoint();
}

/**
 * @brief This function returns the sum of an absolute time and a signed relative time.
 * @param sysTime: Absolute time expressed in internal time units.
 * @param msTime: Signed relative time expressed in ms.
 * @note abs(msTime) <= 5242879
 * @return 32bit resulting absolute time expressed in internal time units.
 */
uint64_t HAL_VTimerAcc_sysT32_ms(uint64_t sysTime, int32_t msTime)
{
  int32_t sysTick = (msTime * TIMER_SYSTICK_PER_10MS)/10;
  return (sysTime+sysTick);
}

/**
 * @brief Returns the difference between two absolute times: sysTime1-sysTime2.
 * The resulting value is expressed in ms.
 * @param sysTime2: Absolute time expressed in internal time units.
 * @param sysTime1: Absolute time expressed in internal time units.
 * @return resulting signed relative time expressed in ms.
 */
int64_t HAL_VTimerDiff_ms_sysT32(uint64_t sysTime1, uint64_t sysTime2)
{
    return ((sysTime1-sysTime2)*10)/TIMER_SYSTICK_PER_10MS;
}

/**
 * @brief Returns the absolute expiry time of a running timer expressed in internal time units.
 * @param timerHandle: The virtual timer
 * @param sysTime: Absolute time expressed in internal time units.
 */
int HAL_VTimerExpiry_sysT32(VTIMER_HandleType *timerHandle, uint64_t *sysTime)
{
  *sysTime = timerHandle->expiryTime;
  return 0;
}

/**
 * @brief Returns the number of timers in the queue.
 * @return number of timers in the queue.
 */
uint32_t HAL_VTIMER_GetPendingTimers(void)
{
  VTIMER_HandleType *curr = HAL_VTIMER_Context.rootNode;
  uint32_t counter = 0;
  while (curr != NULL) {
    counter++;
    curr = curr->next;
  }
  return counter;
}

/**
 * @brief Initialize the timer module. It must be placed in the initialization
 * section of the application.
 * @retval None
 */
void HAL_VTIMER_Init(HAL_VTIMER_InitType *HAL_TIMER_InitStruct)
{
  TIMER_InitType TIMER_InitStruct;
  TIMER_EnableBlueIRQ();
  TIMER_InitStruct.TIMER_InitialCalibration = HAL_TIMER_InitStruct->EnableInitialCalibration;
  TIMER_InitStruct.TIMER_PeriodicCalibration = HAL_TIMER_InitStruct->PeriodicCalibrationInterval;
  TIMER_InitStruct.XTAL_StartupTime = HAL_TIMER_InitStruct->XTAL_StartupTime;
  TIMER_Init(&TIMER_InitStruct);
  TIMER_GetCurrentCalibrationData(&calibrationData);
  HAL_VTIMER_Context.rootNode = NULL;
  HAL_VTIMER_Context.enableTimeBase = TRUE; /*TBR*/
  radioTimer.active = FALSE;
  radioTimer.pending = FALSE;
  HAL_VTIMER_Context.hs_startup_time = HAL_TIMER_InitStruct->XTAL_StartupTime; /*TBR*/
  HAL_VTIMER_Context.expired_count=0;
  HAL_VTIMER_Context.served_count=0;
  HAL_VTIMER_Context.PeriodicCalibrationInterval = (TIMER_SYSTICK_PER_10MS * HAL_TIMER_InitStruct->PeriodicCalibrationInterval)/10;
  HAL_VTIMER_Context.calibration_in_progress = FALSE;
  
  if (HAL_VTIMER_Context.PeriodicCalibrationInterval == 0)
    HAL_VTIMER_Context.PeriodicCalibrationInterval = TIMER_MachineTimeToSysTime(TIMER_MAX_VALUE-TIMER_WRAPPING_MARGIN);
  else
    HAL_VTIMER_Context.PeriodicCalibrationInterval = MIN(HAL_VTIMER_Context.PeriodicCalibrationInterval,
                                                       TIMER_MachineTimeToSysTime(TIMER_MAX_VALUE-TIMER_WRAPPING_MARGIN));
  calibrationTimer.callback = calibration_callback;
  calibrationTimer.userData = NULL;
  _start_timer(&calibrationTimer, TIMER_GetCurrentSysTime() + HAL_VTIMER_Context.PeriodicCalibrationInterval);
}

/**
 * @brief Timer module state machine. Check and schedule the calibration.
 *        Check expired timers and execute user callback.
 *        It must be placed inside the infinite loop.
 * @retval None
 */
void HAL_VTIMER_Tick(void)
{
  uint8_t expired = 0;

  if(radioTimer.active){
    if(radioTimer.expiryTime < TIMER_GetCurrentSysTime()){
      radioTimer.active = FALSE;
    }
  }
  
  /* Check for expired timers */  
  while (DIFF8(HAL_VTIMER_Context.expired_count,HAL_VTIMER_Context.served_count)) {
    VTIMER_HandleType *expiredList, *curr;
    uint8_t to_be_served = DIFF8(HAL_VTIMER_Context.expired_count,HAL_VTIMER_Context.served_count);
    
    HAL_VTIMER_Context.rootNode = _check_callbacks(HAL_VTIMER_Context.rootNode, &expiredList);
    /* Call all the user callbacks */
    curr=expiredList;
    while (curr != NULL) {
      /* Save next pointer, in case callback start the timer again */
      VTIMER_HandleType *next = curr->next;
      curr->active = FALSE;
      if (curr->callback)
        curr->callback(curr); 
      curr = next;
    }
    HAL_VTIMER_Context.rootNode = _update_user_timeout(HAL_VTIMER_Context.rootNode, &expired);
    if (expired == 1) {
      /* A new root timer is already expired, mimic timer expire */
      INCREMENT_EXPIRE_COUNT;
    }
    HAL_VTIMER_Context.served_count += to_be_served;
  }

  /* Check for periodic calibration */
  if (HAL_VTIMER_Context.calibration_in_progress) {
    if (TIMER_IsCalibrationRunning() == FALSE) {
      /* Calibration is completed */
      HAL_VTIMER_Context.calibration_in_progress = FALSE;
      /* Collect calibration data */
      TIMER_UpdateCalibrationData();
      TIMER_GetCurrentCalibrationData(&calibrationData);
      _check_radio_activity(&radioTimer,&expired);
      /* Schedule next calibration event */
      _start_timer(&calibrationTimer, TIMER_GetCurrentSysTime() + HAL_VTIMER_Context.PeriodicCalibrationInterval);
    }		
  }
  /* if there is a periodic calibration, start it in advance during the active phase */
  else{
    if(calibrationData.periodic_calibration){
      if( TIMER_GetCurrentSysTime() > calibrationData.last_calibration_time + TIMER_SYSTICK_PER_SECOND)
      {
        HAL_VTimer_Stop(&calibrationTimer);
        calibration_callback(&calibrationTimer);
      }
    }
  }
}

/**
 * @brief  Returns the admitted sleep mode according to the next timer activity.
 * @return Sleep mode 
 */
SleepModes HAL_VTIMER_TimerSleepCheck(SleepModes sleepMode)
{
  uint32_t nextRadioActivity, nextHostActivity;
  uint8_t timerState;
  
  if (HAL_VTIMER_SleepCheck() == FALSE)
  {
    return SLEEPMODE_RUNNING;
  }

  TIMER_GetNextTimerActivity(&nextRadioActivity,&nextHostActivity);
  timerState = TIMER_GetRadioTimerValue(&nextRadioActivity);
  
  if((__blue_RAM.BlueGlobVar.Config & 8U) != 0U)
  {
    if((radioTimer.active || radioTimer.pending)  && !(timerState == TIMER1_BUSY))
    {
      /*No Radio SPI transactions must occur within a certain margin*/
      if(radioTimer.expiryTime < (TIMER_GetCurrentSysTime() + \
                              TIMER_GetActivitySysDelay(radioTimer.event_type,radioTimer.cal_req) + \
                              HAL_VTIMER_Context.hs_startup_time) + \
                              SLEEP_TIME_MIN)
      {
        return(SLEEPMODE_CPU_HALT);
      }
      
      TIMER_SetTimeoutReg(nextRadioActivity);
    }
    else
    {
      /*A radio timer has been programmed through the TxRxPack*/ 
      /*Timer1 and wakeup timer are programmed only through 
      the timer module*/
      if ((timerState == TIMER2_BUSY) || (timerState == TIMER1_BUSY))
      {
        return SLEEPMODE_CPU_HALT;
      }
    }
    sleepMode = SLEEPMODE_WAKETIMER;
  }

  if(HAL_VTIMER_Context.rootNode->active && HAL_VTIMER_Context.rootNode != NULL)
  {
    if((HAL_VTIMER_Context.rootNode->next == NULL) && (HAL_VTIMER_Context.rootNode == &calibrationTimer))
    {
      if(sleepMode == SLEEPMODE_NOTIMER)
      {
        _virtualTimeBaseEnable(DISABLE);
        TIMER_SetHostTimeoutReg(BLUE_SLEEP_ENABLE);
        return SLEEPMODE_NOTIMER;
      }
    }
    if(HAL_VTIMER_Context.rootNode->expiryTime < (TIMER_GetCurrentSysTime() + SLEEP_TIME_MIN))
    {
      return SLEEPMODE_CPU_HALT; 
    }
    TIMER_SetHostTimeoutReg(nextHostActivity | BLUE_SLEEP_ENABLE);
    sleepMode = SLEEPMODE_WAKETIMER;
  }
  return sleepMode;
}

/**
 * @brief Return the consensus of the Virtual timer management to go in sleep.
 * @retval TRUE if all vtimers have been served and the calibration procedure has already finished. 
 * @retval FALSE if the vtimer Tick is still busy.
 */
uint8_t HAL_VTIMER_SleepCheck(void)
{
  return ((HAL_VTIMER_Context.expired_count == HAL_VTIMER_Context.served_count) && (HAL_VTIMER_Context.calibration_in_progress == FALSE));
}

/**
  * @}
  */

  /******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

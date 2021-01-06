/**
 ******************************************************************************
 * @file    BlueNRG1_timer.c
 * @warning This module must not be included in applications based on the 
 *          BLE stack library. In that case, the API implementation is managed by the library itself.
 *          The API calls remain the same. In the future release the BLE stack library will rely on 
 *          the Timer module library.
 * @author  RF Application Team
 * @date    Jan-2020
 * @brief   This file provides a set of function to manage the sleep timer.
 * @details The basic support is provided by a low speed oscillator with a nominal speed of 32.768 kHz.
 * The low speed oscillator provides a counter that increments at a nominal rate of 30.52 us.
 * The counter is 24 bits wide and the low speed oscillator only increments bits 23:4 (most significant 20 bits)
 * The remaining 4 bits are interpolated using the high speed 16 MHz clock. The counter unit are called conventionally
 * machine time unit (MTU).
 * At the end, the counter resolution will be about 1.91 us and it will wrap every 32 sec.
 * It is important to notice that the interpolated bits will only be functional when the system is out of sleep.
 * While in sleep only the 20 most significant bits will tick.
 * The user can setup a timer that will wakeup the system (if asleep) and it will trigger an interrupt when the timer expires.
 * The user timer will not take into account the high speed XTAL wakeup time.
 * The possible options for the low speed oscillator are:
 * - external 32.768 kHz XTAL
 * - internal ring oscillator (RO)
 *
 * The external 32 kHz XTAL meets the assumption made above regarding the nominal speed. The internal RO,
 * due to technology contraints, is very sensitive to temperature and so the frequency absolute value and its stability
 * have a very wide spread compared to external 32 kHz XTAL. Due to this reason, the concept of calibration or compensation is
 * introduced. The calibration process involves measuring the nominal 32 kHz period using the high speed 16 MHz clock, once
 * this operation is performed, the software will compensate all the timing using the most recent calibration value,
 * hardware will also use the calibration value to tune the interpolator for the least significant 4 bits.
 * The calibration is started by hardware at power-on and software in all the other cases.
 * As a rule of thumb, it can be assumed that the internal RO varies between 30kHz and 60 kHz and in general
 * the lower the temperature, the higher the frequency.
 * In order to cope with this variation a concept of system time unit (STU) is introduced.
 * The STU is defined as scaled clock compared to MTU in order to:
 * - handle variation of internal RO
 * - express easily timing dictated by Bluetooth Low Energy protocol.
 *
 * The STU are absolute unit of time and each unit represent 625/256 us, that is about 2.44 us.
 * The MTU, on the other hand, have a nominal value of 1.91 us, and this value can be assumed constant when using
 * external 32 kHz clock, but it will vary (mainly with temperature variation) when using internal RO.
 * As a design choice, use of MTU is reserved as much as possible to the driver, while STU is exposed to user.
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
#include "BlueNRG1_timer.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @addtogroup TIMER_Peripheral  TIMER Peripheral
* @{
*/

/** @defgroup TIMER_Private_TypesDefinitions Private Types Definitions
* @{
*/
typedef struct timer_context_s {
  uint64_t cumulative_time; /** Absolute system time since power up */
  uint64_t last_calibration_time; /** Absolute system time when last calibration was performed */
  uint32_t hs_startup_time; /** XTAL startup time in STU */
  uint8_t wakeup_time_offset; /** wakeup up time offset expressed in half slow period unit*/
  uint16_t period_slow; /** Number of 16 MHz cycles in a low speed oscillator period*/
  uint16_t last_period_slow; /** Number of 16 MHz cycles in a low speed oscillator period most recent */
  uint32_t period; /** Number of 16 MHz clock cycles in (2*(SLOW_COUNT+1)) low speed oscillator periods */
  uint32_t freq; /** 2^39/period */
  uint32_t freq1; /** Round(((freq/64)*0x753)/256) */
  uint32_t period1; /** Round (( ((period /256) * 0x8BCF6) + (((period % 256)* 0x8BCF6)/256)) / 32) */
  uint32_t last_period1; /** Period global in last calibration */
  uint32_t last_machine_time; /** Last machine time used to update cumulative time */
  uint32_t last_calibration_machine_time; /** Last machine time when calibration was performed */
  uint8_t  periodic_calibration; /** Tells whether periodic hardware calibration is needed or not, i.e. lsosc speed varies with temperature, etc. */
  uint32_t host_time; /**Last programmed value in HOST_WKUP_TIMER register**/
  uint32_t wakeup_time; /**Last programmed value in TIMEOUT register**/
  uint8_t tx_cal_delay_st; /** Delay in STU between TIMEOUT+wakeup_time_offset and first transmitted bit when cal is requested**/
  uint8_t tx_no_cal_delay_st; /** Delay in STU between TIMEOUT+wakeup_time_offset and first transmitted bit when no cal is requested**/
  uint8_t rx_cal_delay_st; /** Delay in STU between TIMEOUT+wakeup_time_offset and rx window start when cal is requested**/
  uint8_t rx_no_cal_delay_st; /** Delay in STU between TIMEOUT+wakeup_time_offset and rx window start when no cal is requested**/
  int16_t roundingError;
} TIMER_ContextType;

/**
* @}
*/

/** @defgroup TIMER_Private_Defines Private Defines
* @{
*/
/**
* @}
*/

/** @defgroup TIMER_Private_Macros Private Macros
* @{
*/

#define TIMER_20MSB_MASK 0xFFFFF0
#define TIME24_DIFF(a, b)       (((int32_t)((a - b) << 8)) >> 8)
#define TIME_DIFF(a, b)       (((int32_t)((a - b) << (32-TIMER_BITS))) >> (32-TIMER_BITS))

/* This define assumes that a is always greater than b */
#define TIME_ABSDIFF(a, b)       ((a - b) & TIMER_MAX_VALUE)

#define MIN(a,b) ((a) < (b) )? (a) : (b)
#define MAX(a,b) ((a) < (b) )? (b) : (a)

#define ATOMIC_SECTION_BEGIN() uint32_t uwPRIMASK_Bit = __get_PRIMASK(); \
                                __disable_irq(); \
/* Must be called in the same scope of ATOMIC_SECTION_BEGIN */
#define ATOMIC_SECTION_END() __set_PRIMASK(uwPRIMASK_Bit)

#define TIMER_SEL_THR 20//75

#define TIMER_TX_EVENT 1
#define TIMER_RX_EVENT 0
                                  
#define WAKEUP_TIMER_EN (1<<24)
#define TIMER1_EN       (1<<25)
                                  
/**
* @}
*/

/** @defgroup TIMER_Private_Variables Private Variables
* @{
*/
TIMER_ContextType TIMER_Context;
/**
* @}
*/

/** @defgroup TIMER_Private_FunctionPrototypes Private Function Prototypes
* @{
*/
static void _timer_start_calibration(void)
{
  /* Clear any pending interrupt */
  CKGEN_BLE->CLK32K_IT = 1;
  CKGEN_BLE->CLK32K_PERIOD = 0;
}

static void _get_calibration_data(TIMER_ContextType *context)
{
  uint32_t mult = 0x753 ;
  uint32_t freq = CKGEN_BLE->CLK32K_FREQ;
  uint32_t period = CKGEN_BLE->CLK32K_PERIOD;
  uint32_t a1 = freq >> 6 ;
  uint32_t a2 = a1 * mult ;
  
  uint32_t mul1 = 0x8BCF6 ;
  uint32_t b1 = period >> 8 ;
  uint32_t b2 = period & 0xff ;
  
  context->period1 = ((mul1 * b1) + ((b2 * mul1)>>8) + 16 )>>5 ;
  context->freq1 = (a2 + 128) >> 8 ;      
  context->period = period;
  context->freq = freq;
  context->period_slow = ((period * 1365) >> 16);
}

static void _timer_calibrate(TIMER_ContextType *context)
{
  _timer_start_calibration();
  
  while(CKGEN_BLE->CLK32K_IT == 0);
  _get_calibration_data(context);
}

static void update_interpolator(uint16_t period_slow)
{
  __blue_RAM.BlueGlobVar.period_slow = period_slow;
  /* The only mechanism to reload the table is to trigger a fake wakeup with no
  radio activity. Otherwise the value of period slow will be loaded at the next wakeup timer timeout*/
  if ((__blue_RAM.BlueGlobVar.Config & 8U) == 0) {
    /* Fake wakeup to make sure that interpolator take into account period_slow stored in RAM */
    /* Interrupt should be disabled because we can risk to wait for wrapping ??? */
    uint32_t timeout = (BLUE_CTRL->CURRENT_TIME + 2) & TIMER_MAX_VALUE;
    while (timeout < 2) {
      timeout = (BLUE_CTRL->CURRENT_TIME + 2) & TIMER_MAX_VALUE;
    }
    BLUE_CTRL->TIMEOUT = (timeout)| (1<<24);
    while (BLUE_CTRL->CURRENT_TIME < (timeout));
    /*After the fake wake up, the interpolator needs about half slow clock 
    cycle before to start counting correctly. In this time interval, if TIMEOUT
    register is written, the value of period_slow is ignored. This is a problem
    if the first radio activity is scheduled with the timer1. */
    timeout = (BLUE_CTRL->CURRENT_TIME + 16) & TIMER_20MSB_MASK;
    while (BLUE_CTRL->CURRENT_TIME < (timeout));
    /*Disabling timer*/
    BLUE_CTRL->TIMEOUT = (1 << 28);
  }
}

/**
 * @brief Translate STU to MTU, it is implemented using integer operation and it is equivalent to:
 *        result=(time*625/256)*(frequency * 16)/1e6, where frequency is the current low speed frequency
 * @param time: Time expressed in STU to be translated in STU
 * @param freq: Frequency of lsosc expressed as (2048*f)/25 where f is the lsosc frequency expressed in Hz.
 * @return Machine Time Units
 */
static uint32_t systime_to_machinetime(uint32_t time, uint32_t freq)
{
 #if TIME_CONVERSION2 
  static volatile uint64_t time_tmp, time_tmp2;
  time_tmp = (uint64_t)time*(uint64_t)freq;
  time_tmp2 = ((time_tmp+(1U<<20))>>21);
  return (uint32_t)time_tmp2;
#else
  uint32_t time1 = (time >> 15) ;
  uint32_t time2 = time & 0x7fff ;
  uint32_t freq1 = freq >> 7 ;
  uint32_t result = freq*time1 + ((time2 * freq1) >> 8) ;
  return((result + 32) >> 6) ;
#endif
}

/**
 * @brief Translate microseconds in STU
 * @param time: Time expressed in microseconds to be translated in STU
 * @return System Time Units
 */
static uint32_t us_to_systime(uint32_t time)
{
  uint32_t t1, t2;
  t1 = time * 0x68;
  t2 = time * 0xDB;
  return (t1 >> 8) + (t2 >> 16);
}

/**
 * @brief Translate microseconds in MTU
 * @param time: Time expressed in microseconds to be translated in MTU
 * @return Machine Time Units
 */
static uint32_t us_to_machinetime(uint32_t time)
{
  return systime_to_machinetime(us_to_systime(time),TIMER_Context.freq1);
}

/**
 * @brief Translate MTU to STU, it is implemented using integer operation and it is equivalent to:
 *         result=(time*25600)*period, where period is the current low speed period in seconds.
 * @param time: Time expressed in STU to be translated in STU
 * @param period1: Period of lsosc expressed as (3*62500*286331)*p where p is the lsosc period expressed in seconds.
 * @return System Time Units
 */
static uint32_t machinetime_to_systime(uint32_t time, uint32_t period1, int16_t* round_err)
{
#if TIME_CONVERSION2
  uint64_t period_global = (uint64_t)period1;
  uint64_t result_tmp = period_global*(uint64_t)time;
  uint32_t result_round = (uint32_t)((result_tmp+(1<<20))>>21);
  return result_round;
#else
  uint32_t time1 = (time >> 14);
  uint32_t time2 = time & 0x3fff;
  uint32_t per1 = period1 >> 8;
  uint32_t result = period1*time1 + ((time2 * per1) >> 6) - (uint32_t)(int32_t)*round_err;
  uint32_t result_round = (result + 64) >> 7;
  uint32_t result_round_u32 = (result_round<<7)-result;
  *round_err = (int16_t)result_round_u32;
  return result_round;
#endif
}

static uint64_t get_system_time_and_machine(TIMER_ContextType *context, uint32_t *current_machine_time)
{
  uint32_t difftime;
  uint64_t new_time = context->cumulative_time;
  *current_machine_time = BLUE_CTRL->CURRENT_TIME;
  difftime = TIME_ABSDIFF(*current_machine_time, context->last_machine_time);
  new_time += machinetime_to_systime(difftime,context->period1,&(context->roundingError));
  return new_time;
}

static uint64_t get_system_time(TIMER_ContextType *context)
{
  uint32_t current_machine_time;
  return get_system_time_and_machine(context, &current_machine_time);
}

static void update_system_time(TIMER_ContextType *context)
{
  uint32_t current_machine_time;
  current_machine_time = BLUE_CTRL->CURRENT_TIME;
  uint32_t period = context->last_period1;
  context->cumulative_time = context->last_calibration_time + \
  machinetime_to_systime(TIME_ABSDIFF(current_machine_time,
                                        context->last_calibration_machine_time),
                                        period,
                                        &(context->roundingError));
  context->last_machine_time = current_machine_time;
  context->last_calibration_machine_time = current_machine_time;
  context->last_calibration_time = context->cumulative_time;
  context->last_period1 = context->period1;
}

static void _update_xtal_startup_time(uint16_t hs_startup_time, uint32_t freq1)
{
  uint32_t time1 = systime_to_machinetime(hs_startup_time, freq1) ;
  if(time1 >= 1024)
    time1 = 1023 ;
  if (time1 < 8)
    time1 = 8;
  TIMER_Context.wakeup_time_offset = time1 >> 3;
  if((__blue_RAM.BlueGlobVar.Config & 8U) == 0U)
    __blue_RAM.BlueGlobVar.wakeup_time_offset = TIMER_Context.wakeup_time_offset;
}

/**
* @}
*/

/** @defgroup TIMER_Public_Functions Public Functions
* @{
*/

/**
 * @brief Initialize the TIMER functionality
 * @retval None
 */
void TIMER_Init(TIMER_InitType* TIMER_InitStruct)
{
  TIMER_Context.hs_startup_time = TIMER_InitStruct->XTAL_StartupTime;
  
  if (TIMER_InitStruct->TIMER_PeriodicCalibration || TIMER_InitStruct->TIMER_InitialCalibration) {
    /* Make sure any pending calibration is over */
    while(CKGEN_BLE->CLK32K_IT == 0);
    /* Set SLOW_COUNT to 23, that is calibrate over 24 clock periods, this number
    cannot be changed without changing all the integer maths function in the
    file	*/
    CKGEN_BLE->CLK32K_COUNT_b.SLOW_COUNT=23;
    _timer_calibrate(&TIMER_Context);
    /* For first time set last to current */
    TIMER_Context.last_period1 = TIMER_Context.period1;
  }
  else {
    /* Assume fix frequency at 32.768 kHz */
    TIMER_Context.last_period1 = 0x00190000;
    TIMER_Context.period1 = 0x00190000 ;
    TIMER_Context.freq1 = 0x0028F5C2 ;
    TIMER_Context.period = 23437;
    TIMER_Context.freq = 23456748;
    TIMER_Context.period_slow = 488;
  }
  TIMER_Context.periodic_calibration = TIMER_InitStruct->TIMER_PeriodicCalibration;
  _update_xtal_startup_time(TIMER_Context.hs_startup_time, TIMER_Context.freq1);
  update_interpolator(TIMER_Context.period_slow);
  TIMER_Context.last_period_slow = TIMER_Context.period_slow;
  TIMER_Context.cumulative_time = 0;
  TIMER_Context.last_machine_time = BLUE_CTRL->CURRENT_TIME;
  TIMER_Context.last_calibration_machine_time = TIMER_Context.last_machine_time;
  TIMER_Context.last_calibration_time = 0;
  TIMER_Context.tx_cal_delay_st = DELAY_TX + us_to_systime((__blue_RAM.BlueGlobVar.TxDelay1) & 0x7F);
  TIMER_Context.tx_no_cal_delay_st = DELAY_TX_NO_CAL + us_to_systime((__blue_RAM.BlueGlobVar.TxDelay) & 0x7F);
  TIMER_Context.rx_cal_delay_st = DELAY_RX + us_to_systime((__blue_RAM.BlueGlobVar.RcvDelay1) & 0x7F);
  TIMER_Context.rx_no_cal_delay_st = DELAY_RX_NO_CAL + us_to_systime((__blue_RAM.BlueGlobVar.RcvDelay1) & 0x7F); 
}

/**
 * @brief Perform a low speed clock calibration and store results in the global context.
 *        It updates the XTAL startup time and the interpolator configuration.
 *        It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *        regardless of actual need of periodic calibration.
 * @warning This function is not re-entrant since it updates the context variable storing the system time.
 * @retval None
 */
void TIMER_Calibrate(void)
{
  if (TIMER_Context.periodic_calibration) {
    TIMER_ContextType ContextToUpdate = TIMER_Context;
    _timer_calibrate(&ContextToUpdate);
    _update_xtal_startup_time(ContextToUpdate.hs_startup_time, ContextToUpdate.freq1);
    if (ContextToUpdate.last_period_slow != ContextToUpdate.period_slow) {
      update_interpolator(ContextToUpdate.period_slow);
      ContextToUpdate.last_period_slow = ContextToUpdate.period_slow;
    }
    ATOMIC_SECTION_BEGIN();
    /* Copying only the updated fields.
    Faster than memcpy: TIMER_Context = ContextToUpdate;*/
    TIMER_Context.wakeup_time_offset = ContextToUpdate.wakeup_time_offset;
    TIMER_Context.period_slow = ContextToUpdate.period_slow;
    TIMER_Context.last_period_slow = ContextToUpdate.last_period_slow;
    TIMER_Context.period = ContextToUpdate.period;
    TIMER_Context.freq = ContextToUpdate.freq;
    TIMER_Context.freq1 = ContextToUpdate.freq1;
    TIMER_Context.period1 = ContextToUpdate.period1;
    TIMER_Context.last_period1 = ContextToUpdate.last_period1;
    TIMER_Context.host_time = ContextToUpdate.host_time;
    TIMER_Context.wakeup_time = ContextToUpdate.wakeup_time;
    TIMER_Context.roundingError = ContextToUpdate.roundingError;
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
  else
  {
    ATOMIC_SECTION_BEGIN();
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
}


/**
 * @brief Start the calibration routine.
 * @retval None
 */
void TIMER_StartCalibration(void)
{
  if (TIMER_Context.periodic_calibration) {
    _timer_start_calibration();
  }
}

/**
 * @brief Return TRUE if a calibration is on going. It relies on the value of the interrupt status.
 *        Assumption is that no other modules are managing the interrupt status of the calibrator.
 * @retval TRUE if calibration is running, FALSE otherwise.
 */
BOOL TIMER_IsCalibrationRunning(void)
{
	/* Normally the calibration is triggered automatically by hardware only at power up (or hardware reset),
	   all other calibrations are started by software */
  return (CKGEN_BLE->CLK32K_IT == 0);
}

/**
 * @brief Records the result of the last calibration in the internal context.
 *        It updates the XTAL startup time and the interpolator configuration.
 *        It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *        regardless of actual need of periodic calibration.
 * @warning This function is not re-entrant since it updates the context variable storing the system time.
 *          It should be called only in user context and not in interrupt context.
 * @retval None
 */
void TIMER_UpdateCalibrationData(void)
{
  if (TIMER_Context.periodic_calibration) {
    TIMER_ContextType ContextToUpdate = TIMER_Context;
    _get_calibration_data(&ContextToUpdate);
    _update_xtal_startup_time(ContextToUpdate.hs_startup_time, ContextToUpdate.freq1);
    if (ContextToUpdate.last_period_slow != ContextToUpdate.period_slow) {
      update_interpolator(ContextToUpdate.period_slow);
      ContextToUpdate.last_period_slow = ContextToUpdate.period_slow;
    }
    ATOMIC_SECTION_BEGIN();
    /* Copying only the updated fields.
    Faster than memcpy: TIMER_Context = ContextToUpdate;*/
    TIMER_Context.wakeup_time_offset = ContextToUpdate.wakeup_time_offset;
    TIMER_Context.period_slow = ContextToUpdate.period_slow;
    TIMER_Context.last_period_slow = ContextToUpdate.last_period_slow;
    TIMER_Context.period = ContextToUpdate.period;
    TIMER_Context.freq = ContextToUpdate.freq;
    TIMER_Context.freq1 = ContextToUpdate.freq1;
    TIMER_Context.period1 = ContextToUpdate.period1;
    TIMER_Context.last_period1 = ContextToUpdate.last_period1;
    TIMER_Context.host_time = ContextToUpdate.host_time;
    TIMER_Context.wakeup_time = ContextToUpdate.wakeup_time;
    TIMER_Context.roundingError = ContextToUpdate.roundingError;
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
  else
  {
    ATOMIC_SECTION_BEGIN();
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
}

/**
 * @brief Return the current system time in system time unit (STU).
 *        This is a counter that grows since the power up of the system and it never wraps.
 * @return Current system time
 */
uint64_t TIMER_GetCurrentSysTime(void)
{
  return get_system_time(&TIMER_Context);
}

/**
 * @brief Disable Wakeup Timer, Timer1 and Timer2 enabling sleep.
 * @retval None
 */
void TIMER_ClearRadioTimerValue(void)
{
  BLUE_CTRL->TIMEOUT = 1<<28;
}

/**
 * @brief Return the system time referred to the absolute machine time passed as parameter.
 * @param time: Absolute machine time in the future
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return System time value 
 */
uint64_t TIMER_GetFutureSysTime(uint32_t time)
{
  uint32_t delta_systime = machinetime_to_systime(TIME_DIFF(time, TIMER_Context.last_machine_time),
                                                  TIMER_Context.period1,
                                                  &TIMER_Context.roundingError);
  return TIMER_Context.cumulative_time+delta_systime;
}

/**
 * @brief Return the system time referred to the absolute machine time passed as parameter.
 * @param time: Absolute machine time in the past
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return System time value 
 */
uint64_t TIMER_GetPastSysTime(uint32_t time)
{
  uint32_t delta_systime, current_machine_time;
  uint64_t current_system_time = get_system_time_and_machine(&TIMER_Context, &current_machine_time);
  delta_systime = machinetime_to_systime(TIME_DIFF(current_machine_time, time),
                                         TIMER_Context.period1,
                                         &TIMER_Context.roundingError);
  return current_system_time-delta_systime;
}

/**
 * @brief Return the current calibration data.
 * @retval None
 */
void TIMER_GetCurrentCalibrationData(TIMER_CalibrationType *data)
{
  data->periodic_calibration = TIMER_Context.periodic_calibration;
  data->freq = TIMER_Context.freq;
  data->last_calibration_time = TIMER_Context.last_calibration_time;
  data->period = TIMER_Context.period;
  data->last_calibration_machine_time = TIMER_Context.last_calibration_machine_time; /** Last machine time when calibration was performed */
}

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
uint32_t TIMER_SetWakeupTime(uint32_t delay, BOOL allow_sleep)
{
  uint32_t current_time;
  delay = systime_to_machinetime(delay, TIMER_Context.freq1) ;
  /* If the delay is too small (less than 1 tick) round to minimum 1 tick */
  delay = MAX(16,delay); 
  current_time = BLUE_CTRL->CURRENT_TIME;
  /* 4 least significant bits are not taken into account. Let's round the value summing 8 */
  BLUE_CTRL->HOST_WKUP_TIMER = ((current_time + (delay + 8)) & TIMER_MAX_VALUE) | (1 << 24) | (allow_sleep ? (1 << 28): 0);
  /*Warning: SLEEP_EN bit is not readable*/
  TIMER_Context.host_time = BLUE_CTRL->HOST_WKUP_TIMER;
  return current_time;
}

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
uint8_t TIMER_SetRadioTimerValue(uint32_t timeout, BOOL event_type, BOOL cal_req)
{
  uint32_t current_time,
           relTimeout,
           delay,
           destination,
           device_delay_st;
             
  uint8_t enable_sleep;
  
  if(event_type == TIMER_TX_EVENT)
  {
    if(cal_req)
    {
      device_delay_st = TIMER_Context.tx_cal_delay_st;
    }
    else
    {
      device_delay_st = TIMER_Context.tx_no_cal_delay_st; 
    }
  }
  else
  {
    if(cal_req)
    {
      device_delay_st = TIMER_Context.rx_cal_delay_st;
    }
    else
    {
      device_delay_st = TIMER_Context.rx_no_cal_delay_st;
    }
  }
  
  /* At this point, it is care of the upper layers to guarantee that the timeout represents an absolute time in the future */
  relTimeout = timeout - (uint32_t)get_system_time_and_machine(&TIMER_Context,&current_time) - device_delay_st;
  /* check if the relative timeout is bigger than wakeup time offset */
  if (relTimeout > (TIMER_SEL_THR + TIMER_Context.hs_startup_time)){
    /*time is not in the past. Program the wakeup_timer*/
    destination =  WAKEUP_TIMER_EN; 
    enable_sleep = TRUE;
    __blue_RAM.BlueGlobVar.wakeup_time_offset = TIMER_Context.wakeup_time_offset;
  }else{
    /*This means that wakeup_timer will be programmed with a time in the past and it is not possible to program it.
    Therefore Timer1 is selected and no sleep is requested */
    destination =  TIMER1_EN;
    enable_sleep = FALSE;
  }

  delay = systime_to_machinetime(relTimeout, TIMER_Context.freq1) - \
          (__blue_RAM.BlueGlobVar.wakeup_time_offset << 3);
  
  BLUE_CTRL->TIMEOUT = ((current_time + delay) & TIMER_MAX_VALUE) | destination | (enable_sleep ? (1 << 28): 0);
  /*Check if delay for the wakeup timer is in the past*/
  if ((int32_t)delay < 0 && (destination == WAKEUP_TIMER_EN)){
    BLUE_CTRL->TIMEOUT = 0;
    return 1;
  }

  TIMER_Context.wakeup_time = BLUE_CTRL->TIMEOUT;
  return 0;
}

/**
 * @brief Return the status of the Radio timers and the last value programmed in the register.
 * @note When Timer2 is on schedule, the time is expressed in microseconds.
 * @param  time: pointer to value which is going to have time value.
 * @retval 0 if no timer has been programmed.
 * @retval 1 if Timer1 has been programmed.
 * @retval 2 if Timer2 has been programmed.
 * @retval 3 if Wakeup Timer has been programmed.
 */
uint8_t TIMER_GetRadioTimerValue(uint32_t* time)
{
  uint8_t retValue = 0;
  uint32_t timeOut = BLUE_CTRL->TIMEOUT;
  *time = timeOut;
  timeOut = (timeOut >> 24) & 0x07;
  if(timeOut == TIMER2_TIMER_MASK)
  {
    retValue =  TIMER2_BUSY;
  }
  else if(timeOut == TIMER1_TIMER_MASK)
  {
    retValue =  TIMER1_BUSY;
  }
  else if(timeOut == WAKEUP_TIMER_MASK)
  {
    retValue =  WAKEUP_TIMER_BUSY;
  }
  return retValue;
}

/**
 * @brief Update the value of period slow variable in RAM.
 * @retval None
 */
void TIMER_updatePeriodSlow(void)
{
  __blue_RAM.BlueGlobVar.period_slow = TIMER_Context.period_slow;
}

/**
 * @brief Update the value of wakeup_time_offset variable in RAM.
 * @retval None
 */
void TIMER_updateWkupTimeOffset(void)
{
  __blue_RAM.BlueGlobVar.wakeup_time_offset = TIMER_Context.wakeup_time_offset;
}

/**
 * @brief Returns the next activities scheduled by the Timer Module.
 * @param nextRadioActivity: pointer to value that is going to have last TIMEOUT value programmed
 * @param nextRadioActivity: pointer to value that is going to have last HOST_WKUP_TIMER value programmed
 * @retval None
 */
void TIMER_GetNextTimerActivity(uint32_t* nextRadioActivity, uint32_t* nextHostActivity)
{
  *nextRadioActivity = TIMER_Context.wakeup_time;
  *nextHostActivity = TIMER_Context.host_time;
}

/**
 * @brief Return the MTU corresponding to the STU passed as parameter.
 * @param time: STU amount to be converted in MTU
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *     user context and not in interrupt context.
 * @return MTU value 
 */
uint32_t TIMER_SysTimeToMachineTime(uint32_t time)
{
  return systime_to_machinetime(time, TIMER_Context.freq1);
}

/**
 * @brief Return the STU corresponding to the MTU passed as parameter.
 * @param time: MTU amount to be converted in STU
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *     user context and not in interrupt context.
 * @return STU value 
 */
uint32_t TIMER_MachineTimeToSysTime(uint32_t time)
{
  return machinetime_to_systime(time, TIMER_Context.period1, &TIMER_Context.roundingError);
}

/**
 * @brief Return the system time referred to the time in microseconds passed as parameter.
 * @param time: time in microseconds
 * @return System time value 
 */
uint32_t TIMER_UsToSystime(uint32_t time)
{
  return us_to_systime(time);
}

/**
 * @brief Return the machine time referred to the time in microseconds passed as parameter.
 * @param time: time in microseconds
 * @return Machine time value 
 */
uint32_t TIMER_UsToMachinetime(uint32_t time)
{
  return us_to_machinetime(time);
}

/**
 * @brief Return timer capture register value in STU.
 * @return value in STU 
 */
uint64_t TIMER_GetAnchorPoint(void){
  return TIMER_GetPastSysTime(BLUE_CTRL->TIMER_CAPTURE);
}

/**
 * @brief Program the radio timer (a.k.a Timer1) as close as possible.
 * @retval None
 */
void TIMER_SetRadioCloseTimeout(void)
{
  ATOMIC_SECTION_BEGIN();
  uint32_t current_time = BLUE_CTRL->CURRENT_TIME;
  uint32_t delay = 3 - (__blue_RAM.BlueGlobVar.wakeup_time_offset << 3);
  BLUE_CTRL->TIMEOUT = ((current_time + delay ) & TIMER_MAX_VALUE) | TIMER1_EN;
  ATOMIC_SECTION_END();
}

/**
 * @brief Get the radio setup delay in system time.
 * @param event_type: Specify if it is a TX (1) or RX (0) event.
 * @param cal_req: Specify if PLL calibration is requested (1) or not (0).
 * @return Radio setup delay in system time.
 */
uint8_t TIMER_GetActivitySysDelay(uint8_t event_type, uint8_t cal_req)
{
  if(event_type == TIMER_TX_EVENT)
  {
    if(cal_req)
    {
      return TIMER_Context.tx_cal_delay_st;
    }
    else
    {
      return TIMER_Context.tx_no_cal_delay_st; 
    }
  }
  else
  {
    if(cal_req)
    {
      return TIMER_Context.rx_cal_delay_st;
    }    
    else
    {
      return TIMER_Context.tx_no_cal_delay_st;
    }
  }
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
/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

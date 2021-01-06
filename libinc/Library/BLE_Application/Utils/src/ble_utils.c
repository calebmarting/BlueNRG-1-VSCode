/*
  ******************************************************************************
  * @file    ble_utils.c 
  * @author  AMG - RF Application Team
  * @version V1.0.0
  * @date    23 - 10 - 2017
  * @brief   BLE utility for multiple connections scenarios (formula)
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
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */ 
 
/* Includes-----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ble_utils.h"
#include "bluenrg1_stack.h"

#ifndef DEBUG_FORMULA
#define DEBUG_FORMULA 0
#endif

#if DEBUG_FORMULA
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/**
 * ------ Multiple connection parameters formula internal parameters: do not modify 
 */

/**
 * @brief Max number of connection/advertsing/scanning State Machines at same time
 */
#define  ALLOWED_MAX_NUMBER_OF_BLE_SM 8

/**
 * @brief Number of packets per connection interval
 */
#define  PACKETS_PER_CI 6 

/**
 * @brief Guard Time 
 */
#define GUARD_TIME 1.6

/**
 * @brief Guard Time End
 */
#define GUARD_TIME_END 2.5

/**
 * @brief Advertising event length rounded to 0.625 ms(14.6 + 0.5 to be multiple of 0.625)
 */
#define ADV_LEN_MS 14.6
   
/**
 * @brief Connection event length
 */
#define CE_LENGTH_MS (float)(1.25*PACKETS_PER_CI)

/**
  * @brief Utility function for rounding a val to a specific round_value
  * @param val
  * @param round_value
  * @retval Rounded value
*/
static float round_to_x(float val, float round_value) //TBR 
{
  return ((ceil(val/round_value)) *  round_value);
}

/**
  * @brief This API calculates the multiple connections parameters to be used based on the input values.
  * @param Num_Masters Specifies numer of master devices to which connects
  * @param Num_Slaves Specifies numer of slave devices to which connects
  * @param Scan_Window Specifies minimal scan window length requested from user application
  * @param Sleep_Time Specifies additional time to be added to the minimal anchor period (for optimizig power consumption)
  * @param[out] Device_Connection_Params Specifies the connections parameters to be used for handling the multiple connections
  * @retval Value indicating success or error code.
*/
/* Formula for getting the device connection parameters on Master/Slave device */
uint8_t GET_Master_Slave_device_connection_parameters(uint8_t Num_Masters, uint8_t Num_Slaves, float Scan_Window, float Sleep_Time, Multiple_Connection_type *Device_Connection_Params)
{ 
  float Additional_time = 0; 
  float Scan_Window_ms = 0; 
  float Connection_Interval_ms = 0; 
  float Advertising_Interval_ms =0;
  float Scan_Interval_ms = 0; 
  uint16_t status = 0; 

  /* Check on input parameters */
  if (Num_Slaves > (ALLOWED_MAX_NUMBER_OF_BLE_SM - Num_Masters))
  {
    PRINTF("Num_Slaves can be up to %d with  Num_Masters = %d\r\n", Num_Slaves,Num_Masters);
    return (1);
  }

  /* Additional time to be added to the minimal anchor period */
  Additional_time = MAX(Sleep_Time, GUARD_TIME_END);
  /* Scan Window (ms) */
  Scan_Window_ms =    round_to_x(round_to_x(Scan_Window + GUARD_TIME, CE_LENGTH_MS + GUARD_TIME) - GUARD_TIME, 0.625) ; 

  if (Num_Masters == 0)
  {
    /* No Master for Master_Slave device: it doesn't need to enter in advertising */
    Device_Connection_Params->AnchorPeriodLength = round_to_x((Num_Slaves - 1) * (CE_LENGTH_MS + GUARD_TIME) + Scan_Window_ms + Additional_time, 1.25);
  }			     
  else
  {
    Device_Connection_Params->AnchorPeriodLength =  round_to_x((Num_Slaves - 1) * (CE_LENGTH_MS + GUARD_TIME) + ADV_LEN_MS + Scan_Window_ms + Additional_time, 1.25);
  }
  
  /* Define Master_Slave device connection parameters based on the calculated anchor period in ms */
  
  /* connection interval (ms) */
  Connection_Interval_ms = Device_Connection_Params->AnchorPeriodLength;  
  /* advertising interval (ms) */
  Advertising_Interval_ms = Connection_Interval_ms - 5; 
  /* Scan interval (ms) */
  Scan_Interval_ms = Connection_Interval_ms;            

  /* Master_Slave device output connection parameters in internal BLE time units (values to be used on BLE APIs) */
  Device_Connection_Params->Connection_Interval = (int)(Connection_Interval_ms/1.25);
  Device_Connection_Params->Advertising_Interval = (int)(Advertising_Interval_ms/0.625);
  Device_Connection_Params->CE_Length = (int)(CE_LENGTH_MS/0.625);
  Device_Connection_Params->Scan_Interval = (int)(Scan_Interval_ms/0.625);
  Device_Connection_Params->Scan_Window = (int)(Scan_Window_ms/0.625); 
  
#if DEBUG_FORMULA == 1
  PRINTF("****** Input Connection Parameters *******************************\r\n");
  PRINTF("\r\n");
  PRINTF("Num of Masters: %d\r\n", MASTER_SLAVE_NUM_MASTERS);
  PRINTF("Num of Slaves: %d\r\n", MASTER_SLAVE_NUM_SLAVES);
  PRINTF("Minimal Scan Window: %.2f ms\r\n", MASTER_SLAVE_SCAN_WINDOW);
  PRINTF("Sleep time: %.2f ms\r\n", MASTER_SLAVE_SLEEP_TIME);
  PRINTF("\r\n");
  
  PRINTF("****** Output Connection Parameters ******************************\r\n");
  PRINTF("\r\n");
  PRINTF("Anchor Period Length: %.2f ms\r\n", Device_Connection_Params->AnchorPeriodLength);
  PRINTF("\r\n");
  PRINTF("****** BLE APIs Connection Parameters: BLE time units/(ms)********\r\n");
  PRINTF("\r\n");
  PRINTF("Scan Window: %d (%.2f ms)\r\n", Device_Connection_Params->Scan_Window, Scan_Window_ms);
  PRINTF("Connection Interval: %d (%.2f ms)\r\n", Device_Connection_Params->Connection_Interval, Connection_Interval_ms);
  PRINTF("Scan Interval: %d (%.2f ms)\r\n", Device_Connection_Params->Scan_Interval, Scan_Interval_ms);
  PRINTF("Advertising Interval: %d (%.2f ms)\r\n", Device_Connection_Params->Advertising_Interval, Advertising_Interval_ms);
  PRINTF("CE Event Length: %d (%.2f ms)\r\n", Device_Connection_Params->CE_Length, CE_LENGTH_MS);
  
  //PRINTF("Connection bandwidth: %.2f \r\n", (1000*PACKETS_PER_CI*20*8)/Connection_Interval_ms);
  PRINTF("******************************************************************\r\n");
#endif  

   return (status);
} /* end GET_Master_Slave_device_connection_parameters() */

/* Multiple connection parameters formula API call example:

#include "ble_utils.h"

  // Multiple connection parameters variable
  Multiple_Connection_type MS_Connection_Parameters; 

//Input parameters x Multiple connection formula 
#if 1 //use default values 
  uint8_t num_masters = MASTER_SLAVE_NUM_MASTERS; 
  uint8_t num_slaves = MASTER_SLAVE_NUM_SLAVES;
  float scan_window = MASTER_SLAVE_SCAN_WINDOW;
  float sleep_time = MASTER_SLAVE_SLEEP_TIME;
#endif 
  
  // Call Multiple connections parameters formula 
  ret = GET_Master_Slave_device_connection_parameters(num_masters,
                                                      num_slaves,
                                                      scan_window,
                                                      sleep_time,
                                                      &MS_Connection_Parameters);
  if (ret !=0)
    PRINTF("GET_Master_Slave_device_connection_parameters() failure: %d\r\n", status);  

*/


/** \endcond 
*/

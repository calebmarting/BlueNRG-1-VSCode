/**
  ******************************************************************************
  * @file    ble_utils.h
  * @author  AMG - RF Application team
  * @version V1.0.0
  * @date    23 - 10 - 2017
  * @brief   Header file for BLE utility for multiple connections scenarios (formula)
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
#ifndef __BLE_UTILS_H__
#define __BLE_UTILS_H__

#include <stdint.h>

/**
 * ------ Default input parameters x Multiple connection formula 
 */

/**
 * @brief Number of Masters to which Master_Slave device should connect
 */
#ifndef MASTER_SLAVE_NUM_MASTERS
#define MASTER_SLAVE_NUM_MASTERS 1     //DEFAULT INPUT VALUE
#endif

/**
 * @brief Number of Slaves to which Master_Slave device should connect
 */
#ifndef MASTER_SLAVE_NUM_SLAVES
#define MASTER_SLAVE_NUM_SLAVES 6      //DEFAULT INPUT VALUE
#endif

/**
 * @brief Minimal Scan Window (ms) for  Master_Slave device 
 */
#ifndef MASTER_SLAVE_SCAN_WINDOW
#define MASTER_SLAVE_SCAN_WINDOW 20.00 //DEFAULT INPUT VALUE
#endif

/**
 * @brief Additional offset (ms) for  Master_Slave device 
 */
#ifndef MASTER_SLAVE_SLEEP_TIME
#define MASTER_SLAVE_SLEEP_TIME  0.00  //DEFAULT INPUT VALUE
#endif

/* ***************************************************************************** */

/**
 * @brief Structure definition of Multiple connection parameters 
 */
typedef struct Multiple_Connection_s{
  /** Connection Interval
  */
  uint16_t Connection_Interval; 
  /** Connection Event Length
  */
  uint16_t CE_Length;
  /** Advertising Interval
  */
  uint16_t Advertising_Interval;
  /** Scan Interval
  */
  uint16_t Scan_Interval;
  /** Scan Window
  */
  uint16_t Scan_Window;
  /** Anchor Period Length (ms)
  */
  float AnchorPeriodLength;
} Multiple_Connection_type;

/**
  * @brief This API calculates the multiple connections parameters to be used based on the input values.
  * @param Num_Masters Specifies numer of master devices to which connects
  * @param Num_Slaves Specifies numer of slave devices to which connects
  * @param Scan_Window Specifies minimal scan window length requested from user application
  * @param Sleep_Time Specifies additional time to be added to the minimal anchor period (for optimizig power consumption)
  * @param[out] Device_Connection_Params Specifies the connections parameters to be used for handling the multiple connections
  * @note  Refer to the Multiple_Connections_Formula.html guideline for more information about how formula works
  * @retval Value indicating success or error code.
*/
/* Formula for getting the device connection parameters on Master/Slave device */
uint8_t GET_Master_Slave_device_connection_parameters(uint8_t Num_Masters, uint8_t Num_Slaves, float Scan_Window, float Sleep_Time, Multiple_Connection_type *Device_Connection_Params);

#endif /* __BLE_CONST_H__ */

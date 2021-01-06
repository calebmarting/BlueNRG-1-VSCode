/******************** (C) COPYRIGHT 2020 STMicroelectronics ********************
* File Name          : bluenrg_utils.h
* Author             : AMS - RF Application Team
* Version            : V1.0.0
* Date               : 03-February-2020
* Description        : Header file for BlueNRG-1,2 utility functions 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLUENRG_UTILS_H
#define __BLUENRG_UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "compiler.h"
#include "hal_types.h"

/* Exported types ------------------------------------------------------------*/

/**
 * Structure inside IFR for configuration options. 
 */
typedef  PACKED(struct) devConfigS  {
  uint8_t  HS_crystal;
  uint8_t  LS_source;
  uint8_t  SMPS_management;
  uint8_t  Reserved;
  uint16_t HS_startup_time;
  uint16_t SlaveSCA;
  uint8_t  MasterSCA;
  uint32_t max_conn_event_length;
  uint8_t  Test_mode;
} devConfig_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define FROM_US_TO_SYS_TIME(us)      ((uint32_t)(us/2.4414)+1)
#define FROM_SYS_TIME_TO_US(sys)     ((uint32_t)(sys*2.4414))

/* Convert 2 digit BCD number to an integer */
#define BCD_TO_INT(bcd) ((bcd & 0xF) + ((bcd & 0xF0) >> 4)*10)

/* Convert 2 digit number to a BCD number */
#define INT_TO_BCD(n) ((((uint8_t)n/10)<<4) + (uint8_t)n%10)

/** 
  * Return values 
  */ 
#define BLE_UTIL_SUCCESS                 0
#define BLE_UTIL_UNSUPPORTED_VERSION     1
#define BLE_UTIL_WRONG_IMAGE_SIZE        2
#define BLE_UTIL_ACI_ERROR               3
#define BLE_UTIL_CRC_ERROR               4
#define BLE_UTIL_PARSE_ERROR             5
#define BLE_UTIL_WRONG_VERIFY            6

/* Exported functions ------------------------------------------------------- */ 
/**
  * @brief  Flash a new firmware using internal bootloader.
  * @param  fw_image     Pointer to the firmware image (raw binary data,
  *                      little-endian).
  * @param  fw_size      Size of the firmware image. The firmware image size shall
  *                      be multiple of 4 bytes.
  * @retval int      It returns 0 if successful, or a number not equal to 0 in
  *                  case of error (ACI_ERROR, UNSUPPORTED_VERSION,
  *                  WRONG_IMAGE_SIZE, CRC_ERROR)
  */
int program_device(const uint8_t *fw_image, uint32_t fw_size);


/**
  * @brief  Verify raw data from Device Configuration block.
  * @param  ifr_data Pointer to the buffer that will contain the data to verify.
  * @retval int      It returns 0 if successful, or a number not equal to 0 in
                     case of error (ACI_ERROR, BLE_UTIL_WRONG_VERIFY)
  */
uint8_t verify_DEV_CONFIG(const devConfig_t *ifr_data);

/**
  * @brief  Program raw data to Device Configuration block
  * @param  ifr_image     Pointer to the buffer that will contain the data to program.                
  * @retval int      It returns 0 if successful
  */
int program_DEV_CONFIG(const devConfig_t *ifr_image);

   
#ifdef __cplusplus
}
#endif

#endif /*__BLUENRG_UTILS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

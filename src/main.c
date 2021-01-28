
/******************** (C) COPYRIGHT 2018 STMicroelectronics ********************
* File Name          : BLE_Beacon_main.c
* Author             : RF Application Team
* Version            : 1.1.0
* Date               : 15-January-2016
* Description        : Code demostrating the BLE Beacon application
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/**
 * @file BLE_Beacon_main.c
 * @brief This is a BLE beacon demo that shows how to configure a BlueNRG-1,2 device 
 * in order to advertise specific manufacturing data and allow another BLE device to
 * know if it is in the range of the BlueNRG-1 beacon device. 
 * It also provides a reference example about how using the 
 * BLE Over-The-Air (OTA) Service Manager firmware upgrade capability.
 * 

* \section ATOLLIC_project ATOLLIC project
  To use the project with ATOLLIC TrueSTUDIO for ARM, please follow the instructions below:
  -# Open the ATOLLIC TrueSTUDIO for ARM and select File->Import... Project menu. 
  -# Select Existing Projects into Workspace. 
  -# Select the ATOLLIC project
  -# Select desired configuration to build from Project->Manage Configurations
  -# Select Project->Rebuild Project. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \section KEIL_project KEIL project
  To use the project with KEIL uVision 5 for ARM, please follow the instructions below:
  -# Open the KEIL uVision 5 for ARM and select Project->Open Project menu. 
  -# Open the KEIL project
     <tt> C:\Users\{username}\ST\BlueNRG-1_2 DK x.x.x\\Project\\BLE_Examples\\BLE_Beacon\\MDK-ARM\\BlueNRG-1\\BLE_Beacon.uvprojx </tt> or
     <tt> C:\Users\{username}\ST\BlueNRG-1_2 DK x.x.x\\Project\\BLE_Examples\\BLE_Beacon\\MDK-ARM\\BlueNRG-2\\BLE_Beacon.uvprojx </tt>
  -# Select desired configuration to build
  -# Select Project->Rebuild all target files. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \section IAR_project IAR project
  To use the project with IAR Embedded Workbench for ARM, please follow the instructions below:
  -# Open the Embedded Workbench for ARM and select File->Open->Workspace menu. 
  -# Open the IAR project
     <tt> C:\Users\{username}\ST\BlueNRG-1_2 DK x.x.x\\Project\\BLE_Examples\\BLE_Beacon\\EWARM\\BlueNRG-1\\BLE_Beacon.eww </tt> or
     <tt> C:\Users\{username}\ST\BlueNRG-1_2 DK x.x.x\\Project\\BLE_Examples\\BLE_Beacon\\EWARM\\BlueNRG-2\\BLE_Beacon.eww </tt>
  -# Select desired configuration to build
  -# Select Project->Rebuild All. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download and Debug to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \subsection Project_configurations Project configurations
- \c Release - Release configuration
- \c Use_OTA_ServiceManager - Configuration for Application using OTA Service Manager

     
* \section Board_supported Boards supported
- \c STEVAL-IDB007V1
- \c STEVAL-IDB007V2
- \c STEVAL-IDB008V1
- \c STEVAL-IDB008V1M
- \c STEVAL-IDB008V2
- \c STEVAL-IDB009V1


 * \section Power_settings Power configuration settings
@table

==========================================================================================================
|                                         STEVAL-IDB00XV1                                                |
----------------------------------------------------------------------------------------------------------
| Jumper name |            |  Description                                                                |
| JP1         |   JP2      |                                                                             |
----------------------------------------------------------------------------------------------------------
| ON 1-2      | ON 2-3     | USB supply power                                                            |
| ON 2-3      | ON 1-2     | The supply voltage must be provided through battery pins.                   |
| ON 1-2      |            | USB supply power to STM32L1, JP2 pin 2 external power to BlueNRG1           |


@endtable 

* \section Jumper_settings Jumper settings
@table

========================================================================================================================================================================================
|                                                                             STEVAL-IDB00XV1                                                                                          |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
| Jumper name |                                                                Description                                                                                             |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------          
| JP1         | 1-2: to provide power from USB (JP2:2-3). 2-3: to provide power from battery holder (JP2:1-2)                                                                          |          
| JP2         | 1-2: to provide power from battery holder (JP1:2-3). 2-3: to provide power from USB (JP1:1-2). Pin2 to VDD  to provide external power supply to BlueNRG-1 (JP1: 1-2)   |
| JP3         | pin 1 and 2 UART RX and TX of MCU. pin 3 GND.                                                                                                                          |          
| JP4         | Fitted: to provide VBLUE to BlueNRG1. It can be used also for current measurement.                                                                                     |
| JP5         | Fitted : TEST pin to VBLUE. Not fitted:  TEST pin to GND                                                                                                               |


@endtable 

* \section Pin_settings Pin settings
@table
|            |                                                           Release                                                           ||||||                                                                     Use_OTA_ServiceManager                                                                      ||||||
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|  PIN name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |  STEVAL-IDB008V1M  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |      STEVAL-IDB007V1     |      STEVAL-IDB007V2     |      STEVAL-IDB008V1     |     STEVAL-IDB008V1M     |      STEVAL-IDB008V2     |      STEVAL-IDB009V1     |
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|    ADC1    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    ADC2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     GND    |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |           N.A.           |           N.A.           |           N.A.           |         Not Used         |           N.A.           |           N.A.           |
|     IO0    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO1    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO11    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO12    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO13    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO14    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO15    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO16    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO17    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO18    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO19    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|     IO2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    IO20    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO21    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO22    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO23    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO24    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|    IO25    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |
|     IO3    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO4    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO5    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO6    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO7    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|     IO8    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|   RESETN   |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |           N.A.           |           N.A.           |           N.A.           |         Not Used         |           N.A.           |           N.A.           |
|    TEST1   |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |
|    VBLUE   |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |           N.A.           |           N.A.           |           N.A.           |         Not Used         |           N.A.           |           N.A.           |

@endtable 

* \section Serial_IO Serial I/O
@table
| Parameter name  | Value            | Unit      |
----------------------------------------------------
| Baudrate        | 115200 [default] | bit/sec   |
| Data bits       | 8                | bit       |
| Parity          | None             | bit       |
| Stop bits       | 1                | bit       |
@endtable

* \section LEDs_description LEDs description
@table
|            |                                                           Release                                                           ||||||                                                                                                                           Use_OTA_ServiceManager                                                                                                                            ||||||
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|  LED name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |  STEVAL-IDB008V1M  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |               STEVAL-IDB007V1              |               STEVAL-IDB007V2              |               STEVAL-IDB008V1              |              STEVAL-IDB008V1M              |               STEVAL-IDB008V2              |               STEVAL-IDB009V1              |
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|     DL1    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |
|     DL2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |
|     DL3    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |
|     DL4    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |

@endtable


* \section Buttons_description Buttons description
@table
|                |                                                           Release                                                           ||||||                                                                                                                                                                                                               Use_OTA_ServiceManager                                                                                                                                                                                                                ||||||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|   BUTTON name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |  STEVAL-IDB008V1M  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |                             STEVAL-IDB007V1                            |                             STEVAL-IDB007V2                            |                             STEVAL-IDB008V1                            |                            STEVAL-IDB008V1M                            |                             STEVAL-IDB008V2                            |                             STEVAL-IDB009V1                            |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|      PUSH1     |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |  It allows BLE Beacon application to activate the OTA Service Manager  |  It allows BLE Beacon application to activate the OTA Service Manager  |  It allows BLE Beacon application to activate the OTA Service Manager  |  It allows BLE Beacon application to activate the OTA Service Manager  |  It allows BLE Beacon application to activate the OTA Service Manager  |  It allows BLE Beacon application to activate the OTA Service Manager  |
|      PUSH2     |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                                Not Used                                |                                Not Used                                |                                Not Used                                |                                Not Used                                |                                Not Used                                |                                Not Used                                |
|      RESET     |   Reset BlueNRG1   |   Reset BlueNRG1   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG2   |                             Reset BlueNRG1                             |                             Reset BlueNRG1                             |                             Reset BlueNRG2                             |                             Reset BlueNRG2                             |                             Reset BlueNRG2                             |                             Reset BlueNRG2                             |

@endtable

* \section Usage Usage

The Beacon demo configures a BlueNRG-1,2 device in advertising mode (non-connectable mode) with specific manufacturing data.
It transmits advertisement packets at regular intervals which contain the following manufacturing data:
@table   
------------------------------------------------------------------------------------------------------------------------
| Data field              | Description                       | Notes                                                  |
------------------------------------------------------------------------------------------------------------------------
| Company identifier code | SIG company identifier (1)        | Default is 0x0030 (STMicroelectronics)                 |
| ID                      | Beacon ID                         | Fixed value                                            |
| Length                  | Length of the remaining payload   | NA                                                     |
| Location UUID           | Beacons UUID                      | It is used to distinguish specific beacons from others |
| Major number            | Identifier for a group of beacons | It is used to group a related set of beacons           |                                              
| Minor number            | Identifier for a single beacon    | It is used to identify a single beacon                 |                                       
| Tx Power                | 2's complement of the Tx power    | It is used to establish how far you are from device    |                                       
@endtable

 - (1): SIG company identifiers are available on https://www.bluetooth.org/en-us/specification/assigned-numbers/company-identifiers
 - NA : Not Applicable;
NOTEs:
     - OTA Service Manager support requires to build application by enabling only ST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 (preprocessor, linker) options and through files: OTA_btl.[ch] (refer to Release_with_OTA_ServiceManager IAR workspace).

**/
   
/** @addtogroup BlueNRG1_demonstrations_applications
 *  BlueNRG-1 Beacon demo \see BLE_Beacon_main.c for documentation.
 *
 *@{
 */

/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "bluenrg1_hal.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "sleep.h"
#include "SDK_EVAL_Config.h"
#include "Beacon_config.h"
#include "OTA_btl.h"
#include "clock.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLE_BEACON_VERSION_STRING "1.1.0"
#define LOCAL_NAME  'B','l','u','e','N','R','G','1'


/* Set to 1 for enabling Flags AD Type position at the beginning 
   of the advertising packet */
#define ENABLE_FLAGS_AD_TYPE_AT_BEGINNING 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Device_Init(void)
{
  uint8_t ret;
  uint16_t service_handle;
  uint16_t dev_name_char_handle;
  uint16_t appearance_char_handle;
  
  /* Set the TX Power to -2 dBm */
  ret = aci_hal_set_tx_power_level(1,4);
  if(ret != 0) {
    printf ("Error in aci_hal_set_tx_power_level() 0x%04xr\n", ret);
    while(1);
  }

  /* Init the GATT */
  ret = aci_gatt_init();
  if (ret != 0) 
    printf ("Error in aci_gatt_init() 0x%04xr\n", ret);
  else
    printf ("aci_gatt_init() --> SUCCESS\r\n");
  
  /* Init the GAP */
  ret = aci_gap_init(0x01, 0x00, 0x08, &service_handle, 
                     &dev_name_char_handle, &appearance_char_handle);
  if (ret != 0)
    printf ("Error in aci_gap_init() 0x%04x\r\n", ret);
  else
    printf ("aci_gap_init() --> SUCCESS\r\n");

  uint8_t name[] = {LOCAL_NAME };

    /* Set the device name */
  ret = aci_gatt_update_char_value_ext(0,service_handle, dev_name_char_handle,0,sizeof(name),0, sizeof(name), name);
  if (ret != BLE_STATUS_SUCCESS) {
    printf ("Error in Gatt Update characteristic value 0x%02x\r\n", ret);
    return ret;
  } else {
    printf ("aci_gatt_update_char_value_ext() --> SUCCESS\r\n");
  }

}


/**
* @brief  Start beaconing
* @param  None 
* @retval None
*/
static void Start_Beaconing(void)
{  
  uint8_t ret = BLE_STATUS_SUCCESS;

uint8_t local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, LOCAL_NAME };

#if ENABLE_FLAGS_AD_TYPE_AT_BEGINNING
  /* Set AD Type Flags at beginning on Advertising packet  */
  uint8_t adv_data[] = {
      /* Advertising data: Flags AD Type */
      0x02, 
      0x01, 
      0x06, 
      sizeof(local_name),                      /* Length of AD type Complete Local Name */
      0x09,                   /* AD type Complete Local Name */ 
      LOCAL_NAME,             /* Local Name */            
      /* Advertising data: manufacturer specific data */
      26, //len
      AD_TYPE_MANUFACTURER_SPECIFIC_DATA,  //manufacturer type
      0x30, 0x00, //Company identifier code (Default is 0x0030 - STMicroelectronics: To be customized for specific identifier)
      0x02,       // ID
      0x15,       //Length of the remaining payload
      0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, //Location UUID
      0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61,
      0x00, 0x00, // Major number 
      0x00, 0x00, // Minor number 
      0xC8        //2's complement of the Tx power (-56dB)};      
   };
#else
   uint8_t manuf_data[] = {
      26, //len
      AD_TYPE_MANUFACTURER_SPECIFIC_DATA, //manufacturer type
      0x30, 0x00, //Company identifier code (Default is 0x0030 - STMicroelectronics: To be customized for specific identifier)
      0x02,       // ID
      0x15,       //Length of the remaining payload
      0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, //Location UUID
      0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61,
      0x00, 0x00, // Major number 
      0x00, 0x00, // Minor number 
      0xC8        //2's complement of the Tx power (-56dB)};      
   };
#endif
   
  /* disable scan response */
  ret = hci_le_set_scan_response_data(0,NULL);
  if (ret != BLE_STATUS_SUCCESS)
  {
    printf ("Error in hci_le_set_scan_resp_data() 0x%04x\r\n", ret);
    return;
  }
  else
    printf ("hci_le_set_scan_resp_data() --> SUCCESS\r\n");


  /* put device in non connectable mode */
  ret = aci_gap_set_discoverable(ADV_NONCONN_IND, 160, 160, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                sizeof(local_name), local_name, 0, NULL, 0, 0); 
  if (ret != BLE_STATUS_SUCCESS)
  {
    printf ("Error in aci_gap_set_discoverable() 0x%04x\r\n", ret);
    return;
  }
  else
    printf ("aci_gap_set_discoverable() --> SUCCESS\r\n");

#if ENABLE_FLAGS_AD_TYPE_AT_BEGINNING
  /* Set the  ADV data with the Flags AD Type at beginning of the 
     advertsing packet,  followed by the beacon manufacturer specific data */
  ret = hci_le_set_advertising_data (sizeof(adv_data), adv_data);
  if (ret != BLE_STATUS_SUCCESS)
  {
    printf ("Error in hci_le_set_advertising_data() 0x%04x\r\n", ret);
    return;
  }
  else
    printf ("hci_le_set_advertising_data() --> SUCCESS\r\n");
#else
  /* Delete the TX power level information */
  ret = aci_gap_delete_ad_type(AD_TYPE_TX_POWER_LEVEL); 
  if (ret != BLE_STATUS_SUCCESS)
  {
    printf ("Error in aci_gap_delete_ad_type() 0x%04x\r\n", ret);
    return;
  }
  else
    printf ("aci_gap_delete_ad_type() --> SUCCESS\r\n");

  /* Update the ADV data with the BEACON manufacturing data */
  ret = aci_gap_update_adv_data(27, manuf_data);  
  if (ret != BLE_STATUS_SUCCESS)
  {
    printf ("Error in aci_gap_update_adv_data() 0x%04x\r\n", ret);
    return;
  }
  else
    printf ("aci_gap_update_adv_data() --> SUCCESS\r\n");
#endif
}

tClockTime lastClock = 0;
uint16_t delay = 500;

int main(void) {
  uint8_t ret;

  /* System Init */
  SystemInit();
  
  /* Identify BlueNRG-1 platform */
  SdkEvalIdentification();


  /* Init the UART peripheral */
  SdkEvalComUartInit(UART_BAUDRATE); 
  
  //Enable Systick Clock (required for delays and such)
  Clock_Init();

  //Enable peripheral clock for LED
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

  GPIO_InitType GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_Pull = ENABLE;
  GPIO_InitStructure.GPIO_HighPwr = ENABLE;
  GPIO_Init(&GPIO_InitStructure);

  /* Put the LEDs off */
  GPIO_WriteBit(GPIO_Pin_14, LED_ON);

  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
 //The EMB1061 has a preprogrammed mac address, which is also printed on the QR code
  //If you are using some other module, you may need to change this address or replace this with a byte array
  uint8_t *macAddressLocation = (uint8_t *)0x10066800;

  // For example, mac address 047863AB209D would be
  // uint8_t macAddressLocation[] = {0x9D, 0x20, 0xAB, 0x63, 0x78, 0x04};

  // Set the mac address in the BLE stack to the value stored by the manufacturer.
  ret=aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,CONFIG_DATA_PUBADDR_LEN, macAddressLocation);
  if(ret) {printf("Setting address failed.\n");}
  
  /* Init the BlueNRG-1 device */
  Device_Init();

    /* Configures Button pin as input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Input;
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init(&GPIO_InitStructure);


  /* Start Beacon Non Connectable Mode*/
  Start_Beaconing();
  
  printf("BlueNRG-1 BLE Beacon Application (version: %s)\r\n", BLE_BEACON_VERSION_STRING); 
  
  
  while(1) 
  {
    //printf("%lu\n",(uint32_t)Clock_Time());
    if (!GPIO_ReadBit(GPIO_Pin_13))
    {
      if(delay != 100){
        printf("Pressed!\n");
      }
      delay = 100;
    }else{
      if(delay != 500){
        printf("Released!\n");
      }
      delay = 500;
    }

    //Every 500 ms toggle the LED, making a 1hz flash
    if(((uint32_t)lastClock)+delay<=(uint32_t)Clock_Time()){
      lastClock = lastClock+delay;
      printf("%lu\n",(uint32_t)Clock_Time());
      GPIO_ToggleBits(GPIO_Pin_14);
    }
    /* BlueNRG-1 stack tick */
    BTLE_StackTick();
        
    /* Enable Power Save according the Advertising Interval */
    // ! NOTE: This can mess with things like UART, systick/timers, and uploading code.
    // ! If you are getting errors like "Failed to initialize GDB server" or something,
    // ! hold the boot button, press reset, then release the boot button
    // ! If you are getting errors like " Error erasing flash with vFlashErase ...",
    // ! When you run the debugger, quickly press and release the reset button right after running
    //BlueNRG_Sleep(SLEEPMODE_NOTIMER, 0, 0);
    
#if ST_USE_OTA_SERVICE_MANAGER_APPLICATION
    if (SdkEvalPushButtonGetState(USER_BUTTON) == RESET)
    {
    	GPIO_WriteBit(GPIO_Pin_14, LED_OFF);

    	OTA_Jump_To_Service_Manager_Application();
    }else{
    	GPIO_WriteBit(GPIO_Pin_14, LED_ON);
    }
#endif /* ST_USE_OTA_SERVICE_MANAGER_APPLICATION */
  }
}

/* Hardware Error event. 
   This event is used to notify the Host that a hardware failure has occurred in the Controller. 
   Hardware_Code Values:
   - 0x01: Radio state error
   - 0x02: Timer overrun error
   - 0x03: Internal queue overflow error
   After this event is recommended to force device reset. */

void hci_hardware_error_event(uint8_t Hardware_Code)
{
   NVIC_SystemReset();
}


/****************** BlueNRG-1 Sleep Management Callback ********************************/

SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
  if(SdkEvalComIOTxFifoNotEmpty() || SdkEvalComUARTBusy())
    return SLEEPMODE_RUNNING;
  
  return SLEEPMODE_NOTIMER;
}

/***************************************************************************************/

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
/** \endcond
 */

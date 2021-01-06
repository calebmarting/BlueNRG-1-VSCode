/**
  ******************************************************************************
  * @file    freertos_ble.h 
  * @author  RF Application team
  * @version V1.0.0
  * @date    September-2014
  * @brief   Header file to support use of BlueNRG stack with FreeRTOS
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
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FREERTOS_BLE_H
#define FREERTOS_BLE_H

/* Binary semaphore used to synchronize Stack Tick and radio ISR. */
extern SemaphoreHandle_t radioActivitySemaphoreHandle;
/* Mutex used to avoid that the BLE Stack Tick can be interrupted by an ACI
   function in another thread. */
extern SemaphoreHandle_t BLETickSemaphoreHandle;

/* Macro to be used to call ACI functions from a thread different from the BLETask.
  It also unblock it.
E.g.: BLE_ACI_PROTECTED(ret = hci_le_set_advertising_data(sizeof(data),data));
*/
#define BLE_ACI_PROTECTED(expression) {                                                         \
                                        xSemaphoreTake(BLETickSemaphoreHandle, portMAX_DELAY);  \
                                        expression;                                             \
                                        xSemaphoreGive(radioActivitySemaphoreHandle);           \
                                        xSemaphoreGive(BLETickSemaphoreHandle);                 \
                                       }

/* Set wakeup GPIOs to be used when FreeRTOS Idle Task will call BlueNRG sleep function */
void BlueNRG_SetWakeupMask(uint8_t gpioWakeBitMask, uint8_t gpioWakeLevelMask);


#endif /* FREERTOS_BLE_H */

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

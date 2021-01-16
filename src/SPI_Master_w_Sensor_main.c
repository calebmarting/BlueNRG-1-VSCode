/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : SPI/Master_w_Sensor/main.c 
* Author             : RF Application Team
* Version            : V1.0.0
* Date               : September-2015
* Description        : Code demostrating the SPI master functionality using the inertial sensor
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include <stdio.h>
#include "BlueNRG1_conf.h"
#include "BlueNRG1_it.h"
#include "SDK_EVAL_Config.h"
#include "lsm6ds3.h"
#include "string.h"

/** @addtogroup BlueNRG1_StdPeriph_Examples BlueNRG1 Standard Peripheral Examples
* @{
*/

/** @addtogroup SPI_Examples SPI Examples
* @{
*/

/** @addtogroup SPI_Master_w_Sensor SPI Master w Sensor
* @{
*/

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  int32_t AXIS_X;
  int32_t AXIS_Y;
  int32_t AXIS_Z;
} AxesAccRaw_TypeDef, AxesGyrRaw_TypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile FlagStatus GpioIrqFlag;
static IMU_6AXES_DrvTypeDef *Imu6AxesDrv = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#include "LSM6DS3_hal.h"

/**
* @brief  Main program
*/  
int main(void)
{
  AxesAccRaw_TypeDef pAccData;
  AxesGyrRaw_TypeDef pGyrData;
  
  /* System initialization function */
  SystemInit();
  
  /* Identify BlueNRG1 platform */
  SdkEvalIdentification();

  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);


  GPIO_InitType GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_Pull = ENABLE;
  GPIO_InitStructure.GPIO_HighPwr = ENABLE;
  GPIO_Init(&GPIO_InitStructure);


  /* Put the LEDs off */
  GPIO_WriteBit(GPIO_Pin_14, LED_ON);
  /* UART initialization */
 SdkEvalComUartInit(UART_BAUDRATE);

 /* LSM6DS3 library setting */
 IMU_6AXES_InitTypeDef InitStructure;
 Imu6AxesDrv = &LSM6DS3Drv;

 InitStructure.G_FullScale      = 125;
 InitStructure.G_OutputDataRate = 52;
 InitStructure.G_X_Axis         = 1;
 InitStructure.G_Y_Axis         = 1;
 InitStructure.G_Z_Axis         = 1;
 InitStructure.X_FullScale      = 2;
 InitStructure.X_OutputDataRate = 52;
 InitStructure.X_X_Axis         = 1;
 InitStructure.X_Y_Axis         = 1;
 InitStructure.X_Z_Axis         = 1;

  printf("oof\n");

 /* LSM6DS3 initiliazation */
 Imu6AxesDrv->Init(&InitStructure);
    
 printf("ACC_X, ACC_Y, ACC_Z, GYR_X, GYR_Y, GYR_Z\r\n");

  /* Infinite loop */
  while(1) {
	  // printf("F\n");
   /* Waiting for interrupt flag to be SET */
   if(GpioIrqFlag == SET) {
     GpioIrqFlag = RESET;

     /* Read data from LSM6DS3 */
    Imu6AxesDrv->Get_X_Axes((int32_t *)&pAccData);
    Imu6AxesDrv->Get_G_Axes((int32_t *)&pGyrData);
    printf("%d, %d, %d, %d, %d, %d\r\n", (int)pAccData.AXIS_X, (int)pAccData.AXIS_Y, (int)pAccData.AXIS_Z, (int)pGyrData.AXIS_X, (int)pGyrData.AXIS_Y, (int)pGyrData.AXIS_Z);
    SdkEvalLedToggle(LED1);
   }
   else {
     /* Clear first previous data */
     Imu6AxesDrv->Get_X_Axes((int32_t *)&pAccData);
     Imu6AxesDrv->Get_G_Axes((int32_t *)&pGyrData);
   }
    
  }
  
}



#ifdef USE_FULL_ASSERT

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

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

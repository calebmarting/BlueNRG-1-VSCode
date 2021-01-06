/**
******************************************************************************
* @file    HTS221_HAL.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
* @brief   HTS221 hal driver header file

* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>

******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HTS221_HAL__H
#define __HTS221_HAL__H
 
#include <stdint.h>
#include "BlueNRG1_conf.h"
#include "HTS221_Driver.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* SPI baudrate */
#define SPI_HTS221_BAUDRATE            (1000000)
   
/* SPI port config */
#define SPI_CS_HTS221_PIN              GPIO_Pin_6
#define SPI_MISO_HTS221_PIN            GPIO_Pin_3
#define SPI_MOSI_HTS221_PIN            GPIO_Pin_2
#define SPI_CLOCK_HTS221_PIN           GPIO_Pin_0   
   

void HTTS221_HAL_SPI_Init(void);
HTS221_Error_et HAL_ReadReg(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer);
HTS221_Error_et HAL_WriteReg(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer);


#ifdef __cplusplus
}
#endif


#endif /* __HTS221_HAL__H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

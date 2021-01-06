/**
******************************************************************************
* @file    HTS221_HAL.c
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
* @brief   HTS221 hal driver file

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

/* Includes ------------------------------------------------------------------*/
#include "HTS221_HAL.h"

/* Private Functions Declaration */
HTS221_Error_et HAL_ReadRegLowLvl(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer);
HTS221_Error_et HAL_WriteRegLowLvl(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer);

#define LOW_SPI_CS_HTS221()             GPIO_ResetBits(SPI_CS_HTS221_PIN)
#define HIGH_SPI_CS_HTS221()            GPIO_SetBits(SPI_CS_HTS221_PIN)


/* Public Functions Definition */
void HTTS221_HAL_SPI_Init(void)
{
  GPIO_InitType GPIO_InitStructure;
  SPI_InitType SPI_InitStructure;
  
  /* Enable SPI and GPIO clocks */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);   
  
  /* Configure SPI pins */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_HTS221_PIN;
  GPIO_InitStructure.GPIO_Mode = Serial0_Mode;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = SPI_MISO_HTS221_PIN;
  GPIO_InitStructure.GPIO_Mode = Serial0_Mode;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = SPI_CLOCK_HTS221_PIN;
  GPIO_InitStructure.GPIO_Mode = Serial0_Mode;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = SPI_CS_HTS221_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_Init(&GPIO_InitStructure);
  GPIO_SetBits(SPI_CS_HTS221_PIN);
  
  /* Configure SPI in master mode */
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_BaudRate = SPI_HTS221_BAUDRATE;
  SPI_Init(&SPI_InitStructure);
  
  /* Set communication mode */ 
  SPI_FrameFormatConfig(SPI_FrmFrmt_Microwire);
}

HTS221_Error_et HAL_ReadReg(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer)
{  
  uint8_t cnt = 0;
  
  /* If num_regs is 0 exit */
  if(num_regs == 0) {
    return HTS221_ERROR;
  }
  
  /* If num_regs is greater than 4, read 4 registers at time */
  while(num_regs > 4) {
    if(HAL_ReadRegLowLvl(reg_addr, 4, &buffer[cnt]) == HTS221_ERROR) {
      return HTS221_ERROR;
    }
    num_regs -= 4;
    reg_addr += 4;
    cnt += 4;
  }
  /* If num_regs is max 4, make a read operation */
  if(HAL_ReadRegLowLvl(reg_addr, num_regs, &buffer[cnt])) {
    return HTS221_ERROR;
  }
  return HTS221_OK;
}


HTS221_Error_et HAL_WriteReg(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer)
{  
  uint8_t cnt = 0;
  
  /* If num_regs is 0 exit */
  if(num_regs == 0) {
    return HTS221_ERROR;
  }
  
  /* If num_regs is greater than 3, write 3 registers at time */
  while(num_regs > 3) {
    if(HAL_WriteRegLowLvl(reg_addr, 3, &buffer[cnt]) == HTS221_ERROR) {
      return HTS221_ERROR;
    }
    num_regs -= 3;
    reg_addr += 3;
    cnt += 3;
  }
  /* If num_regs is max 3, make a write operation */
  if(HAL_WriteRegLowLvl(reg_addr, num_regs, &buffer[cnt])) {
    return HTS221_ERROR;
  }
  return HTS221_OK;
}

/* Private Function Definition */
HTS221_Error_et HAL_ReadRegLowLvl(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer)
{
  uint8_t i = 0;
  uint32_t data = 0;
  
  /* Maximum number of byte that can be read is 4 */
  if(num_regs>4 || num_regs == 0) {
    return HTS221_ERROR;
  }
  
  /* Configure the command data size */
  SPI_CommandSizeConfig(SPI_DataSize_8b);
  
  /* Configure the data size */
  SPI_DataSizeConfig((num_regs*8)-1);
  
  /* Load the TX FIFO buffer with the data to send */
  if(num_regs==1) {
    SPI_SendData(0x80 | reg_addr);
  }
  else {
    SPI_SendData(0x80 | reg_addr | 0x40);
  }
  
  /* Low the CS pin */
  LOW_SPI_CS_HTS221();
  
  /* Enable the SPI and send the command header */
  SPI_Cmd(ENABLE);
  
  /* Read the data */
//  while(RESET == READ_BIT(SPI->SR, SPI_FLAG_RNE));
  while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
  data = SPI->DR;
  
  /* Wait for the end of the SPI operation */
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  
  /* High the CS pin */
  GPIO_SetBits(SPI_CS_HTS221_PIN);
  
  /* Disable the SPI */
  SPI_Cmd(DISABLE);
  
  /* Copy the buffer */
  for(i = 0; i < num_regs; i++) {
    buffer[i] = (uint8_t)(data>>(8*(num_regs-1-i)));
  }
  return HTS221_OK;
}


HTS221_Error_et HAL_WriteRegLowLvl(uint8_t reg_addr, uint8_t num_regs, uint8_t* buffer)
{
  uint8_t i = 0;
  uint32_t command = 0;
  
  /* Configure the command data size */
  SPI_CommandSizeConfig(((num_regs+1)*8)-1);
  
  /* Reset the data size */
  SPI_DataSizeConfig(0);
  
  /* Set the command frame */
  command |= (((uint32_t)reg_addr)<<(8*num_regs));
  for(i = 0; i < num_regs; i++) {
    command |= (((uint32_t)buffer[i])<<(8*(num_regs-i-1)));
  }
  
  /* Load the TX FIFO buffer with the data to send */
  SPI_SendData(command);
  
  /* Low the CS pin */
  LOW_SPI_CS_HTS221();
  
  /* Enable the SPI and send the command header */
  SPI_Cmd(ENABLE);
  
  /* Wait for the end of the SPI operation */
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  
  /* Dummy read operation */
  command = SPI->DR;
  
  /* High the CS pin */
  GPIO_SetBits(SPI_CS_HTS221_PIN);
  
  /* Disable the SPI */
  SPI_Cmd(DISABLE);
  
  return HTS221_OK;
}

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

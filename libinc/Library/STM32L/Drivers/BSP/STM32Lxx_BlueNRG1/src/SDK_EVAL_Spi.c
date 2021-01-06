/**
* @file    SDK_EVAL_SPI_Driver.c
* @author  AMS VMA RF application team
* @version V1.0.3
* @date    May 15, 2015
* @brief   This file provides all the low level SPI API to access to BlueNRG module
* @details It uses STM32L1 Cube APIs + Optimized APIs for SPI Transmit, Receive, 
*          Transmit & Receive (these APIs are needed for BLE communication
*          throughput) 
*
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
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
*/

/* Includes ------------------------------------------------------------------*/
#include "clock.h" 
#include "osal.h"
#include "gp_timer.h"
#include <stdio.h>
#include "SDK_EVAL_Config.h"

/** @addtogroup SDK_EVAL_STM32L
* @{
*/

/** @addtogroup SDK_EVAL_Spi                    SDK EVAL Spi
* @brief SPI functions implementation.
* @details This file implements the BlueNRG Library SPI interface functions. 
* @{
*/

/** @defgroup STM32_BLUENRG_BLE_Private_Defines 
* @{
*/ 

#define HEADER_SIZE 5


uint8_t sdk_buffer[SDK_BUFFER_SIZE];

/**
* @}
*/

/** @defgroup SPI_Private_Variables
* @{
*/

/**
* @}
*/

/** @defgroup SPI_Functions
* @{
*/

volatile uint32_t clock_time = 0;
/* timeout is defined in unit of 100 ms */
#define WAIT_IRQ_LOW(func, timeout)                     \
{                                                       \
  clock_time = 0;                                       \
  LL_TIM_GenerateEvent_UPDATE(TIM2);                    \
  LL_TIM_EnableCounter(TIM2);                           \
    while( clock_time < timeout)                        \
        if(func == 0)                                   \
          break;                                        \
  LL_TIM_DisableCounter(TIM2);                          \
}


void  Configure_TIMTimeBase(void)
{
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); 
  
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);

  /* Set the pre-scaler value to have TIM2 counter clock equal to 10 kHz      */
  /*
   In this example TIM2 input clock TIM2CLK is set to APB2 clock (PCLK2),   
   since APB2 pre-scaler is equal to 1.                                     
      TIM2CLK = PCLK2                                                       
      PCLK2 = HCLK                                                          
      => TIM2CLK = SystemCoreClock (80 MHz)                                 
  */
  LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));
  
  /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
//  InitialAutoreload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM2), 10);
  LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM2), 10));
  
  /* Enable the update interrupt */
  LL_TIM_EnableIT_UPDATE(TIM2);
  
  /* Configure the NVIC to handle TIM2 update interrupt */
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);

}


/**
* @brief  Initializes the SPI 
* @param  xSpiMode: SPI mode - GPIO (polling) or EXTI (IRQ)
* @retval status
*/
void SdkEvalSpiInit(void)
{
  /* Enable the GPIO clocks */
  DTM_SPI_SCLK_CLK_ENABLE();
  DTM_SPI_MISO_CLK_ENABLE();
  DTM_SPI_MOSI_CLK_ENABLE();
  DTM_SPI_CS_CLK_ENABLE();
  DTM_SPI_IRQ_CLK_ENABLE();
  
  /* Configure SCK Pin connected to pin 31 of CN10 connector */
  LL_GPIO_SetPinMode(DTM_SPI_SCLK_PORT, DTM_SPI_SCLK_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(DTM_SPI_SCLK_PORT, DTM_SPI_SCLK_PIN, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(DTM_SPI_SCLK_PORT, DTM_SPI_SCLK_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(DTM_SPI_SCLK_PORT, DTM_SPI_SCLK_PIN, LL_GPIO_PULL_DOWN);
  
  /* Configure MISO Pin connected to pin xx of CNxx connector */
  LL_GPIO_SetPinMode(DTM_SPI_MISO_PORT, DTM_SPI_MISO_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(DTM_SPI_MISO_PORT, DTM_SPI_MISO_PIN, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(DTM_SPI_MISO_PORT, DTM_SPI_MISO_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(DTM_SPI_MISO_PORT, DTM_SPI_MISO_PIN, LL_GPIO_PULL_DOWN);
  
  /* Configure MOSI Pin connected to pin 29 of CN10 connector */
  LL_GPIO_SetPinMode(DTM_SPI_MOSI_PORT, DTM_SPI_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(DTM_SPI_MOSI_PORT, DTM_SPI_MOSI_PIN, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(DTM_SPI_MOSI_PORT, DTM_SPI_MOSI_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(DTM_SPI_MOSI_PORT, DTM_SPI_MOSI_PIN, LL_GPIO_PULL_DOWN);
  
  /* NSS/CSN/CS */
  LL_GPIO_SetPinMode(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
  
  /* IRQ -- INPUT */
  LL_GPIO_SetPinMode(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN, LL_GPIO_PULL_DOWN); 
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_SYSCFG_SetEXTISource(DTM_SPI_IRQ_SYSCFG_PORT, DTM_SPI_IRQ_SYSCFG_LINE);
  
  DTM_SPI_IRQ_EXTI_LINE_ENABLE();
  DTM_SPI_IRQ_EXTI_TRIG_ENABLE();
  
  NVIC_EnableIRQ(DTM_SPI_EXTI_IRQn); 
  NVIC_SetPriority(DTM_SPI_EXTI_IRQn, 4);
  
  /* SPI initialization */  
  DTM_SPI_CLK_ENABLE();
  
  /* Configure SPI1 communication */
  LL_SPI_SetBaudRatePrescaler(DTM_SPI_INSTANCE, DTM_SPI_BAUDRATEPRESCALER);
  LL_SPI_SetTransferDirection(DTM_SPI_INSTANCE, LL_SPI_FULL_DUPLEX);
  LL_SPI_SetClockPhase(DTM_SPI_INSTANCE, DTM_SPI_CLKPHASE);
  LL_SPI_SetClockPolarity(DTM_SPI_INSTANCE, DTM_SPI_CLKPOLARITY);
  LL_SPI_SetTransferBitOrder(DTM_SPI_INSTANCE, DTM_SPI_FIRSTBIT);
  LL_SPI_SetDataWidth(DTM_SPI_INSTANCE, DTM_SPI_DATASIZE);
  LL_SPI_SetNSSMode(DTM_SPI_INSTANCE, LL_SPI_NSS_SOFT);
  LL_SPI_SetMode(DTM_SPI_INSTANCE, LL_SPI_MODE_MASTER);
#ifdef STM32L476xx
  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);
#endif
  /* Enable SPI1 */
  LL_SPI_Enable(DTM_SPI_INSTANCE);
  
  Configure_TIMTimeBase();
}


/**
* @brief  Read from BlueNRG SPI buffer and store data into local buffer 
* @param  buffer:    buffer where data from SPI are stored
*         buff_size: buffer size
* @retval number of read bytes
*/
uint8_t BlueNRG_SPI_Read(uint8_t *buffer, uint16_t buff_size)
{
  uint16_t byte_count;
  const uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];
  
  if(LL_GPIO_IsInputPinSet(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN) != 1) {
    return 0;
  }

  Disable_IRQ();
  
  LL_GPIO_ResetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
  
  /* Read the header */
  for (uint8_t i = 0; i < HEADER_SIZE; i++) {
    while(LL_SPI_IsActiveFlag_TXE(DTM_SPI_INSTANCE) == RESET);
    LL_SPI_TransmitData8(DTM_SPI_INSTANCE, header_master[i]);    
    while(LL_SPI_IsActiveFlag_RXNE(DTM_SPI_INSTANCE) == RESET);
    header_slave[i] = LL_SPI_ReceiveData8(DTM_SPI_INSTANCE);
  }
    
  byte_count = ((uint16_t)header_slave[4])<<8 | (uint16_t)header_slave[3];
    
  if (byte_count > 0) {
    if (byte_count > buff_size)
      byte_count = buff_size;
    
    for (uint16_t i = 0; i < byte_count; i++) {
      while(LL_SPI_IsActiveFlag_TXE(DTM_SPI_INSTANCE) == RESET);
      LL_SPI_TransmitData8(DTM_SPI_INSTANCE, 0x00); 
      while(LL_SPI_IsActiveFlag_RXNE(DTM_SPI_INSTANCE) == RESET);
      buffer[i] = LL_SPI_ReceiveData8(DTM_SPI_INSTANCE);
    }
  }
  
#ifndef BTLE_NWK_COPROC
  for(uint16_t i = 0; i< byte_count;i++)
    putchar(buffer[i]);
#endif
  
  /* To be aligned to the SPI protocol. Can bring to a delay inside the frame, due to the BlueNRG-1 that needs to check if the header is received or not */
  for(volatile int a =0; a<0xF;a++);
  WAIT_IRQ_LOW(LL_GPIO_IsInputPinSet(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN), 10);
  Enable_IRQ();
  
  LL_GPIO_SetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
  
  return byte_count;
  
}

    
/**
* @brief  Write data from local buffer to SPI
* @param  data1:    first data buffer to be written, used to send header of higher
*                   level protocol
*         data2:    second data buffer to be written, used to send payload of higher
*                   level protocol
*         Nb_bytes1: size of header to be written
*         Nb_bytes2: size of payload to be written
* @retval Number of payload bytes that has been sent. If 0, all bytes in the header has been
*         written.
*/
uint8_t BlueNRG_SPI_Write(uint8_t* data, uint16_t Nb_bytes)
{  
  uint16_t rx_bytes;
  struct timer t;
  
  const uint8_t header_master[5] = {0x0A, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5]  = {0x00};
  
  Disable_IRQ();
  
  Timer_Set(&t, CLOCK_SECOND/10);
  
  LL_GPIO_ResetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
  
  while(LL_GPIO_IsInputPinSet(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN) != 1) {
    if(Timer_Expired(&t)){
      LL_GPIO_SetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
      Enable_IRQ();
      return 1;
    }
  }
  
  for (uint8_t i = 0; i < HEADER_SIZE; i++) {
    while(LL_SPI_IsActiveFlag_TXE(DTM_SPI_INSTANCE) == RESET);
    LL_SPI_TransmitData8(DTM_SPI_INSTANCE, header_master[i]);
    while(LL_SPI_IsActiveFlag_RXNE(DTM_SPI_INSTANCE) == RESET);
    header_slave[i] = LL_SPI_ReceiveData8(DTM_SPI_INSTANCE);
  }
  
  rx_bytes = (((uint16_t)header_slave[2])<<8) | ((uint16_t)header_slave[1]);
  
  if((Nb_bytes-1) > rx_bytes){
    LL_GPIO_SetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
    Enable_IRQ();
    return 1;
  }
  
  for (uint16_t i = 0; i < Nb_bytes; i++) {
    while(LL_SPI_IsActiveFlag_TXE(DTM_SPI_INSTANCE) == RESET);
    LL_SPI_TransmitData8(DTM_SPI_INSTANCE, data[i]);
    while(LL_SPI_IsActiveFlag_RXNE(DTM_SPI_INSTANCE) == RESET);
    LL_SPI_ReceiveData8(DTM_SPI_INSTANCE);
  }
  
  /* To be aligned to the SPI protocol. Can bring to a delay inside the frame, due to the BlueNRG-1 that needs to check if the header is received or not */
  for(volatile int a =0; a<0xF;a++);
  WAIT_IRQ_LOW(LL_GPIO_IsInputPinSet(DTM_SPI_IRQ_PORT, DTM_SPI_IRQ_PIN), 10);
  
  Enable_IRQ();
  
  LL_GPIO_SetOutputPin(DTM_SPI_CS_PORT, DTM_SPI_CS_PIN);
  
  return 0;
}


void Hal_Write_Serial(const void* data1, const void* data2, uint16_t n_bytes1, uint16_t n_bytes2)
{
  Osal_MemCpy(sdk_buffer, data1, n_bytes1);
  Osal_MemCpy(&sdk_buffer[n_bytes1], data2, n_bytes2);
  n_bytes1 += n_bytes2;
  
  BlueNRG_SPI_Write(sdk_buffer, n_bytes1);
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



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
        

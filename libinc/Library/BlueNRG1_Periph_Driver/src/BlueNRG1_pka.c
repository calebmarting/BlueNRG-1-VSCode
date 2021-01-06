/**
******************************************************************************
* @file    BlueNRG1_pka.c
* @author  VMA Application Team
* @version V1.0.0
* @date    27-February-2017
* @brief   This file provides all the PKA firmware functions.
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
* <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_pka.h"
#include "osal.h"
/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @defgroup PKA_Peripheral  PKA Peripheral
* @{
*/ 

/** @defgroup PKA_Private_TypesDefinitions Private Types Definitions
* @{
*/

/**
* @}
*/ 

/** @defgroup PKA_Private_Defines Private Defines
* @{
*/ 

/**
* @}
*/ 

/** @defgroup PKA_Private_Macros Private Macros
* @{
*/


/**
* @}
*/ 

/** @defgroup PKA_Private_Variables Private Variables
* @{
*/

/* 0b0000 00XY. X: PKA_DATA_PCX error Y: PKA_DATA_PCY error */
static uint8_t PKA_SetDataError = 0x00;   

/**
* @}
*/ 

/** @defgroup PKA_Private_Functions Private Functions 
* @{
*/


/**
* @}
*/

/** @defgroup PKA_Public_Functions Public Functions
* @{
*/


/**
* @brief  PKA Software Reset.
* @param  None
* @retval None
*/
void PKA_Reset(void)
{
  /* Set Bit SFT_RST of CSR */
  PKA->CSR_b.SFT_RST = SET;
  
  /* the Bit SFT_RST must be reset after set */
  PKA->CSR_b.SFT_RST = RESET;
}


/**
* @brief  PKA start processing command.
* @param  None
* @retval ErrorStatus
* @note   Assumes that Input Data have been already loaded into PKA memory!
*/
void PKA_StartProcessing(void)
{ 
  /* Set Bit GO of CSR */
  PKA->CSR_b.GO = SET;
  
  /* the Bit GO must be reset before the end of calculation */
  PKA->CSR_b.GO = RESET;
  
}


/**
* @brief  Wait loop for READY status
* @param  None
* @retval None
*/
void PKA_WaitProcess(void)
{
  /* Wait PKA processing end */
  while(PKA->CSR_b.READY != SET);
}


/**
* @brief  Return the READY status
* @param  None
* @retval FlagStatus RESET if PKA is busy
*/
FlagStatus PKA_GetProcessStatus(void)
{
  /* Wait PKA processing end */
  return (FlagStatus)(PKA->CSR_b.READY);
}


/**
* @brief  PKA verification of process command.
* @param  None
* @retval ErrorStatus
*/
ErrorStatus PKA_VerifyProcess(void)
{   
  uint32_t errorCode;
  
  if(PKA_SetDataError != 0x00) 
    return ERROR;

  errorCode = *(uint32_t *)(PKA_RAM_ECC_ADDR_KP_ERROR);
  
  if (errorCode == 0) {
    return SUCCESS;
  }
  else {
    return ERROR;
  }
}


/**
* @brief  Enables or disables the specified PKA interrupts.
* @param  PkaFlag: the PKA flag to clear, this can be
*         @arg PKA_PROCEND PKA process end
*         @arg PKA_RAMERR PKA RAM error
*         @arg PKA_ADDERR PKA address invalid
* @param  NewState: functional state @ref FunctionalState
*   This parameter can be: ENABLE or DISABLE.
* @retval None 
*/
void PKA_ITConfig(uint8_t PkaFlag, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_PKA_FLAG(PkaFlag)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE) {
    SET_BIT(PKA->IEN, PkaFlag);
  }
  else if (NewState == DISABLE) {
    CLEAR_BIT(PKA->IEN, PkaFlag);
  }
}


/**
* @brief  Clears the specific PKA pending IT bit.
* @param  PkaFlag: the PKA flag to clear, this can be
*         @arg PKA_PROCEND PKA process end
*         @arg PKA_RAMERR PKA RAM error
*         @arg PKA_ADDERR PKA address invalid
* @retval None
*/
void PKA_ClearITPendingBit(uint8_t PkaFlag)
{
  assert_param(IS_PKA_FLAG(PkaFlag)) ;
  
  /* Clear the flags */
  PKA->ISR = PkaFlag;
}


/**
* @brief  Checks whether the specified PKA flag is set or not.
* @param  PkaFlag: the PKA flag to clear, this can be
*         @arg PKA_PROCEND PKA process end
*         @arg PKA_RAMERR PKA RAM error
*         @arg PKA_ADDERR PKA address invalid
* @retval FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus PKA_GetFlagStatus(uint8_t PkaFlag)
{
  /* Check the parameters */
  assert_param(IS_PKA_FLAG(PkaFlag));
  
  if((PKA->ISR & PkaFlag) != (uint16_t)RESET) {
    return SET;
  }
  else {
    return RESET;
  }
}


/**
* @brief  Internal Utility for PKA key range check
* @param  a: pka key
*         b: reference key
*         bufferSize: key size
* @retval check result 
*/
static int rev_memcmp(uint8_t *a, const uint8_t *b, uint8_t  bufferSize)
{
    uint_fast8_t i = bufferSize;
    int retval = 0;

    do
    {
        i--;
        retval = (int)a[i] - (int)b[i];
        if (retval !=0)
        {
            break;
        }
    } while (i != 0U);

   return retval;
}


/**
* @brief  Write the PKA RAM with the input data.
* @param  dataType: select the region of PKA RAM to write:
*         @arg PKA_DATA_SK is the K value
*         @arg PKA_DATA_PCX is the point X coordinate
*         @arg PKA_DATA_PCY is the point Y coordinate
* @retval ErrorStatus
*/
ErrorStatus PKA_SetData(uint8_t dataType, uint32_t* srcData)
{
  const uint8_t P256_P_LE[32] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xff,0xff,0xff,0xff}; 
  const uint8_t BLE_P256_ABELIAN_ORDER_R_LE[32] = {0x51,0x25,0x63,0xFC,0xC2,0xCA,0xB9,0xF3,0x84,0x9E,0x17,0xA7,0xAD,0xFA,0xE6,0xBC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF};
  uint32_t StartAddress;
  uint8_t idx;
  
  /* Check the parameters */
  assert_param(IS_PKA_CMD(dataType));
  
  if (dataType == PKA_DATA_SK) {
    if (rev_memcmp((uint8_t *) srcData, (uint8_t *)BLE_P256_ABELIAN_ORDER_R_LE, 32) >= 0) {
      PKA_SetDataError |= 0x04;
      return ERROR;
    }
    else {
      PKA_SetDataError &= ~0x04;
    }
    StartAddress = PKA_RAM_ECC_ADDR_K;
  }
  
  else if (dataType == PKA_DATA_PCX) {
    if (rev_memcmp((uint8_t *) srcData, (uint8_t *)P256_P_LE, 32) >= 0) {
      PKA_SetDataError |= 0x01;
      return ERROR;
    }
    else {
      PKA_SetDataError &= ~0x01;
    }
    StartAddress = PKA_RAM_ECC_ADDR_PX;

  }
  
  else if (dataType == PKA_DATA_PCY) {
    if (rev_memcmp((uint8_t *) srcData, (uint8_t *)P256_P_LE, 32) >= 0) {
      PKA_SetDataError |= 0x02;
      return ERROR;
    }
    else {
      PKA_SetDataError &= ~0x02;
    }
    StartAddress = PKA_RAM_ECC_ADDR_PY;
  }
  else 
    return ERROR;

  /* Write the source data to target PKA RAM address. */
  for (idx = 0; idx<8; idx++)
  {
    *(uint32_t *)(StartAddress + 4*idx) = srcData[idx];
  }
  
  /* A 9th word of zeros must be added */
  *(uint32_t *)(StartAddress + 32UL) = 0x00000000;
  
  return SUCCESS;
}


/**
* @brief  Get from the PKA RAM the output data.
* @param  dataType: select the region of PKA RAM to read:
*         @arg PKA_DATA_SK is the K value
*         @arg PKA_DATA_PCX is the point X coordinate
*         @arg PKA_DATA_PCY is the point Y coordinate
* @retval ErrorStatus
*/
ErrorStatus PKA_GetData(uint8_t dataType, uint8_t* dataTarget)
{
  uint32_t StartAddress;
  
  /* Check the parameters */
  assert_param(IS_PKA_CMD(dataType));
  
  if (dataType == PKA_DATA_SK)
    StartAddress = PKA_RAM_ECC_ADDR_K;
  else if (dataType == PKA_DATA_PCX)
    StartAddress = PKA_RAM_ECC_ADDR_PX;
  else if (dataType == PKA_DATA_PCY)
    StartAddress = PKA_RAM_ECC_ADDR_PY;
  else return ERROR;
  
  /* Read the data to target PKA RAM address. */
  for(uint8_t i=0;i<32;i++) {
    dataTarget[i] = ((uint8_t *)StartAddress)[i];
  }
  
  return SUCCESS;
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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

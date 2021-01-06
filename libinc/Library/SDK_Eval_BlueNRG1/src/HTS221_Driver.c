/**
******************************************************************************
* @file    HTS221_Driver.c
* @author  HESA Application Team
* @version 1.0.0
* @date    07/04/2014
* @brief   HTS221 driver file

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
* <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>

******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "HTS221_Driver.h"
#ifdef  USE_FULL_ASSERT_HTS221
#include <stdio.h>
#endif
#include "HTS221_HAL.h"

/** @addtogroup Environmental_Sensor
* @{
*/

/** @defgroup HTS221_DRIVER
* @brief HTS221 DRIVER
* @{
*/


/** @defgroup HTS221_Public_Functions
* @{
*/


/**
* @brief  Get the version of this driver.
* @param  pxVersion pointer to a HTS221_DriverVersion_st structure that contains the version information.
*         This parameter is a pointer to @ref HTS221_DriverVersion_st.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_DriverVersion(HTS221_DriverVersion_st* version)
{
  version->Major = HTS221_DRIVER_VERSION_MAJOR;
  version->Minor = HTS221_DRIVER_VERSION_MINOR;
  version->Point = HTS221_DRIVER_VERSION_POINT;
  
  return HTS221_OK;
}

/**
* @brief  Get device type ID.
* @param  deviceid pointer to the returned device type ID.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_DeviceID(uint8_t* deviceid)
{  
  if(HTS221_ReadReg(HTS221_WHO_AM_I_REG, 1, deviceid))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
 * @brief  Initializes the HTS221 with the specified parameters in HTS221_Init_st struct.
 * @param  pxInit pointer to a HTS221_Init_st structure that contains the configuration.
 *         This parameter is a pointer to @ref HTS221_Init_st.
 * @retval Error code [HTS221_OK, HTS221_ERROR].
 */
HTS221_Error_et HTS221_Set_InitConfig(HTS221_Init_st* pxInit)
{
  uint8_t buffer[3];
  
  HTS221_assert_param(IS_HTS221_AVGH(pxInit->avg_h));
  HTS221_assert_param(IS_HTS221_AVGT(pxInit->avg_t));
  HTS221_assert_param(IS_HTS221_ODR(pxInit->odr));
  HTS221_assert_param(IS_HTS221_State(pxInit->bdu_status));
  HTS221_assert_param(IS_HTS221_State(pxInit->heater_status));
  
  HTS221_assert_param(IS_HTS221_DrdyLevelType(pxInit->irq_level));
  HTS221_assert_param(IS_HTS221_OutputType(pxInit->irq_output_type));
  HTS221_assert_param(IS_HTS221_State(pxInit->irq_enable));
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, buffer))
    return HTS221_ERROR;
  
  buffer[0] &= ~(HTS221_AVGH_MASK | HTS221_AVGT_MASK);
  buffer[0] |= (uint8_t)pxInit->avg_h;
  buffer[0] |= (uint8_t)pxInit->avg_t;
  
  if(HTS221_WriteReg(HTS221_AV_CONF_REG, 1, buffer))
    return HTS221_ERROR;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 3, buffer))
    return HTS221_ERROR;
  
  buffer[0] &= ~(HTS221_BDU_MASK | HTS221_ODR_MASK);
  buffer[0] |= (uint8_t)pxInit->odr;
  buffer[0] |= ((uint8_t)pxInit->bdu_status)<<HTS221_BDU_BIT;
  
  buffer[1] &= ~HTS221_HEATHER_BIT;
  buffer[1] |= ((uint8_t)pxInit->heater_status)<<HTS221_HEATHER_BIT;
  
  buffer[2] &= ~(HTS221_DRDY_H_L_MASK | HTS221_PP_OD_MASK | HTS221_DRDY_MASK);
  buffer[2] |= ((uint8_t)pxInit->irq_level)<<HTS221_DRDY_H_L_BIT;
  buffer[2] |= (uint8_t)pxInit->irq_output_type;
  buffer[2] |= ((uint8_t)pxInit->irq_enable)<<HTS221_DRDY_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 3, buffer))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
 * @brief  Returns a HTS221_Init_st struct with the actual configuration.
 * @param  pxInit pointer to a HTS221_Init_st structure.
 *         This parameter is a pointer to @ref HTS221_Init_st.
 * @retval Error code [HTS221_OK, HTS221_ERROR].
 */
HTS221_Error_et HTS221_Get_InitConfig(HTS221_Init_st* pxInit)
{
  uint8_t buffer[3];
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, buffer))
    return HTS221_ERROR;
  
  pxInit->avg_h = (HTS221_Avgh_et)(buffer[0] & HTS221_AVGH_MASK);
  pxInit->avg_t = (HTS221_Avgt_et)(buffer[0] & HTS221_AVGT_MASK);
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 3, buffer))
    return HTS221_ERROR;
  
  pxInit->odr = (HTS221_Odr_et)(buffer[0] & HTS221_ODR_MASK);
  pxInit->bdu_status = (HTS221_State_et)((buffer[0] & HTS221_BDU_MASK) >> HTS221_BDU_BIT);
  pxInit->heater_status = (HTS221_State_et)((buffer[1] & HTS221_HEATHER_MASK) >> HTS221_HEATHER_BIT);
  
  pxInit->irq_level = (HTS221_DrdyLevel_et)(buffer[2] & HTS221_DRDY_H_L_MASK);
  pxInit->irq_output_type = (HTS221_OutputType_et)(buffer[2] & HTS221_PP_OD_MASK);
  pxInit->irq_enable = (HTS221_State_et)((buffer[2] & HTS221_DRDY_MASK) >> HTS221_DRDY_BIT);
  
  return HTS221_OK;
}

/**
* @brief  De initialization function for HTS221.
*         This function put the HTS221 in power down, make a memory boot and clear the data output flags.
* @param  None.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_DeInit(void)
{    
  uint8_t buffer[4];
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 2, buffer))
    return HTS221_ERROR;
  
  /* HTS221 in power down */
  buffer[0] |= 0x01<<HTS221_PD_BIT;
  
  /* Make HTS221 boot */
  buffer[1] |= 0x01<<HTS221_BOOT_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 2, buffer))
    return HTS221_ERROR;
  
  /* Dump of data output */
  if(HTS221_ReadReg(HTS221_HR_OUT_L_REG, 4, buffer))
    return HTS221_ERROR;
    
  return HTS221_OK;
}

/**
* @brief  Read HTS221 output registers, and calculate humidity and temperature.
* @param  humidity pointer to the returned humidity value that must be divided by 10 to get the value in [%].
* @param  temperature pointer to the returned temperature value that must be divided by 10 to get the value in ['C].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Measurement(uint16_t* humidity, int16_t* temperature)
{
  int16_t H0_T0_out, H1_T0_out, H_T_out;
  int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
  int16_t H0_rh, H1_rh;   
  int16_t T0_degC, T1_degC; 
  uint8_t buffer[8];  
  int32_t tmp;
  
  if(HTS221_ReadReg(HTS221_HR_OUT_L_REG, 4, buffer))
    return HTS221_ERROR;
  
  H_T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];    
  T_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];
  
  if(HTS221_ReadReg(HTS221_H0_RH_X2, 4, buffer))
    return HTS221_ERROR;
  
  H0_rh = buffer[0]>>1;
  H1_rh = buffer[1]>>1;
  
  T0_degC_x8_u16 = (uint16_t)buffer[2];  
  T1_degC_x8_u16 = (uint16_t)buffer[3];
  
  if(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2, 3, buffer))
    return HTS221_ERROR;
  
  T0_degC_x8_u16 |= (((uint16_t)(buffer[0] & 0x03)) << 8);  
  T1_degC_x8_u16 |= (((uint16_t)(buffer[0] & 0x0C)) << 6);
  T0_degC = T0_degC_x8_u16>>3;
  T1_degC = T1_degC_x8_u16>>3;  
  
  H0_T0_out = (((uint16_t)buffer[2])<<8) | (uint16_t)buffer[1];
  
  if(HTS221_ReadReg(HTS221_H1_T0_OUT_L, 6, buffer))
    return HTS221_ERROR;
  
  H1_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
  T0_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];  
  T1_out = (((uint16_t)buffer[5])<<8) | (uint16_t)buffer[4];

  tmp = ((uint32_t)(H_T_out - H0_T0_out)) * ((uint32_t)(H1_rh - H0_rh)*10);
  *humidity =    tmp/(H1_T0_out - H0_T0_out)  + H0_rh*10;
  
  tmp = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC)*10);
  *temperature = tmp /(T1_out - T0_out) + T0_degC*10;
  
  if(*humidity>1000) *humidity = 1000; 
  
  return HTS221_OK;
}

/**
* @brief  Read HTS221 output registers. Humidity and temperature.
* @param  humidity pointer to the returned humidity raw value.
* @param  temperature pointer to the returned temperature raw value.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_RawMeasurement(int16_t* humidity, int16_t* temperature)
{
  uint8_t buffer[4];
  
  if(HTS221_ReadReg(HTS221_HR_OUT_L_REG, 4, buffer))
    return HTS221_ERROR;
  
  *humidity = (int16_t)((((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0]);
  *temperature = (int16_t)((((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2]); 
  
  return HTS221_OK; 
}

/**
* @brief  Read HTS221 Humidity output registers, and calculate humidity.
* @param  Pointer to the returned humidity value that must be divided by 10 to get the value in [%].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Humidity(uint16_t* value)
{
  int16_t H0_T0_out, H1_T0_out, H_T_out;
  int16_t H0_rh, H1_rh;    
  uint8_t buffer[2];  
uint32_t tmp;
  
  if(HTS221_ReadReg(HTS221_H0_RH_X2, 2, buffer))
    return HTS221_ERROR;
  H0_rh = buffer[0]>>1;
  H1_rh = buffer[1]>>1;
  
  if(HTS221_ReadReg(HTS221_H0_T0_OUT_L, 2, buffer))
    return HTS221_ERROR;
  H0_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
  
  if(HTS221_ReadReg(HTS221_H1_T0_OUT_L, 2, buffer))
    return HTS221_ERROR;
  H1_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
  
  if(HTS221_ReadReg(HTS221_HR_OUT_L_REG, 2, buffer))
    return HTS221_ERROR;
  H_T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];      
  
  tmp = ((uint32_t)(H_T_out - H0_T0_out)) * ((uint32_t)(H1_rh - H0_rh)*10);
  *value =    tmp/(H1_T0_out - H0_T0_out)  + H0_rh*10;
  
  if(*value>1000) *value = 1000;

  return HTS221_OK;
}

/**
* @brief  Read HTS221 humidity output registers.
* @param  Pointer to the returned humidity raw value.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_HumidityRaw(int16_t* value)
{
  uint8_t buffer[2];
  
  if(HTS221_ReadReg(HTS221_HR_OUT_L_REG, 2, buffer))
    return HTS221_ERROR;
  
  *value = (int16_t)((((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0]);
  
  return HTS221_OK;
}

/**
* @brief  Read HTS221 temperature output registers, and calculate temperature.
* @param  Pointer to the returned temperature value that must be divided by 10 to get the value in ['C].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Temperature(int16_t *value)
{
  int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
  int16_t T0_degC, T1_degC;
  uint8_t buffer[4], tmp;
uint32_t tmp32;
  
  if(HTS221_ReadReg(HTS221_T0_DEGC_X8, 2, buffer))
    return HTS221_ERROR;
  if(HTS221_ReadReg(HTS221_T0_T1_DEGC_H2, 1, &tmp))
    return HTS221_ERROR;
  
  T0_degC_x8_u16 = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);  
  T1_degC_x8_u16 = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
  T0_degC = T0_degC_x8_u16>>3;
  T1_degC = T1_degC_x8_u16>>3;
  
  if(HTS221_ReadReg(HTS221_T0_OUT_L, 4, buffer))
    return HTS221_ERROR;
  
  T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];  
  T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];
  
  if(HTS221_ReadReg(HTS221_TEMP_OUT_L_REG, 2, buffer))
    return HTS221_ERROR;
  
  T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];  
  
  tmp32 = ((uint32_t)(T_out - T0_out)) * ((uint32_t)(T1_degC - T0_degC)*10);
  *value = tmp32 /(T1_out - T0_out)  + T0_degC*10;
  
  return HTS221_OK;
}

/**
* @brief  Read HTS221 temperature output registers.
* @param  Pointer to the returned temperature raw value.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_TemperatureRaw(int16_t* value)
{
  uint8_t buffer[2];
  
  if(HTS221_ReadReg(HTS221_TEMP_OUT_L_REG, 2, buffer))
    return HTS221_ERROR;
  
  *value = (int16_t)((((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0]);
  
  return HTS221_OK;  
}

/**
* @brief  Get the availability of new data for humidity and temperature.
* @param  humidity pointer to the returned humidity data status [HTS221_SET/HTS221_RESET].
* @param  temperature pointer to the returned temperature data status [HTS221_SET/HTS221_RESET].
*         This parameter is a pointer to @ref HTS221_BitStatus_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_DataStatus(HTS221_BitStatus_et* humidity, HTS221_BitStatus_et* temperature)
{
  uint8_t tmp;

  if(HTS221_ReadReg(HTS221_STATUS_REG, 1, &tmp))
    return HTS221_ERROR;
  
  *humidity = (HTS221_BitStatus_et)((tmp & HTS221_HDA_MASK)>>HTS221_H_DA_BIT);  
  *temperature = (HTS221_BitStatus_et)(tmp & HTS221_TDA_MASK);
  
  return HTS221_OK;  
}

/**
* @brief  Exit from power down mode.
* @param  void.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Activate(void)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_PD_MASK;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK; 
}

/**
* @brief  Put the sensor in power down mode.
* @param  void.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_DeActivate(void)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp |= HTS221_PD_MASK;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK; 
}



/**
  * @brief  Check if the single measurement has completed.
* @param  tmp is set to 1, when the measure is completed
  * @retval Status [LPS25H_ERROR, LPS25H_OK]
*/  
HTS221_Error_et HTS221_IsMeasurementCompleted(HTS221_BitStatus_et* Is_Measurement_Completed)
{  
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_STATUS_REG, 1, &tmp))
			return HTS221_ERROR;
	
  if((tmp & (uint8_t)(HTS221_HDA_MASK | HTS221_TDA_MASK)) == (uint8_t)(HTS221_HDA_MASK | HTS221_TDA_MASK))
    *Is_Measurement_Completed = HTS221_SET;
  else
    *Is_Measurement_Completed = HTS221_RESET;
  
		return HTS221_OK;
 }
 
 
/**
* @brief  Set_ humidity and temperature average mode.
* @param  avgh is the average mode for humidity, this parameter is @ref HTS221_Avgh_et.
* @param  avgt is the average mode for temperature, this parameter is @ref HTS221_Avgt_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_AvgHT(HTS221_Avgh_et avgh, HTS221_Avgt_et avgt)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_AVGH(avgh));
  HTS221_assert_param(IS_HTS221_AVGT(avgt));
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~(HTS221_AVGH_MASK | HTS221_AVGT_MASK);
  tmp |= (uint8_t)avgh;
  tmp |= (uint8_t)avgt;
  
  if(HTS221_WriteReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Set humidity average mode.
* @param  avgh is the average mode for humidity, this parameter is @ref HTS221_Avgh_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_AvgH(HTS221_Avgh_et avgh)
{
  uint8_t tmp;  
  
  HTS221_assert_param(IS_HTS221_AVGH(avgh));
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_AVGH_MASK;
  tmp |= (uint8_t)avgh;
  
  if(HTS221_WriteReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Set temperature average mode.
* @param  avgt is the average mode for temperature, this parameter is @ref HTS221_Avgt_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_AvgT(HTS221_Avgt_et avgt)
{
  uint8_t tmp;  
  
  HTS221_assert_param(IS_HTS221_AVGT(avgt));
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_AVGT_MASK;
  tmp |= (uint8_t)avgt;
  
  if(HTS221_WriteReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Get humidity and temperature average mode.
* @param  avgh pointer to the returned value with the humidity average mode.
* @param  avgt pointer to the returned value with the temperature average mode.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_AvgHT(HTS221_Avgh_et* avgh, HTS221_Avgt_et* avgt)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_AV_CONF_REG, 1, &tmp))
    return HTS221_ERROR;
  
  *avgh = (HTS221_Avgh_et)(tmp & HTS221_AVGH_MASK);
  *avgt = (HTS221_Avgt_et)(tmp & HTS221_AVGT_MASK);
  
  return HTS221_OK;  
}

/**
* @brief  Set block data update mode.
* @param  status can be HTS221_ENABLE: enable the block data update, output data registers are updated once both MSB and LSB are read.
* @param  status can be HTS221_DISABLE: output data registers are continuously updated.
*         This parameter is a @ref HTS221_BitStatus_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_BduMode(HTS221_State_et status)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_State(status));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_BDU_MASK;
  tmp |= ((uint8_t)status)<<HTS221_BDU_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Get block data update mode.
* @param  Pointer to the returned value with block data update mode status.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_BduMode(HTS221_State_et* status)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  *status = (HTS221_State_et)((tmp & HTS221_BDU_MASK) >> HTS221_BDU_BIT);
  
  return HTS221_OK;  
}

/**
* @brief  Enter or exit from power down mode.
* @param  status can be HTS221_SET: HTS221 in power down mode.
* @param  status can be HTS221_REET: HTS221 in active mode.
*         This parameter is a @ref HTS221_BitStatus_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_PowerDownMode(HTS221_BitStatus_et status)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_BitStatus(status));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_PD_MASK;
  tmp |= ((uint8_t)status)<<HTS221_PD_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Get if HTS221 is in active mode or in power down mode.
* @param  Pointer to the returned value with HTS221 status.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_PowerDownMode(HTS221_BitStatus_et* status)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  *status = (HTS221_BitStatus_et)((tmp & HTS221_PD_MASK) >> HTS221_PD_BIT);
  
  return HTS221_OK;
}

/**
* @brief  Set the output data rate mode.
* @param  odr is the output data rate mode.
*         This parameter is a @ref HTS221_Odr_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_Odr(HTS221_Odr_et odr)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_ODR(odr));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_ODR_MASK;
  tmp |= (uint8_t)odr;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Get the output data rate mode.
* @param  Pointer to the returned value with output data rate mode.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_Odr(HTS221_Odr_et* odr)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG1, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= HTS221_ODR_MASK;
  *odr = (HTS221_Odr_et)tmp;
  
  return HTS221_OK;
}

/**
* @brief  Reboot Memory Content.
* @param  None.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_MemoryBoot(void)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  tmp |= HTS221_BOOT_MASK;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Configure the internal heater.
* @param  The status of the internal heater [HTS221_ENABLE/HTS221_DISABLE].
*         This parameter is a @ref HTS221_State_et.
* @retval Error code [HTS221_OK, HTS221_ERROR]
*/
HTS221_Error_et HTS221_Set_HeaterState(HTS221_State_et status)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_State(status));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_HEATHER_MASK;
  tmp |= ((uint8_t)status)<<HTS221_HEATHER_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
}

/**
* @brief  Get the internal heater.
* @param  Pointer to the returned status of the internal heater [HTS221_ENABLE/HTS221_DISABLE].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_HeaterState(HTS221_State_et* status)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  *status = (HTS221_State_et)((tmp & HTS221_HEATHER_MASK) >> HTS221_HEATHER_BIT);
  
  return HTS221_OK;
}

/**
* @brief  Set ONE_SHOT bit to start a new conversion (ODR mode has to be 00).
*         Once the measurement is done, ONE_SHOT bit is self-cleared.
* @param  None.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_StartOneShotMeasurement(void)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  tmp |= HTS221_ONE_SHOT_MASK;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG2, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;
  
}

/**
* @brief  Set level configuration of the interrupt pin DRDY.
* @param  status can be HTS221_LOW_LVL: active level is LOW.
* @param  status can be HTS221_HIGH_LVL: active level is HIGH.
*         This parameter is a @ref HTS221_State_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_IrqActiveLevel(HTS221_DrdyLevel_et value)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_DrdyLevelType(value));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_DRDY_H_L_MASK;
  tmp |= (uint8_t)value;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;  
}

/**
* @brief  Get level configuration of the interrupt pin DRDY.
* @param  Pointer to the returned status of the level configuration [HTS221_ENABLE/HTS221_DISABLE].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_IrqActiveLevel(HTS221_DrdyLevel_et* value)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  *value = (HTS221_DrdyLevel_et)(tmp & HTS221_DRDY_H_L_MASK);
  
  return HTS221_OK;
}

/**
* @brief  Set Push-pull/open drain configuration for the interrupt pin DRDY.
* @param  value is the output type configuration.
*         This parameter is a @ref HTS221_OutputType_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_IrqOutputType(HTS221_OutputType_et value)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_OutputType(value));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_PP_OD_MASK;
  tmp |= (uint8_t)value;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;  
}

/**
* @brief  Get the configuration for the interrupt pin DRDY.
* @param  Pointer to the returned value with output type configuration.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_IrqOutputType(HTS221_OutputType_et* value)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  *value = (HTS221_OutputType_et)(tmp & HTS221_PP_OD_MASK);
  
  return HTS221_OK;
}

/**
* @brief  Enable/disable the interrupt mode.
* @param  status is the enable/disable for the interrupt mode.
*         This parameter is a @ref HTS221_State_et.
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Set_IrqEnable(HTS221_State_et status)
{
  uint8_t tmp;
  
  HTS221_assert_param(IS_HTS221_State(status));
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  tmp &= ~HTS221_DRDY_MASK;
  tmp |= ((uint8_t)status)<<HTS221_DRDY_BIT;
  
  if(HTS221_WriteReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  return HTS221_OK;  
}

/**
* @brief  Get the interrupt mode.
* @param  Pointer to the returned status of the interrupt mode configuration [HTS221_ENABLE/HTS221_DISABLE].
* @retval Error code [HTS221_OK, HTS221_ERROR].
*/
HTS221_Error_et HTS221_Get_IrqEnable(HTS221_State_et* status)
{
  uint8_t tmp;
  
  if(HTS221_ReadReg(HTS221_CTRL_REG3, 1, &tmp))
    return HTS221_ERROR;
  
  *status = (HTS221_State_et)((tmp & HTS221_DRDY_MASK) >> HTS221_DRDY_BIT);
  
  return HTS221_OK;
}


#ifdef  USE_FULL_ASSERT_HTS221
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void HTS221_assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, (int)line);
  
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

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/

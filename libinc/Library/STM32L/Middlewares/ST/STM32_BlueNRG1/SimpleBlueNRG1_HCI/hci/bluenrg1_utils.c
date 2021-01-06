
#include "ble_status.h"
#include "bluenrg1_types.h"
#include "bluenrg1_hal_aci.h"
#include "bluenrg_utils.h"
#include "SDK_EVAL_Config.h"
#include "osal.h"


#define BASE_ADDRESS    (0x10040000)

#ifdef BLUENRG2_DEVICE
#define FLASH_END    0x1007FFFF // BlueNRG-2
#warning "BlueNRG-2 device selected"

#elif BLUENRG1_DEVICE
#define FLASH_END    0x10067FFF // BlueNRG-1
#warning "BlueNRG-1 device selected"

#else
#error "Please, add the preprocessor symbols for the device"
#endif

#define FW_OFFSET       (8*1024)  // 8 KB
#define SECTOR_SIZE     (2*1024)  // 2 KB

// x**32 + x**26 + x**23 + x ** 22 + x**16 + x**12 + x**11 +
// x**10 + x**8 + x**7 + x**5 + x**4 + x**2 + x**1 + x**0
#define CRC_POLY        0x04C11DB7      // the poly without the x**32


#define DEV_CONFIG_SIZE 14
#define DEV_CONFIG_BASE_ADDRESS (FLASH_END - SECTOR_SIZE + 1)

#define DEV_CONFIG_WRITE_OFFSET_BEGIN 0x100


#define BLUE_FLAG_OFFSET        0x8C0
#define MIN_WRITE_BLOCK_SIZE    4
#define MAX_WRITE_BLOCK_SIZE    64        // 64 bytes
#define READ_BLOCK_SIZE         64        // 64 bytes


#define MIN(a,b) (((a)<(b))?(a):(b))

/* This function calculates the CRC of a sector of flash, if bytes passed are less than sector size, 
   they are extended with 0xFF until sector size is reached
*/
static uint32_t updater_calc_crc(const uint8_t* data, uint16_t nr_of_bytes)
{
  uint32_t i, j, a1;
  uint32_t crc, value;
  
  crc = 0;
  for (i = 0; i < SECTOR_SIZE; i += 4) {
    uint8_t *dataw = (uint8_t *) &value;
    
    dataw[0] = (i < nr_of_bytes) ? data[i] : 0xFF;
    dataw[1] = ((i + 1) < nr_of_bytes) ? data[i+1] : 0xFF;
    dataw[2] = ((i + 2) < nr_of_bytes) ? data[i+2] : 0xFF;
    dataw[3] = ((i + 3) < nr_of_bytes) ? data[i+3] : 0xFF;
    
    crc = crc ^ value;
    for (j = 0; j < 32; j ++) {
      a1 = (crc >> 31) & 0x1;
      crc = (crc << 1) ^ (a1 * CRC_POLY);
    }
  }
  
  return crc;
}

int program_device(const uint8_t *fw_image, uint32_t fw_size)
{
//  uint8_t version;
  uint8_t status, write_block_size;
  uint32_t crc, crc2, crc_size;
  
  /* Put the device in DTM_Updater */
  BlueNRG_HW_Updater();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED
  
//  if(aci_hal_get_updater_version(&version))
//    return BLE_UTIL_ACI_ERROR;
  
  status = aci_hal_updater_erase_blue_flag();
  if (status != BLE_STATUS_SUCCESS)
    return status;
  
//  for(int i = FW_OFFSET; i < (FW_OFFSET + fw_size); i += SECTOR_SIZE) {
//    if (aci_hal_updater_erase_sector(BASE_ADDRESS + i) != BLE_STATUS_SUCCESS)
//      return status;
//    
//    for (int j=i; ((j<i+SECTOR_SIZE)&&(j<fw_size)); j += write_block_size) {
//      write_block_size = MIN(fw_size+FW_OFFSET-j, MAX_WRITE_BLOCK_SIZE);  
//      
//      status = aci_hal_updater_prog_data_blk(BASE_ADDRESS+j, write_block_size, (uint8_t*)fw_image+j-FW_OFFSET);
//      if (status != BLE_STATUS_SUCCESS)
//        return status;
//    }
//  }
  for(int i = 0; i < fw_size; i += SECTOR_SIZE) {
    if (aci_hal_updater_erase_sector(BASE_ADDRESS + FW_OFFSET + i) != BLE_STATUS_SUCCESS) {
      return status;
    }
    
    for (int j=i; ((j<i+SECTOR_SIZE)&&(j<fw_size)); j += write_block_size) {
      write_block_size = MIN(fw_size-j, MAX_WRITE_BLOCK_SIZE);  
      
      status = aci_hal_updater_prog_data_blk(BASE_ADDRESS + FW_OFFSET + j, write_block_size, (uint8_t*)fw_image+j);
      if (status != BLE_STATUS_SUCCESS)
        return status;
    }
  }
  
//  for(int i = FW_OFFSET; i < (FW_OFFSET + fw_size); i += SECTOR_SIZE){
//    address = BASE_ADDRESS + i;
//    if(aci_hal_updater_calc_crc(address, 1, &crc))
//      return BLE_UTIL_ACI_ERROR;
//    
//    crc_size = MIN(fw_size-i,SECTOR_SIZE);
//    
//    crc2 = updater_calc_crc(fw_image+i-FW_OFFSET,crc_size);
//    if(crc != crc2)
//      return BLE_UTIL_CRC_ERROR;
//  }
  for(int i = 0; i < fw_size; i += SECTOR_SIZE){
    if(aci_hal_updater_calc_crc((BASE_ADDRESS + FW_OFFSET + i), 1, &crc))
      return BLE_UTIL_ACI_ERROR;
    
    crc_size = MIN(fw_size-i,SECTOR_SIZE);
    
    crc2 = updater_calc_crc(fw_image+i,crc_size);
    if(crc != crc2)
      return BLE_UTIL_CRC_ERROR;
  }
  /***********************************************************************
  * Write BLUE flag
  ************************************************************************/
  status = aci_hal_updater_reset_blue_flag();
  if (status != BLE_STATUS_SUCCESS)
    return status;
  
  BlueNRG_RST();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED
  
  return BLE_STATUS_SUCCESS;
}


int program_DEV_CONFIG(const devConfig_t *dev_config_image)
{
  tBleStatus ret;
  uint8_t dev_config_data[SECTOR_SIZE];
  uint8_t dev_config_updated[READ_BLOCK_SIZE];
  
  /* Put the device in DTM_Updater */
  BlueNRG_HW_Updater();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED
  
  for(int i = 0; i < SECTOR_SIZE; i += READ_BLOCK_SIZE){
    ret = aci_hal_updater_read_data_blk(DEV_CONFIG_BASE_ADDRESS+i, READ_BLOCK_SIZE, dev_config_data+i);
    if(ret != BLE_STATUS_SUCCESS){
      return ret;
    }
  }
  
  Osal_MemCpy((void*)&dev_config_data[0x100], (void*)dev_config_image, sizeof(devConfig_t));
  
  ret = aci_hal_updater_erase_sector(DEV_CONFIG_BASE_ADDRESS);
  if(ret != BLE_STATUS_SUCCESS){
    return ret;
  }
  
  for(int i = 0; i < SECTOR_SIZE; i += MAX_WRITE_BLOCK_SIZE) {
    ret = aci_hal_updater_prog_data_blk(DEV_CONFIG_BASE_ADDRESS + i, MAX_WRITE_BLOCK_SIZE, dev_config_data+i);
    if (ret != BLE_STATUS_SUCCESS)
      return BLE_UTIL_ACI_ERROR;
  }
  
  for(int i = 0, j = 0; i < SECTOR_SIZE; i += READ_BLOCK_SIZE, j += READ_BLOCK_SIZE) {
    ret = aci_hal_updater_read_data_blk(DEV_CONFIG_BASE_ADDRESS + i, READ_BLOCK_SIZE, dev_config_updated);
    if(ret != BLE_STATUS_SUCCESS){
      return ret;
    }
    if (Osal_MemCmp(dev_config_updated, dev_config_data+j, READ_BLOCK_SIZE) != 0)
      return BLE_UTIL_WRONG_VERIFY;
  }
  
  BlueNRG_RST();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED
  
  return BLE_STATUS_SUCCESS;
}


uint8_t verify_DEV_CONFIG(const devConfig_t *dev_config_data)
{
//  uint8_t version;
  uint8_t ret = BLE_STATUS_SUCCESS;
  uint8_t dev_config_updated[READ_BLOCK_SIZE];
  
  /* Put the device in DTM_Updater */
  BlueNRG_HW_Updater();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED
  
  //  if(aci_hal_get_updater_version(&version))
  //    return BLE_UTIL_ACI_ERROR;
  
  ret = aci_hal_updater_read_data_blk((DEV_CONFIG_BASE_ADDRESS+DEV_CONFIG_WRITE_OFFSET_BEGIN), sizeof(devConfig_t), dev_config_updated);
  if(ret != BLE_STATUS_SUCCESS) {
    return ret;
  }
  if (Osal_MemCmp(dev_config_updated, ((uint8_t*)dev_config_data), sizeof(devConfig_t)) != 0) {
    ret = BLE_UTIL_WRONG_VERIFY;
    return ret;
  }
  
  BlueNRG_RST();
  BTLE_StackTick(); // To receive the EVT_INITIALIZED

  return ret;
}


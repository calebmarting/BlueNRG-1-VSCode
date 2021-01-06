/**
  ******************************************************************************
  * @file    system_bluenrg1.h
  * @author  AMS RF-Application Team
  * @version V1.1.0
  * @date    6-April-2018
  * @brief   This file contains all the functions prototypes for the CRMU firmware 
  *          library.
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
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_BLUENRG1_H
#define SYSTEM_BLUENRG1_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup system_bluenrg1 system bluenrg1
  * @{
  */  

#include "compiler.h"
#include "hal_types.h"
#include <core_cm0.h>
#include "system_bluenrg.h"


/** @addtogroup system_bluenrg1_Exported_Constants Exported Constants
  * @{
  */


/**
 * @brief RAM base address
 */   
#define _MEMORY_RAM_BEGIN_    0x20000000
#define _MEMORY_RAM_SIZE_     0x6000           /* 24KB  */
#define _MEMORY_RAM_END_      0x20005FFF
   
/**
 * @brief User FLASH base address
 */
#define _MEMORY_FLASH_BEGIN_  0x10040000
#define _MEMORY_FLASH_SIZE_   0x28000          /* 160KB */
#define _MEMORY_FLASH_END_    0x10067FFF
#define _MEMORY_BYTES_PER_PAGE_  (2048)

/**
 * @brief ROM base address
 */   
#define _MEMORY_ROM_BEGIN_    0x10000000
#define _MEMORY_ROM_SIZE_     0x800             /* 2KB */
#define _MEMORY_ROM_END_      0x100007FF


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_BLUENRG1_H */

/**
  * @}
  */
  
/**
  * @}
  */

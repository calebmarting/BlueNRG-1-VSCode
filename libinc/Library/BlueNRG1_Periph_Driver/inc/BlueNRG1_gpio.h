/**
  ******************************************************************************
  * @file    BlueNRG1_gpio.h
  * @author  VMA Application Team
  * @version V2.2.0
  * @date    31-January-2017
  * @brief   This file contains all the functions prototypes for the GPIO 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_GPIO_H
#define BLUENRG1_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup GPIO_Peripheral  GPIO Peripheral
  * @{
  */


/** @defgroup GPIO_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief  Structure definition of GPIO initialization
  */
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_Pins_Definition */
  
  uint8_t GPIO_Mode;             /*!< Specifies the operating mode for the selected pin.
                                      This parameter can be a value of @ref GPIO_Mode_Definition */

  FunctionalState GPIO_HighPwr;   /*!< Specifies the GPIO strength for the selected pin.
                                      This parameter can be any value of @ref FunctionalState */
  
  FunctionalState GPIO_Pull;          /*!< Specifies the GPIO pull down state for the selected pin.
                                      This parameter can be a value of @ref FunctionalState */
} GPIO_InitType;



/**
  * @brief  Structure definition of GPIO EXTernal Interrupt initialization
  */
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_Pins_Definition */
  
  uint32_t GPIO_IrqSense;         /*!< Specifies the GPIO interrupt sense to be configured.
                                      This parameter can be any value of @ref GPIO_IRQ_Sense_Definition */
  
  uint32_t GPIO_Event;            /*!< Specifies the GPIO level sense to be configured.
                                      This parameter can be any value of @ref GPIO_Event_Definition */
  
} GPIO_EXTIConfigType;


/** 
  * @brief  Bit_SET and Bit_RESET enumeration  
  */
typedef enum
{
  Bit_RESET = 0,  /*!< Specifies the GPIO reset state */
  Bit_SET         /*!< Specifies the GPIO set state */
} BitAction;

/** This macro checks if ACTION is a valid bit action */
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
  * @}
  */


/** @defgroup GPIO_Exported_Constants Exported Constants
  * @{
  */


	
/** @defgroup GPIO_Mode_Definition Mode Definition
  * @{
  */  

#define GPIO_Input                  ((uint8_t)0)  /*!< GPIO input selected */
#define Serial1_Mode                ((uint8_t)1)  /*!< Serial1 mode selected */
#define Serial0_Mode                ((uint8_t)4)  /*!< Serial0 mode selected */
#define Serial2_Mode                ((uint8_t)5)  /*!< Serial2 mode selected */
#define GPIO_Output                 ((uint8_t)6)  /*!< GPIO output mode selected */

/* This macro checks if MODE is a valid mode value for GPIO, please refer to the DATASHEET GPIO mode table */
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Input) || \
                            ((MODE) == GPIO_Output) || \
                            ((MODE) == Serial1_Mode) || \
                            ((MODE) == Serial0_Mode) || \
                            ((MODE) == Serial2_Mode))

/**
  * @}
  */



/** @defgroup GPIO_Pins_Definition GPIO Pins Definition
  * @{
  */
#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x00000002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint32_t)0x00000100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint32_t)0x00000200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint32_t)0x00000400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint32_t)0x00000800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint32_t)0x00001000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint32_t)0x00002000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint32_t)0x00004000)  /*!< Pin 14 selected */

#define GPIO_Pin_15                ((uint32_t)0x00008000)  /*!< Pin 15 selected */
#define GPIO_Pin_16                ((uint32_t)0x00010000)  /*!< Pin 16 selected */
#define GPIO_Pin_17                ((uint32_t)0x00020000)  /*!< Pin 17 selected */
#define GPIO_Pin_18                ((uint32_t)0x00040000)  /*!< Pin 18 selected */
#define GPIO_Pin_19                ((uint32_t)0x00080000)  /*!< Pin 19 selected */
#define GPIO_Pin_20                ((uint32_t)0x00100000)  /*!< Pin 20 selected */
#define GPIO_Pin_21                ((uint32_t)0x00200000)  /*!< Pin 21 selected */
#define GPIO_Pin_22                ((uint32_t)0x00400000)  /*!< Pin 22 selected */
#define GPIO_Pin_23                ((uint32_t)0x00800000)  /*!< Pin 23 selected */
#define GPIO_Pin_24                ((uint32_t)0x01000000)  /*!< Pin 24 selected */
#define GPIO_Pin_25                ((uint32_t)0x02000000)  /*!< Pin 25 selected */
#define GPIO_Pin_All               ((uint32_t)0x03FFFFFF)  /*!< All pins selected */

/* Number of total available GPIO pins */
#define GPIO_PIN_COUNT               (26)

#define IS_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || ((PIN) == GPIO_Pin_1) || \
                            ((PIN) == GPIO_Pin_2) || ((PIN) == GPIO_Pin_3) || \
                            ((PIN) == GPIO_Pin_4) || ((PIN) == GPIO_Pin_5) || \
                            ((PIN) == GPIO_Pin_6) || ((PIN) == GPIO_Pin_7) || \
                            ((PIN) == GPIO_Pin_8) || ((PIN) == GPIO_Pin_9) || \
                            ((PIN) == GPIO_Pin_10) || ((PIN) == GPIO_Pin_11) || \
                            ((PIN) == GPIO_Pin_12) || ((PIN) == GPIO_Pin_13) || \
                            ((PIN) == GPIO_Pin_14) || ((PIN) == GPIO_Pin_15) || \
                            ((PIN) == GPIO_Pin_16) || ((PIN) == GPIO_Pin_17) || \
                            ((PIN) == GPIO_Pin_18) || ((PIN) == GPIO_Pin_19) || \
                            ((PIN) == GPIO_Pin_20) || ((PIN) == GPIO_Pin_21) || \
                            ((PIN) == GPIO_Pin_22) || ((PIN) == GPIO_Pin_23) || \
                            ((PIN) == GPIO_Pin_24) || ((PIN) == GPIO_Pin_25))


/* This macro checks if PIN is a valid pin combination value */
#define IS_GPIO_PINS(PIN) (((PIN) & GPIO_Pin_All) != 0)


/**
  * @}
  */


/** @defgroup GPIO_IRQ_Sense_Definition IRQ Sense Definition
  * @{
  */

#define GPIO_IrqSense_Edge      (0)   /*!< Edge detection */
#define GPIO_IrqSense_Level     (1)   /*!< Level detection */

/** This macro checks if IS is a valid interrupt sense value */
#define IS_GPIO_IRQSENSE(IS) (((IS) == GPIO_IrqSense_Edge) || ((IS) == GPIO_IrqSense_Level))

/**
  * @}
  */


/** @defgroup GPIO_Event_Definition Event	Definition
  * @{
  */
#define GPIO_Event_Low           (0)   /*!< Low level or falling edge detection */
#define GPIO_Event_High          (1)   /*!< High level or rising edge detection */
#define GPIO_Event_Both          (2)   /*!< Both edges detection */

/** This macro checks if EV is a valid GPIO_Event value */
#define IS_GPIO_EVENT(EV) (((EV) == GPIO_Event_Low) || \
                           ((EV) == GPIO_Event_High) || \
                           ((EV) == GPIO_Event_Both))

/**
  * @}
  */  

/**
  * @}
  */

/** @defgroup GPIO_Exported_Macros Exported Macros
* @{
*/

/* 
 * GPIO Configuration
 */

/* GPIO Input Signal */
#define GPIO_InitInputPinx(PIN_NAME)    GPIO_Init(&(GPIO_InitType){(PIN_NAME), GPIO_Input, DISABLE, DISABLE});

/* GPIO Output Signal */
#define GPIO_InitOutputPinx(PIN_NAME)   GPIO_Init(&(GPIO_InitType){(PIN_NAME), GPIO_Output, DISABLE, DISABLE});


/* 
 * UART Port Configuration
 */

/* UART RX Signal */
#define GPIO_InitUartRxPin4()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_4, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitUartRxPin11()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_11, Serial1_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitUartRxPin24()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_24, Serial1_Mode, DISABLE, DISABLE});
#endif

/* UART TX Signal */
#define GPIO_InitUartTxPin5()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_5, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitUartTxPin8()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_8, Serial1_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitUartTxPin23()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_23, Serial1_Mode, DISABLE, DISABLE});
#endif

/* UART CTS Signal */
#define GPIO_InitUartCtsPin0()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_0, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitUartCtsPin7()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_7, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitUartCtsPin13()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_13, Serial1_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitUartCtsPin19()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_19, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitUartCtsPin20()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_20, Serial1_Mode, DISABLE, DISABLE});
#endif

/* UART RTS Signal */
#define GPIO_InitUartRtsPin1()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_1, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitUartRtsPin6()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_6, Serial1_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitUartRtsPin18()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_18, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitUartRtsPin25()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_25, Serial1_Mode, DISABLE, DISABLE});
#endif


/* 
 * SPI Port Configuration
 */

/* SPI Clock Signal */
#define GPIO_InitSpiClkPin0()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_0, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiClkPin8()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_8, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiClkPin14()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_14, Serial0_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitSpiClkPin25()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_25, Serial0_Mode, DISABLE, DISABLE});
#endif

/* SPI CS Signal */
#define GPIO_InitSpiCs1Pin1()     GPIO_Init(&(GPIO_InitType){GPIO_Pin_1, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs1Pin11()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_11, Serial0_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitSpiCs1Pin15()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_15, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs2Pin18()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_18, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs2Pin20()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_20, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs3Pin19()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_19, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs1Pin21()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_21, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiCs3Pin22()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_22, Serial1_Mode, DISABLE, DISABLE});
#endif


/* SPI OUT Signal */
#define GPIO_InitSpiOutPin2()     GPIO_Init(&(GPIO_InitType){GPIO_Pin_2, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiOutPin10()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_10, Serial0_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitSpiOutPin17()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_17, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiOutPin23()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_23, Serial0_Mode, DISABLE, DISABLE});
#endif


/* SPI IN Signal */
#define GPIO_InitSpiInPin9()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_9, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiInPin3()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_3, Serial0_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitSpiInPin16()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_16, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitSpiInPin24()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_24, Serial0_Mode, DISABLE, DISABLE});
#endif


/* 
 * I2C1 Port Configuration
 */

/* I2C1 Clock Signal */
#define GPIO_InitI2c1ClkPin12()  GPIO_Init(&(GPIO_InitType){GPIO_Pin_12, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitI2c1ClkPin14()  GPIO_Init(&(GPIO_InitType){GPIO_Pin_14, Serial1_Mode, DISABLE, DISABLE});

/* I2C1 Data Signal */
#define GPIO_InitI2c1DataPin13() GPIO_Init(&(GPIO_InitType){GPIO_Pin_13, Serial0_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitI2c1DataPin15() GPIO_Init(&(GPIO_InitType){GPIO_Pin_15, Serial1_Mode, DISABLE, DISABLE});
#endif

/* 
 * I2C2 Port Configuration
 */

/* I2C2 Clock Signal */
#define GPIO_InitI2c2ClkPin4()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_4, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitI2c2ClkPin6()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_6, Serial0_Mode, DISABLE, DISABLE});

/* I2C2 Data Signal */
#define GPIO_InitI2c2DataPin5()  GPIO_Init(&(GPIO_InitType){GPIO_Pin_5, Serial0_Mode, DISABLE, DISABLE});
#define GPIO_InitI2c2DataPin7()  GPIO_Init(&(GPIO_InitType){GPIO_Pin_7, Serial0_Mode, DISABLE, DISABLE});


/* 
 * PWM Configuration
 */

/* PWM0 Signal */
#define GPIO_InitPwm0Pin2()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_2, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitPwm0Pin4()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_4, Serial2_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitPwm0Pin16()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_16, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitPwm0Pin22()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_22, Serial1_Mode, DISABLE, DISABLE});
#endif

/* PWM1 Signal */
#define GPIO_InitPwm1Pin3()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_3, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitPwm1Pin5()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_5, Serial2_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitPwm1Pin17()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_17, Serial1_Mode, DISABLE, DISABLE});
#define GPIO_InitPwm1Pin21()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_21, Serial1_Mode, DISABLE, DISABLE});
#endif


/* 
 * PDM Configuration
 */

/* PDM Data Signal */
#define GPIO_InitPdmDataPin1()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_1, Serial2_Mode, DISABLE, DISABLE});
#define GPIO_InitPdmDataPin6()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_6, Serial2_Mode, DISABLE, DISABLE});
#define GPIO_InitPdmDataPin8()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_8, Serial2_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitPdmDataPin24()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_24, Serial2_Mode, DISABLE, DISABLE});
#endif

/* PDM Clock Signal */
#define GPIO_InitPdmClockPin2()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_2, Serial2_Mode, DISABLE, DISABLE});
#define GPIO_InitPdmClockPin7()   GPIO_Init(&(GPIO_InitType){GPIO_Pin_7, Serial2_Mode, DISABLE, DISABLE});

#ifdef BLUENRG2_DEVICE
#define GPIO_InitPdmClockPin23()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_23, Serial2_Mode, DISABLE, DISABLE});
#define GPIO_InitPdmClockPin25()    GPIO_Init(&(GPIO_InitType){GPIO_Pin_25, Serial2_Mode, DISABLE, DISABLE});
#endif


/**
  * @}
  */


/** @defgroup GPIO_Exported_Functions Exported Functions
  * @{
  */

/* Initialization */
void GPIO_DeInit(void);									/*!< GPIO deinitialization. */
void GPIO_Init(GPIO_InitType* GPIO_InitStruct);						/*!< GPIO initialization. */
void GPIO_StructInit(GPIO_InitType* GPIO_InitStruct);					/*!< Initialization of the GPIO_InitType Structure. */
void GPIO_InitLowPowerModes(GPIO_InitType* GPIO_InitStruct);                            /*!< GPIO9, GPIO10, GPIO11 initialization during low power modes for BlueNRG-2. */
void GPIO_WriteBitsLowPowerModes(uint32_t GPIO_Pins, BitAction BitVal);                 /*!< Set the output state of the GIO9, GIO10, GIO11 during low power modes. BlueNRG-2 only. */
void GPIO_ToggleBitsLowPowerModes(uint32_t GPIO_Pins);                                  /*!< Toggle the output state of the GIO9, GIO10, GIO11 during low power modes. BlueNRG-2 only. */

/* Data read */
BitAction GPIO_ReadBit(uint32_t GPIO_Pins);						/*!< Read GPIO pin logic state. */

/* Data write */
void GPIO_WriteBit(uint32_t GPIO_Pins, BitAction BitVal);				/*!< Write new GPIO pin logic state. */

/* Bit operations */
void GPIO_SetBits(uint32_t GPIO_Pins);							/*!< Set selected GPIO pin. */
void GPIO_ResetBits(uint32_t GPIO_Pins);						/*!< Reset selected GPIO pin. */
void GPIO_ToggleBits(uint32_t GPIO_Pins);						/*!< Toggle selected GPIO pin. */

/* GPIO interrupts */
void GPIO_EXTIStructInit(GPIO_EXTIConfigType* GPIO_EXTIInitStruct);                     /*!< Initialization of the GPIO_EXTIInit Structure. */
void GPIO_EXTIConfig(GPIO_EXTIConfigType* EXTIConfig);					/*!< Selects the GPIO pin used as EXTI Line. */
void GPIO_EXTICmd(uint32_t GPIO_Pins, FunctionalState NewState);	                /*!< Enables or disables interrupts on specified pins. */
void GPIO_ClearITPendingBit(uint32_t GPIO_Pins);					/*!< Clears the GPIOx interrupt pending bits. */
FlagStatus GPIO_GetITPendingBit(uint32_t GPIO_Pin);					/*!< Checks whether the specified enabled GPIO interrupt is active. */
ITStatus GPIO_GetITStatusBit(uint32_t GPIO_Pin);         /*!< Checks whether the specified GPIO interrupt is active. */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_GPIO_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

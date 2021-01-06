/**
 * @file    SDK_EVAL_Com.c
 * @author  AMS VMA RF application team
 * @version V1.1.0
 * @date    April 3, 2018
 * @brief   This file provides all the low level API to manage UART interface for eval board.
 * @details
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Com.h"
#include <stdio.h>


/** @addtogroup SDK_EVAL_BlueNRG1               SDK EVAL BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_Com                      SDK EVAL Com
 * @{
 */


/** @defgroup SDK_EVAL_Com_Private_TypesDefinitions             SDK EVAL Com Private Types Definitions
 * @{
 */

/**
 * @brief  IO_Receive_Data User callback 
 */
typedef struct _IO_ReceiveCallBack
{
  /**
   * callback to notify the applicatio with the received I/O data
   */ 
  IO_RECEIVE_DATA_USER_CALLBACK_TYPE IO_user_application;
} IO_ReceiveCallBack;

/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Private_Defines                      SDK EVAL Com Private Defines
 * @{
 */
#define READ_DATA_SIZE      1024
#define READ_BUFFER_IS_EMPTY() (Read_ptr_in == Read_ptr_out)

/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Private_Macros                       SDK EVAL Com Private Macros
 * @{
 */


/**
 * @}
 */

/** @defgroup SDK_EVAL_Com_Private_Variables                    SDK EVAL Com Private Variables
 * @{
 */

/* IO User Callback used to handle received data from I/O channel*/
static IO_ReceiveCallBack IO_Receive_Data;

static uint8_t  Read_Buffer[READ_DATA_SIZE]; 
static uint32_t Read_ptr_in = 0;
static uint32_t Read_ptr_out = 0;

/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Private_FunctionPrototypes                   SDK EVAL Com Private Function Prototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Private_Functions                            SDK EVAL Com Private Functions
 * @{
 */

/**
 * @brief  Send a byte on Serial port
 * @param  tx_data byte to be sent
 * @retval None
 */
static void SdkEvalUart_Send_Data(uint8_t tx_data)
{
  /* Wait if TX fifo is full. */
  while (UART_GetFlagStatus(UART_FLAG_TXFF) == SET);
  /* send the data */
  UART_SendData(tx_data);
}/* end SdkEvalUart_Send_Data() */


/**
 * @}
 */


/** @defgroup SDK_EVAL_Com_Public_Functions                            SDK EVAL Com Public Functions
 * @{
 */

/**
 * @brief  Configures Uart port.
 * @param  baudrate UART baudrate
 * @retval None
 */

void SdkEvalComUartInit(uint32_t baudrate)
{
  UART_InitType UART_InitStructure;
  
  /* GPIO Periph clock enable */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_UART | CLOCK_PERIPH_GPIO, ENABLE);
  
  GPIO_InitType GPIO_InitStructure;
  
  /* Configure GPIO_Pin_8 and GPIO_Pin_11 as UART_TXD and UART_RXD*/
  GPIO_InitStructure.GPIO_Pin = SDK_EVAL_UART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = SDK_EVAL_UART_TX_MODE;
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init(&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SDK_EVAL_UART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = SDK_EVAL_UART_RX_MODE;
  GPIO_Init(&GPIO_InitStructure);

  /* 
  ------------ USART configuration -------------------
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  UART_InitStructure.UART_BaudRate = baudrate;
  UART_InitStructure.UART_WordLengthTransmit = UART_WordLength_8b;
  UART_InitStructure.UART_WordLengthReceive = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
  UART_InitStructure.UART_FifoEnable = ENABLE;
  UART_Init(&UART_InitStructure);
  
  /* Interrupt as soon as data is received. */
  UART_RxFifoIrqLevelConfig(FIFO_LEV_1_64);

  /* Enable UART */
  UART_Cmd(ENABLE);
  
}


/**
 * @brief  Enable UART IRQ request.
 * @param  EnableIrq: functional state @ref FunctionalState
 *         This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void SdkEvalComUartIrqConfig(FunctionalState EnableIrq)
{
  /* NVIC configuration */
  NVIC_InitType NVIC_InitStructure;
  
  /* Enable the UART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = EnableIrq;
  NVIC_Init(&NVIC_InitStructure);
  
  UART_ITConfig(UART_IT_RX, EnableIrq);
  
}



/**
 * @brief  Serial IRQ handler body to be put on user UART_IRQHandler()
 * @param  None
 * @retval None
 */
void SdkEvalComIOUartIrqHandler(void)
{
  uint8_t read_data; 
  
  if(UART_GetITStatus(UART_IT_RX) != RESET)   {
    
    while(!UART_GetFlagStatus(UART_FLAG_RXFE)) {
      
      /* Read byte from the receive FIFO */
      read_data = (uint8_t) (UART_ReceiveData() & 0xFF);
      
      /* IO Receive Data: it handles the received data within the  User Application Function 
      defined as parameter of SdkEvalComIOConfig() */
      SdkEvalComIOReceiveData(&read_data,1);
    }
  }  
}


/**
 * @brief  Configure the selected I/O communciation channel
 * @param  io_receive_function user callback
 * @retval None
 */
void SdkEvalComIOConfig(IO_RECEIVE_DATA_USER_CALLBACK_TYPE io_receive_function)
{
  IO_Receive_Data.IO_user_application = io_receive_function;
  /* Init Serial Uart */
  SdkEvalComUartInit(UART_BAUDRATE);
  SdkEvalComUartIrqConfig(ENABLE);
#ifdef __GNUC__
  setvbuf(stdout, NULL, _IONBF, 0);
#endif  
}


/**
 * @brief  Send a byte on selected I/O port
 * @param  tx_data byte to be sent
 * @retval None
 */
void SdkEvalComIOSendData(uint8_t tx_data)
{
  SdkEvalUart_Send_Data(tx_data);
}

/**
 * @brief  Receive data on selected I/O port
 * @param  rx_data pointer to buffer of received
 * @param  data_size number of received data 
 * @retval None
 */
void SdkEvalComIOReceiveData(uint8_t * rx_data, uint16_t data_size)
{
  if(IO_Receive_Data.IO_user_application)
    IO_Receive_Data.IO_user_application(rx_data,data_size);
}

/**
 * @brief  Check if UART TX FIFO is empty or not.
 * @retval TRUE if FIFO is not empty, FALSE otherwise.
 */
uint8_t SdkEvalComIOTxFifoNotEmpty(void)
{
  return (UART_GetFlagStatus(UART_FLAG_TXFE) == RESET);
}

/**
 * @brief  Check if UART is BUSY or not.
 * @retval TRUE if UART is BUSY, FALSE otherwise.
 */
uint8_t SdkEvalComUARTBusy(void)
{
  if(UART_GetFlagStatus(UART_FLAG_BUSY) == SET)
    return TRUE;
  
  return FALSE;
}


/**
 * @brief  Push a byte in the read buffer
 * @param  byte to be pushed
 * @retval None
 */
void Read_Buffer_Push(uint8_t byte)
{
  Read_Buffer[Read_ptr_in] = byte;
  
  Read_ptr_in++;
  
  /* To avoid buffer overflow */
  if(Read_ptr_in == READ_DATA_SIZE)
  {
    Read_ptr_in = 0;
  }        
}

/**
 * @brief  Pop out a byte from the read buffer
 * @param  byte to be read
 * @retval ERROR if buffer empty, SUCCESS otherwise
 */
ErrorStatus Read_Buffer_Pop(uint8_t *byte)
{
  if(READ_BUFFER_IS_EMPTY())
    return ERROR;
  
  *byte = Read_Buffer[Read_ptr_out];
  Read_ptr_out++;
  
  if(Read_ptr_out == READ_DATA_SIZE)
    Read_ptr_out = 0;  
  
  return SUCCESS;
}


/**
 * @brief  Push a buffer in the read buffer
 * @param  rx_data data to be pushed
 * @param  data_size number of byte to read
 * @retval None
 */
void SdkEvalComIOProcessInputData(uint8_t * rx_data, uint16_t data_size)
{
  for(uint16_t i = 0; i < data_size; i++){
      Read_Buffer_Push(rx_data[i]);
  }
}




#ifdef __ICCARM__
#include <yfuns.h>


size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;

  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
    return _LLIO_ERROR;
  }

  if (buffer == 0) {
    // This means that we should flush internal buffers.
    //spin until TX complete (TX is idle)
    return 0;
  }

  while(size--) {
    SdkEvalUart_Send_Data((uint8_t)*buffer++);
    ++nChars;
  }

  return nChars;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
  size_t bytes = 0;
  
  while (bytes < size) {
    if (Read_Buffer_Pop(buffer+bytes)==SUCCESS)
    bytes++;
  }
  return bytes;
}

uint8_t __io_getcharNonBlocking(uint8_t *data)
{
  if(Read_Buffer_Pop(data) == SUCCESS) {
    return 1;
  }
  else {
    return 0;
  }
}


#else
#ifdef __CC_ARM

#include <stdio.h>
/* keil debug port defines */
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever needed */ };
FILE __stdout;
FILE __stdin;

/* KEIL fputc implementation template allowing to redirect printf output towards serial port (UART/USB) */
int fputc(int c, FILE *f)
{  
  SdkEvalUart_Send_Data((uint8_t) c);
  
  return 1;
}

uint8_t __io_getcharNonBlocking(uint8_t *data)
{
  int c = -1;

  Read_Buffer_Pop((uint8_t *)&c);
  
  if (c == EOF)
    return 0;
  else {
    *data = (uint8_t)c;
    return 1;
  }
}

int fgetc (FILE *f) {
  int data = 0;
  while (Read_Buffer_Pop((uint8_t *)&data) != SUCCESS);
  return data;
}

#else
#ifdef __GNUC__
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

/* Variables */
#undef errno
extern int errno;

#undef putchar

int putchar(int c)
{
  SdkEvalUart_Send_Data((uint8_t) c);
  return c;
}


static uint8_t heap[4096];
caddr_t _sbrk(int32_t incr)
{
  static uint8_t *heap_end;
  uint8_t *prev_heap_end;

  if (heap_end == 0)
    heap_end = &heap[0];

  prev_heap_end = heap_end;
  if ((heap_end + incr) > (heap+sizeof(heap)))
  {
//    write(1, "Heap and stack collision\n", 25);
//    abort();
    errno = ENOMEM;
    return (caddr_t) -1;
  }

  heap_end += incr;

  return (caddr_t) prev_heap_end;
}


int _close(int32_t file)
{
  return -1;
}


int _fstat(int32_t file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int32_t file)
{
  return 1;
}

int _lseek(int32_t file, int32_t ptr, int32_t dir)
{
  return 0;
}

int _write(int fd, char *str, int len)
{
  for(int i=0;i<len;i++) {
    SdkEvalComIOSendData((uint8_t) str[i]);
  }
  return len;
  
}

int _read(int handle, unsigned char * buffer, size_t size)
{
  int bytes = 0;

  while (bytes < size) {
    if (Read_Buffer_Pop(buffer+bytes)==SUCCESS)
    bytes++;
    else {
      if (bytes)
        break;
    }
  }
  return bytes;
}


uint8_t __io_getcharNonBlocking(uint8_t *data)
{
  if(Read_Buffer_Pop(data) == SUCCESS) {
    return 1;
  }
  else {
    return 0;
  }
}


#include <stdlib.h>
#include <string.h>

void __aeabi_memcpy(void *dest, const void *src, size_t n)
{
  memcpy(dest, src, n);
}

void __aeabi_memcpy4(void *dest, const void *src, size_t n)
{
  memcpy(dest, src, n);
}

void __aeabi_memclr(void *dest, size_t n)
{
    memset(dest, 0, n);
}

void __aeabi_memclr4(void *dest, size_t n)
{
    memset(dest, 0, n);
}

void __aeabi_memset(void *dest, char c, size_t n)
{
    memset(dest, c, n);
}

#endif
#endif
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

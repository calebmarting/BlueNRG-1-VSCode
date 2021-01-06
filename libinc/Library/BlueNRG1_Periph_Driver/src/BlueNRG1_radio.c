/**
******************************************************************************
* @file    BlueNRG1_radio.c
* @author  RF Application Team
* @date    Jan-2020
* @brief   This file provides all the BlueNRG-1,2 Radio Driver APIs.
*          This driver is based on undocumented hardware features and 
*          it is strongly recommend to do not change it.
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
* <h2><center>&copy; COPYRIGHT 2020 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_radio.h"
#include "vtimer.h"
/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @addtogroup Radio_Peripheral  Radio Peripheral
* @{
*/


/** @defgroup Radio_Exported_Types Exported Types
* @{
*/

 

/**
* @}
*/


/** @defgroup Radio_Exported_Constants Exported Constants
* @{
*/ 

/** @defgroup Radio_TXRX_Delay_Constants TXRX Delay Constants
* @{
*/
  
#define TX_DELAY     0x09
#define TX_DELAY1    0x09
#define RX_DELAY     0x05
#define RX_DELAY1    0x05

/**
* @}
*/
#define IRQBLUE                 (BLUE_CTRL_IRQn)

#define TIMER_OFF                 (0)
#define OFF_R_SLEEP               (28)

/**
* @}
*/

/** @defgroup Radio_Exported_Macros Exported Macros
* @{
*/
#define DISABLE_INTERRUPT(irqNum)            {NVIC_DisableIRQ(irqNum);}
#define ENABLE_INTERRUPT(irqNum)             {NVIC_EnableIRQ(irqNum);}
#define BLUE_STRUCT_PTR_CAST(x) (((BlueTransBlueStrctPtr_t)(int)(x)) & 0xffff)
#define BLUE_DATA_PTR_CAST(x) (((BlueTransBlueDataPtr_t)(int)(x)) & 0xffff)
#define TIME24_DIFF(a, b)       (((int32_t)((a - b) << 8)) >> 8)
#define IS_STATE_VALID(STATEMACHINE_NO) (STATEMACHINE_NO < STATEMACHINE_COUNT)
#define IS_POWERLEVEL_VALID(POWER) (POWER < 16)
#define IS_RFCHANNEL_VALID(RF_CHANNEL) (RF_CHANNEL <40)
  
#define MASK_INTERRUPTS() __disable_irq(); 
#define UNMASK_INTERRUPTS()  __enable_irq();


/**
* @}
*/ 

/** @defgroup Radio_Private_Variables Private Variables
* @{
*/

static volatile BlueGlob* const blueglob = &__blue_RAM.BlueGlobVar; // (BlueGlob*) BLUEGLOB_BASE;
static volatile BlueGlobPerMaster* const bluedata = __blue_RAM.BlueGlobPerMasterVar;// (BlueGlobPerMaster*) (BLUEGLOB_BASE+12);

/* glabal variables */
RadioGlobalParameters_t globalParameters;

/**
* @}
*/ 




/** @defgroup Radio_Private_Functions Private Functions 
* @{
*/

/**
* @brief  Weak definition of RADIO_TimerOverrunCallback
* @param  void
* @retval void
*/
WEAK_FUNCTION(void RADIO_TimerOverrunCallback(void));

/**
* @brief  Read RSSI
* @param  None
* @retval int32_t: RSSI in dBm
*/
static int32_t read_RSSI(void)
{
  int i = 0 ;
  int rsi_dbm;
  
  while(i < 100 && (BLUE_CTRL->RADIO_CONFIG & 0x10000) != 0)
  {
    //        dump_it = i++;
    rsi_dbm = i++; // use rsi_dbm instead of dump_it which is in RAM2
  }
  
  int rssi_int16 = ((globalParameters.rssiLevel[0] >> 16) & 0xff00) | (globalParameters.rssiLevel[1] & 0xff) ;
  int reg_agc = (globalParameters.rssiLevel[0] >> 16) & 0xff ;
  
  if(rssi_int16 == 0 || reg_agc > 0xb)
  {
    rsi_dbm = 127 ;
  }
  else
  {
    rsi_dbm = reg_agc * 6 -127 ;
    while(rssi_int16 > 30)
    {
      rsi_dbm = rsi_dbm + 6 ;
      rssi_int16 = rssi_int16 >> 1 ;
    }
    rsi_dbm = rsi_dbm + ((417*rssi_int16 + 18080)>>10) ;
  }
  
  return rsi_dbm ;
}

/**
* @brief  write_radio_config.
* @param  ptr
* @retval None
*/
static void write_radio_config(char *ptr)
{
  /* rc_config[0] = 0x005b3402 ; // turn on X-tal */
  int var11 = ((int)ptr & 0xffff) | 0x10000 ;
  BLUE_CTRL->RADIO_CONFIG = var11 ;
  int i = 0 ;
  while((BLUE_CTRL->RADIO_CONFIG & 0x10000) != 0)
  {
    i = i+1 ;
  }
}

/**
* @}
*/



/** @defgroup Radio_Exported_Functions Exported Functions
* @{
*/


/**
* @brief  Radio ISR. Completed event is handled here.
*         Besides, next packet is scheduled here.
* @param  None
* @retval None
*/
void RADIO_IRQHandler(void)
{
  uint32_t time;
  uint32_t int_value = BLUE_CTRL->INTERRUPT;
  
  if ( (int_value & IRQ_DONE) != 0) {
    /* Indicates that there is no scheduling for wakeup Timer */
    TIMER_updatePeriodSlow();
    /* Restore period_slow since it is rewritten by hardware */
    
    /* if (!Is_Flash_Write_On_Going())
    {
      BLUE_CTRL->RADIO_CONFIG = ((int)GVAR(rssi_level) & 0xffff) | 0x10000;
    } */
    BLUE_CTRL->RADIO_CONFIG = ((int)(globalParameters.rssiLevel) & 0xffff) | 0x10000;
    
    ActionPacket* next, *actionPacketBackup;
    BlueTransStruct *p;
    
    /* Copy status in order for callback to access it. */ 
    globalParameters.current_action_packet->status = int_value;
    
    if( ( globalParameters.current_action_packet->condRoutine(globalParameters.current_action_packet) ) == TRUE) {
      next = globalParameters.current_action_packet->next_true;
    }
    else {
      next = globalParameters.current_action_packet->next_false;
    }
    
    if(next == NULL_0) {
      /* timer2 off */
      BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
      blueglob->Config = 0;
    }
    else {
      blueglob->Config = (blueglob->Config & 0xFFF0) | (next->StateMachineNo | 0x48);
      p= &next->trans_packet;
      (bluedata + next->StateMachineNo)->stat_config  = next->trans_config;
      (bluedata + next->StateMachineNo)->txpoint  = BLUE_STRUCT_PTR_CAST(p);
      (bluedata + next->StateMachineNo)->rcvpoint  = BLUE_STRUCT_PTR_CAST(p);
      
#if defined(BLUENRG2_DEVICE)
    (bluedata + next->StateMachineNo)->remap_chan = (bluedata+next->StateMachineNo)->remap_chan | 0x80;
#endif

      /* Packet will execute on time specified by WakeupTime */
      if( (next->ActionTag & TIMER_WAKEUP ) !=0 ) {
        /* timer2 off */
        BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
        
        /* program timer at next->wakeuptime */
        if((next->ActionTag & RELATIVE ) !=0) {
          time = (uint32_t)TIMER_GetCurrentSysTime() + TIMER_UsToSystime(next->WakeupTime);
          HAL_VTimer_SetRadioTimerValue(time,(next->ActionTag & TXRX), (next->ActionTag & PLL_TRIG));
        }
        else {
          HAL_VTimer_SetRadioTimerValue(next->WakeupTime,(next->ActionTag & TXRX), (next->ActionTag & PLL_TRIG));
        }
        
      }
      /* back to back */
      else {
      }
    }  
    
    /* Accept the packet even with CRC Error */
    if(   ( (int_value & IRQ_RCV_OK) != 0) || ( (int_value & IRQ_CRC_ERR) != 0) ) {
      /* read RSSI */
      globalParameters.current_action_packet->rssi =  read_RSSI();
      
      /* bluedata->config = bluedata->config ^ 0x80 ;  toggle NESN bit
      bluedata->config = bluedata->config & 0x7F ;  //reset NESN bit */
      
      /* read time stamp */
      globalParameters.current_action_packet->timestamp_receive = TIMER_GetAnchorPoint();
    }
    
    actionPacketBackup = globalParameters.current_action_packet;
    globalParameters.current_action_packet = next;
    actionPacketBackup->dataRoutine(actionPacketBackup, next);
  }
  
  if ( (int_value & IRQ_WAKEUP_2) != 0) {
    HAL_VTIMER_TimeoutCallback(); 
  }
  if (int_value & BIT_TIME_OVERRUN) {
    // Timer overrun!!!
    RADIO_TimerOverrunCallback();
  }
  
  /* clear interrupts */
  BLUE_CTRL->INTERRUPT = int_value;
  
  return ;
}

/**
* @brief  This routine force the radio to be stopped. After calling this function,
*         the ISR is not triggered unless, "MakeActionPacketPending" API called again.
* @param  None
* @retval TRUE
*/
uint8_t RADIO_StopActivity(void)
{
  uint8_t retVal;
  MASK_INTERRUPTS();
  blueglob->Config = 0; 
  retVal = HAL_VTimer_ClearRadioTimerValue();
  globalParameters.forceRadiotoStop = TRUE;
  UNMASK_INTERRUPTS();
  return retVal;     
}


/**
* @brief  This routine sets the 40-bit receive and transmit packet count, to be used in
*         encryption calculation.
*         Both are pointers to char, and set the 39-bit counter + 1-bit MSB as defined in the
*         Bluetooth Low Energy spec. One is for transmit packets, the other is for receive packets.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  count_tx: 40-bit transmit packet count, to be used in encryption nounce calculation.
* @param  count_rcv: 40-bit receive packet count, to be used in encryption nounce calculation.
* @retval None
*/
void RADIO_SetEncryptionCount(uint8_t StateMachineNo, uint8_t *count_tx, uint8_t *count_rcv) 
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  
  for(uint8_t i = 0; i < 5; i++) {
    (bluedata + StateMachineNo)->Pack_count_Rcv[i] = count_rcv[i];
    (bluedata + StateMachineNo)->Pack_count_Tx[i] = count_tx[i];
  }
  return; 
}


/**
* @brief  This routines sets the 8-byte encryption initial vector, and the 16-byte encryption key.
*         The definition of the encryption key and its initial value is according the Bluetooth Low Energy spec.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  enc_iv: 8-byte encryption initial vector.
* @param  enc_key: 16-byte encryption key.
* @retval None
*/
void RADIO_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t *enc_iv, uint8_t *enc_key) 
{   
  uint8_t i = 0;
  
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  
  for(i = 0; i < 8; i++) {
    (bluedata + StateMachineNo)->enc_iv[i] = enc_iv[i];
    (bluedata + StateMachineNo)->enc_key[i] = enc_key[i];
  }
  for(i = 8; i < 16; i++) {
    (bluedata + StateMachineNo)->enc_key[i] = enc_key[i];
  }
  
  return ; 
}


/**
* @brief  This routine sets the channel map. If channel map is not in use, do not use it. The
*         chan_remap constant is a 37-bit vector, one bit per channel from 0 to 36, with the
*         LSB corresponding to the channel 0. If the corresponding bit is set, the channel is in use; if it is cleared,
*         there is an automatic remap to another channel, as defined in the Bluetooth Low
*         Energy specification. 
*         Setting all bits of the array to one, disables the channel remapping, and this is the mode
*         that should be used if the Bluetooth channel remap is not in use.
*
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  chan_remap: a 37-bit vector, one bit per channel from 0 to 36, with the LSB corresponding to the channel 0 
* @retval None
*/
void RADIO_SetChannelMap(uint8_t StateMachineNo,uint8_t *chan_remap)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));  
  
  for(uint8_t i = 0; i < 5; i++) {
    (bluedata + StateMachineNo)->chan_remap[i] = chan_remap[i];
  }     
  return ; 
}


/**
* @brief  This routine sets the channel and the channel increment. Channel and channel
*         increment follows channel mapping corresponding to Bluetooth specification.
*         RF channel 0:  2402 MHz -> channel 37
*         RF channel 1:  2404 MHz -> channel 0
*         RF channel 2:  2406 MHz -> channel 1
*         ...
*         RF channel 11: 2424 MHz -> channel 10
*         RF channel 12: 2426 MHz -> channel 38
*         RF channel 13: 2428 MHz -> channel 11
*         RF channel 14: 2430 MHz -> channel 12
*         ...
*         RF channel 38: 2478 MHz -> channel 36
*         RF channel 39: 2480 MHz -> channel 39
*
*         For the channel 37, 38 and 39 (RF channel 0, 12, 39):
*          - The crc_init value should always be 0x555555.
*          - There is no encryption.
*          - The channel increment is dedicated, the channel sequence is:
*            37 -> 38 -> 39 -> 37 -> ... while for the other channels is:
*            0 -> 1 -> ... -> 36 -> 0 -> ...
*            There is no channel map for these three channels.
*            Valid values for packet length for these channels are from 6 to 37.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  channel: frequency channel. From 0 to 39.
* @param  channel_increment: determines the hoping value.
* @retval None
*/
void RADIO_SetChannel(uint8_t StateMachineNo, uint8_t channel, uint8_t channel_increment) 
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo)); 
  assert_param(IS_RFCHANNEL_VALID(channel)); 
  
  (bluedata + StateMachineNo)->unmapped_chan = channel;  
  (bluedata + StateMachineNo)->hop_incr = channel_increment;
  
  return ; 
}


/**
* @brief  This routine sets the NetworkID, the CRC init, and the SCA.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  NetworkID: The NetworkID is the ID of the device.
*         The user shall ensure that the NetworkID meets the following requirements:
*           - It shall have no more than six consecutive zeros or ones.
*           - It shall not have all four octets equal.
*           - It shall have no more than 24 transitions.
*           - It shall have a minimum of two transitions in the most significant six bits.   
* @param  crc_init: CRC initilization value.
*         This value must be 0x555555 when channel frequency is one of these values: 37, 38, 39.
* @param  sca: slow clock accuracy. Period duration of the 32 kHz clock (slow clock).
*              "period_slow" defines the number of 16 MHz clock cycle in a 32 kHz period. 
*              Note: the number 16 MHz clock cycle in a 32 kHz period can be measured by the clockgen block. 
*              Note: If the application uses a CLK32K crystal then the value is a constant provided by the firmware. (488 = 16,000,000 / 32768) 
*              Note: If the application uses the 32 kHz RC clock, then its value is measured in the clock_gen. 
*         Note: not used here. This value should be set automatically in RADIO_CrystalCheck routine. 
* @retval None
*/
void RADIO_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo)); 
  
  (bluedata + StateMachineNo)->access_address = NetworkID;
  (bluedata + StateMachineNo)->crc_init = crc_init;      
  
  return; 
}



/**
* @brief  Initializes the time between back-to-back radio transmissions.
* @param  back_to_back_time: time between two packets if wakeupTimer is not used. Resolution is 1 us.
* @retval None
*/
void RADIO_SetBackToBackTime(uint32_t back_to_back_time)
{
  globalParameters.back2backTime = back_to_back_time;
  return;
}


/**
* @brief  Configures the transmit power level.
* @param  PowerLevel: power level which should set to this value:
*          - 0 = -18 dBm,
*          - 1 = -15 dBm,
*          - 2 = -12 dBm,
*          - 3 = -9 dBm,
*          - 4 = -6 dBm,
*          - 5 = -2 dBm,
*          - 6 =  0 dBm,
*          - 7 =  5 dBm. 
*
*          - 8 = -14 dBm,
*          - 9 = -11 dBm,
*          - 10 = -8 dBm,
*          - 11 = -5 dBm,
*          - 12 = -2 dBm,
*          - 13 =  2 dBm,
*          - 14 =  4 dBm,
*          - 15 =  8 dBm.
* @retval None
*/
void RADIO_SetTxPower(uint8_t PowerLevel)
{
  /* Check the parameters */
  assert_param(IS_POWERLEVEL_VALID(PowerLevel)); 
  
  PowerLevel <<= 3;
  for(int n = 0; n <STATEMACHINE_COUNT ; n++) {
    (bluedata+n)->conf_byte5 = PowerLevel;
  }
  return;
}

  
/**
* @brief  This routine turns encrypt ON and OFF. When the encryption is enabled, the hardware will add 4 bytes
*         at the end of the packet as MIC (Message Authentication Code). So the user need to add 4 byte to the
*         length of the packet. The parameters enable the encryption for both. There are two separated parameters
*         for this function just compliance with previous version of the driver.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  EncryptFlagTx: DISABLE: encryption is turned OFF for both TX and RX operations.
*              ENABLE: encryption is turned OFF for both TX and RX operations.
*         This parameter can be: ENABLE or DISABLE.
* @param  EncryptFlagRcv: encryption is turned OFF for both TX and RX operations.
*              ENABLE: encryption is turned OFF for both TX and RX operations.
*          This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void RADIO_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  assert_param(IS_FUNCTIONAL_STATE(EncryptFlagTx));
  assert_param(IS_FUNCTIONAL_STATE(EncryptFlagRcv));
  
  if(EncryptFlagTx==ENABLE || EncryptFlagRcv==ENABLE) {
    (bluedata + StateMachineNo)->config = (bluedata + StateMachineNo)->config | 0x0A;
  }
  else {
    (bluedata + StateMachineNo)->config = (bluedata + StateMachineNo)->config & 0xF5;
  }
  return;
}


/**
* @brief  This routine should be called after writing/modifying an action packet, and before it
*         is executed via either the API mechanism, or the next mechanism.
* @param  p: pointer to action packet.
* @retval None
*/
void RADIO_SetReservedArea(ActionPacket *p)
{
  //p->WakeupTime = (uint32_t) (p->WakeupTime * 0.525); /* convert us to appropriated value.  1000 * 1/512000 = 1953.125 */
  
  /* Check the parameters */
//  assert_param(IS_DELAY_VALID(p->WakeupTime));
   
  p->trans_packet.datptr = BLUE_DATA_PTR_CAST(p->data);
  
  p->trans_packet.next = BLUE_STRUCT_PTR_CAST(&(p->trans_packet));
  
  /* rcvlen[17:0] */
  if(p->ReceiveWindowLength < 0x40000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength)       & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 8)  & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 16) & 0x00000003 ;
  }
  else if(p->ReceiveWindowLength < 0x100000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 2)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 10) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 18) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x04;
  }
  else if(p->ReceiveWindowLength < 0x400000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 4)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 12) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 20) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x08;  
  }
  else if(p->ReceiveWindowLength < 0x1000000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 6)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 14) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 22) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x0C; 
  }
  else {
    /* error */
  }
  
  if((p->ActionTag & TIMESTAMP_POSITION) == 0) {
    p->trans_packet.rcvlen[2] |= TIMESTAMP_POSITION_LASTBIT;
  }
  else {
    p->trans_packet.rcvlen[2] |= TIMESTAMP_POSITION_ACCESSADDRESS;
  }
  
  /* p->trans_packet.rcvlen[2] = p->trans_packet.rcvlen[2] | 0x30 | 0x40 | TIMESTAMP_POSITION; */
  p->trans_packet.rcvlen[2] = p->trans_packet.rcvlen[2] | 0x30;
  
  p->trans_packet.timeout[0] = (globalParameters.back2backTime >> 0)  & 0x000000FF;
  p->trans_packet.timeout[1] = (globalParameters.back2backTime >> 8)  & 0x000000FF;
  p->trans_packet.timeout[2] = (globalParameters.back2backTime >> 16) & 0x0000000F; /* only 20 bits */
  
  p->trans_packet.byte10 =0;
  p->trans_packet.byte10 = (p->ActionTag & INC_CHAN) | (p->ActionTag & PLL_TRIG) | (p->ActionTag & TXRX);
  
  /* next state is zero */
  p->trans_packet.byte10 = p->trans_packet.byte10 & 0x1F;
  
  /* sleep . do not go to sleep*/
  p->trans_packet.byte10 = p->trans_packet.byte10 & 0xFB;
  
  /* for multi-state automatically increment the channel */
  p->trans_packet.byte10 = p->trans_packet.byte10 | ((p->StateMachineNo)<<5);

  /* enable all interrupts */
  p->trans_packet.byte11 = 0xFF;
  
  p->trans_config =0;
  if( (p->ActionTag & TXRX) != 0) {
    p->trans_config = 0x80;
  }
  
  return ;
}


/**
* @brief  This routine should be called for the first actionPacket that needs to be scheduled
*         on the radio. For it to work OK, the WakeupTime should be valid, and the TIMER_WAKEUP flag set.
*         Subsequent packets can be dispatched using the same process, or by providing non-NULL pointers
*         to the next_true and next_false pointers of the ActionPacket.
*
* @param  p: pointer to action packet.
* @retval uint8_t with following values:
*          - 0x00 : Success.
*          - 0xC4 : Radio is busy, action packet has not been executed.
*/
uint8_t RADIO_MakeActionPacketPending(ActionPacket *p)
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t time,dummyTime;
  
  if(RADIO_GetStatus(&dummyTime) == BLUE_IDLE_0)
  {
    globalParameters.forceRadiotoStop = FALSE;
    uint16_t  statemachineNo;
    BlueTransStruct *p1 ; 
    
    /* timer2 off */
    BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
    globalParameters.rssiLevel[0] = 0x2e86 ;
    globalParameters.rssiLevel[1] = 0 ;
    
    int32_t* temp ;
    temp = globalParameters.hot_ana_config_table_a;
    blueglob->RadioConfigPtr = BLUE_DATA_PTR_CAST(temp) ; /* to be discussed */
    
    /* Set_Radio_Config_Data(); */   /* to be discussed */
    
    /* tell the schedular which table in SRAM should be applied and activate it */
    statemachineNo = 0x0007 & p->StateMachineNo;
    
    blueglob->Config = (blueglob->Config & 0xFFF0) | (statemachineNo | 0x48);
    
    (bluedata+statemachineNo)->txpoint_prev = 0;
    (bluedata+statemachineNo)->rcvpoint_prev = 0;
    (bluedata+statemachineNo)->txpoint_next = 0;

    p1= &p->trans_packet;
    (bluedata+statemachineNo)->txpoint =  BLUE_STRUCT_PTR_CAST(p1);
    (bluedata+statemachineNo)->rcvpoint = BLUE_STRUCT_PTR_CAST(p1);
    
#if defined(BLUENRG2_DEVICE)
    (bluedata+statemachineNo)->remap_chan = (bluedata+statemachineNo)->remap_chan | 0x80;
#endif

    globalParameters.current_action_packet = p;
    (bluedata+statemachineNo)->stat_config = p->trans_config; /* Transmision or reception for first packet */
    
    /* program timer at next->wakeuptime */
    MASK_INTERRUPTS();
    if((p->ActionTag & RELATIVE ) !=0) {
      time = (uint32_t)TIMER_GetCurrentSysTime() + TIMER_UsToSystime(p->WakeupTime);
      returnValue = HAL_VTimer_SetRadioTimerValue(time,(p->ActionTag & TXRX), (p->ActionTag & PLL_TRIG));
    }
    else {
      returnValue = HAL_VTimer_SetRadioTimerValue(p->WakeupTime,(p->ActionTag & TXRX), (p->ActionTag & PLL_TRIG));
    }   
    UNMASK_INTERRUPTS();
  }
  else {
    returnValue = RADIO_BUSY_C4;
  }
  
  return returnValue;
}


/**
* @brief  Initializes the radio.
* @param  hs_startup_time: start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us).
* @param  low_speed_osc: source for the 32 kHz slow speed clock: 1: internal RO; 0: external crystal.
* @param  hot_table: reserved for future use, set to NULL.
* @param  whitening: DISABLE: disable whitening for TX and RX. ENABLE: enable whitening for TX and RX.
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void RADIO_Init(uint32_t* hot_table, FunctionalState whitening)
{     
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(whitening));
    
  DISABLE_INTERRUPT(IRQBLUE);
  
  blueglob->TxDelay = TX_DELAY;
  blueglob->TxDelay1 = TX_DELAY1;        
  blueglob->RcvDelay = RX_DELAY;
  blueglob->RcvDelay1 = RX_DELAY1;
  
  if(whitening == DISABLE) {
    /* disable Transmision Whitening */
    blueglob->TxDelay |= 0x80;
    blueglob->TxDelay1 |= 0x80; 
    
    /* disable Receiving Whitening */
    blueglob->RcvDelay |= 0x80;
    blueglob->RcvDelay1 |= 0x80; 
  }
  
  globalParameters.hardware_config.hot_ana_config_table = hot_table;
  
  blueglob->Config = 0;
  BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE ;
  BLUE_CTRL->HOST_WKUP_TIMER = TIMER_OFF  | (1<<OFF_R_SLEEP);
  int int_value = BLUE_CTRL->INTERRUPT ;
  BLUE_CTRL->INTERRUPT = int_value ; 
    
  globalParameters.back2backTime = 71; /* 150 micro second */
  globalParameters.forceRadiotoStop = FALSE;
  
  RADIO_SetTxPower(11);
  RADIO_SetEncryptFlags(0, DISABLE, DISABLE);
  
  ENABLE_INTERRUPT(IRQBLUE);

  return;
}


/**
* @brief  This routine returns the status of the Radio whether it is busy or not.
*         If it is busy, the time argument will be filled with absolute time value of next radio activity.
*         User can use this value (in comparison with current time) to check that 
*         how long it is far to force the device to sleep or not.
*         Note: when Timer2 is on schedule, time value is not absolute time.
* @param  time: pointer to int value which is going to have time value.
* @retval uint8_t with following values:
*          - 0 : BLUE_IDLE_0. Radio is not busy.
*          - 1 : BLUE_BUSY_NOWAKEUP_T1. Radio is busy, but there is no wakeup timer on schedule but Timer1 is.
*          - 2 : BLUE_BUSY_NOWAKEUP_T2. Radio is busy, but there is no wakeup timer on schedule but Timer2 is.
*          - 3 : BLUE_BUSY_WAKEUP. Radio is busy and wakeup timer is on the schedule.
*/
uint8_t RADIO_GetStatus(uint32_t *time)
{
  uint8_t retValue = BLUE_IDLE_0;
  if((blueglob->Config & 0x8) != 0)
  {   
    retValue = TIMER_GetRadioTimerValue(time);
  }
  return retValue;
}


/**
* @brief  Starts tone transmission on selected channel.
*         This API is dedicated for test and destroys context and multistate.
*         So, after calling this function the radio should be re-initialized.
* @param  RF_Channel: radio frequency channel number from 0 to 39.
* @param  PowerLevel: power level which should set to this value:
*          - 0 = -18 dBm,
*          - 1 = -15 dBm,
*          - 2 = -12 dBm,
*          - 3 = -9 dBm,
*          - 4 = -6 dBm,
*          - 5 = -2 dBm,
*          - 6 =  0 dBm,
*          - 7 =  5 dBm. 
*
*          - 8 = -14 dBm,
*          - 9 = -11 dBm,
*          - 10 = -8 dBm,
*          - 11 = -5 dBm,
*          - 12 = -2 dBm,
*          - 13 =  2 dBm,
*          - 14 =  4 dBm,
*          - 15 =  8 dBm.
* @retval None
*/
void RADIO_StartTone(uint8_t RF_Channel, uint8_t powerLevel)
{
  /* Check the parameters */
  assert_param(IS_RFCHANNEL_VALID(RF_Channel)); 
  assert_param(IS_POWERLEVEL_VALID(powerLevel));
  
  uint8_t channel;
  uint8_t value1;
  uint8_t value2;
  uint32_t dummy;
  char data[24];
  
  if (RADIO_GetStatus(&dummy) == BLUE_IDLE_0) {
    /* [EM:] Calculate the synt divide factor for 16 MHz quarts and +250 kHz offset wrt the channel center frequency
    *  Algorithmically MAK = F_rf*(2^20)/16 = F_rf*(2^16)
    *  With F_rf = (2402+2*RF_Channel)+0.25 MHz
    *  K corresponds to b19-b0  of MAK
    *  A corresponds to b22-b20 of MAK
    *  M corresponds to b27-b23 of MAK
    */
    uint32_t kHz_250_scaled = 16384;    /* = 0.250*2^20/16 */
    uint32_t MAK = ((2402+2*RF_Channel)<<16) - kHz_250_scaled;
    uint8_t M  = (MAK>>23)&0x1F;                                             
    uint8_t A  = (MAK>>20)&0x07;
    uint32_t K = (MAK&0x000FFFFF)+1;
    uint8_t MOD3_DIG_TEST = (M<<3) | (A & 0x7);
    uint8_t MOD2_DIG_TEST = ((K>>12) & 0xFF);
    uint8_t MOD1_DIG_TEST = ((K>> 4) & 0xFF);
    uint8_t MOD0_DIG_TEST = ((K&0x0F)<<4) | 0x09;
    
    value1  =  0x7E & 0x87;        
    
    value1 |= (powerLevel     << 3);        
    value2  =  0x3F & 0xF1; 
    powerLevel &= 0x07;        
    value2 |= (powerLevel     << 1);
    
    channel = RF_Channel;
    channel <<= 1;
    data[0]  = 0x03;
    data[1]  = 0x04;
    data[2]  = channel;         // addr 0x4, value channel
    data[3]  = value1;          // addr 0x5, value TX power
    data[4]  = 0x02;
    data[5]  = 0x1C;            // addr 0x1C, value 0x73
    data[6]  = 0x73;
    data[7]  = 0x03;
    data[8]  = 0x24;
    data[9]  = value2;          // addr 0x24, value 0x3F
    data[10] = 0x3F;            // addr 0x25, value 0x3F
    data[11] = 0x02;
    data[12] = 0x23;
    data[13] = 0x03;            // addr 0x23, value 0x03
    data[14] = 0x02;
    data[15] = 0x23;
    data[16] = 0x07;            // addr 0x23, value 0x07�
    /*[EM:]	Fix 204: force MAK values through MODx test registers */
    data[17] = 0x05; 
    data[18] = 0x28;
    data[19] = MOD3_DIG_TEST;
    data[20] = MOD2_DIG_TEST;
    data[21] = MOD1_DIG_TEST;
    data[22] = MOD0_DIG_TEST;
    
    data[23] = 0x0;
    
    write_radio_config(data);
  }
}


/**
* @brief  This routine starts tone transmission.
*         This API is dedicated for test and destroys context and multistate.
*         So, after calling this function the radio should be re-initialized.
* @param  None
* @retval None
*/
void RADIO_StopTone()
{
  char data[9];
  
  data[0] = 0x04;
  data[1] = 0x23;
  data[2] = 0x00;             // addr 0x23, value 0x00
  data[3] = 0x0E;             // addr 0x24, value 0x0E
  data[4] = 0x00;             // addr 0x25, value 0x00
  data[5] = 0x02;	      //[EM:]	Fix 204
  data[6] = 0x2B;             // Reg MOD0_DIG_TEST@0x2B
  data[7] = 0x08;             // Reset to default value (disable mod test mode)
  data[8] = 0x00;
  write_radio_config(data);
}


/**
* @brief  Encrypts plain text data using AES encryption with given input key and
*         128 bit block size
* @param  Key: encryption key to be used.
* @param  plainData: text data to be encrypted.
* @param  cypherData: contains the encrypted data.
* @retval None
*/
void RADIO_EncryptPlainData(uint8_t *Key, 
                            uint8_t *plainData,
                            uint8_t *cypherData)
{   
  uint32_t key[4] ;
  uint32_t cleartext[4] ;
  uint32_t ciphertext[4] ;
  volatile uint32_t ii ;
  
  for(uint8_t i = 0; i < 4; i++) {
    key[i] = ((uint32_t)Key[i*4]) | ((uint32_t)Key[i*4+1])<<8 | ((uint32_t)Key[i*4+2])<<16 | ((uint32_t)Key[i*4+3])<<24;
    cleartext[i] = ((uint32_t)plainData[i*4]) | ((uint32_t)plainData[i*4+1])<<8 | ((uint32_t)plainData[i*4+2])<<16 | ((uint32_t)plainData[i*4+3])<<24;
  }
  
  BLUE_CTRL->AES_KEY0 = key[3];
  BLUE_CTRL->AES_KEY1 = key[2];
  BLUE_CTRL->AES_KEY2 = key[1];
  BLUE_CTRL->AES_KEY3 = key[0];
  
  BLUE_CTRL->CLEAR_TEXT0 = cleartext[3];
  BLUE_CTRL->CLEAR_TEXT1 = cleartext[2];
  BLUE_CTRL->CLEAR_TEXT2 = cleartext[1];
  BLUE_CTRL->CLEAR_TEXT3 = cleartext[0];
  
  ii = 0 ;
  while( ( BLUE_CTRL->STATUS & BIT_AES_READY) == 0 && ii < 100) { 
    ii++ ;}
  
  ciphertext[0] = BLUE_CTRL->AES_CYPHERTEXT3;
  ciphertext[1] = BLUE_CTRL->AES_CYPHERTEXT2;
  ciphertext[2] = BLUE_CTRL->AES_CYPHERTEXT1;
  ciphertext[3] = BLUE_CTRL->AES_CYPHERTEXT0;

  for(uint8_t i = 0; i < 4; i++) {
    cypherData[i*4] = ((uint8_t)ciphertext[i]);
    cypherData[i*4+1] = (uint8_t)(ciphertext[i]>>8);
    cypherData[i*4+2] = (uint8_t)(ciphertext[i]>>16);
    cypherData[i*4+3] = (uint8_t)(ciphertext[i]>>24);
  }
  
  return ;
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

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

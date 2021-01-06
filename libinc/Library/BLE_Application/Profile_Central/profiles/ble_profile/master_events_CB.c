/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : master_events_CB.c
* Author             : AMS - VMA, RF Application Team
* Version            : V1.0.0
* Date               : 14-February-2017
* Description        : This file manages the Stack CB events for BlueNRG-1 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "bluenrg1_stack.h"
#include "master_basic_profile_events.h"

/* External variables --------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************** EVENT STACK CALLBACKS ********************/

/*******************************************************************************
 * Function Name  : hci_encryption_change_event.
 * Description    : This event indicates that the change of the encryption
 *                  mode has been completed
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_encryption_change_event(uint8_t Status,
                                 uint16_t Connection_Handle,
                                 uint8_t Encryption_Enabled)
{
  Master_Library_encryption_change_event(Status, 
                                         Connection_Handle,
                                         Encryption_Enabled); 
  
}

/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
  Master_Library_disconnection_complete_event(Status,
                                              Connection_Handle,
                                              Reason);
}

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)
{
  Master_Library_le_connection_complete_event(Status,
                                              Connection_Handle,
                                              Peer_Address);
}

/*******************************************************************************
 * Function Name  : hci_le_advertising_report_event.
 * Description    : This event indicates that a Bluetooth device have responded 
 *                  to an active scan
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_advertising_report_event(uint8_t Num_Reports,
                                     Advertising_Report_t Advertising_Report[])
{
  Master_Library_le_advertising_report_event(Num_Reports,
                                             Advertising_Report);
}

/*******************************************************************************
 * Function Name  : aci_gap_proc_complete_event.
 * Description    : This event is sent by the GAP to the upper layers when 
 *                  a procedure previously started has been terminated
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gap_proc_complete_event(uint8_t Procedure_Code,
                                 uint8_t Status,
                                 uint8_t Data_Length,
                                 uint8_t Data[])
{
  Master_Library_gap_proc_complete_event(Procedure_Code);
}

/*******************************************************************************
 * Function Name  : aci_gatt_read_permit_req_event.
 * Description    : This event is given to the application when a read request 
 *                  or read blob request is received by the server from the client
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                    uint16_t Attribute_Handle,
                                    uint16_t Offset)
{
  Master_Library_gatt_read_permit_req_event(Connection_Handle,
                                            Attribute_Handle,
                                            Offset);
}


/*******************************************************************************
 * Function Name  : aci_gatt_tx_pool_available_event.
 * Description    : This event is generated each time BLE FW stack raises the 
 *                  error code BLE_STATUS_INSUFFICIENT_RESOURCES (0x64), as soon 
 *                  as there are at least two buffers available for notifications 
 *                  or write commands.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers)
{
  Master_Library_gatt_tx_pool_available_event(Connection_Handle,
                                             Available_Buffers);
}

/*******************************************************************************
 * Function Name  : aci_gatt_write_permit_req_event.
 * Description    : This event is given to the application when a write request, 
 *                  write or signed write command is received by the server from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                     uint16_t Attribute_Handle,
                                     uint8_t Data_Length,
                                     uint8_t Data[])
{
  Master_Library_gatt_write_permit_req_event(Connection_Handle,
                                             Attribute_Handle,
                                             Data_Length,
                                             Data);
}                                                 

/*******************************************************************************
 * Function Name  : aci_att_find_info_resp_event.
 * Description    : This event is generated in response to a Find Information Request
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_att_find_info_resp_event(uint16_t Connection_Handle,
                                  uint8_t Format,
                                  uint8_t Event_Data_Length,
                                  uint8_t Handle_UUID_Pair[])
{
   Master_Library_att_find_info_resp_event(Connection_Handle,
                                           Format,
                                           Event_Data_Length,
                                           Handle_UUID_Pair);                        
}

/*******************************************************************************
* Function Name  : aci_att_read_by_group_type_resp_event.
* Description    : This event is generated in response to a Read By Group Type Request
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_att_read_by_group_type_resp_event(uint16_t Connection_Handle,
                                           uint8_t Attribute_Data_Length,
                                           uint8_t Data_Length,
                                           uint8_t Attribute_Data_List[])  
{
  Master_Library_att_read_by_group_type_resp_event( Connection_Handle,
                                                   Attribute_Data_Length,
                                                   Data_Length,
                                                   Attribute_Data_List);
}

/*******************************************************************************
* Function Name  : aci_att_read_by_type_resp_event.
* Description    : This event is generated in response to a Read By Type Request
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/    
void aci_att_read_by_type_resp_event(uint16_t Connection_Handle,
                                     uint8_t Handle_Value_Pair_Length,
                                     uint8_t Data_Length,
                                     uint8_t Handle_Value_Pair_Data[])
{
  Master_Library_att_read_by_type_resp_event(Connection_Handle,
                                             Handle_Value_Pair_Length,
                                             Data_Length,
                                             Handle_Value_Pair_Data);     
}
                            
/*******************************************************************************
* Function Name  : aci_gatt_notification_event.
* Description    : This event is generated when a notification is received from the server
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_gatt_notification_event(uint16_t Connection_Handle,
                                 uint16_t Attribute_Handle,
                                 uint8_t Attribute_Value_Length,
                                 uint8_t Attribute_Value[])
{
  Master_Library_gatt_notification_event(Connection_Handle,
                                         Attribute_Handle,
                                         Attribute_Value_Length,
                                         Attribute_Value);
}

/*******************************************************************************
* Function Name  : aci_gatt_indication_event.
* Description    : This event is generated when a indication is received from the server
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_gatt_indication_event(uint16_t Connection_Handle,
                               uint16_t Attribute_Handle,
                               uint8_t Attribute_Value_Length,
                               uint8_t Attribute_Value[])
{
  Master_Library_gatt_indication_event( Connection_Handle,
                                       Attribute_Handle,
                                       Attribute_Value_Length,
                                       Attribute_Value);
}

/*******************************************************************************
* Function Name  : aci_gatt_attribute_modified_event.
* Description    : This event is generated to the application by the GATT server 
*                  when a client modifies any attribute on the server
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
/* BLE stack FW version >= 2.x*/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{
  Master_Library_gatt_attribute_modified_event( Connection_Handle,
                                               Attr_Handle,
                                               Offset,
                                               Attr_Data_Length,
                                               Attr_Data);
}

/*******************************************************************************
* Function Name  : aci_gatt_proc_complete_event.
* Description    : This event is generated when a GATT client procedure completes 
 *                 either with error or successfully
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_gatt_proc_complete_event(uint16_t Connection_Handle,
                                  uint8_t Error_Code)
{
  Master_Library_gatt_proc_complete_event(Connection_Handle,
                                          Error_Code);
}

/*******************************************************************************
* Function Name  : aci_att_read_resp_event.
* Description    : This event is generated in response to a Read Request
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_att_read_resp_event(uint16_t Connection_Handle,
                             uint8_t Event_Data_Length,
                             uint8_t Attribute_Value[])
{
  Master_Library_att_read_resp_event(Connection_Handle,
                                     Event_Data_Length,
                                     Attribute_Value);
}

/*******************************************************************************
* Function Name  : aci_att_read_blob_resp_event.
* Description    : This event is generated in response to a Read Request
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_att_read_blob_resp_event(uint16_t Connection_Handle,
                                  uint8_t Event_Data_Length,
                                  uint8_t Attribute_Value[])
{
  Master_Library_att_read_blob_resp_event(Connection_Handle,
                                          Event_Data_Length,
                                          Attribute_Value);
}

/*******************************************************************************
* Function Name  : aci_gap_pairing_complete_event.
* Description    : This event is generated when the pairing process has completed 
*                  successfully or a pairing procedure timeout has occurred or 
*                  the pairing has failed
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
/* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */
void aci_gap_pairing_complete_event(uint16_t Connection_Handle,
                                    uint8_t Status,
                                    uint8_t Reason)
{
  Master_Library_gap_pairing_complete_event(Connection_Handle,
                                            Status, 
                                            Reason);
  
}

/*******************************************************************************
* Function Name  : aci_gatt_error_resp_event.
* Description    : This event is generated when an Error Response is received from the server
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_gatt_error_resp_event(uint16_t Connection_Handle,
                               uint8_t Req_Opcode,
                               uint16_t Attribute_Handle,
                               uint8_t Error_Code)
{
  Master_Library_gatt_error_resp_event(Connection_Handle,
                                       Req_Opcode,
                                       Attribute_Handle,
                                       Error_Code);
}

/*******************************************************************************
* Function Name  : aci_gap_pass_key_req_event.
* Description    : This event is generated by the Security manager to the application 
*                  when a passkey is required for pairing
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_gap_pass_key_req_event(uint16_t Connection_Handle)
{
  Master_Library_gap_pass_key_req_event(Connection_Handle);
}

/*******************************************************************************
* Function Name  : aci_l2cap_connection_update_req_event.
* Description    : This event is given by the L2CAP layer when a connection update 
*                  request is received from the slave
* Input          : See file bluenrg1_events.h
* Output         : See file bluenrg1_events.h
* Return         : See file bluenrg1_events.h
*******************************************************************************/
void aci_l2cap_connection_update_req_event(uint16_t Connection_Handle,
                                           uint8_t Identifier,
                                           uint16_t L2CAP_Length,
                                           uint16_t Interval_Min,
                                           uint16_t Interval_Max,
                                           uint16_t Slave_Latency,
                                           uint16_t Timeout_Multiplier)
{
  Master_Library_l2cap_connection_update_req_event(Connection_Handle,
                                                   Identifier,
                                                   Interval_Min,
                                                   Interval_Max,
                                                   Slave_Latency,
                                                   Timeout_Multiplier);
}

void aci_gap_numeric_comparison_value_event(uint16_t Connection_Handle, uint32_t Numeric_Value)
{
  Master_Library_numeric_comparison_value_event(Connection_Handle, Numeric_Value); 
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

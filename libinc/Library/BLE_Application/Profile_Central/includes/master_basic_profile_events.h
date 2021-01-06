
/**
  ******************************************************************************
  * @file    master_basic_profile_events.h
  * @author  AMG - RF Application team
  * @version V1.2.0
  * @date    04-Decemberr-2016
  * @brief   Header file for BlueNRG-1 master/central library events.
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
#ifndef __MASTER_BASIC_PROFILE_EVENTS_H
#define __MASTER_BASIC_PROFILE_EVENTS_H


/******************************************************************************
* Function Declarations
******************************************************************************/
/**
 * Master_Library_encryption_change_event
 * 
 * @param[in] Status: BLE status related to the encryption change event 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[i] Encryption_Enabled: recyption enabled value
 *              
 * Master basic profile callback which handles actions
 * when a BLE encryption change event happens
 * 
 * @return None
 */ 
void Master_Library_encryption_change_event(uint8_t Status,
                                            uint16_t Connection_Handle,
                                            uint8_t Encryption_Enabled); 

/**
 * Master_Library_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Reason: Reason for disconnection.
 *              
 * Master basic profile callback which handles actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void Master_Library_disconnection_complete_event(uint8_t Status,
                                                 uint16_t Connection_Handle,
                                                 uint8_t Reason);

/**
 * Master_Library_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Peer_Address: Public Device Address or Random Device Address 
 * of the peer device
 *              
 * Master basic profile callback which handles actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */ 
void Master_Library_le_connection_complete_event(uint8_t Status,
                                                 uint16_t Connection_Handle,
                                                 uint8_t Peer_Address[6]);

/**
 * Master_Library_le_advertising_report_event
 * 
 * @param[in] Status: BLE status related to the advertising report event 
 * @param[in] Advertising_Report: advertising report data
 *              
 * Master basic profile callback which handles actions
 * when a BLE advertising report event happens
 * 
 * @return None
 */
void Master_Library_le_advertising_report_event(uint8_t Num_Reports,
                                                Advertising_Report_t Advertising_Report[]);

/**
 * Master_Library_gap_proc_complete_event
 * 
 * @param[in] Procedure_Code: code of terminated procedure 
 *              
 * Master basic profile callback which handles actions
 * when a BLE gap procedure complete event happens
 * 
 * @return None
 */
void Master_Library_gap_proc_complete_event(uint8_t Procedure_Code);

/**
 * Master_Library_gatt_read_permit_req_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Handle: handle of the attribute
 * @param[in] Offset: offset from which the read has been requested
 *              
 * Master basic profile callback which handles actions
 * when a BLE gatt read permit request  event happens
 * 
 * @return None
 */
void Master_Library_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                               uint16_t Attribute_Handle,
                                               uint16_t Offset);

/**
 * Master_Library_gatt_tx_pool_available_event
 * 
 * @param[in] Connection_Handle: Connection handle related to the request
 * @param[in] Available_Buffers: Number of buffers available
 *              
 * Master basic profile callback which handles actions
 * when BLE FW stack raises the error code
 * BLE_STATUS_INSUFFICIENT_RESOURCES (0x64)
 * 
 * @return None
 */      
void Master_Library_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                                 uint16_t Available_Buffers);

/**
 * Master_Library_gatt_write_permit_req_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Handle: handle of the attribute
 * @param[in] Data_Length: Length of Data field
 * @param[in] Data: The data that the client has requested to write
 *              
 * Master basic profile callback which handles actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void Master_Library_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                                uint16_t Attribute_Handle,
                                                uint8_t Data_Length,
                                                uint8_t Data[]);

/**
 * Master_Library_att_find_info_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Format: Format of the hanndle-uuid pairs
 * @param[in] Event_Data_Length: Length of Handle_UUID_Pair in octets
 * @param[in] Handle_UUID_Pair: A sequence of handle-uuid pairs. 
 *
 * Master basic profile callback which handles actions
 * when a BLE att find info response event happens
 * 
 * @return None
 */
void Master_Library_att_find_info_resp_event(uint16_t Connection_Handle,
                                             uint8_t Format,
                                             uint8_t Event_Data_Length,
                                             uint8_t Handle_UUID_Pair[]);

/**
 * Master_Library_att_read_by_group_type_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Data_Length: The size of each attribute data
 * @param[in] Data_Length: Length of Attribute_Data_List in octets
 * @param[in] Attribute_Data_List: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 *
 * Master basic profile callback which handles actions
 * when a BLE att read by group type response event happens
 * 
 * @return None
 */                                                
void Master_Library_att_read_by_group_type_resp_event(uint16_t Connection_Handle,
                                                      uint8_t Attribute_Data_Length,
                                                      uint8_t Data_Length,
                                                      uint8_t Attribute_Data_List[]);

/**
 * Master_Library_att_read_by_type_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Handle_Value_Pair_Length: The size of each attribute handle-value pair
 * @param[in] Data_Length: Length of Attribute_Data_List in octets
 * @param[in] Handle_Value_Pair_Data: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 *
 * Master basic profile callback which handles actions
 * when a BLE att read by type response event happens
 * 
 * @return None
 */  
void Master_Library_att_read_by_type_resp_event(uint16_t Connection_Handle,
                                                uint8_t Handle_Value_Pair_Length,
                                                uint8_t Data_Length,
                                                uint8_t Handle_Value_Pair_Data[]);

/**
 * Master_Library_gatt_notification_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Attribute_Value_Length: Length of Attribute_Value in octets
 * @param[in] Attribute_Value: The current value of the attribute
 *
 * Master basic profile callback which handles actions
 * when a BLE gatt notification event happens
 * 
 * @return None
 */ 
void Master_Library_gatt_notification_event(uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint8_t Attribute_Value_Length,
                                            uint8_t Attribute_Value[]);

/**
 * Master_Library_gatt_indication_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Attribute_Value_Length: Length of Attribute_Value in octets
 * @param[in] Attribute_Value: The current value of the attribute
 *
 * Master basic profile callback which handles actions
 * when a BLE gatt indication event happens
 * 
 * @return None
 */
void Master_Library_gatt_indication_event(uint16_t Connection_Handle,
                                          uint16_t Attribute_Handle,
                                          uint8_t Attribute_Value_Length,
                                          uint8_t Attribute_Value[]);

/**
 * Master_Library_gatt_attribute_modified_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices.
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Offset: Offset from which the write has been performed by the peer device.
 * @param[in] Attr_Data_Length: Length of Attr_Data in octets
 * @param[in] Attr_Data: The modified value
 *
 * Master basic profile callback which handles actions
 * when a BLE gatt attribute modified event happens
 * 
 * @return None
 */
void Master_Library_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                                  uint16_t Attr_Handle,
                                                  uint16_t Offset,
                                                  uint16_t Attr_Data_Length,
                                                  uint8_t Attr_Data[]);


/**
 * Master_Library_gatt_proc_complete_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Error_Code: Indicates whether the procedure completed with an 
 * error or was successful
 *              
 * Master basic profile callback which handles actions
 * when a BLE gatt procedure complete event happens
 * 
 * @return None
 */ 
void Master_Library_gatt_proc_complete_event(uint16_t Connection_Handle,
                                             uint8_t Error_Code);

/**
 * Master_Library_att_read_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Event_Data_Length: Length of following data
 * @param[in] Attribute_Value: The value of the attribute
 *              
 * Master basic profile callback which handles actions
 * when a BLE att read response event happens
 * 
 * @return None
 */ 
void Master_Library_att_read_resp_event(uint16_t Connection_Handle,
                                        uint8_t Event_Data_Length,
                                        uint8_t Attribute_Value[]);

/**
 * Master_Library_att_read_blob_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Event_Data_Length: Length of following data
 * @param[in] Attribute_Value: Part of the attribute value
 *              
 * Master basic profile callback which handles actions
 * when a BLE att read blob response event happens
 * 
 * @return None
 */ 
void Master_Library_att_read_blob_resp_event(uint16_t Connection_Handle,
                                             uint8_t Event_Data_Length,
                                             uint8_t Attribute_Value[]);

/**
 * Master_Library_gap_pairing_complete_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Status: Pairing status
 * @param[in] Reason: pairing status reason (succes or failure reason)
 *              
 * Master basic profile callback which handles actions
 * when a BLE gap pairing complete event happens
 * 
 * @return None
 */ 
void Master_Library_gap_pairing_complete_event(uint16_t Connection_Handle,
                                               uint8_t Status, 
                                               uint8_t Reason);

/**
 * Master_Library_numeric_comparison_value_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Numeric_Value: Numeric value to be confirmed or not 
 * 
 * Master basic profile callback which handles actions
 * when a BLE confirm numeri value yesno event happens
 * 
 * @return None
 */
void Master_Library_numeric_comparison_value_event(uint16_t Connection_Handle, 
                                                   uint32_t Numeric_Value);  

/**
 * Master_Library_gatt_error_resp_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Req_Opcode: The request that generated this error response
 * @param[in] Attribute_Handle: The attribute handle that generated this error response
 * @param[in] Error_Code: The reason why the request has generated an error response 
 *              
 * Master basic profile callback which handles actions
 * when a BLE gatt error response event happens
 * 
 * @return None
 */
void Master_Library_gatt_error_resp_event(uint16_t Connection_Handle,
                                          uint8_t Req_Opcode,
                                          uint16_t Attribute_Handle,
                                          uint8_t Error_Code);

/**
 * Master_Library_gap_pass_key_req_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 *              
 * Master basic profile callback which handles actions
 * when a BLE gap pass key request event happens
 * 
 * @return None
 */ 
void Master_Library_gap_pass_key_req_event(uint16_t Connection_Handle);

/**
 * Master_Library_l2cap_connection_update_req_event
 * 
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * @param[in] Identifier: This is the identifier which associate the request to the
 * response.
 * @param[in] Interval_Min: Minimum value for the connection event interval 
 * @param[in] Interval_Max: Maximum value for the connection event interval.
 * @param[in] Slave_Latency: Slave latency for the connection in number of connection events
 * @param[in] Timeout_Multiplier: Defines connection timeout parameter
 *              
 * Master basic profile callback which handles actions
 * when a BLE l2cap connection update request event happens
 * 
 * @return None
 */
void Master_Library_l2cap_connection_update_req_event(uint16_t Connection_Handle,
                                                      uint8_t Identifier,
                                                      uint16_t Interval_Min,
                                                      uint16_t Interval_Max,
                                                      uint16_t Slave_Latency,
                                                      uint16_t Timeout_Multiplier);   
                                                                                           

#endif /*__MASTER_BASIC_PROFILE_EVENTS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

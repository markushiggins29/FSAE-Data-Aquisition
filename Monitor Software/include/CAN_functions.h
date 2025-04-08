/************************************************************************
                              CAN Header File
                        ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                3/6/2025

This file contains the includes, constants, types,  and function prototypes 
necessary to run the CAN functions on the systems peripheral devices that
interact directly with the sensors in the system.   
*************************************************************************/
#ifndef CAN_H
#define CAN_H

  /************************************************************************
                                  INCLUDES
  *************************************************************************/  
  #include <Arduino.h>
  #include <esp32_can.h>

  /************************************************************************
                                 PROTOTYPES
  *************************************************************************/  
  void sendCanMessage(uint16_t message_ID, uint8_t message_DLC, uint8_t *p_outgoing_data);
  void readCanBus(CAN_FRAME *message, uint16_t *p_message_ID, uint8_t *p_message_DLC, uint8_t *p_incoming_data);

#endif

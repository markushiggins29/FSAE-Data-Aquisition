 #include "CAN_functions.h"
 
  /************************************************************************
                            SEND CAN MESSAGE
  *************************************************************************/
  void sendCanMessage(uint16_t message_ID, uint8_t message_DLC, uint8_t *p_outgoing_data)
  {
    CAN_FRAME txFrame;
    txFrame.rtr = 0;
    txFrame.id = message_ID;
    txFrame.extended = false;
    txFrame.length = message_DLC;
    for(uint8_t i = 0; i < message_DLC; i++)
    {
      txFrame.data.uint8[i] = *(p_outgoing_data+i) ;
    }
    CAN0.sendFrame(txFrame);
  }

  /************************************************************************
                            READ CAN BUS
  *************************************************************************/
  void readCanBus(CAN_FRAME *message, uint16_t *p_message_ID, uint8_t *p_message_DLC, uint8_t *p_incoming_data)
  {
    *p_message_ID = message->id;
    *p_message_DLC = message->length;
    for (int i = 0; i < message->length; i++) 
    {
      *(p_incoming_data+i) = message->data.byte[i];
    }
  }
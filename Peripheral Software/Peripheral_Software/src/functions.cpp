/************************************************************************
                                functions.c
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the functions called in the main program. These functions
abstract and modularize the software so that it is easily human readable.
*************************************************************************/
#include "main.h"

bool initializeCanBus(CANSAME5x *CAN_Read, CANSAME5x *CAN_Write)
{

    pinMode(PIN_CAN_STANDBY, OUTPUT);
    digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
    pinMode(PIN_CAN_BOOSTEN, OUTPUT);
    digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

    if (!(*CAN_Write).begin(CAN_BAUD_RATE)) 
    {
        return false; 
    }
    if (!(*CAN_Read).begin(CAN_BAUD_RATE)) 
    {
        return false;
    }    

    return true;
}

void readCanBus(CANSAME5x *CAN_Read, uint16_t *p_incoming_id, uint8_t *p_incoming_dlc, uint8_t *p_incoming_data )
{
    int incoming_packetSize = (*CAN_Read).parsePacket();

    // Error check to ensure dlc matches sizeof data

    if (incoming_packetSize)
    {
        *p_incoming_id  = (*CAN_Read).packetId();
        *p_incoming_dlc = (*CAN_Read).packetDlc();

        for(uint8_t i = 0; i < *p_incoming_dlc; i++)
        {
            *(p_incoming_data+i) = ( (*CAN_Read).read() & (BYTE_ONE<<(2*i)) );
        }
    }
}

void writeCanBus(CANSAME5x *CAN_Write, uint16_t outgoing_id, uint8_t outgoing_dlc, uint8_t *p_outgoing_data)
{

    // Error check to ensure dlc matches sizeof data

    (*CAN_Write).beginPacket( outgoing_id );

    for(uint8_t i = 0; i < outgoing_dlc; i++)
    {
        (*CAN_Write).write( *(p_outgoing_data+i) );
    }

    (*CAN_Write).endPacket();
}

void sendFaultMessage( uint8_t faultIdentifier, uint8_t faultCode, taskTimer *task, CANSAME5x *CAN_Write)
{
    if ( (faultCode > 0) )
    {
      if( (*task).b_timePassed() )
      {
        uint32_t currentTime = millis();
        uint8_t openFaultDataArray[TX_FAULT_DLC] = {faultIdentifier, faultCode, SEL_BYTE_ONE(currentTime), SEL_BYTE_TWO(currentTime), SEL_BYTE_THREE(currentTime), SEL_BYTE_FOUR(currentTime) };
        writeCanBus(CAN_Write, TX_SENSOR_FAULT_ID, TX_FAULT_DLC, openFaultDataArray);
      }
    }    
}

void sendSensorData( uint16_t peripheralIdentifier, uint8_t dataDLC, uint8_t sensorIdentifier, uint16_t sensorData, taskTimer *task, CANSAME5x *CAN_Write)
{
    if( (*task).b_timePassed() )
    {
        uint32_t currentTime = millis();
        uint8_t sensorDataArray[dataDLC] = {sensorIdentifier, SEL_BYTE_ONE(sensorData), SEL_BYTE_TWO(sensorData), SEL_BYTE_ONE(currentTime), SEL_BYTE_TWO(currentTime), SEL_BYTE_THREE(currentTime), SEL_BYTE_FOUR(currentTime)};
        writeCanBus(CAN_Write, peripheralIdentifier, dataDLC, sensorDataArray);
    }  
}

void updateState(t_STATE *state, uint16_t *p_incoming_id, uint8_t *p_incoming_data )
{
    if(*p_incoming_id == RX_STATE_ID)
    {
        switch( *p_incoming_data ) // Assuming for now that the first byte contains the state we want
        {
            case RX_STATE_IDLE:
            *state = IDLE;
            break;

            case RX_STATE_COLLECT:
            *state = COLLECT;
            break;
            
            case RX_STATE_FAULT:
            *state = FAULT;
            break;
        }
    }
}

void stateControl(CANSAME5x *CAN_Read,  t_STATE *state)
{
    uint16_t incoming_id;
    uint8_t  incoming_dlc;
    uint8_t  incoming_data;

    readCanBus( CAN_Read, &incoming_id, &incoming_dlc, &incoming_data );
    updateState(state, &incoming_id, &incoming_data ); 

}
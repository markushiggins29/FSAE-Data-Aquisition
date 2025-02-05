/************************************************************************
                                Functions
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

void readCanBus(CANSAME5x *CAN_Read, uint32_t *incoming_id, uint32_t *incoming_dlc, uint8_t *p_incoming_data )
{
    int incoming_packetSize = (*CAN_Read).parsePacket();

    if (incoming_packetSize)
    {
        *incoming_id  = (*CAN_Read).packetId();
        *incoming_dlc = (*CAN_Read).packetDlc();

        for(uint8_t i = 0; i < *incoming_dlc; i++)
        {
            *(p_incoming_data+i) = ( (*CAN_Read).read() & (BYTE_ONE<<(2*i)) );
        }
    }
}

void masterStateControl(t_STATE *state, uint32_t *p_incoming_id, uint8_t *p_incoming_data )
{
    if(*p_incoming_id == RX_STATE_ID)
    {
    switch( p_incoming_data[1] ) // Assuming for now that the first byte contains the state we want
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

void readSensors(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6)
{
    (*sns1).getRawValue( (*sns1).getPin() );
    (*sns2).getRawValue( (*sns2).getPin() );
    (*sns3).getRawValue( (*sns3).getPin() );
    (*sns4).getRawValue( (*sns4).getPin() );
    (*sns5).getRawValue( (*sns5).getPin() );
    (*sns6).getRawValue( (*sns6).getPin() );
}


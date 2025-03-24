/************************************************************************
                                  main.h
                        ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the includes, constants, types,  and function prototypes 
necessary to run the main program on the systems peripheral devices that
interact directly with the sensors in the system.   
*************************************************************************/

#ifndef MAIN_H
#define MAIN_H

    /************************************************************************
                                    INCLUDES
    *************************************************************************/
    #include <Arduino.h>
    #include <stdint.h>
    #include <stdbool.h>

    #include "CANController.h"
    #include "CANSAME5x_port.h"
    #include "CANSAME5x.h"

    #include "timer.h"
    #include "sensors.h"    

    /************************************************************************
                                    CONSTANTS
    *************************************************************************/
    #define SERIAL_RATE   9600
    #define CAN_BAUD_RATE 500000

    #define FAULT_LIGHT 7

    #define RX_STATE_ID       0x181
    #define RX_STATE_IDLE     0x01
    #define RX_STATE_COLLECT  0x02
    #define RX_STATE_FAULT    0x03

    #define TX_FAULT_DLC      0x6 // 1 byte for the fault code ID, 1 byte for the code itself, 4 bytes per timestamp
    #define TX_DATA_DLC       0x7 // 1 byte for the sensor ID, 2 bytes for the code itself, 4 bytes per timestamp
    #define TX_SHORT_FAULT    0x01
    #define TX_OPEN_FAULT     0x02

    // Sensor Message IDs are shifted up so that the lower bits (higher priority) are reserved
    // Which sensor data is being sent from is held in the first byte of the data field 
    // Each sensor will have a unique data id that way the master board knows from what peripheral
    // it is recieving data.
    #define PERIPHERAL_1

    #ifdef PERIPHERAL_1
        #define TX_SENSOR_DATA_ID   0x100 // Standard 11 bit identifier, 2048 possible IDs
        #define TX_SENSOR_FAULT_ID  0x010

    #elif defined(PERIPHERAL_2) 
        #define TX_SENSOR_DATA_ID   0x200
        #define TX_SENSOR_FAULT_ID  0x020

    #elif defined(PERIPHERAL_3) 
        #define TX_SENSOR_DATA_ID   0x300
        #define TX_SENSOR_FAULT_ID  0x030

    #elif defined(PERIPHERAL_4) 
        #define TX_SENSOR_DATA_ID   0x400
        #define TX_SENSOR_FAULT_ID  0x040

    #elif defined(PERIPHERAL_5) 
        #define TX_SENSOR_DATA_ID   0x500
        #define TX_SENSOR_FAULT_ID  0x050

    #elif defined(PERIPHERAL_6) 
        #define TX_SENSOR_DATA_ID   0x600 // 0b 110 0000 0000 MAX ID is 0x700 for standard CAN message with 11 bit identifier
        #define TX_SENSOR_FAULT_ID  0x060

    #endif

    /************************************************************************
                                      MASKS
    *************************************************************************/
    #define BYTE_ONE   (0x00000000000000FF)
    #define BYTE_TWO   (0x000000000000FF00)
    #define BYTE_THREE (0x0000000000FF0000)
    #define BYTE_FOUR  (0x00000000FF000000)
    #define BYTE_FIVE  (0x000000FF00000000)
    #define BYTE_SIX   (0x0000FF0000000000)
    #define BYTE_SEVEN (0x00FF000000000000)
    #define BYTE_EIGHT (0xFF00000000000000)

    #define SEL_BYTE_ONE(variable)   ( (uint8_t)((variable & BYTE_ONE) >> 0) )
    #define SEL_BYTE_TWO(variable)   ( (uint8_t)((variable & BYTE_TWO) >> 8) )
    #define SEL_BYTE_THREE(variable) ( (uint8_t)((variable & BYTE_THREE) >> 16) )
    #define SEL_BYTE_FOUR(variable)  ( (uint8_t)((variable & BYTE_FOUR) >> 24) )
    #define SEL_BYTE_FIVE(variable)  ( (uint8_t)((variable & BYTE_FIVE) >> 32) )
    #define SEL_BYTE_SIX(variable)   ( (uint8_t)((variable & BYTE_SIX) >> 40) )
    #define SEL_BYTE_SEVEN(variable) ( (uint8_t)((variable & BYTE_SEVEN) >> 48) )
    #define SEL_BYTE_EIGHT(variable) ( (uint8_t)((variable & BYTE_EIGHT) >> 56) )

    /************************************************************************
                                      TYPES
    *************************************************************************/
    typedef enum { INIT, IDLE, COLLECT , FAULT } t_STATE; 

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    bool initializeCanBus(CANSAME5x *CAN_Read, CANSAME5x *CAN_Write);
    void readCanBus(CANSAME5x *CAN_Read, uint16_t *p_incoming_id, uint8_t *incoming_dlc, uint8_t *p_incoming_data );
    void writeCanBus(CANSAME5x *CAN_Write, uint16_t outgoing_id, uint8_t outgoing_dlc, uint8_t *p_outgoing_data);
    void sendFaultMessage( uint8_t faultIdentifier, uint8_t faultCode, taskTimer *task, CANSAME5x *CAN_Write);
    void sendSensorData( uint16_t peripheralIdentifier, uint8_t dataDLC, uint8_t sensorIdentifier, uint16_t sensorData, taskTimer *task, CANSAME5x *CAN_Write);
    void updateState(t_STATE *state, uint16_t *p_incoming_id, uint8_t *p_incoming_data );
    void stateControl(CANSAME5x *CAN_Read,  t_STATE *state);

#endif 
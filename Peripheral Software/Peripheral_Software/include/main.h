/************************************************************************
                        Main Program Header File
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

    /************************************************************************
                                    CONSTANTS
    *************************************************************************/

    #define SERIAL_RATE 9600
    #define CAN_BAUD_RATE 500000

    #define SENSOR_1 1
    #define SENSOR_2 2
    #define SENSOR_3 3
    #define SENSOR_4 4
    #define SENSOR_5 5
    #define SENSOR_6 6

    #define FAULT_LIGHT 7

    #define RX_STATE_ID      0x181
    #define RX_STATE_IDLE    0x1
    #define RX_STATE_COLLECT 0x2
    #define RX_STATE_FAULT   0x3

    // Sensor Message IDs are shifted up so that the lower bits (higher priority) are reserved
    // Which sensor data is being sent from is held in the first byte of the data field 
    // Each sensor will have a unique data id that way the master board knows from what peripheral
    // it is recieving data.

    #ifdef PERIPHERAL_1
        #define TX_SENSOR_DATA_ID   0x010

    #elif defined(PERIPHERAL_2) 
        #define TX_SENSOR_DATA_ID   0x020

    #elif defined(PERIPHERAL_3) 
        #define TX_SENSOR_DATA_ID   0x030

    #elif defined(PERIPHERAL_4) 
        #define TX_SENSOR_DATA_ID   0x040

    #elif defined(PERIPHERAL_5) 
        #define TX_SENSOR_DATA_ID   0x050

    #elif defined(PERIPHERAL_6) 
        #define TX_SENSOR_DATA_ID   0x060
                    
    #endif

    /************************************************************************
                                      MASKS
    *************************************************************************/

    #define BYTE_ONE   0x00000000000000FF
    #define BYTE_TWO   0x000000000000FF00
    #define BYTE_THREE 0x0000000000FF0000
    #define BYTE_FOUR  0x00000000FF000000
    #define BYTE_FIVE  0x000000FF00000000
    #define BYTE_SIX   0x0000FF0000000000
    #define BYTE_SEVEN 0x00FF000000000000
    #define BYTE_EIGHT 0xFF00000000000000

    /************************************************************************
                                      TYPES
    *************************************************************************/

    typedef enum { INIT, IDLE, COLLECT , FAULT } t_STATE; 

    /************************************************************************
                                     CLASSES
    *************************************************************************/

    class sensor
    {
        public:

            sensor(int pinNumber)
            {
                pinMode(pinNumber, INPUT); 
                pin = pinNumber;
            }

            void getRawValue(uint8_t pinNumber);
            uint8_t getPin();

        private:

            uint8_t  pin;
            uint16_t rawValue;

    };

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    bool initializeCanBus(CANSAME5x *CAN_Read, CANSAME5x *CAN_Write);
    void readCanBus(CANSAME5x *CAN_Read, uint32_t *p_incoming_id, uint32_t *incoming_dlc, uint8_t *p_incoming_data );
    void masterStateControl(t_STATE *state, uint32_t *incoming_id, uint8_t *p_incoming_data );
    void readSensors(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6);

#endif 
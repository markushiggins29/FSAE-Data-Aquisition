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

    #define STATE_ID      0x181
    #define STATE_IDLE    0x1
    #define STATE_COLLECT 0x2
    #define STATE_FAULT   0x3

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

            uint16_t getRawValue(uint8_t pinNumber);

        private:

            uint8_t pin;

    };

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    void configureSensors();

#endif 
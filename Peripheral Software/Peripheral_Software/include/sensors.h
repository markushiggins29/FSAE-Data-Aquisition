/************************************************************************
                                sensors.h
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the sensor related functions prototypes and macros
*************************************************************************/
#ifndef SENSORS_H
#define SENSORS_H

    /************************************************************************
                                    INCLUDES
    *************************************************************************/
    #include "main.h"

    /************************************************************************
                                    CONSTANTS
    *************************************************************************/
    #define SENSOR_1 A0
    #define SENSOR_2 A1
    #define SENSOR_3 A2
    #define SENSOR_4 A3
    #define SENSOR_5 A4
    #define SENSOR_6 A5

    #define SENSOR_SHORT 1000 // UPDATE ME BASED ON ADC VALUES
    #define SENSOR_OPEN  100 

    /************************************************************************
                                      MASKS
    *************************************************************************/
    #define BIT_ZERO  (1<<0)
    #define BIT_ONE   (1<<1)
    #define BIT_TWO   (1<<2)
    #define BIT_THREE (1<<3)
    #define BIT_FOUR  (1<<4)
    #define BIT_FIVE  (1<<5)
    #define BIT_SIX   (1<<6)
    #define BIT_SEVEN (1<<7)


    /************************************************************************
                                CLASSES & STRUCTURES
    *************************************************************************/
    typedef struct
    {
        uint8_t opens; 
        uint8_t shorts; 
    }sensorFault;

    class sensor
    {
        public:

            sensor(int pinNumber)
            {
                pinMode(pinNumber, INPUT); 
                pin = pinNumber;
            }

            void updateRawValue(int pinNumber);
            int getPin();
            uint16_t  getRawValue();

        private:

            int  pin;
            uint16_t rawValue;

    };

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    void    readSensors(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6);
    uint8_t checkSensorShort(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6);
    uint8_t checkSensorOpen(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6);
    void    checkSensorFaults(sensorFault * sensorCodes, sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6);

#endif
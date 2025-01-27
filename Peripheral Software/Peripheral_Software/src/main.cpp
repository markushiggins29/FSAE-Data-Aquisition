/************************************************************************
                               Main Program
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file runs the main state machine that governs the behavior of the
systems peripheral board. The states can be broken down as follows:

  INIT    - 

              Initialize ports & communication busses
              Check for OOR sensor values

  IDLE    - 

              Check for COLLECT message from main board
              Check for OOR sensor values

  COLLECT - 

              Analog sensors are sampled
              Data is timestamped
              Data is broadcasted
              Check for IDLE message from main board
              Check for OOR sensor values

  FAULT   -

              Broadcast fault message to main board
              Fault LED On
              Collection is stopped

*************************************************************************/

#include "main.h"

t_STATE state; 
CANSAME5x CAN;

void setup() 
{
  state = INIT;
}

void loop()
{

  switch(state)
  {

    case INIT:

        Serial.begin(SERIAL_RATE);

        if (!CAN.begin(500000)) 
        {
          // Add custom error handling
          state = FAULT; 
        }

        // Driver-Side
        sensor sns_dr_shock(SENSOR_1);
        sensor sns_dr_brake_temp(SENSOR_2);
        sensor sns_dr_wheel_speed(SENSOR_3); 

        // Passenger-Side
        sensor sns_ps_shock(SENSOR_4);
        sensor sns_ps_brake_temp(SENSOR_5);
        sensor sns_ps_wheel_speed(SENSOR_6); 

        state = IDLE;

    break;



    case IDLE:

    break;



    case COLLECT: 

    break;



    case FAULT: 

    break;

  }

}


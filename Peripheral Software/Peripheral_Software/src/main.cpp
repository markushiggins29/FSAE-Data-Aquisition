/************************************************************************
                                 main.cpp
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
#include "sensors.h"
#include "timer.h"

t_STATE state; 

void setup() 
{
  state = INIT;
}

void loop()
{

  /************************************************************************
                                  Variables
  *************************************************************************/

  static CANSAME5x CAN_Write;
  static CANSAME5x CAN_Read;

  // static uint16_t *p_incoming_id      = new uint16_t;
  // static uint8_t *p_incoming_dlc      = new uint8_t;
  // static uint8_t   p_incoming_data[8];

  static sensor sns1(SENSOR_1); 
  static sensor sns2(SENSOR_2);
  static sensor sns3(SENSOR_3); 
  static sensor sns4(SENSOR_4);
  static sensor sns5(SENSOR_5);
  static sensor sns6(SENSOR_6); 

  static sensorFault sensorCodes;

  static taskTimer task_sendOpenFaultCode(1000);
  static taskTimer task_sendShortFaultCode(1000); 
  static taskTimer task_sendData(100); 

  /************************************************************************
                                State Machine
  *************************************************************************/  

  switch(state)
  {

    case INIT:
      Serial.println("Current State: INIT");

      pinMode(FAULT_LIGHT, OUTPUT);
      Serial.begin(SERIAL_RATE);
      if(!initializeCanBus(&CAN_Read, &CAN_Write))
      {
        state = FAULT;
      }

      state = IDLE;

    break;


    case IDLE:
      Serial.println("Current State: IDLE");

    break;


    case COLLECT: 
      Serial.println("Current State: COLLECT");

      readSensors(&sns1, &sns2, &sns3, &sns4, &sns5, &sns6);
      sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns1.getPin(), sns1.getRawValue(), &task_sendData, &CAN_Write);    

    break;


    case FAULT: 
      Serial.println("Current State: FAULT");

      digitalWrite(FAULT_LIGHT, HIGH);

    break;

  }

  /************************************************************************
                              Default Behavior
  *************************************************************************/
  stateControl(&CAN_Read,  &state);

  checkSensorFaults(&sensorCodes, &sns1, &sns2, &sns3, &sns4, &sns5, &sns6);
  sendFaultMessage( TX_OPEN_FAULT, sensorCodes.opens, &task_sendOpenFaultCode, &CAN_Write);
  sendFaultMessage( TX_SHORT_FAULT, sensorCodes.shorts, &task_sendShortFaultCode, &CAN_Write);

}
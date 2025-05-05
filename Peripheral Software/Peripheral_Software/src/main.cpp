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

  static sensor sns1(SENSOR_1); 
  static sensor sns2(SENSOR_2);
  static sensor sns3(SENSOR_3); 
  static sensor sns4(SENSOR_4);
  static sensor sns5(SENSOR_5);
  static sensor sns6(SENSOR_6); 

  static sensorFault sensorCodes;
  static uint8_t connectedSensors;

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

      pinMode(SENSOR_1_CONNECTED, INPUT);
      pinMode(SENSOR_2_CONNECTED, INPUT);
      pinMode(SENSOR_3_CONNECTED, INPUT);
      pinMode(SENSOR_4_CONNECTED, INPUT);
      pinMode(SENSOR_5_CONNECTED, INPUT);
      pinMode(SENSOR_6_CONNECTED, INPUT);

      state = IDLE;

    break;


    case IDLE:
      Serial.println("Current State: IDLE");

    break;


    case COLLECT: 
       Serial.println("Current State: COLLECT");

      readSensors(&sns1, &sns2, &sns3, &sns4, &sns5, &sns6);
      connectedSensors = 0;
      connectedSensors = checkSensorConnected();
      if(connectedSensors & BIT_ZERO)
      {
        Serial.println("Sensor 1");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns1.getPin(), sns1.getRawValue(), &task_sendData, &CAN_Write); 
      }
      if(connectedSensors & BIT_ONE)
      {
        Serial.println("Sensor 2");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns2.getPin(), sns2.getRawValue(), &task_sendData, &CAN_Write); 
      }
      if(connectedSensors & BIT_TWO)
      {
        Serial.println("Sensor 3");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns3.getPin(), sns3.getRawValue(), &task_sendData, &CAN_Write); 
      }
      if(connectedSensors & BIT_THREE)
      {
       Serial.println("Sensor 4");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns4.getPin(), sns4.getRawValue(), &task_sendData, &CAN_Write); 
      }
      if(connectedSensors & BIT_FOUR)
      {
        Serial.println("Sensor 5");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns5.getPin(), sns5.getRawValue(), &task_sendData, &CAN_Write); 
      }
      if(connectedSensors & BIT_FIVE)
      {
        Serial.println("Sensor 6");
        sendSensorData( TX_SENSOR_DATA_ID, TX_DATA_DLC, sns6.getPin(), sns6.getRawValue(), &task_sendData, &CAN_Write); 
      }

    break;


    case FAULT: 
      Serial.println("Current State: FAULT");

      digitalWrite(FAULT_LIGHT, HIGH);

    break;

  }

  /************************************************************************
                              Default Behavior
  *************************************************************************/
  stateControl(&CAN_Read, &state);

}








  /*************************************************************************/
  // NEEDS POLISHING
  /*************************************************************************/
  // checkSensorFaults(&sensorCodes, &sns1, &sns2, &sns3, &sns4, &sns5, &sns6);
  // sendFaultMessage( TX_OPEN_FAULT, sensorCodes.opens, &task_sendOpenFaultCode, &CAN_Write);
  // sendFaultMessage( TX_SHORT_FAULT, sensorCodes.shorts, &task_sendShortFaultCode, &CAN_Write);
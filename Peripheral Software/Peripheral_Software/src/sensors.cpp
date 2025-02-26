/************************************************************************
                               sensors.cpp
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the sensor related functions, most of which are class   
methods. They are called in the main program and adjacent main program 
functions, and are ran on the peripheral board.
*************************************************************************/
#include "sensors.h"

void sensor::updateRawValue(uint8_t pinNumber)
{
  rawValue = analogRead(pinNumber);
}

uint8_t sensor::getPin()
{
  return pin;
}

uint8_t sensor::getRawValue()
{
  return rawValue;
}

void readSensors(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6)
{
    (*sns1).updateRawValue( (*sns1).getPin() );
    (*sns2).updateRawValue( (*sns2).getPin() );
    (*sns3).updateRawValue( (*sns3).getPin() );
    (*sns4).updateRawValue( (*sns4).getPin() );
    (*sns5).updateRawValue( (*sns5).getPin() );
    (*sns6).updateRawValue( (*sns6).getPin() );
}

uint8_t checkSensorShort(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6)
{
  uint8_t shortedSensor = 0;

  if( (*sns1).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<0;
  }
  if( (*sns2).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<1;
  }
  if( (*sns3).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<2;
  }
  if( (*sns4).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<3;
  }
  if( (*sns5).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<4;
  }
  if( (*sns6).getRawValue() > SENSOR_SHORT )
  {
    shortedSensor |= 1<<5;
  }   
  
  return shortedSensor;
}

uint8_t checkSensorOpen(sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6)
{
  uint8_t openSensor = 0;

  if( (*sns1).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_ZERO;
  }
  if( (*sns2).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_ONE;
  }
  if( (*sns3).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_TWO;
  }
  if( (*sns4).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_THREE;
  }
  if( (*sns5).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_FOUR;
  }
  if( (*sns6).getRawValue() < SENSOR_OPEN )
  {
    openSensor |= BIT_FIVE;
  }   
  
  return openSensor;  
}

void checkSensorFaults(sensorFault * sensorCodes, sensor * sns1, sensor * sns2, sensor * sns3, sensor * sns4, sensor * sns5, sensor * sns6)
{
  sensorCodes->shorts = checkSensorShort(sns1, sns2, sns3, sns4, sns5, sns6);
  sensorCodes->opens  = checkSensorOpen(sns1, sns2, sns3, sns4, sns5, sns6);

  //Serial.println(sensorCodes->shorts);
  //Serial.println(sensorCodes->opens);

}
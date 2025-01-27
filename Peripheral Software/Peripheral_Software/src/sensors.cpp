/************************************************************************
                                Functions
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the functions called in the main program ran on the 
peripheral board.
*************************************************************************/

#include "main.h"

void configureSensors()
{
  // Driver-Side
  sensor sns_dr_shock(SENSOR_1); // maybe we should arbitrate this to just sensor 1, 2, 3 ... still trying to envision whats the most modular
  sensor sns_dr_brake_temp(SENSOR_2);
  sensor sns_dr_wheel_speed(SENSOR_3); 

  // Passenger-Side
  sensor sns_ps_shock(SENSOR_4);
  sensor sns_ps_brake_temp(SENSOR_5);
  sensor sns_ps_wheel_speed(SENSOR_6); 
}

uint16_t sensor::getRawValue(uint8_t pinNumber)
{
  return analogRead(pinNumber);
}
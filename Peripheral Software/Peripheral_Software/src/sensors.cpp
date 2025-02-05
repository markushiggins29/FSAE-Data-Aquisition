/************************************************************************
                                Sensors
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the sensor related functions, most of which are class   
methods. They are called in the main program and adjacent main program 
functions, and are ran on the peripheral board.
*************************************************************************/
#include "main.h"

void sensor::getRawValue(uint8_t pinNumber)
{
  rawValue = analogRead(pinNumber);
}

uint8_t sensor::getPin()
{
  return pin;
}
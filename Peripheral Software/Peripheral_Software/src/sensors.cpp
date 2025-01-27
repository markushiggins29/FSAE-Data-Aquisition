/************************************************************************
                                Functions
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the functions called in the main program ran on the 
peripheral board.
*************************************************************************/

#include "main.h"

uint32_t sensor::getRawValue(uint8_t pinNumber)
{
  return analogRead(pinNumber);
}
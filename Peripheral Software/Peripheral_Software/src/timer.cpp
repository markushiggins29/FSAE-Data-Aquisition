/************************************************************************
                               timer.cpp
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                2/25/2025

This file contains the timer related functions, most of which are class   
methods. They are called in the main program and adjacent main program 
functions, and are ran on the peripheral board.
*************************************************************************/

#include "timer.h"

bool taskTimer::b_timePassed()
{
    if( millis() >= (previousTime + scheduledDelay) )
    {
        previousTime = millis();
        return true;
    }
    else
    {
        return false;
    }

}
/************************************************************************
                                timer.h
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                2/25/2025

This file contains the timer related functions prototypes and macros
*************************************************************************/

#ifndef TIMER_H
#define TIMER_H

    /************************************************************************
                                    INCLUDES
    *************************************************************************/
    #include <Arduino.h>
    #include <stdint.h>
    #include <stdbool.h>

    /************************************************************************
                                    CONSTANTS
    *************************************************************************/


    /************************************************************************
                                CLASSES & STRUCTURES
    *************************************************************************/
    class taskTimer
    {
        public:
            taskTimer( uint32_t input_scheduledDelay)
            {
                scheduledDelay  = input_scheduledDelay;
                previousTime = millis();
            }

            bool b_timePassed();    

        private:  
            uint32_t previousTime; 
            uint32_t scheduledDelay;

    };

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/


#endif
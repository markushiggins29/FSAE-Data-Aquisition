/************************************************************************
                               Main Program
                      ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file runs the main state machine that governs the behavior of the
systems monitor board. The states can be broken down as follows:

  INIT    - 

              Initialize ports & communication busses

  IDLE    - 
              Broadcast a COLLECT message to peripheral board

  COLLECT - 
              Parse for Message ID's for local storage and wireless transmission
              Listen for any FAULT messages 
  FAULT   -   
              Stop data collection 
              Log faulted sensor

              
*************************************************************************/

#include "main.h"
#include "CAN_functions.h"
#include "SD_functions.h"

/************************************************************************
                                    SETUP
*************************************************************************/
void setup() 
{   
  
  Serial.begin(115200);                                                                   //Start the serial communication 
  delay(1000);  
  initializeLittleFS();                                                                   //Initialize filesystem    
  Serial.println("Got past FS");      
  
  initializeWifi();                                                                       //Initialize Wifi
  Serial.println("Got past Wifi"); 
  initializeWebSocket();                                                                  //Initiazlie WebSocket
  Serial.println("Got past WebSocket"); 
  //ServerBegin();                                                                          //Initial page serving 
  Serial.println("Got past Server Begin");
    
  Serial.print("setup complete");
}

/************************************************************************
                                    MAIN LOOP
*************************************************************************/


void loop() 
{        
  delay(1000);
  
  updateClientWebpage(getSensorReadings()); 
  ws.cleanupClients();

}

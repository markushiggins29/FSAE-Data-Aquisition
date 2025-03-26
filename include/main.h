/************************************************************************
                        Main Program Header File
                        ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                1/26/2025

This file contains the includes, constants, types,  and function prototypes 
necessary to run the main program on the systems peripheral devices that
interact directly with the sensors in the system.   
*************************************************************************/
#ifndef MAIN_H
#define MAIN_H

    /************************************************************************
                                    INCLUDES
    *************************************************************************/  
    #include <Arduino.h>                                                        // Specify framework 
    //#include <CAN.h>                                                            // Can Library for sending and receiving data using a CAN bus 
    #include <Wifi.h>                                                           // Wifi library
    #include <AsyncTCP.h>                                                       // Library for asynch TCP 
    #include <ESPAsyncWebServer.h>                                              // Library for web server functionality 
    #include "LittleFS.h"                                                      // Filesystem for handling files  
    #include <Arduino_JSON.h>                                                   // Library for JSON variabe/functions 
    #include <ctime>
    #include <cstdlib>
    #include <iostream>


     /************************************************************************
                                    CONSTANTS
    *************************************************************************/
    #define ESP32_TX_GPIO_NUM 1                                                 // Define TX GPIO pin  
    #define ESP32_RX_GPIO_NUM 3                                                 // Define RX GPIO pin
    //#define CONFIG_TASK_WDT_TIMEOUT_S 5

    extern const char* ssid;                                                          // Configure Name of the network
    extern const char* password;                                                      // Password of the network, leave blank quotes if no pw (must be at least 8 characters)
    
   /************************************************************************
                                    CLASS INSTANCES
    *************************************************************************/
    extern JSONVar sensorReadings;                                                     // JSON variable to store the sensor readings into 
    extern AsyncWebServer server;                                                // Creates an instance (object) of the AsyncWebServer class on port 80, port 80 is the default port used for HTTP handshake
    extern AsyncWebSocket ws;                                                   // Creates an instance of the AsyncWebSocket, "/ws" is the path on the URL link. Note: WebSocket URL format ws://<host>:<port><path>

    
    /************************************************************************
                                    VARIABLES
    *************************************************************************/
    extern int startCollection; 


    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    
    void initializeWifi();
    void WebSocketResponse(void *pArg, uint8_t *pData, size_t len);
    void WebSocketEventHandling(AsyncWebSocket *pSocket, AsyncWebSocketClient *pClient, AwsEventType type, void *pArg, uint8_t *pData, size_t len);
    void initializeWebSocket();
    void initializeLittleFS();
    String getSensorReadings();
    void updateClientWebpage(String sensorReadings);
    void ServerBegin();
    void StartReading();

#endif
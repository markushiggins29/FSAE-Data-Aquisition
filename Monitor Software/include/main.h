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
    #include <WiFi.h>                                                           // Wifi library
    #include <AsyncTCP.h>                                                       // Library for asynch TCP 
    #include <ESPAsyncWebServer.h>                                              // Library for web server functionality 
    #include "LittleFS.h"                                                       // Filesystem for handling files  
    #include <Arduino_JSON.h>                                                   // Library for JSON variabe/functions 
    #include <ctime>
    #include <cstdlib>
    #include <iostream>


     /************************************************************************
                                    CONSTANTS
    *************************************************************************/
    #define ESP32_TX_GPIO_NUM 1                                                 // Define TX GPIO pin  
    #define ESP32_RX_GPIO_NUM 3                                                 // Define RX GPIO pin

    #define SERIAL_RATE 9600

    #define TX_STATE_ID       0x181
    #define TX_STATE_DLC      0x1
    #define TX_STATE_IDLE     0x01
    #define TX_STATE_COLLECT  0x02
    #define TX_STATE_FAULT    0x03

    #define RX_QUEUE_SIZE     10

    extern const char* ssid;                                                          // Configure Name of the network
    extern const char* password;                                                      // Password of the network, leave blank quotes if no pw (must be at least 8 characters)
    
   /************************************************************************
                                    CLASS INSTANCES
    *************************************************************************/
    extern JSONVar sensorReadings;                                                     // JSON variable to store the sensor readings into 
    extern AsyncWebServer server;                                                // Creates an instance (object) of the AsyncWebServer class on port 80, port 80 is the default port used for HTTP handshake
    extern AsyncWebSocket ws;                                                   // Creates an instance of the AsyncWebSocket, "/ws" is the path on the URL link. Note: WebSocket URL format ws://<host>:<port><path>

    /************************************************************************
                                      MASKS
    *************************************************************************/
    #define BYTE_ONE   (0x00000000000000FF)
    #define BYTE_TWO   (0x000000000000FF00)
    #define BYTE_THREE (0x0000000000FF0000)
    #define BYTE_FOUR  (0x00000000FF000000)
    #define BYTE_FIVE  (0x000000FF00000000)
    #define BYTE_SIX   (0x0000FF0000000000)
    #define BYTE_SEVEN (0x00FF000000000000)
    #define BYTE_EIGHT (0xFF00000000000000)

    #define SEL_BYTE_ONE(variable)   ( (uint8_t)((variable & BYTE_ONE) >> 0) )
    #define SEL_BYTE_TWO(variable)   ( (uint8_t)((variable & BYTE_TWO) >> 8) )
    #define SEL_BYTE_THREE(variable) ( (uint8_t)((variable & BYTE_THREE) >> 16) )
    #define SEL_BYTE_FOUR(variable)  ( (uint8_t)((variable & BYTE_FOUR) >> 24) )
    #define SEL_BYTE_FIVE(variable)  ( (uint8_t)((variable & BYTE_FIVE) >> 32) )
    #define SEL_BYTE_SIX(variable)   ( (uint8_t)((variable & BYTE_SIX) >> 40) )
    #define SEL_BYTE_SEVEN(variable) ( (uint8_t)((variable & BYTE_SEVEN) >> 48) )
    #define SEL_BYTE_EIGHT(variable) ( (uint8_t)((variable & BYTE_EIGHT) >> 56) )
    
    /************************************************************************
                                      TYPES
    *************************************************************************/
    typedef enum {INIT, IDLE, COLLECT, FAULT}t_state;
    
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
    void initializeCAN();
    void initializeLittleFS();
    String getSensorReadings();
    void updateClientWebpage(String sensorReadings);
    void ServerBegin();
    bool startCollectingStateControl();

#endif

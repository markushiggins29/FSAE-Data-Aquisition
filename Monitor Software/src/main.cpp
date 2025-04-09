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

t_state state;

/************************************************************************
                                    SETUP
*************************************************************************/
void setup() 
{
  Serial.begin(SERIAL_RATE); 

  state = INIT;
}

/************************************************************************
                                    MAIN LOOP
*************************************************************************/
void loop() 
{        
  static uint16_t incomingMessageID;
  static uint8_t  incomingMessageDLC;
  static uint8_t  incomingData[8];
  static uint8_t  sensorID;
  static uint16_t sensorReading;
  static uint32_t timestamp;
  static float    scaledLengthReadingIn;
  static char     scaledLengthReadingIn_txt[64];
  static char     fileName[64];
  static char     sensorNumber[4]; 

  switch(state)
  {
    case INIT:
      Serial.println("State: INIT");

      SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);   
      if (!SD.begin(SPI_CS)) 
      {
        Serial.println("Card Mount Failed");
        state = FAULT;
      }
      CAN0.setCANPins(GPIO_NUM_35, GPIO_NUM_32);
      CAN0.begin(500000);
      CAN0.watchFor();

      delay(1000);  

      // Web Server & Wifi Initializations
      initializeLittleFS(); 
      initializeWifi();
      initializeWebSocket();
      ServerBegin();    

      state = IDLE;
    break;

    case IDLE:
      Serial.println("State: IDLE");
      if( /*WebSocketResponse(void *pArg,uint8_t *pData, size_t len)*/ startCollectingStateControl())
      {
        uint8_t collect_message = TX_STATE_COLLECT;
        delay(100);
        sendCanMessage(TX_STATE_ID, TX_STATE_DLC, &collect_message);

        state = COLLECT;
      }
    break;

    case COLLECT:
    {
      //Serial.println("State: COLLECT");
      CAN_FRAME message;
      if(CAN0.read(message))
      {
        readCanBus(&message, &incomingMessageID, &incomingMessageDLC, incomingData);
        // All Data is timestamped with a 4 byte timestamp
        // Every message follows the structure:Byte 0 - Sensor ID, Byte 1:2 - Sensor Data, Byte 3:6 - Timestamp
        sensorID = incomingData[0];
        itoa(sensorID, sensorNumber, 10);
        sensorReading  = 0;
        sensorReading |= incomingData[1];
        sensorReading |= incomingData[2]<<8;

        for(uint8_t i=2; i < 6; i++)
        {
          timestamp |= (incomingData[i+1] << (i*8) );
        }

        // If message is a shock travel length
        scaledLengthReadingIn = (((float)sensorReading/(float)1024)*(11.19-(float)8))+(float)8;
        // itoa(scaledLengthReadingIn, scaledLengthReadingIn_txt, 10);
        dtostrf(scaledLengthReadingIn, 6, 2, scaledLengthReadingIn_txt);
        strcat(scaledLengthReadingIn_txt,",");

        // Create file name based on message ID and the first byte
        switch(incomingMessageID)
        {
          case 0x100:
            strcpy(fileName, "/Peripheral_1_Sensor_");
            strcat(fileName,sensorNumber);
          break; 
          case 0x200:
            strcpy(fileName, "/Peripheral_2_Sensor_");
            strcat(fileName, sensorNumber);
          break;
          case 0x300:
            strcpy(fileName, "/Peripheral_3_Sensor_");
            strcat(fileName, sensorNumber);
          break;
          case 0x400:
            strcpy(fileName, "/Peripheral_4_Sensor_");
            strcat(fileName, sensorNumber);
          break;
          case 0x500:
            strcpy(fileName, "/Peripheral_5_Sensor_");
            strcat(fileName, sensorNumber);
          break;
          case 0x600:
            strcpy(fileName, "/Peripheral_6_Sensor_");
            strcat(fileName, sensorNumber);
          break;
          default:
          break;
        }

        writeFile(SD, fileName, scaledLengthReadingIn_txt);        
      }    

      //Create string of sensor readings from acc and gyro, needs shock data + timestamp added to it in getSensorReadings function
      String sensorReadings = getSensorReadings(scaledLengthReadingIn);
      updateClientWebpage(sensorReadings); 

    }
    break;  


    case FAULT:
      Serial.println("State: FAULT");
    break;
  }

  /************************************************************************
                              Default Behavior
  *************************************************************************/  
  ws.cleanupClients();
  delay(100);

}
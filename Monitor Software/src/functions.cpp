#include "main.h"

  const char* ssid = "ESP32";                                         // Configure Name of the network
  const char* password = "password";  
  AsyncWebServer server(80);                                                  // Creates an instance (object) of the AsyncWebServer class on port 80, port 80 is the default port used for HTTP handshake
  AsyncWebSocket ws("/ws"); 
  JSONVar sensorReadings; 
   

  /************************************************************************
                              WIFI INIT
  *************************************************************************/
  void initializeWifi() 
  {
    WiFi.mode(WIFI_AP);                                                       // Set the wifi mode as an access point
    Serial.println("Connecting to Wifi..");          
    bool status = WiFi.softAP(ssid,password,1);                               // Returns either 1 or 0 for connection verification
    
    Serial.printf("status: %i",status);
  
    //if (status){
      Serial.println("Connected to WiFi ");                             
      Serial.println(WiFi.softAPIP());                                        //Print the IP address
    //}
    /*
    else while(!status){
      Serial.println("Connecting to WiFi ");
      Serial.print(".");
      delay(1000);
    }
      
    else{
      Serial.print("Error");
    }
    */
  }

  /************************************************************************
                          FILESYSTEM INIT
  *************************************************************************/
  void initializeLittleFS() 
  {
    if (!LittleFS.begin(true)) 
    {                                             // Initializing (mounting) the file system  
      Serial.println("LittleFS error");
    }
  
    Serial.println("LittleFS configured successfully");
  }


  /************************************************************************
                          UPDATE CLIENT PAGE
  *************************************************************************/
  void updateClientWebpage(String sensorReadings)
  {
    ws.textAll(sensorReadings);                                       // Sends the sensorReadings string to all clients connected to the WebSocket
  }

  /************************************************************************
                       CONVERT JSON VARIABLE to STRING
  *************************************************************************/
  String getSensorReadings()
  {                                                 ////JSON format:{"key":"value", ....}
    std::srand(std::time(0));
    int randomnumber = std::rand() % 100; 
    int randomnumber1 = std::rand() % 100; 
    int randomnumber2 = std::rand() % 100; 
    int randomnumber3 = std::rand() % 100; 
    int randomnumber4 = std::rand() % 100; 
    int randomnumber5 = std::rand() % 100; 
  
    sensorReadings["Time Recorded"] = "8:30";   
    sensorReadings["Packet ID"]; 
    sensorReadings["Vehicle Acceleration"] = randomnumber;                       
    sensorReadings["Vehicle Orientation"] = randomnumber1;                                   //JSONVar are dynamic, can hold different types of data, inside the [] is the key, the parameter inside String() is the value 
    sensorReadings["Shock Travel Sensor: Front Right"] = randomnumber2;     
    sensorReadings["Shock Travel Sensor: Front Left"] = randomnumber3;     
    sensorReadings["Shock Travel Sensor: Right Rear"] = randomnumber4;     
    sensorReadings["Shock Travel Sensor: Left Rear"] = randomnumber5;     
    sensorReadings["Packet ID"];    
  
    String jsonString = JSON.stringify(sensorReadings);                       // Serialization: converting a JS object/array into a string, when transmitting to a server, string type is best because it fits most data structures
    Serial.println("Data string created");
    Serial.println();
    return jsonString;                                                         
  }

  /************************************************************************
                          SEND DATA AFTER CLIENT REQUEST
  *************************************************************************/
  void WebSocketResponse(void *pArg,uint8_t *pData, size_t len)
  {
  
       // String incomingMessage = (char*) pData;
       // Serial.print(incomingMessage); 
  
        if(strcmp((char*) pData,"Start Collection")){
  
        Serial.printf("Collection Started!");
        }
  }      
    


  /************************************************************************
                          EVENT HANDLING
  *************************************************************************/
  
  void WebSocketEventHandling(AsyncWebSocket *pSocket, AsyncWebSocketClient *pClient, AwsEventType type, void *pArg, uint8_t *pData, size_t len)
  {
  //pSocket: points to the created Websocket object
  //pClient: points to the client that started one of the cases
  //type: the diff event types
  //id() creates an id, increments through every call 
    //bool *pCollection = NULL; 
    switch(type) {
      case WS_EVT_CONNECT:
        Serial.printf("New WebSocket client has connected!");                             // With a new connection
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client has disconnected \n");                            // With a disconnection, print the client ID that left
        break;
      case WS_EVT_ERROR:
        Serial.printf("WebSocket error occured");     
        break;                                                                            // Print error, could add some function in this event 
      case WS_EVT_DATA:
        WebSocketResponse(pArg,pData,len);                                                                                        // If request for data, send the response function
        break;  
    }
  }


  /************************************************************************
                          WEBSOCKET INIT
  *************************************************************************/
  void initializeWebSocket() 
  {
    ws.onEvent(WebSocketEventHandling);                                                   //Define what function handles the events on any event,
    server.addHandler(&ws);                                                               //Registers the ws handler with the HTTP server, the HTTP server then has access to manage connections and events
  }


  /************************************************************************
                          PAGE SERVING
  *************************************************************************/
  void ServerBegin()
  {
    server.on("/",HTTP_GET, [](AsyncWebServerRequest *pRequest) {                           // "/" is the root path, HTTP_GET means the handler will handle GET requests
      pRequest->send(LittleFS, "/index.html","text/html");                                 // Serves the index.html file, "text/html" specifies it's an HTML doc
    });
      
    server.serveStatic("/", LittleFS, "/");                                                //Serve any files that are static like pictures of styling files like CSS or JS
    server.begin();                                                                        //Begin listening for new client requests
  
  }

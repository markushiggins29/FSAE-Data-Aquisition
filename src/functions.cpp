#include "main.h"

const char* ssid = "ESP32";                                         // Configure Name of the network
const char* password = "password";  
AsyncWebServer server(80);                                                  // Creates an instance (object) of the AsyncWebServer class on port 80, port 80 is the default port used for HTTP handshake
AsyncWebSocket ws("/ws"); 
JSONVar sensorReadings; 

/************************************************************************
                            WIFI INIT
*************************************************************************/
 void initializeWifi() {
  WiFi.mode(WIFI_AP);                                                       // Set the wifi mode as an access point
  Serial.println("Connecting to Wifi..");    
        
                       
  /*
  
  
  bool status = WiFi.softAP(ssid,password,1);                               // Returns either 1 or 0 for connection verification
  
  Serial.printf("status: %i",status);

  if (status){
    Serial.println("Connected to WiFi ");                             
    Serial.println(WiFi.softAPIP());                                        //Print the IP address
  }
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
void initializeLittleFS() {
  if (!LittleFS.begin(true)) {                                             // Initializing (mounting) the file system  
    Serial.println("LittleFS error");
  }

  Serial.println("LittleFS configured successfully");
  }

/************************************************************************
                        FILESYSTEM TEST
*************************************************************************/
void TestLittleFS() {
    //int usedBytes = LittleFS.usedBytes();
    //int bytesLeft = LittleFS.totalBytes() - usedBytes; 
    //File file = LittleFS.open("main.cpp");
    //while(file.available()){
    //    Serial.print(file.read());
    //}
    //Serial.printf("File size is :%i",file.size());
    //file.close(); 

    //Serial.printf("Number of usedBytes on the Flash:%i",usedBytes);
    
    //Serial.printf("number of Bytes open on the Flash:%i",bytesLeft)
    

}


/************************************************************************
                        UPDATE CLIENT PAGE
*************************************************************************/
void updateClientWebpage(String sensorReadings){
  ws.textAll(sensorReadings);                                       // Sends the sensorReadings string to all clients connected to the WebSocket
}

/************************************************************************
                     CONVERT JSON VARIABLE to STRING
*************************************************************************/
String getSensorReadings(){                                                 ////JSON format:{"key":"value", ....}
   
  pinMode(34,INPUT);
  pinMode(33 ,INPUT);
  pinMode(32 ,INPUT);
  pinMode(35 ,INPUT);
  
  analogReadResolution(12);       //9-12
  //analogSetClockDiv(1);           //1 2 3 or 4
  //analogSetAttenuation(ADC_0db); //or 0db, 2_5db, 6db 
  //analogSetVRefPin(34)               //25, 26, or 27
  int CalGyroReading = analogRead(34) - 1700; 
  int AccZReading = analogRead(33);
  int AccYReading = analogRead(32);
  int AccXReading = analogRead(35);


  
  if (CalGyroReading < 0 || CalGyroReading > 0 && CalGyroReading < 120 ){
    CalGyroReading = 0;
  }
  


  sensorReadings["Time Recorded"] = 1;               
  sensorReadings["Vehicle Orientation"] = CalGyroReading;                                 //JSONVar are dynamic, can hold different types of data, inside the [] is the key, the parameter inside String() is the value 
  sensorReadings["Vehicle Acceleration x"]= AccXReading;    
  sensorReadings["Vehicle Acceleration Y"]= AccYReading;    
  sensorReadings["Vehicle Acceleration Z"]= AccZReading;       
  //sensorReadings["Shock Travel Sensor: Front Left"];     
  //sensorReadings["Shock Travel Sensor: Right Rear"];    
  //sensorReadings["Shock Travel Sensor: Left Rear"];     
  

  String jsonString = JSON.stringify(sensorReadings);                       // Serialization: converting a JS object/array into a string, when transmitting to a server, string type is best because it fits most data structures
  return jsonString;                                                         
}


void StartReading(){
  String sensorReadings = getSensorReadings();                          // Get sensor readings string
  Serial.print(sensorReadings);                                     
   updateClientWebpage(sensorReadings);  
   
} 

void InitializeSensors(){
  Serial.begin(115200); 
  pinMode(34,OUTPUT);
  analogReadResolution(12);       //9-12
  analogSetClockDiv(1);           //1 2 3 or 4
  analogSetAttenuation(ADC_0db); //or 0db, 2_5db, 6db 
  //analogSetVRefPin(34)               //25, 26, or 27
}




/************************************************************************
                        SEND DATA AFTER CLIENT REQUEST
*************************************************************************/
void WebSocketResponse(void *pArg,uint8_t *pData, size_t len){

      String incomingMessage = (char*) pData;
      Serial.print(incomingMessage); 
      //if (strcmp((char*)pData,"Start Collection") == 0){
     }
  


  /************************************************************************
                        EVENT HANDLING
*************************************************************************/

  void WebSocketEventHandling(AsyncWebSocket *pSocket, AsyncWebSocketClient *pClient, AwsEventType type, void *pArg, uint8_t *pData, size_t len){
//pSocket: points to the created Websocket object
//pClient: points to the client that started one of the cases
//type: the diff event types
//id() creates an id, increments through every call 

  switch(type) {
    case WS_EVT_CONNECT:
      Serial.printf("New WebSocket client has connected!");    // WIth a new connection print the client ID and IP address
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client has disconnected \n");                                                 // With a disconnection, print the client ID that left
      break;
    case WS_EVT_ERROR:
      Serial.printf("WebSocket error occured");     
      break;                                                                            // Print error, could add some function in this event 
    case WS_EVT_DATA:
      WebSocketResponse(pArg,pData,len);                                                                                        // If request for data, send the response function
      break;  
  }
}

/************************************************************************vvb
                        WEBSOCKET INIT
*************************************************************************/
  void initializeWebSocket() {
    ws.onEvent(WebSocketEventHandling);                                                   //Define what function handles the events on any event,
    server.addHandler(&ws);                                                               //Registers the ws handler with the HTTP server, the HTTP server then has access to manage connections and events
  }






/************************************************************************
                        PAGE SERVING
*************************************************************************/
void ServerBegin(){
  server.on("/",HTTP_GET, [](AsyncWebServerRequest *pRequest) {                           // "/" is the root path, HTTP_GET means the handler will handle GET requests
    pRequest->send(LittleFS, "/index.html","text/html");                                 // Serves the index.html file, "text/html" specifies it's an HTML doc
  });
    
  server.serveStatic("/", LittleFS, "/");                                                //Serve any files that are static like pictures of styling files like CSS or JS
  server.begin();                                                                        //Begin listening for new client requests

}
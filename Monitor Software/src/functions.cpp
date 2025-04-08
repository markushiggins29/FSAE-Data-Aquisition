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
  WiFi.mode(WIFI_AP);         
  //WiFi.begin("SSID", "PASSWORD");                                              // Set the wifi mode as an access point
  Serial.println("Connecting to Wifi..");    
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
                        UPDATE CLIENT PAGE
*************************************************************************/
void updateClientWebpage(String sensorReadings){
  ws.textAll(sensorReadings);                                       // Sends the sensorReadings string to all clients connected to the WebSocket
}

/************************************************************************
                     CONVERT JSON VARIABLE to STRING
*************************************************************************/
String getSensorReadings(){                                                 ////JSON format:{"key":"value", ....}
   
  pinMode(36 ,INPUT);
  pinMode(39 ,INPUT);
  pinMode(34 ,INPUT);
  pinMode(33 ,INPUT);
  
  //GYRO (GPIO 33)
  float RawGyro = analogRead(33); 
  float Vout = (RawGyro/4095)*(3.3); 
  float Rotation = (Vout - 2.5)/(0.0125) + 84; 
  int CalRotation = static_cast<int>(Rotation);
  if(CalRotation < 0 && CalRotation >= -15 || CalRotation > 0 && CalRotation <= 15 ){
    CalRotation = 0; 
  }
  else if(CalRotation > 250){
    CalRotation = 250; 
  }


  //ACCZ (GPIO 34)
  //Reading 
  double RawAccZ = analogRead(34)-1280;
  //Slice out bad values
  if(RawAccZ < 0 && RawAccZ >= -15 || RawAccZ > 0 && RawAccZ <= 15 ){
    RawAccZ = 0;}
  //convert to Output voltage
  double Vout1 = (RawAccZ/4095)*(3.3); 
  //convert to value in G using output voltage, bias voltage, and sensitivity
  double AccelZ = (Vout1 - 1.5)/(0.42)+3.57;
  //Ronud to two decimal places
  double RoundedAccelZ = std::round(AccelZ *100) /100.0;



  //ACCY (GPIO 39)
  //reading
  double RawAccY = analogRead(39)-1860;
  //Slice out bad values
  if(RawAccY < 0 && RawAccY >= -50 || RawAccY > 0 && RawAccY <= 50 ){
    RawAccY = 0; }
  //convert to Output voltage
  double Vout2 = (RawAccY/4095)*(3.3); 
  //convert to value in G using output voltage, bias voltage, and sensitivity
  double AccelY= (Vout2 - 1.5)/(0.42)+3.57;// - 2184; 
  //Round to two decimal
  double RoundedAccelY = std::round(AccelY *100) /100.0;


  //ACCX (GPIO 36)
  //reading
  double RawAccX = analogRead(36)-1790;
  //Slice out bad values
  if(RawAccX < 0 && RawAccX >= -50 || RawAccX > 0 && RawAccX <= 50 ){
    RawAccX = 0; }
   //convert to Output voltage
  double Vout3 = (RawAccX/4095)*(3.3); 
  //convert to value in G using output voltage, bias voltage, and sensitivity
  double AccelX= (Vout3 - 1.5)/(0.42)+3.57; 
  //Round to two decimal places
  double RoundedAccelX = std::round(AccelX *100) /100.0;

 
 
  //String creation
  sensorReadings["Time Recorded"] = 1;               
  sensorReadings["Vehicle Orientation"] = CalRotation;                                 //JSONVar are dynamic, can hold different types of data, inside the [] is the key, the parameter inside String() is the value 
  sensorReadings["Vehicle Acceleration X"]= RoundedAccelX;  
  sensorReadings["Vehicle Acceleration Y"]= RoundedAccelY;  
  sensorReadings["Shock Travel"]= ;   //put shock travel value here 

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
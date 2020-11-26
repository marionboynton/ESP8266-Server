#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>   // Include the SPIFFS library

static const char *wifi_ssid = "ESP8266";
static const char *wifi_pass = "testing1";

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

MDNSResponder mdns;

static bool wifi_connected;
int stations = WiFi.softAPgetStationNum();
int prev = 0;
int delay_loop=0;
String output;

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
bool apmode;

static void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Problem with filesystem!\n");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}


void sendValues(){
  server.send(200, "text/json", output);
}

void connectEsp(){
  stations = WiFi.softAPgetStationNum();
  prev = stations;
  
  Serial.println("Connecting ...");
  int i = 0;
  while ((wifiMulti.run() != WL_CONNECTED)&&(i<=20)){ // Wait for the Wi-Fi to connect
    delay(250);
    i++;
    Serial.print('.');
  }
  if ((wifiMulti.run() != WL_CONNECTED)&&(i>20)){
    apmode = WiFi.softAP(wifi_ssid, wifi_pass);
    if (apmode==true){
      Serial.println("Access Point Mode");
      Serial.println("\nattempting to connect to stations...");
      while (WiFi.softAPgetStationNum()==0)              //loop here while no AP is connected to this station
      {
        delay(1);
        delay_loop++;
        if (delay_loop>=120000)                          
        {
            Serial.println("ESP going to deep sleep mode for 60 seconds");
            ESP.deepSleep(30e6); //go to sleep for 60 sec -wake pin externally connected to reset pin
            delay_loop = 0;
        }  
      }
      if (WiFi.softAPgetStationNum() > 0){
          Serial.printf("Connected to %d stations", WiFi.softAPgetStationNum());
           prev = WiFi.softAPgetStationNum();
      }
    }
    else{
      Serial.println("Access Point Mode Failed");
    }
    i = 0;
  }
  else if (wifiMulti.run() == WL_CONNECTED){
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  }
  
}

const size_t CAPACITY = JSON_OBJECT_SIZE(5)+ JSON_ARRAY_SIZE(16);
const size_t arrsize = JSON_ARRAY_SIZE(16);

void writeJson(){
  StaticJsonDocument<CAPACITY> doc1;
  StaticJsonDocument<arrsize> doc2;
  double cellArray[16];
  int i;  int maxV = 35; int minV = 27; double ten = 10.0;
  doc1["BEMUID"] = "123456";
  doc1["voltage"]= ((rand()%(52-48+1))+48);
  doc1["current"] = ((rand()%(20+20+1))-20);
  doc1["charge"] = rand()%(101);
  for (i=0; i<16; i++){
  cellArray[i] = ((rand()%(maxV-minV+1))+minV)/ten;
  }
  copyArray(cellArray, doc2.to<JsonArray>());
  doc1["cell voltages"] = doc2;
  serializeJson(doc1, output);  
}

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

 wifiMulti.addAP("balancell_office1", "bms314159");   // add Wi-Fi networks you want to connect to
 wifiMulti.addAP("Marion Boynton's iPhone1", "Marion123");
 wifiMulti.addAP("The Palm", "palmpalm");
 //wifiMulti.addAP("a", "a");
  
  connectEsp();
  
  WiFi.hostname("esp8266.local");
  if (MDNS.begin("esp8266", WiFi.localIP())) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
   Serial.println(WiFi.hostname().c_str());
   
  //server.on("/", handleRoot);
  SPIFFS.begin();                           // Start the SPI Flash Files System
  
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);
  
}
 
void loop(void) {
  writeJson();
  server.handleClient();
  MDNS.update();
  server.on("/values.json", sendValues);
  output = "";
  delay(1000);  
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

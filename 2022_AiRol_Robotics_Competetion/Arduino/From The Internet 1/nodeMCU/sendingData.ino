#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
     
    const char* ssid     = "SLT-4G-****";
    const char* password = "*********";     
    
    WiFiClient wificlient;
    int wifiStatus;
    HTTPClient http;
    SoftwareSerial NodeMCU(D7,D8);
    

    void setup() {
      
      Serial.begin(9600);
      NodeMCU.begin(115200);
      delay(200);
      Serial.setDebugOutput(true);
     
     
      // We start by connecting to a WiFi network
     
      Serial.println();
      Serial.println();
      Serial.print("Your are connecting to;");
      Serial.println(ssid);
      
      WiFi.begin(ssid, password);
      WiFi.mode(WIFI_STA);
      
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
      wifiStatus = WiFi.status();

      if(wifiStatus == WL_CONNECTED){
         Serial.println("");
         Serial.println("Your ESP is connected!");  
         Serial.println("Your IP address is: ");
         Serial.println(WiFi.localIP());  
      }
      else{
        Serial.println("");
        Serial.println("WiFi not connected");
      }
      auth("content",1);
      delay(5000);
     // auth("content",1);
 }   

 boolean auth(String eNum, int id ){
   int port=80;
  //char* host="https://3b38817b-985f-44b2-ba78-2d2269697a3a.mock.pstmn.io";
  String data="/test.php?status=up";//="?enum="+eNum+"&id="+id;
  char * host="shehandinuka123.000webhostapp.com";
  boolean status=false;
  do {
     if(!wificlient.connect(host,port)){
        Serial.println("connection failed");
        delay(1000);
        return false;
        }
        Serial.print("Requesting URL: ");
        Serial.println(host+data);
        String url =host+data;
    
       int i= wificlient.print(String("GET ") + data + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");
    
                  // Serial.println("HHHH :");
    //               Serial.println(i);
    
     /*   String url =host+data;
        http.begin(url);
        int httpCode = http.GET();
        String payload = http.getString();
          Serial.println();
      Serial.println("closing connection");
      Serial.println(payload);
      Serial.println(httpCode);*/
      
      while(wificlient.available()){
        String line = wificlient.readStringUntil('\r');
        char buf[200];
        Serial.print(line);
        if(line=="HTTP/1.1 200 OK") status=true;
       /* if(status && line=="<html>"){
          Serial.print(line);
        //  Serial.print("done2");
         do {line = wificlient.readStringUntil('\r');
                Serial.println(line);
              line.toCharArray(buf,200);
              char* str = strtok(buf, "-");
              Serial.print(buf);
              while (str != NULL){
                Serial.println(str);
                str = strtok(NULL, "-");
              }
         
          }while(wificlient.available()&&(line!="</html>"));
          }*/
      }delay(2000);

DynamicJsonBuffer jsonBuffer;

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(wificlient);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
//    return;
  }
  Serial.println(F("Response:"));
  Serial.println(root["0"].as<char*>());
  Serial.println(root["1"].as<char*>());
      
  }while(!status);
  return true;
  }
     
void loop() {
     
      delay(1000); // check for connection every once a second
    /*  String content ;
      char character;
      boolean receive = false;
      while(NodeMCU.available()) {
      character = NodeMCU.read();
      content.concat(character);
      receive =true;
      }
    if(receive){
    Serial.println(content);
    while(!auth(content,1)){delay(1000);}
    NodeMCU.flush();*/
  //  auth("content",1);
    delay(1000);
    //}
}

#include <ArduinoJson.h>
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include<WebServer.h>

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WebServer server(80);

void setup() {
 
  server.enableCORS(); //This is the magic

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
     
  StaticJsonDocument<100> testDocument;
   
  testDocument["sensorType"] = "Temperature";
  testDocument["value"] = 10;
 
  char buffer[100];
 
  serializeJson(testDocument, buffer);
  Serial.println(buffer);

  server.on("/", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    
    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done."));
    });
    

  server.on("/F", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "move forward";
    doc["speed"] = 15;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward."));
    });
    
  server.on("/B", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "move backward";
    doc["speed"] = 10;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward."));
    });
    
  server.on("/L", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "turning left";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward."));
    });
    
  server.on("/R", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "turning right";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward."));
    });
    
  server.on("/S", [](){
     DynamicJsonDocument doc(512);
    doc["status"] = "stop moving";
    doc["speed"] = 0;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward."));
    });
}
 
void loop() {
 server.handleClient();
}

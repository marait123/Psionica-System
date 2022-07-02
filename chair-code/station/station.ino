#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include"motor.h"
#include"car.h"
// the follownig variables enables the car to execute 1 motion command for no more that MOVETIME milliseconds
int motion_start_time = 0;

Motor leftMotor(15, 2), rightMotor(5, 18);

// car definition
Car car (leftMotor, rightMotor);

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";
const char *ssid_orange = "The Quite World";
const char *password_orange = "mohammed1";

bool station_mode = false;
WebServer server(80);

void setup()
{
  leftMotor.setup();
  rightMotor.setup();

  server.enableCORS(); // This is the magic

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  if (station_mode)
  {
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();
  }
  else
  {

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid_orange);

    WiFi.begin(ssid_orange, NULL);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
  }

  Serial.println("Server started");

  StaticJsonDocument<100> testDocument;

  testDocument["sensorType"] = "Temperature";
  testDocument["value"] = 10;

  char buffer[100];

  serializeJson(testDocument, buffer);
  Serial.println(buffer);

  server.on("/", []()
            {
     DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    
    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done.")); });

  server.on("/F", []()
            {
     motion_start_time = millis();
     DynamicJsonDocument doc(512);
    doc["status"] = "move forward";
    doc["speed"] = 15;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    car.moveForward();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/B", []()
            {
       motion_start_time = millis();

     DynamicJsonDocument doc(512);
    doc["status"] = "move backward";
    doc["speed"] = 10;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    car.moveBackward();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/L", []()
            {
     motion_start_time = millis();
    DynamicJsonDocument doc(512);
    doc["status"] = "turning left";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    car.moveLeft();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/R", []()
            {
     motion_start_time = millis();
             
     DynamicJsonDocument doc(512);
    doc["status"] = "turning right";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    car.moveRight();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/S", []()
            {
     DynamicJsonDocument doc(512);
    doc["status"] = "stop moving";
    doc["speed"] = 0;
    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    leftMotor.stop();
    rightMotor.stop();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });
}

void loop()
{

  car.loop();
  server.handleClient();
}

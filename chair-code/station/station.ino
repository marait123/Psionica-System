#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include"motor.h"
#include"car.h"


#include <ESP32Servo.h> 

// the follownig variables enables the car to execute 1 motion command for no more that MOVETIME milliseconds
int motion_start_time = 0;
// speed pin
unsigned int right_speed_pin = 32; // the PWM pin the LED is attached to

unsigned int left_speed_pin = 33; // the PWM pin the LED is attached to

Motor leftMotor(15, 2), rightMotor(5, 18);
// static ip


// Set your Static IP address
const char* loc_ip = "192.168.1.159";
IPAddress local_IP(192, 168, 1, 159);
// Set your Gateway IP address
IPAddress gateway(10, 10, 10, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


// car definition
Car car (leftMotor, rightMotor);

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";
const char *ssid_orange = "Orange-Orange5";
const char *password_orange = "mohammed1";

bool station_mode = false;
int port = 12345;
WebServer server(port);

void setup()
{
//  wifi issue that sta goes to sleep and hangs incoming connection
  WiFi.setSleep(false);
  
  // speed pin pwm
  ledcAttachPin(right_speed_pin, 0); // assign a led pins to a channel

  ledcAttachPin(left_speed_pin, 1); // assign a led pins to a channel

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution

  
  //motor configuration
  leftMotor.setup();
  rightMotor.setup();

  // enable cross origin requests
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

 // Configures static IP address


    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid_orange);

    WiFi.begin(ssid_orange, password_orange);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

      Serial.println("");
    Serial.println("WiFi connected before");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("Gateway IP address: ");
    Serial.println(WiFi.gatewayIP());
    
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
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
    car.stop();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });
    server.on("/who", []()
    {
     DynamicJsonDocument doc(512);
    doc["answer"] = "car";
    doc["ip"] = loc_ip;
    doc["port"] = port;
    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);

    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });
}

void loop()
{
// write the pwm

  server.handleClient();

  car.loop();
  delay(100);
}

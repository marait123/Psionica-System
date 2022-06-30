#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>

// classes area
class Arduino
{
public:
  virtual void setup() = 0;
};

// motor
/**
 * @brief https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
 *
 */
class Motor : Arduino
{
public:
  int pin0, pin1;
  Motor(int pin0, int pin1)
  {
    this->pin0 = pin0;
    this->pin1 = pin1;
  }
  void setup()
  {
    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
  }
  void moveForward()
  {
    digitalWrite(pin0, LOW);
    digitalWrite(pin1, HIGH);
  }
  void moveBackward()
  {
    digitalWrite(pin0, HIGH);
    digitalWrite(pin1, LOW);
  }
  void stop()
  {
    digitalWrite(pin0, LOW);
    digitalWrite(pin1, LOW);
  }
};

Motor leftMotor(15, 2), rightMotor(5, 18);
// this car moves in one of 4 directions
class Car
{
private:
public:
  void setup()
  {
  }
  void stop()
  {
    leftMotor.stop();
    rightMotor.stop();
  }
  void moveLeft()
  {
    leftMotor.stop();
    rightMotor.moveForward();
  }
  void moveRight()
  {
    leftMotor.moveForward();
    rightMotor.stop();
  }
  void moveForward()
  {
    rightMotor.moveForward();
    leftMotor.moveForward();
  }
  void moveBackward()
  {
    rightMotor.moveBackward();
    leftMotor.moveBackward();
  }
};
// car definition
Car car;

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";
const char *ssid_orange = "Orange-Orange5";
const char *password_orange = "mohammed1";

bool station_mode = true;
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

    WiFi.begin(ssid_orange, password_orange);

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
     DynamicJsonDocument doc(512);
    doc["status"] = "move forward";
    doc["speed"] = 15;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    rightMotor.moveForward();
    leftMotor.moveForward();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/B", []()
            {
     DynamicJsonDocument doc(512);
    doc["status"] = "move backward";
    doc["speed"] = 10;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    rightMotor.moveBackward();
    leftMotor.moveBackward();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/L", []()
            {
     DynamicJsonDocument doc(512);
    doc["status"] = "turning left";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    leftMotor.stop();
    rightMotor.moveForward();
    server.send(201, F("application/json"), buf);
    Serial.print(F("done forward.")); });

  server.on("/R", []()
            {
     DynamicJsonDocument doc(512);
    doc["status"] = "turning right";
    doc["speed"] = 5;

    Serial.print(F("Stream..."));
    String buf;
    serializeJson(doc, buf);
    leftMotor.moveForward();
    rightMotor.stop();
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
  server.handleClient();
}

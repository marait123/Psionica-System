/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32
31.01.2017 by Jan Hendrik Berlin

 */

#include <WiFi.h>

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
// definitions area
const char *ssid = "Orange-Orange5";
const char *password = "mohammed1";

WiFiServer server(80);

void setup()
{
  // setup the motors
  leftMotor.setup();
  rightMotor.setup();

  Serial.begin(115200);
  // pinMode(5, OUTPUT); // set the LED pin mode

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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

int value = 0;

void loop()
{
  WiFiClient client = server.available(); // listen for incoming clients

  if (client)
  {                                // if you get a client,
    Serial.println("New Client."); // print a message out the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/F\">here</a> to go forward.<br>");
            client.print("Click <a href=\"/B\">here</a>  to go backward.<br>");
            client.print("Click <a href=\"/L\">here</a>  to go left.<br>");
            client.print("Click <a href=\"/R\">here</a>  to go right.<br>");
            client.print("Click <a href=\"/R\">here</a>  to stop.<br>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /F"))
        {
          car.moveForward();
        }
        if (currentLine.endsWith("GET /B"))
        {
          car.moveBackward();

        }
         if (currentLine.endsWith("GET /L"))
        {
          car.moveLeft();

        }
         if (currentLine.endsWith("GET /R"))
        {
          car.moveRight();

        }
		
         if (currentLine.endsWith("GET /S"))
        {
          car.stop();

        }
		
		
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */
#include "DHT.h"

#define DHTPIN 13     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
// if tempPrev + .5 < ft or tempPrev - .5 > ft
DHT dht(DHTPIN, DHTTYPE);

  String dataServer = "10.0.100.100";
  String deviceName = "GarageSensorUnit";  
  float tempPrev;
  float humPrev;
  int loopTo300 = 0;




#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

  String dataServer = "10.0.100.100";
  String deviceName = "GarageSensorUnit"; 
  
  dht.begin();


  
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("FoS", "");

}

void loop() {

delay(5000); // Wait 5 seconds   

  
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float ct = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float ft = dht.readTemperature(true);

  

  String outputString = "/getInfoPage/index.php?deviceName=";
  outputString = outputString + deviceName;
  outputString = outputString + "&sensorName=tempSensor&sensorReading=";
  outputString = outputString + ft;

// if tempPrev + .25 < ft or tempPrev - .25 > ft
  // wait for WiFi connection
    if((tempPrev + .25) < ft || (tempPrev - .25) > ft || loopTo300 > 300){
    USE_SERIAL.print("\nFT = ");
    USE_SERIAL.print(ft);
    USE_SERIAL.print("\ntempPrev = ");
    USE_SERIAL.print(tempPrev);
    USE_SERIAL.print("\n");
    
      if((WiFiMulti.run() == WL_CONNECTED)) {
  
          HTTPClient http;
  
          USE_SERIAL.print("[HTTP] begin...\n");
          // configure traged server and url
          //http.begin("192.168.1.12", 443, "/test.html", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
          http.begin(dataServer, 80, outputString); //HTTP
          USE_SERIAL.print(outputString);
          USE_SERIAL.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          int httpCode = http.GET();
          if(httpCode) {
              // HTTP header has been send and Server response header has been handled
              USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
  
              // file found at server
              if(httpCode == 200) {
                  String payload = http.getString();
                  USE_SERIAL.println(payload);
              }
          } else {
              USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
          }
      }
      tempPrev = ft;
      } else {

    USE_SERIAL.print("Previous temp: ");
    USE_SERIAL.print(tempPrev);
    USE_SERIAL.print(".\t\t Current temp: ");
    USE_SERIAL.print(ft);
    USE_SERIAL.print(".\t  Difference not enough.\tNot posting.\n");

      
    }

  outputString = outputString + deviceName;
  outputString = outputString + "&sensorName=humSensor&sensorReading=";
  outputString = outputString + h;

  // wait for WiFi connection
  if((humPrev + .75) < h || (humPrev - .75) > h || loopTo300 > 300){  
    USE_SERIAL.print("\nH = ");
    USE_SERIAL.print(h);
    USE_SERIAL.print("\nHumPrev = ");
    USE_SERIAL.print(humPrev);
    USE_SERIAL.print("\n");
      if((WiFiMulti.run() == WL_CONNECTED)) {
  
          HTTPClient http;
  
          USE_SERIAL.print("[HTTP] begin...\n");
          // configure traged server and url
          //http.begin("192.168.1.12", 443, "/test.html", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
          http.begin(dataServer, 80, outputString); //HTTP
          USE_SERIAL.print(outputString);
          USE_SERIAL.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          int httpCode = http.GET();
          if(httpCode) {
              // HTTP header has been send and Server response header has been handled
              USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
  
              // file found at server
              if(httpCode == 200) {
                  String payload = http.getString();
                  USE_SERIAL.println(payload);
              }
          } else {
              USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
          }
      }
    humPrev = h;
    loopTo300 = 0;
    } else {

    USE_SERIAL.print("Previous humidity: ");
    USE_SERIAL.print(humPrev);
    USE_SERIAL.print(".\t Current humidity: ");
    USE_SERIAL.print(h);
    USE_SERIAL.print(".  Difference not enough.\tNot posting.\n");


      
    }
loopTo300 = loopTo300 + 1;
USE_SERIAL.print("\n Loop number: ");
USE_SERIAL.print(loopTo300);
USE_SERIAL.print("\n\n");

    
delay(2000); // Wait a minute  
//600000  / 2000 = 300

}


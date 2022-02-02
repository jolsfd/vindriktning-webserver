/* 
   Vindrikting Webserver 
 */

#include "ESP8266WiFi.h"
#include <SoftwareSerial.h>
#include "WiFiClient.h"
#include "ESP8266WebServer.h"

#include "index.h"

ESP8266WebServer server(80);

// Sensor Variablen
constexpr static const uint8_t PIN_UART_RX = 4; // D2 an Wemos D1 Mini
constexpr static const uint8_t PIN_UART_TX = 13; // UNUSED

SoftwareSerial sensorSerial(PIN_UART_RX, PIN_UART_TX);

uint8_t serialRxBuf[80];
uint8_t rxBufIdx = 0;
int spm25 = 0;
int last = 0;
unsigned long timepast = millis();

const int sendinterval = 20; // in Sekunden

// Wifi Variablen
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Read data from sensor
int getSensorData() {
  uint8_t rxBufIdx = 0;
  uint8_t checksum = 0;

  // Sensor Serial aushorchen
  while ((sensorSerial.available() && rxBufIdx < 127) || rxBufIdx < 20) 
  {
    serialRxBuf[rxBufIdx++] = sensorSerial.read();
    // Wenn es Probleme mit dem Empfang gibt:
    //    delay(15);
  }

  // Prüfsumme berechnen
  for (uint8_t i = 0; i < 20; i++) 
  {
    checksum += serialRxBuf[i];
  }

  // Header und Prüfsumme checken
  if (serialRxBuf[0] == 0x16 && serialRxBuf[1] == 0x11 && serialRxBuf[2] == 0x0B && checksum == 0)
  {
    return (serialRxBuf[5] << 8 | serialRxBuf[6]);
  }
  else
  {
    return (-1);
  }
}

// HTML route
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

// Sensor route
void handleSensor() {
  String particleValue = String(last);
  
  server.send(200, "text/plane", particleValue); //Send value only to client ajax request
}

void setup() {
  // Software Serial für Sensor
  sensorSerial.begin(9600);

  // Debug Serial
  Serial.begin(115200);
  while (! Serial);
  Serial.println("Booting Sketch...");

  /*
  // Acces Point mode
  WiFi.setMod
  boolean result = WiFi.softAP("esp8266", "magma");
  
  if(result == false){
    Serial.println("Access Point could not be established.");
  }
  Serial.println("Access Point is working correct!");
  */


  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your*/
  
  server.on("/", handleRoot);
  server.on("/sensor", handleSensor);

  server.begin();
  Serial.println("HTTP server started.");
}


void loop() {
  // Sensor abfragen
  spm25 = getSensorData();
  delay(500);

  if (spm25 > 0)
  {
    last = spm25;
  }


  if (millis() > timepast + sendinterval * 1000 && last>0)
  {
    timepast = millis();
    // Ausgabe Debug Monitor
    Serial.printf("Time: %d --- PM2.5=%d µg/m3 (Unkalibriert) \n", millis() / 1000, last);
    delay(100);
  }
  
  server.handleClient();
}
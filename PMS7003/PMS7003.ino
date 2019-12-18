#include "PMS.h"
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#if defined(ESP8266) && !defined(D5)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#endif


SoftwareSerial softwareSerial(D5, D6);
pms::PMS7003 pms7003(softwareSerial);
auto ssid = "Orange_Swiatlowod_2A92_2.4GHz";
auto password = "";

ESP8266WebServer server(80);

String toString(const pms::Measures& measures) {
  if (!measures.is_ok) {
    return "{ \"ok\": false }";
  }
  String str = "{ \"ok\": true"
               " ,\"PM1_0\": " + String(measures.PM1_0) + 
               " ,\"PM2_5\": " + String(measures.PM2_5) + 
               " ,\"PM10_0\": " + String(measures.PM10_0) +
               "}";

  return str;
}

void getMeasures() {
    Serial.println("trying to read from sensor");

    auto measures = pms7003.read();

    Serial.print("OK: ");
    Serial.println(measures.is_ok);
    Serial.print("PM 1: ");
    Serial.println(measures.PM1_0);
    Serial.print("PM 2.5: ");
    Serial.println(measures.PM2_5);
    Serial.print("PM 10.0: ");
    Serial.println(measures.PM10_0);

    Serial.println("sending");

    server.send(200, "text/plain", toString(measures));
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void wifiConnect() {
  WiFi.disconnect();
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.printDiag(Serial);
  WiFi.begin(ssid, password);
  for (int i = 0; i < 40; ++i) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
}

void setup() {
    Serial.begin(57600);
    softwareSerial.begin(9600);

    wifiConnect();
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("pmsensor")) {
      Serial.println("MDNS responder started");
    }

    server.on("/measures", getMeasures);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    wifiConnect();
  }
  server.handleClient();
  MDNS.update();
}

#include "PMS7003/PMS7003.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#if defined(ESP8266) && !defined(D5)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#endif

SoftwareSerial softwareSerial(D5, D6);
pms::Sensor<SoftwareSerial> pms7003(softwareSerial, pms::mode::active);

auto ssid = "DASAN_6a";
auto password = "zaq12wsx";

ESP8266WebServer server(80);

String toString(const pms::Measurements& measurements) {
  if (!measurements.is_ok) {
    return "{ \"ok\": false }";
  }
  String str = "{ \"ok\": true"
               " ,\"PM1_0\": " + String(measurements.atmospheric.PM1_0) + 
               " ,\"PM2_5\": " + String(measurements.atmospheric.PM2_5) + 
               " ,\"PM10_0\": " + String(measurements.atmospheric.PM10_0) +
               "}";

  return str;
}

void get_measurements() {
    Serial.println("trying to read from sensor");

    auto measurements = pms7003.read();

    Serial.print("OK: ");
    Serial.println(measurements.is_ok);
    Serial.print("PM 1: ");
    Serial.println(measurements.atmospheric.PM1_0);
    Serial.print("PM 2.5: ");
    Serial.println(measurements.atmospheric.PM2_5);
    Serial.print("PM 10.0: ");
    Serial.println(measurements.atmospheric.PM10_0);

    Serial.println("sending");

    server.send(200, "text/plain", toString(measurements));
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

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
    pinMode(D5, INPUT);
    pinMode(D6, OUTPUT);

    Serial.begin(57600);
    
    wifiConnect();

    if (MDNS.begin("pmsensor")) {
      Serial.println("MDNS responder started");
    }

    server.on("/measurements", get_measurements);
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

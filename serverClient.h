#pragma once

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uri/UriRegex.h>
#include "constants.h"
#include "secrets.h"

#define HOSTNAME "syncStatusServer"
ESP8266WebServer server(80);

class ServerClient {

  int8_t* ledStatePointer;
  static const int validateLedState = 1 << LabelOffset::LENGTH;

  public:
    ServerClient(int8_t* ledState)
      : ledStatePointer(ledState)
    {}

    void setup() {
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, passPhrase);
      Serial.println("");
      WiFi.setHostname(HOSTNAME);

      // Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      if (MDNS.begin(HOSTNAME)) { Serial.println("MDNS responder started"); }
      MDNS.addService("http", "tcp", 80);

      server.on(F("/"), []() {
        server.send(200, "text/plain", HOSTNAME);
      });

      server.on(F("/state"), HTTP_GET, [this]() {
        server.send(200, "text/plain", String(*ledStatePointer));
      });

      server.on(UriRegex("^\\/state\\/([0-9]+)$"), HTTP_PUT, [this]() {
        String str = server.pathArg(0);
        if(str.toInt() < validateLedState){
          *ledStatePointer = str.toInt();
          // ledState.draw();
          server.send(200, "text/plain", "success!");
        } else {
          server.send(400, "text/plain", "This operation is out of scope.");
        }
      });

      server.on(F("/labels"), HTTP_PUT, []() {
        Serial.print("labels args: ");
        Serial.println(server.args());
        for (uint8_t i = 0; i < server.args(); i++) { Serial.println(server.argName(i) + ": " + server.arg(i)); }
        if(server.args() == 0) Serial.println(server.arg("plain"));
        server.send(200, "text/plain", "success!");
      });

      server.begin();
      Serial.println("HTTP server started");
    }

    void setloop() {
      server.handleClient();
      MDNS.update();
    }
};
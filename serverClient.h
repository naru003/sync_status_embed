#pragma once

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uri/UriRegex.h>

#include "secrets.h"
#include "ledState.h"

#define HOSTNAME "syncStatusServer"
ESP8266WebServer server(80);

class ServerClient {

  int* ledStatePointer;
  LedState ledState;


  public:
    ServerClient(int* ledState)
      : ledStatePointer(ledState),
        ledState(ledState)
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
        if(ledState.isValid(str.toInt())){
          *ledStatePointer = str.toInt();
          ledState.draw();
          server.send(200, "text/plain", "success!");
        } else {
          server.send(400, "text/plain", "This operation is out of scope.");
        }
      });

      server.begin();
      Serial.println("HTTP server started");
    }

    void setloop() {
      server.handleClient();
      MDNS.update();
    }
};
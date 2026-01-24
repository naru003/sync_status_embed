#pragma once

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uri/UriRegex.h>
#include <UrlEncode.h>
#include "label.h"
#include "constants.h"
#include "secrets.h"

#define HOSTNAME "syncStatusServer"
ESP8266WebServer server(80);

class ServerClient {

  int8_t* ledStatePointer;
  Label* labelsPointer;
  static const int validateLedState = 1 << LabelOffset::LENGTH;

  int utf8Length(const String& s) {
    int count = 0;
    for (uint16_t i = 0; i < s.length(); i++) {
      // UTF-8 の先頭バイトまたは単一バイト文字の判定（非継続バイト判定）
      if ((s[i] & 0xC0) != 0x80) {
        count++;
      }
    }
    return count;
  }

  String validateArgs(ESP8266WebServer& server) {
    if (server.args() != 4) return "invalid args length";

    for (uint8_t i = 0; i < server.args(); i++) {
      String key = server.argName(i);
      String value = server.arg(i);

      if (key.length() != 1 || key[0] < '0' || key[0] > '3') {
        return "invalid key: " + key;
      }

      if (utf8Length(value) > 5) {
        return "value too long: " + value;
      }
    }

    return "";
  }


  public:
    ServerClient(int8_t* ledState, Label* labels)
      : ledStatePointer(ledState),
        labelsPointer(labels)
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
          server.send(200, "text/plain", "success!");
        } else {
          server.send(400, "text/plain", "This operation is out of scope.");
        }
      });

      server.on(F("/labels"), HTTP_GET, [this]() {
        String response;
        for(uint8_t i = 0; i < LabelOffset::LENGTH; i++) {
          if(i > 0) response += "&";
          response += String(i) + "=" + urlEncode(labelsPointer[i].getText());
        }
        server.send(200, "application/x-www-form-urlencoded", response);
      });

      server.on(F("/labels"), HTTP_PUT, [this]() {
        String message = validateArgs(server);
        if(message != "") {
          server.send(400, "text/plain", message);
          return;
        }

        for (uint8_t i = 0; i < server.args(); i++) {
          labelsPointer[server.argName(i).toInt()].setText(server.urlDecode(server.arg(i)).c_str());
        }
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
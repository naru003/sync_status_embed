#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uri/UriRegex.h>

#include "secrets.h"

#define HOSTNAME "syncStatusServer"

static const int led_pins[] = {15, 14, 13, 12};
static const int pin_size = sizeof(led_pins) / sizeof(led_pins[0]);
static const int validate_led_state = 1 << pin_size;

void set_led_state(int led_state) {
  for (int i = 0; i < pin_size; i++) {
    digitalWrite(led_pins[i], bitRead(led_state, i));
  }
}


ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passPhrase);
  Serial.println("");
  WiFi.setHostname(HOSTNAME);

  for (int i = 0; i < pin_size; i++) {
    pinMode(led_pins[i], OUTPUT);
  }

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

  server.on(UriRegex("^\\/actions\\/([0-9]+)$"), []() {
    String str = server.pathArg(0);
    int led_state = str.toInt();
    if(led_state < validate_led_state){
      set_led_state(led_state);
      server.send(200, "text/plain", "success!");
    } else {
      server.send(400, "text/plain", "This operation is out of scope.");
    }
    // Serial.println(led_state);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

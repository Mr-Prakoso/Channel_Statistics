#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <YoutubeApi.h>
#include <ArduinoJson.h>

char ssid[] = "Rumah_witjaksono";       // your network SSID (name)
char password[] = "SarangEdii";  // your network key
#define API_KEY "AIzaSyDg3__EQqyj2dy7AsQLmccKZR9uOH0pCOw"  // your google apps API Token
#define CHANNEL_ID "UClg17BImdKKGVncnuQJrozQ" // makes up the url of channel

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 60000;
unsigned long nextRunTime;

long subs = 0;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 4
#define DATA_PIN D5
#define CS_PIN D6
#define CLK_PIN D7

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  Serial.begin(9600);
  P.begin();

  P.setIntensity(50);
  P.displayClear();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    P.print(".");
    delay(500);
  }
  IPAddress ip = WiFi.localIP();

  client.setInsecure();

  api._debug = true;
}

void loop() {

  if (millis() > nextRunTime)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      P.setTextAlignment(PA_CENTER);
      P.print(api.channelStats.subscriberCount);
    }
    nextRunTime = millis() + timeBetweenRequests;
  }
}

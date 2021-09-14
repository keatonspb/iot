#include "Arduino.h"
#include "EspMQTTClient.h" /* https://github.com/plapointe6/EspMQTTClient */

#include "DHT.h"
#include "config.h"

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

EspMQTTClient client(
  SSID,
  WIFIPASSWORD,
  HUB_ADDR,
  DEVICE_NAME
);

void setup() {
  if DEBUG {
    client.enableDebuggingMessages();  
  }
  dht.begin();
  Serial.begin(9600);  
  pinMode(A0, INPUT);
}

void onConnectionEstablished() {
  publishTemperature();
}

bool publishTemperature() {
    if(client.isConnected()) {
      unsigned int raw = analogRead(A0);
      float volt = raw/1023.0;
      volt = volt*4.2;
      
      float temp = dht.readTemperature();
      float hum = dht.readHumidity();
      client.publish("base/state/temperature", String(temp));
      client.publish("base/state/humidity", String(hum));
      client.publish("base/state/battery/voltage", String(volt));
      Serial.println();
      delay(200);
      ESP.deepSleep(600e6);
      return true;
    }
    return false;
}

void loop() {
  client.loop();
}

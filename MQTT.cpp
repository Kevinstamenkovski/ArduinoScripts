#include <Ethernet.h>
#include <MQTT.h>

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0xA9, 0x1B};
EthernetClient net;
MQTTClient client;
unsigned long lastMillis = 0;
void connect() {
  Serial.print("connecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("/hello");
}
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/Hello", "World!");
  }
}

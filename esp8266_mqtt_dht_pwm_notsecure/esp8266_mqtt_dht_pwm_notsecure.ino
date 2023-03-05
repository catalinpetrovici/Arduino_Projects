#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "------------";
const char* password = "------------";

const char* mqtt_server = "------------";
const char* mqtt_username = "NULL";
const char* mqtt_password = "NULL";

const char* clientName = "------------";

const char* willTopic = "microcontroller/------------/online";
uint8_t willQoS = 1;
boolean  willRetain = true;
const char* willMessage = "false";
boolean  cleanSession = false;

// DHT Topics
const char* setDhtNowTopic = "microcontroller/------------/dht22";
const char* getDhtTemperatureTopic = "microcontroller/------------/dht22/temperature";
const char* getDhtHumidityTopic = "microcontroller/------------/dht22/humidity";
const char* getDhtHeatIndexTopic = "microcontroller/------------/dht22/heatindex";
const char* getDhtIntervalTopic = "microcontroller/------------/dht22/interval";
const char* setDhtIntervalValueTopic = "microcontroller/------------/dht22/interval/value";

// LED Topics
const char* setLedNowTopic = "microcontroller/------------/led";
const char* setLed0Topic = "microcontroller/------------/led/0";
const char* getLed0ValueTopic = "microcontroller/------------/led/0/value";
const char* setLed1Topic = "microcontroller/------------/led/1";
const char* getLed1ValueTopic = "microcontroller/------------/led/1/value";

//

WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

unsigned long dht_lastMsg = 0;
int dht_interval = 10;

#define DHTPIN D7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LED_ROOM_0 D6
#define LED_ROOM_1 D5
int LED_ROOM_0_VALUE = 0;
int LED_ROOM_1_VALUE = 0;

void callback(char* received_topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(received_topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(received_topic, setDhtNowTopic) == 0){
    delay(300);

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    float hic = dht.computeHeatIndex(temp, hum, false);

    client.publish(getDhtTemperatureTopic, String(temp).c_str(), true);
    client.publish(getDhtHumidityTopic, String(hum).c_str(), true);
    client.publish(getDhtHeatIndexTopic, String(hic).c_str(), true);

    return;
  }

  
  if(strcmp(received_topic, getDhtIntervalTopic) == 0){
    payload[length] = '\0';
    int dht_interval = atoi((char *)payload);
    
    client.publish(setDhtIntervalValueTopic, String(dht_interval).c_str(), true);

    return;
  }

  
  if(strcmp(received_topic, setLedNowTopic) == 0){
    
    client.publish(getLed0ValueTopic, String(LED_ROOM_0_VALUE).c_str(), true);
    client.publish(getLed1ValueTopic, String(LED_ROOM_1_VALUE).c_str(), true);

    return;
  }

  
  if(strcmp(received_topic, setLed0Topic) == 0){
    payload[length] = '\0';
    int LED_VALUE = atoi((char *)payload);

    if(LED_VALUE < 0 || LED_VALUE > 255){
      return;
    }

    LED_ROOM_0_VALUE = LED_VALUE;

    client.publish(getLed0ValueTopic, String(LED_ROOM_0_VALUE).c_str(), true);
    Serial.print(LED_ROOM_0_VALUE);
    analogWrite(LED_ROOM_0, LED_ROOM_0_VALUE);

    return;
  }

  
  if(strcmp(received_topic, setLed1Topic) == 0){
    payload[length] = '\0';
    int LED_VALUE = atoi((char *)payload);

    if(LED_VALUE < 0 || LED_VALUE > 255){
      return;
    }

    LED_ROOM_1_VALUE = LED_VALUE;

    client.publish(getLed1ValueTopic, String(LED_ROOM_1_VALUE).c_str(), true);
    Serial.print(LED_ROOM_1_VALUE);
    analogWrite(LED_ROOM_1, LED_ROOM_1_VALUE);

    return;
  }

}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  dht.begin();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  unsigned long dht_now = millis();
  if (dht_now - dht_lastMsg > 1000 * 60 * dht_interval) {
    dht_lastMsg = dht_now;

  dht_loop();
  }
}

void reconnect() {
  while (!client.connected()) {
    digitalWrite(BUILTIN_LED, LOW);
    Serial.print("Attempting MQTT connection...");

    if (client.connect(clientName,mqtt_username,mqtt_password,willTopic,1,true,"false",false) ) {
      digitalWrite(BUILTIN_LED, HIGH);

      Serial.println("connected");

      client.publish(willTopic, "true", true);
      client.publish(setDhtIntervalValueTopic, String(dht_interval).c_str(), true);
     
      client.subscribe(setDhtNowTopic);
      client.subscribe(getDhtIntervalTopic);
      client.subscribe(setLedNowTopic);
      client.subscribe(setLed0Topic);
      client.subscribe(setLed1Topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(2500);

      digitalWrite(BUILTIN_LED, HIGH);
      delay(2500);
    }
  }

  init_led_room(); 
  delay(1000);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);

    Serial.print(".");
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
  }
  digitalWrite(BUILTIN_LED, HIGH);

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
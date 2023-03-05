void dht_loop() {

  // Reading temperature and humidity
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  // Compute heat index in Celsius
  float hic = dht.computeHeatIndex(temp, hum, false);

  // Check if any reads failed and exit early
  if (isnan(hum) || isnan(temp) || isnan(hic)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  client.publish(getDhtHumidityTopic, String(hum).c_str(), true);
  client.publish(getDhtTemperatureTopic, String(temp).c_str(), true);
  client.publish(getDhtHeatIndexTopic, String(hic).c_str(), true);

  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.println(F("%"));
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(F("°C"));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C"));

}
void init_led_room() {
  delay(1000);
  LED_ROOM_0_VALUE = 50;
  LED_ROOM_1_VALUE = 50;
  analogWrite(LED_ROOM_0, LED_ROOM_0_VALUE);
  analogWrite(LED_ROOM_1, LED_ROOM_1_VALUE);

  delay(1000);
  LED_ROOM_0_VALUE = 0;
  LED_ROOM_1_VALUE = 0;
  analogWrite(LED_ROOM_0, LED_ROOM_0_VALUE);
  analogWrite(LED_ROOM_1, LED_ROOM_1_VALUE);

  client.publish(getLed0ValueTopic, String(LED_ROOM_0_VALUE).c_str(), true);
  client.publish(getLed0ValueTopic, String(LED_ROOM_1_VALUE).c_str(), true);
}
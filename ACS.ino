#include <WiFi.h>
#include <ThingSpeak.h>

#define WIFI_SSID "xxxxxx"
#define WIFI_PASSWORD "xxxxxx"
#define THINGSPEAK_CHANNEL_ID 2066968
#define THINGSPEAK_API_KEY "G6SIH76B416ACXHF"

const int ACS712_PIN = D34;
const float CALIBRATION_FACTOR = 0.185; // 185mV/A for ACS712-05B

WiFiClient client;
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  ThingSpeak.begin(client);
  pinMode(ACS712_PIN, INPUT);
  analogReadResolution(12);
}

void loop() {
  float current = getCurrent();
  Serial.printf("Current: %.2f A\n", current);

  if (millis() - lastSendTime > 10000) {
    lastSendTime = millis();
    sendToThingSpeak(current);
  }

  delay(1000);
}

float getCurrent() {
  int sensorValue = analogRead(ACS712_PIN);
  float voltage = (sensorValue / 4095.0) * 5000.0;
  float current = (voltage - 2500.0) / CALIBRATION_FACTOR;
  return current;
}

void sendToThingSpeak(float current) {
  ThingSpeak.setField(1, current);
  int statusCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.printf("Data sending failed with status code: %d\n", statusCode);
  }
}
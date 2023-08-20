#include <WiFi.h>

#define apiKey "MJ2170U8RTWKEZT1"
#define TTP_PIN 4

const char *ssid = "xxxxxx";
const char *password = "xxxxxx";

const char *server = "api.thingspeak.com";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  int touchDetected = digitalRead(TTP_PIN);
  if (touchDetected == HIGH) {
    Serial.println("touch detected!");
  delay(1000);
  }
  else
  {
    Serial.println("touch not detected");
      delay(1000);
  }

  String postStr = apiKey;
  postStr += "&field1=";
  postStr += touchDetected;

  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: ");
  client.print(apiKey);
  client.println();

  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
  Serial.print(touchDetected);

  delay(1000);
}

#include <ESP32Servo.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

Servo servoX;
Servo servoY;

const int servoXpin = 18;
const int servoYpin = 19;

const int servoXMin = 0;
const int servoXMax = 180;

const int servoYMin = 90;
const int servoYMax = 130;

int xPos = servoXMin;
int yPos = servoYMin;

// Wi-Fi credentials
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASS";

// HTTP server
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Attach servos
  servoX.attach(servoXpin);
  servoY.attach(servoYpin);

  // Initialize servos
  servoX.write(servoXMin);
  servoY.write(servoYMin);

  for(int i=servoYMin; i<=servoYMax; i++){
    servoY.write(i);
    Serial.println(i);
  }


  for(int i=servoYMax; i>=servoYMin; i--){
    servoY.write(i);
    Serial.println(i);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define POST endpoint
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL,
            [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
              StaticJsonDocument<200> doc;
              DeserializationError error = deserializeJson(doc, data);
              if (error) {
                Serial.println("Failed to parse JSON");
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
                return;
              }

              int deltaX = doc["deltaX"];
              int deltaY = doc["deltaY"];

              // Update positions
              xPos = constrain(xPos + deltaX, servoXMin, servoXMax);
              yPos = constrain(yPos + deltaY, servoYMin, servoYMax);

              // Write to servos
              servoX.write(xPos);
              servoY.write(yPos);

              // Debug output
              Serial.printf("Updated positions: ServoX = %d, ServoY = %d\n", xPos, yPos);

              // Send response
              String response;
              doc.clear();
              doc["status"] = "success";
              doc["servoX"] = xPos;
              doc["servoY"] = yPos;
              serializeJson(doc, response);
              request->send(200, "application/json", response);
            });

  // Start the server
  server.begin();
}

void loop() {
  // Nothing to do here; everything is handled in the HTTP server
}

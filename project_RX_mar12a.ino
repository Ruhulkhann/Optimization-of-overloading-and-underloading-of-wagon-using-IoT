#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>

// OLED display settings
#define SDA_PIN 21  // SDA Pin
#define SCL_PIN 22  // SCL Pin
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

// Buzzer settings
#define BUZZER_PIN 23      // GPIO pin for buzzer
#define THRESHOLD 300.0     // Threshold for buzzer activation

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Data structure for received data
typedef struct struct_message {
  float reading1;
  float reading2;
  float averageReading;
} struct_message;

struct_message receivedData;  // To store incoming data

// Callback function executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // Copy the incoming data into the receivedData structure
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Print received data to Serial Monitor
  Serial.print("Scale 1: ");
  Serial.println(receivedData.reading1, 1);
  Serial.print("Scale 2: ");
  Serial.println(receivedData.reading2, 1);
  Serial.print("Average: ");
  Serial.println(receivedData.averageReading, 1);

  // Clear OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Display the readings
  display.print("Scale 1: ");
  display.println(receivedData.reading1, 1);
  display.print("Scale 2: ");
  display.println(receivedData.reading2, 1);
  display.print("Weight: ");
  display.println(receivedData.averageReading, 1);  // Display weight

  // If weight exceeds threshold, activate buzzer and show alert
  if (receivedData.averageReading > THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    
    // Display OVERWEIGHT warning
    display.setTextSize(1.7);  // Bigger font for visibility
    display.setCursor(10, 45);
    display.println("OVERWEIGHT!");
    delay(500);  // Buzzer ON for 500ms
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Update the OLED display
  display.display();
}

void setup() {
  // Initialize Serial for debugging purposes
  Serial.begin(115200);

  // Initialize the OLED display
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    while (1);  // Stop execution if OLED initialization fails
  }

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // Ensure the buzzer is off initially

  // Clear the display and show the waiting message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Waiting for data...");
  display.display();

  // Initialize Wi-Fi in station mode for ESP-NOW communication
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback to handle incoming data
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // The loop does nothing; it's waiting for data via ESP-NOW
  // The OnDataRecv callback handles the received data and displays it
}
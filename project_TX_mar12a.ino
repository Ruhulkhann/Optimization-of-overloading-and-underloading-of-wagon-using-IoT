#include <Arduino.h>
#include <HX711.h>
#include <esp_now.h>
#include <WiFi.h>

// HX711 circuit wiring for two load cells
const int LOADCELL1_DOUT_PIN = 16;
const int LOADCELL1_SCK_PIN = 4;
const int LOADCELL2_DOUT_PIN = 17;
const int LOADCELL2_SCK_PIN = 5;

// HX711 objects
HX711 scale1;
HX711 scale2;

// Receiver MAC address (replace with actual MAC address)
uint8_t receiverMAC[] = {0xA0, 0xA3, 0xB3, 0x2B, 0xF8, 0x1C};

// Data structure to send
typedef struct struct_message {
  float reading1;
  float reading2;
  float averageReading;
} struct_message;

// Create a struct_message instance
struct_message dataToSend;

// Function to handle the sending status
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Initialize HX711 scales
  scale1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);

  // Calibration and tare for both scales
  scale1.set_scale(98.245);  // Replace with your calibration factor
  scale1.tare();
  scale2.set_scale(93.75);   // Replace with your calibration factor
  scale2.tare();

  // Initialize Wi-Fi in station mode for ESP-NOW
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer (Receiver MAC address)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);  // Use the receiver's MAC address
  peerInfo.channel = 0;  // Use default channel
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read data from the scales
  dataToSend.reading1 = scale1.get_units();
  dataToSend.reading2 = scale2.get_units();
  dataToSend.averageReading = (dataToSend.reading1 + dataToSend.reading2) / 2.0;

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

  if (result == ESP_OK) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Error sending data");
  }

  // Wait for 2 seconds before next reading
  delay(2000);
}
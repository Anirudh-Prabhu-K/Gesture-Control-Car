#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WiFiUdp.h>

Adafruit_MPU6050 mpu;

// WiFi credentials
const char* ssid = "A"; // Replace with your WiFi SSID
const char* password = "1234567890"; // Replace with your WiFi password

// UDP settings
WiFiUDP Udp;
const char* udpAddress = "192.168.86.50"; // Replace with the receiver ESP32 IP address
const int udpPort = 1234;

void setup() {
  Serial.begin(115200);
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Configure the MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  String direction = "";

  // Check tilt direction on the X and Y axes
  if (a.acceleration.y > 8.0) {
    direction = "RIGHT";
  } else if (a.acceleration.y < -8.0) {
    direction = "LEFT";
  } else if (a.acceleration.x > 8.0) {
    direction = "BACKWARD";
  } else if (a.acceleration.x < -8.0) {
    direction = "FORWARD";
  } else if (a.acceleration.x >= -8.0 && a.acceleration.x <= 8.0 && 
             a.acceleration.y >= -8.0 && a.acceleration.y <= 8.0) {
    direction = "STOP";
  }

  // Send direction via UDP
  Udp.beginPacket(udpAddress, udpPort);
  Udp.print(direction);
  Udp.endPacket();

  Serial.print("Sending: ");
  Serial.println(direction);

  delay(500);
}

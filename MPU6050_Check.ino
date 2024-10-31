#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
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

  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Check tilt direction on the X and Y axes
  if (a.acceleration.y > 8.0) {
    Serial.print("Tilting Right - ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(" m/s², Y: ");
    Serial.println(a.acceleration.y);
  } else if (a.acceleration.y < -8.0) {
    Serial.print("Tilting Left - ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(" m/s², Y: ");
    Serial.println(a.acceleration.y);
  } else if (a.acceleration.x > 8.0) {
    Serial.print("Tilting Backward - ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(" m/s², Y: ");
    Serial.println(a.acceleration.y);
  } else if (a.acceleration.x < -8.0) {
    Serial.print("Tilting Forward - ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(" m/s², Y: ");
    Serial.println(a.acceleration.y);
  } else if (a.acceleration.x >= -8.0 && a.acceleration.x <= 8.0 && 
             a.acceleration.y >= -8.0 && a.acceleration.y <= 8.0) {
    Serial.println("Stop");  // Both X and Y are within -5 to 5 range
  } 

  delay(500);
}

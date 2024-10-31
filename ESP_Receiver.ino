#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "A"; // Replace with your WiFi SSID
const char* password = "1234567890"; // Replace with your WiFi password

// UDP settings
WiFiUDP Udp;
const int udpPort = 1234;

const int ENA = 23; // PWM pin for motor A speed control
const int ENB = 32; // PWM pin for motor B speed control
const int IN1 = 21; // Motor A direction pin
const int IN2 = 22; // Motor A direction pin
const int IN3 = 33; // Motor B direction pin
const int IN4 = 25; // Motor B direction pin

int forwardSpeed = 120; // Default speed when moving forward
int turnSpeed = 100; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Print the local IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(udpPort);
  
  // Set motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  char incomingPacket[255]; // buffer for incoming packets

  // Read incoming packets
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0; // Null terminate string
    }
    
    String command = String(incomingPacket);
    Serial.print("Received: ");
    Serial.println(command);
    
    // Control motors based on received command
    if (command == "FORWARD") {
      // Forward motion
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, forwardSpeed);
      analogWrite(ENB, forwardSpeed);
      Serial.println("Moving Forward");

    } else if (command == "BACKWARD") {
      // Backward motion
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      analogWrite(ENA, forwardSpeed);
      analogWrite(ENB, forwardSpeed);
      Serial.println("Moving Backward");

    } else if (command == "LEFT") {
      // Rotate left
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, 0); // Stop Motor A
      analogWrite(ENB, turnSpeed); // Run Motor B at turn speed
      Serial.println("Turning Left");

    } else if (command == "RIGHT") {
      // Rotate right
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, turnSpeed); // Run Motor A at turn speed
      analogWrite(ENB, 0); // Stop Motor B
      Serial.println("Turning Right");

    } else if (command == "STOP") {
      // Stop motion
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      Serial.println("Motors Stopped");
      
    }
  }
}



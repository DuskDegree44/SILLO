#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>

const int ultrasonicPin1 = 2;       // Pin del primer sensor ultrasónico
const int ultrasonicPin2 = 3;       // Pin del segundo sensor ultrasónico
const int motorPin = 4;             // Pin del motor
const int servoPin = 5;             // Pin del servomotor
const int buttonPin = 6;            // Pin del botón

const int detectionDistance = 5;    // Distancia de detección en centímetros

Servo myServo;
NewPing sonar1(ultrasonicPin1, ultrasonicPin1, detectionDistance);
NewPing sonar2(ultrasonicPin2, ultrasonicPin2, detectionDistance);

bool systemActive = false;

void activateMotor() {
  digitalWrite(motorPin, HIGH);
}

void stopMotor() {
  digitalWrite(motorPin, LOW);
}

void activateServo() {
  myServo.write(180);
}

void closeServo() {
  myServo.write(0);
}

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  myServo.attach(servoPin);
}

void loop() {
  if (!systemActive && digitalRead(buttonPin) == LOW) {
    // El botón ha sido presionado y el sistema no está activo
    systemActive = true;
    delay(50); // Debounce
  }
  
  if (systemActive) {
    // Verificar si el primer ultrasonido detecta algo a 5 cm
    if (sonar1.ping_cm() <= detectionDistance) {
      activateMotor();
    } else {
      stopMotor();
    }

    // Verificar si el segundo ultrasonido detecta algo a 5 cm
    if (sonar2.ping_cm() <= detectionDistance) {
      stopMotor();
      activateServo();
      delay(2000);
      closeServo();
      activateMotor();
      delay(3000);
    }
  }
  
  if (systemActive && digitalRead(buttonPin) == LOW) {
    // El botón ha sido presionado y el sistema está activo
    systemActive = false;
    stopMotor();
    closeServo();
    delay(50); // Debounce
  }
}



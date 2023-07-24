//By Edwin Manda

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* define section */

// Motor driver setup L298N
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

const int ENA = 9;
const int ENB = 3;

const int trig = 11;
const int echo = 12;

const int trig2 = 8;
const int echo2 = 13;

int buzzer = 10;
int command = 0;

long duration;
int dis;
int safe;

// New feature: obstacle count
int obstacleCount = 0;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/* Setup section */

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.backlight();
}

/* Loop section */

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    executeCommand(command);
  }

  // First sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  dis = (duration * 0.034) / 2;
  safe = dis;

  if (safe <= 10) {
    stopMotors();
    digitalWrite(buzzer, HIGH);
    displayLCD();
    obstacleCount++; // Increment obstacle count
  } else {
    digitalWrite(buzzer, LOW);
    lcd.clear();
  }

  // Second sensor
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration = pulseIn(echo2, HIGH);
  dis = (duration * 0.034) / 2;
  safe = dis;

  if (safe <= 10) {
    stopMotors();
    digitalWrite(buzzer, HIGH);
    displayLCD();
    obstacleCount++; // Increment obstacle count
  } else {
    digitalWrite(buzzer, LOW);
    lcd.clear();
  }

  // Check if obstacle count reaches 5
  if (obstacleCount >= 4) {
    stopMotors();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not safe to");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("operate vehicle");
    delay(8000);
    
  }
}

void executeCommand(int command) {
  switch (command) {
    case '0':
      moveForward();
      break;
    case '1':
      moveBackward();
      break;
    case '2':
      moveLeft();
      break;
    case '3':
      moveRight();
      break;
    case '4':
      stopMotors();
      break;
    default:
      break;
  }
}

void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
}

void moveBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
}

void moveLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
}

void moveRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 60);
  analogWrite(ENB, 60);
}

void stopMotors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void displayLCD() {
  lcd.setCursor(0, 0);
  lcd.print("SHOPPER DETECTED!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Kindly wait...");
  delay(8000);
}
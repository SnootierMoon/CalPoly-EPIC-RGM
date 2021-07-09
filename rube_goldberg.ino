#include <Servo.h>
#include "SR04.h"
#include "pitches.h"

/*
 *       PINS
 */
#define P_MOTOR_DIRA 2
#define P_MOTOR_ENABLE 3
#define P_MOTOR_DIRB 4

#define P_BUZZER 12
#define P_BUTTON 13

#define P_LED_B 9
#define P_LED_G 10
#define P_LED_R 11

#define P_SERVO 5

#define P_SR04_ECHO 7
#define P_SR04_TRIG 8

/*
 *       GLOBAL STATE
 */
Servo myServo;
SR04 sr04 = SR04(P_SR04_ECHO, P_SR04_TRIG);

struct MelodyNote {
  int note;
  int rgb[3];
  int dur_ms;
} melody[] = {
  MelodyNote { .note = NOTE_C5, .rgb = {0xFF, 0x00, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_D5, .rgb = {0xFF, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_E5, .rgb = {0xBF, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_F5, .rgb = {0x00, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_G5, .rgb = {0x00, 0x00, 0xFF}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_F5, .rgb = {0x00, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_E5, .rgb = {0xBF, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_D5, .rgb = {0xFF, 0xFF, 0x00}, .dur_ms = 500 },
  MelodyNote { .note = NOTE_C5, .rgb = {0xFF, 0x00, 0x00}, .dur_ms = 500 },
};

extern const int melodyLen;
const int IMAX = 0x7FFF; // max on 16 bit architectures, 2^15-1


const int melodyLen = sizeof(melody)/sizeof(melody[0]);

/*
 *       SETUP
 */
void setup() {
  // Setup Pins
  pinMode(P_MOTOR_DIRA,   OUTPUT); 
  pinMode(P_MOTOR_ENABLE, OUTPUT);
  pinMode(P_MOTOR_DIRB,   OUTPUT);
  pinMode(P_BUZZER,       OUTPUT);
  pinMode(P_BUTTON,       INPUT);
  pinMode(P_LED_R,        OUTPUT);
  pinMode(P_LED_G,        OUTPUT);
  pinMode(P_LED_B,        OUTPUT);

  myServo.attach(P_SERVO);
  myServo.write(180);
  
  digitalWrite(P_MOTOR_DIRA, HIGH);
  digitalWrite(P_MOTOR_DIRB, LOW);
  
  // Setup Serial
  Serial.begin(9600);
  delay(500);
  Serial.println(" - Init");
}

/*
 *       LOOP
 */
void loop() {
  // Button Step
  while (digitalRead(P_BUTTON) == HIGH) { // Wait for Button
    delay(50);
  }

  // Servo Step
  myServo.write(0);   // Enable Servo
  delay(1000);        // Wait for Servo to finish
  myServo.write(180); // Reset Servo
  
  delay(3000);

  // Water Step
  analogWrite(P_MOTOR_ENABLE, 255); // Enable Fan
  for (unsigned dist = -1; dist > 10; dist = sr04.Distance()) { // Wait for Distance
    Serial.print("Distance: ");
    Serial.println(dist);
    delay(50);
  }
  analogWrite(P_MOTOR_ENABLE, 0);  // Reset Fan

  // Final Step
  playSong();
}

void playSong() {
  for (int i = 0; i < melodyLen; i++) {
    analogWrite(P_LED_R, melody[i].rgb[0]);
    analogWrite(P_LED_G, melody[i].rgb[1]);
    analogWrite(P_LED_B, melody[i].rgb[2]);
    tone(P_BUZZER, melody[i].note, melody[i].dur_ms);
    delay(melody[i].dur_ms);
  }
  analogWrite(P_LED_R, 0);
  analogWrite(P_LED_G, 0);
  analogWrite(P_LED_B, 0);  
}

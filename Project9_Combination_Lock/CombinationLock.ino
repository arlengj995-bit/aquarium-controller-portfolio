// Project 9 — Keypad Combination Lock
// October 2026
// keypad D9-D2 | servo D10 | green LED A0 | red LED A1 | buzzer A2
// first step toward changing aquarium settings without plugging in a laptop

#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {   // same layout as the labels printed on the keypad
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};   // ribbon pins 1-4 are the rows
byte colPins[COLS] = {5, 4, 3, 2};   // ribbon pins 5-8 are the columns, all 16 keys checked in Stage 1
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo lockServo;   // same SG90 from the radar project, now it's the "bolt"

int greenLed = A0;   // D2-D9 are taken by the keypad so the LEDs moved to analog pins
int redLed = A1;
int buzzer = A2;     // active buzzer, the passive one only clicks with digitalWrite

String password = "1234";   // hardcoded for now, changing it from the keypad is a future upgrade
String entered = "";
int wrongTries = 0;

// 0 = locked, 1 = unlocked, 2 = lockout
int state = 0;                  // same state machine idea as the traffic light, but keys change it not a timer
unsigned long stateStart = 0;   // millis instead of delay so the keypad still works while waiting

void beep(int ms) {   // made this a function because I beep in like 5 places
  digitalWrite(buzzer, HIGH);
  delay(ms);
  digitalWrite(buzzer, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lockServo.attach(10);
  lockServo.write(0);   // always start locked, even after a power cut
  Serial.println("Locked. Enter code and press #");
}

void loop() {
  char key = keypad.getKey();   // returns nothing if no key is pressed, so loop keeps running

  switch (state) {

    case 0:   // locked
      if (key) {
        if (key >= '0' && key <= '9' && entered.length() < 4) {   // only digits, and never more than 4
          entered += key;
          Serial.print("*");   // showing the real digits would defeat the point
          beep(30);            // so you know the press actually counted
        }
        else if (key == '*') {   // * is the "oops" button
          entered = "";
          Serial.println(" cleared");
        }
        else if (key == '#') {   // # means "check my code"
          Serial.println();
          if (entered == password) {
            state = 1;
            stateStart = millis();   // start the 5 second unlock timer
            wrongTries = 0;          // correct code wipes out earlier mistakes
            lockServo.write(90);
            digitalWrite(greenLed, HIGH);
            beep(100);
            delay(50);
            beep(100);   // two short beeps = good, easy to tell apart from the error sound
            Serial.println("Unlocked");
          }
          else {
            wrongTries++;
            Serial.print("Wrong code ");
            Serial.print(wrongTries);
            Serial.println("/3");

            if (wrongTries >= 3) {   // otherwise someone could just try all 10,000 codes
              state = 2;
              stateStart = millis();
              digitalWrite(redLed, HIGH);   // red stays on the whole lockout so you know why nothing works
              beep(1000);
              Serial.println("Too many tries. Locked out for 30 seconds");
            }
            else {
              for (int i = 0; i < 3; i++) {   // delay is fine here, nothing else has to run
                digitalWrite(redLed, HIGH);
                beep(150);
                digitalWrite(redLed, LOW);
                delay(150);
              }
            }
          }
          entered = "";   // never let old digits carry into the next try
        }
        // A, B, C, D do nothing yet, saving them for the capstone menu
      }
      break;

    case 1:   // unlocked
      if (key == '*' || millis() - stateStart >= 5000) {   // auto relock, nobody remembers to do it
        state = 0;
        lockServo.write(0);
        digitalWrite(greenLed, LOW);
        Serial.println("Locked");
      }
      break;

    case 2:   // lockout, keys ignored on purpose
      if (millis() - stateStart >= 30000) {   // 30 s makes guessing all codes take over a day
        state = 0;
        wrongTries = 0;
        digitalWrite(redLed, LOW);
        Serial.println("Lockout over. Locked");
      }
      break;
  }
}

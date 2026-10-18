#include <Servo.h>   // Includes Arduino's Servo library. The library generates the timing pulses needed to control the servo position

Servo radarServo;    //  Creates a Servo object representing the physical radar servo

const int trigPin = 12;
const int echoPin = 11;
const int servoPin = 9;   // Servo signal wire is connected to Arduino D9

// Radar Scan Settings
const int minAngle = 15;    // Lower servo limit
const int maxAngle = 165;   // Upper servo limit
const int angleStep = 2;    // Measure every 2 degrees

void setup() {

  // Configure HC-SR04 pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start Serial communication at 9600 bits per second
  Serial.begin(9600);

  // Connects the Servo object to digital pin 9. After this, the Servo library generates the control pulses required to position the servo
  radarServo.attach(servoPin); 

  // Move servo to the starting position before beginning the scan
  radarServo.write(minAngle);

  // Give the servo time to physically reach the starting position
  delay(500);
}


// FUNCTION: measureDistance() measures ultrasonic distance at the specified servo angle and sends one CSV data point over Serial

void measureDistance(int angle) {

  // Clear the TRIG pin before starting a new measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 us HIGH trigger pulse to the HC-SR04. This tells the sensor to begin an ultrasonic measurement
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  // Measure how long the ECHO pin remains HIGH. pulseIn() returns the pulse duration in microseconds. The 30000 us timeout prevents Arduino from waiting too long if no valid echo is received
  // pulseIn() is declared as returning unsigned long. Its return type is defined to be non-negative because a pulse duration can never be negative.
  unsigned long echoDuration = pulseIn(echoPin, HIGH, 30000);

  // If pulseIn() returns 0, no valid echo was received before the timeout
  if (echoDuration == 0) {
    // For Serial Plotter, 400 cm is used to represent no detected object / maximum display range

  Serial.print(angle);
  Serial.println(",400");

    return;
  }

  // Speed of sound
  // Assume room temperature of 20 deg C (68 deg F).
  // v = 331.3 + (0.606 x T)
  // At T = 20 deg C:
  // v = 331.3 + (0.606 x 20) = 343.42 m/s
  // Convert m/s to cm/us:
  // 343.42 m/s = 0.034342 cm/us

  float speedOfSound = 0.034342;  // cm/us at 20 °C (68 °F)

  // Distance calculation
  // distance = speed x time
  // HC-SR04 echo duration represents:
  // sensor -> object -> sensor
  // therefore the measured path is twice the actual distance.
  // distance = (echoDuration x speedOfSound) / 2
  float distance = (echoDuration * speedOfSound) / 2.0;



  // RANGE VALIDATION
  // HC-SR04 practical operating range is approximately 2 cm to 400 cm. If reading is outside this range, display it as 400 cm on the Serial Plotter

  if (distance < 2.0 || distance > 400.0) {
    distance = 400.0;
  }

  // SERIAL PLOTTER OUTPUT
  // Arduino Serial Plotter recognizes these as two separate labeled data series: Angle and Distance
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance, 1);

}

  void loop() {

  // FORWARD SWEEP 15 degrees -> 165 degrees


  for (int angle = minAngle; angle <= maxAngle; angle += angleStep) {

    // Command servo to move to current angle
    radarServo.write(angle);

    // Allow servo time to move toward the new position
    delay(30);

    // Measure distance at this angle
    measureDistance(angle);
  }


  // REVERSE SWEEP 165 degrees -> 15 degrees

  for (int angle = maxAngle; angle >= minAngle; angle -= angleStep) {

    // Command servo to move to current angle
    radarServo.write(angle);

    // Allow servo time to move
    delay(30);

    // Measure distance at this angle
    measureDistance(angle);
  }
}
  


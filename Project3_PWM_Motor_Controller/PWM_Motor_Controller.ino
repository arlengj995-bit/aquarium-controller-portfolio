// Defining the pins
const int potPin = A0; // Reads the potentiometer position

const int enablePin = 9;   // L293D Pin 1 sends the PWM signal to control motor speed
const int input1Pin = 8;   // L293D Pin 2 controls one side of the H-bridge
const int input2Pin = 7;   // L293D Pin 7 controls the other side of the H-bridge

// Setting the motor direction
void setup() {
  pinMode(enablePin, OUTPUT);
  pinMode(input1Pin, OUTPUT);
  pinMode(input2Pin, OUTPUT);

  digitalWrite(input1Pin, LOW);
  digitalWrite(input2Pin, HIGH);

  Serial.begin(9600); //Starting Serial Monitor at 9600 bits per second
}

void loop() {
  int potValue = analogRead(potPin); // Reading the potentiometer

  int pwmValue = map(potValue, 0, 1023, 0, 255); // Converting the potentiometer reading to PWM (ADC range:0 → 1023 becomes PWM range: 0 → 255)


  if (pwmValue < 40) {  // Adding a low-speed dead zone as DC motor often cannot rotate at very small PWM values
  pwmValue = 0;
  }

  float dutyCycle = (pwmValue / 255.0) * 100.0; // Calculating PWM duty cycle %

  float averageVoltage = (dutyCycle / 100.0) * 5.0; // Calculating theoretical average motor voltage

  analogWrite(enablePin, pwmValue); // Sending the PWM value to the motor

  Serial.print("ADC: ");
  Serial.print(potValue);

  Serial.print("  PWM: ");
  Serial.println(pwmValue);

  Serial.print("  Duty Cycle: ");
  Serial.print(dutyCycle);
  Serial.println("%");

  Serial.print("%  Avg Voltage: ");
  Serial.print(averageVoltage);
  Serial.println(" V");

  delay(2000); // Slow down the Serial Monitor output
}
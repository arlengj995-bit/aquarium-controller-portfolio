const int ledPins[]  = {2,3,4,5,6,7,8,9};
const int pausePin   = 10;
const int resetPin   = 11;

int counter          = 0;
bool counting        = true;
unsigned long lastPauseDebounce = 0;
unsigned long lastResetDebounce = 0;
bool lastPauseState  = HIGH;
bool lastResetState  = HIGH;

void setup() {
  for(int i=0; i<8; i++) pinMode(ledPins[i], OUTPUT);
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void displayBinary(int val) {
  for(int i=0; i<8; i++) {
    digitalWrite(ledPins[i], (val >> i) & 1);
  }
}

void loop() {
  // ── Pause button (rising edge detection) ──────────────
  bool currentPauseState = digitalRead(pausePin);
  if(lastPauseState==LOW && currentPauseState==HIGH
     && millis()-lastPauseDebounce>200) {
    counting = !counting;
    lastPauseDebounce = millis();
  }
  lastPauseState = currentPauseState;

  // ── Reset button (rising edge detection) ──────────────
  bool currentResetState = digitalRead(resetPin);
  if(lastResetState==LOW && currentResetState==HIGH
     && millis()-lastResetDebounce>200) {
    counter  = 0;
    counting = true;
    displayBinary(0);
    lastResetDebounce = millis();
    Serial.println("--- RESET ---");
  }
  lastResetState = currentResetState;

  // ── Counter ───────────────────────────────────────────
  if(counting) {
    displayBinary(counter);
    Serial.print("Decimal: "); Serial.print(counter);
    Serial.print("  Binary: ");
    for(int i=7; i>=0; i--) Serial.print((counter>>i)&1);
    Serial.println();
    counter = (counter+1) % 256;
    delay(500);
  }
}
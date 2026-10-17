#include <LiquidCrystal.h> //Library inclusion to load external code before compilation
#include <DHT.h> //Library inclusion to load external code before compilation

#define DHT_PIN  7
#define DHT_TYPE DHT11
#define LDR_PIN  A0

//Creates an LCD object specifying which Arduino pins connect to which LCD control pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

//Creates a DHT sensor object specifying which pin and which sensor model
DHT dht(DHT_PIN, DHT_TYPE); 

void setup() {
  //Opens serial communication at 9600 bits per second
  Serial.begin(9600); 
  //Initialises the DHT sensor
  dht.begin(); 
  //Initialises the LCD and declares its physical dimensions (16 columns, 2 rows)
  lcd.begin(16, 2); 

//Startup message sequence to display for 2 seconds before measurements begin
  lcd.setCursor(0, 0); 
  lcd.print("Env Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
//Reads temperature and humidity from the DHT11
  float temp  = dht.readTemperature();
  float humid = dht.readHumidity();
//Reads the LDR voltage divider and converts the ADC reading to volts
  int   raw   = analogRead(LDR_PIN);
  float volts = raw * (5.0 / 1023.0);

//Validates DHT11 readings before using them.
  if (isnan(temp) || isnan(humid)) {
    lcd.setCursor(0, 0);
    lcd.print("DHT11 Error!    ");
    Serial.println("ERROR: DHT11 read failed");
    delay(2000);
    return;
  }
//Updates LCD row 0 with temperature and humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
//Prints the degree symbol ° ASCII code 223
  lcd.print((char)223);
  lcd.print("C H:");
  lcd.print(humid, 0);
  lcd.print("%  ");
//Updates LCD row 1 with light level voltage
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  lcd.print(volts, 2);
  lcd.print("V      ");

//Sends all three readings to the computer as CSV comma separated values
  Serial.print(temp);
  Serial.print(",");
  Serial.print(humid);
  Serial.print(",");
  Serial.println(raw);

  delay(2000);
}
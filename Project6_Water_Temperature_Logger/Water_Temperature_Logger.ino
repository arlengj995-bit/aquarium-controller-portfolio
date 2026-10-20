// Testing the DS18B20 sensor alone before I add the RTC and SD card in later stages

//OneWire is a communication protocol invented by Dallas Semiconductor in the 1990s. The name tells what it does; it lets a sensor communicate with the Arduino using only one wire for data. 
//Arduino pulls the line LOW for a specific time  → sensor reads this as a command
//Sensor pulls the line LOW for a specific time   → Arduino reads this as a bit
//Short LOW  = bit 1
//Long LOW   = bit 0
//Very long LOW = reset signal (start of conversation)
//The pull-up resistor is what makes this possible. Between turns, the line must return to HIGH on its own. The 4.7kΩ resistor does that.

//Dallas Semiconductor made the DS18B20 sensor. They also designed a specific way to ask it for a temperature reading using the OneWire protocol.
//Sending the "start measuring" command
//Waiting up to 750ms for the sensor to finish
//Reading the raw 12-bit number back
//Converting that number to degrees Celsius

// Wire is a communication protocol. It lets multiple devices share just two wires and still talk to the Arduino independently.
//SDA  →  Serial Data    (the actual data)
//SCL  →  Serial Clock   (keeps both sides in sync)
//Every device on those two wires has a unique address, like a house number. The Arduino calls out an address and only that device responds. Everything else ignores it.


#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>    // required for SD card
#include <SD.h>

#define SENSOR_PIN 7
#define CS_PIN 10   // chip select for SD card

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);

 // test DS18B20
  sensors.begin();
  // 0 here means pull-up resistor is in wrong row
  Serial.print("Sensors found: ");
  Serial.println(sensors.getDeviceCount());

  // test DS3231
  if (!rtc.begin()) {
    Serial.println("RTC not found. Check SDA=A4, SCL=A5");
    while (1);  // stop here if RTC missing
  }
  Serial.println("RTC ready");

rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!SD.begin(CS_PIN)) {
    Serial.println("SD card not found - check wiring");
    while (1);
  }
  Serial.println("SD card ready");

  // FILE_WRITE appends to existing file, creates it if not there yet
  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("date,time,temperature_C");
    dataFile.close(); // learned this the hard way. Data does not save until closed
  }
}

void loop() {
  DateTime now = rtc.now();

    // get temperature from DS18B20
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // -127 means sensor disconnected
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: check DS18B20 wiring");
    delay(10000);
    return;
  }

  // format: 2026/10/12,21:05:10,26.87
  String line = "";
  line += now.year();   line += "/";
  line += now.month();  line += "/";
  line += now.day();    line += ",";
  line += now.hour();   line += ":";
  line += now.minute(); line += ":";
  line += now.second(); line += ",";
  line += tempC;

  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(line);
    dataFile.close(); // if power cuts here before close, this line is lost
    Serial.println("Saved: " + line);
  } else {
    Serial.println("Error: could not open log.csv");
  }

  delay(10000);  // one reading every 10 seconds
}
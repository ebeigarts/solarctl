#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

void getTemperatures();
float getTemperature(DeviceAddress deviceAddress);
void printTemperature(float tempC, uint8_t position);
void setState(int *state, int pin, int value);
void printState(int state, uint8_t position);
void printAll();

/* Constants ******************************************************************/

#define SOLAR_MIN_TEMPERATURE         25.0 // T1, pie kuras mēģināt palaist sistēmu
#define SOLAR_DELTA_MIN_TEMPERATURE    3.0 // T3-T2, pie kura ir jādarbina sistēma
#define FLOOR_COMFORT_MIN_TEMPERATURE 40.0 // T2, pie kuras pieslēgt grīdas apkuri komforta režīmā
#define FLOOR_STANDBY_MIN_TEMPERATURE 30.0 // T2, pie kuras pieslēgt grīdas apkuri standby režīmā
#define HEATING_DELTA_MIN_TEMPERATURE  7.0 // T3-T2, pie kura pārsniegšanas jāieslēdz boilera sildītājs

/* Digital Pins ***************************************************************/

#define ONE_WIRE_BUS 2 // temperature devices
#define M1           3 // cirkulācijas sūknis
#define M2           4 // kolektoru vārsts
#define M3           5 // cirkulācijas vārsts
#define M4           6 // grīdas kontūra vārsts
#define Q1           7 // boilera sildītājs (HeaterOn)
#define S1           8 // switch (AtHome)

/* Variables ******************************************************************/

// Setup LCD
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// Temperature sensor addresses
DeviceAddress t1Address = { 0x28, 0xFF, 0x4D, 0x86, 0xC1, 0x16, 0x04, 0x42 }; // T1 gray, white, black;
DeviceAddress t2Address = { 0x28, 0xFF, 0x36, 0x3B, 0xB3, 0x16, 0x05, 0xAD }; // T2 green, blue, red
DeviceAddress t3Address = { 0x28, 0xFF, 0x67, 0xD7, 0xB5, 0x16, 0x03, 0xA2 }; // T3 brown, red, orange

// Temperatures
float t1 = 0.0;
float t2 = 0.0;
float t3 = 0.0;
float tDelta = 0.0;

// Relay states
int m1 = LOW;
int m2 = LOW;
int m3 = LOW;
int m4 = LOW;
int q1 = LOW;

// Button states
int s1 = LOW;

// Indicator
boolean loopIndicator = false;

#ifndef UNIT_TEST
void setup() {
  Serial.begin(9600); // Used to type in characters
  Serial.println("SolarCtl");

  lcd.begin(16, 2);    // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.setCursor(0, 0); // char, line
  lcd.print("SolarCtl");
  lcd.setCursor(0, 1);
  lcd.print("Locating...");

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  pinMode(Q1, OUTPUT);
  pinMode(S1, INPUT);

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Make sensor reading super precise
  sensors.setResolution(t1Address, 12);
  sensors.setResolution(t2Address, 12);
  sensors.setResolution(t3Address, 12);

  lcd.clear();
}

void loop() {
  loopIndicator = !loopIndicator;

  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  // Read states
  getTemperatures();
  s1 = digitalRead(S1);

  // Change states
  // if (t1 > t2 && t1 > SOLAR_MIN_TEMPERATURE) {
  //   setSolarActive();
  //   turnOn(1,2,3,4);
  //   turnOff(q1);
  // } else if (tDelta > SOLAR_DELTA_MIN_TEMPERATURE) {
  //   setSolarActive();
  //   turnOn(1,2,3,4);
  //   turnOff(q1);
  // } else {
  //   unsetSolarActive();
  //   turnOff(1,2,3,4);
  //   turnOff(q1);
  // }

  // Print
  printAll();
  delay(1000);

  setState(&m1, M1, HIGH);
  delay(500);
  setState(&m1, M1, LOW);
  delay(500);

  setState(&m2, M2, HIGH);
  delay(500);
  setState(&m2, M2, LOW);
  delay(500);

  setState(&m3, M3, HIGH);
  delay(500);
  setState(&m3, M3, LOW);
  delay(500);

  setState(&m4, M4, HIGH);
  delay(500);
  setState(&m4, M4, LOW);
  delay(500);

  setState(&q1, Q1, HIGH);
  delay(500);
  setState(&q1, Q1, LOW);
  delay(500);
}
#endif

void getTemperatures() {
  t1 = getTemperature(t1Address);
  t2 = getTemperature(t2Address);
  t3 = getTemperature(t3Address);
  tDelta = t3 - t2;
}

float getTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  return tempC;
}

void printAll() {
  printTemperature(t1,  0);
  printTemperature(t2,  5);
  printTemperature(t3, 10);
  printState(m1, 0);
  printState(m2, 1);
  printState(m3, 2);
  printState(m4, 3);
  printState(q1, 5);
  printState(s1, 7);
  lcd.setCursor(15, 0);
  if (loopIndicator) {
    lcd.print("*");
  } else {
    lcd.print(" ");
  }
}

void printTemperature(float tempC, uint8_t position) {
  Serial.print("Temp C: ");
  Serial.print(tempC);
  lcd.setCursor(position, 1); // char, line
  lcd.print("     ");
  lcd.setCursor(position, 1); // char, line
  lcd.print(tempC);
  lcd.setCursor(position + 4, 1);
  lcd.print(" ");
}

void setState(int *state, int pin, int value) {
  digitalWrite(pin, value);
  *state = value;
  printAll();
}

void printState(int state, uint8_t position) {
  Serial.print("State: ");
  Serial.print(state);
  lcd.setCursor(position, 0); // char, line
  lcd.print(state);
}

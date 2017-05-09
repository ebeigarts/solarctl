#ifndef UNIT_TEST

#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include "calculator.h"

void getS1();
void getTemperatures();

void setStates();
void setState(int pin, bool value);

void printAll();
void printLoopIndicator();
void printTemperature(float tempC, uint8_t position);
void printState(bool state, uint8_t position);
void toggleBacklight();

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

Calculator calc;

// Indicator
bool loopIndicator = false;

void setup() {
  Serial.begin(9600); // Used to type in characters
  Serial.println("Liepa Solar Ctl");

  lcd.begin(16, 2);    // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.setCursor(0, 0); // char, line
  lcd.print("Liepa Solar Ctl");
  lcd.setCursor(0, 1);
  lcd.print("Locating...");

  setStates(); // set default relay states before changing pin mode
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
  getS1();

  // Calculte new states
  calc.calculate();

  // Change relay states
  setStates();

  // Print
  toggleBacklight();
  printAll();

  // Sleep before the next tick
  delay(1000);
}

void getS1() {
  calc.setS1(digitalRead(S1) == LOW);
}

void getTemperatures() {
  calc.setT1(sensors.getTempC(t1Address));
  calc.setT2(sensors.getTempC(t2Address));
  calc.setT3(sensors.getTempC(t3Address));
}

void setStates() {
  setState(M1, calc.getM1());
  setState(M2, calc.getM2());
  setState(M3, calc.getM3());
  setState(M4, calc.getM4());
  setState(Q1, calc.getQ1());
}

void setState(int pin, bool value) {
  if (value) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

void printAll() {
  printTemperature(calc.getT1(),  0);
  printTemperature(calc.getT2(),  5);
  printTemperature(calc.getT3(), 10);
  printState(calc.getM1(), 0);
  printState(calc.getM2(), 2);
  printState(calc.getM3(), 4);
  printState(calc.getM4(), 6);
  printState(calc.getQ1(), 8);
  printState(calc.getS1(), 13);
  printLoopIndicator();
}

void printLoopIndicator() {
  if (loopIndicator) {
    lcd.setCursor(15, 0);
    lcd.print((char)252);
    lcd.setCursor(15, 1);
    lcd.print("L");
  } else {
    lcd.setCursor(15, 0);
    lcd.print(" ");
    lcd.setCursor(15, 1);
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

void printState(bool state, uint8_t position) {
  Serial.print("State: ");
  Serial.print(state ? "1" : "0");
  lcd.setCursor(position, 0); // char, line
  lcd.print(state ? "1" : "0");
}

void toggleBacklight() {
  if (calc.getS1()) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}

#endif
